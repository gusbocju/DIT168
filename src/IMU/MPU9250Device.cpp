/**
 * Copyright (C) 2017 openKorp
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#include "MPU9250Device.h"

namespace opendlv {
namespace proxy {
namespace miniature {

/**
 * Constructor for MPU9250 device interfacing through I2C.
 */
MPU9250Device::MPU9250Device(std::string const &a_deviceName)
    : m_deviceFile()
    , m_addressType()
    , m_instrumentAdress()
    , m_calibrationFile()
    , m_gscale(MPU9250Device::G_SCALE::GFS_250DPS)
    , m_ascale(MPU9250Device::A_SCALE::AFS_2G)
    , m_mscale(MPU9250Device::M_SCALE::MFS_14BITS)
    , m_mmode(MPU9250Device::M_MODE::M_100HZ)
{
  m_deviceFile = open(a_deviceName.c_str(), O_RDWR);
  if (m_deviceFile < 0) {
    std::cerr << "[MPU9250] Failed to open the i2c bus." << std::endl;
  } else {
    std::cout << "[MPU9250] I2C bus " << a_deviceName 
        << " opened successfully." << std::endl;
  }
  initialiseMpu();
  std::vector<float> gyroBiasVec = loadGyroCalibrationFile();
  if (gyroBiasVec.empty()) {
    gyroBiasVec = calibrateMPU9250();
    setGyroOffset(gyroBiasVec); 
    saveGyroCalibrationFile(gyroBiasVec);
  }
}


MPU9250Device::~MPU9250Device()
{}

int8_t MPU9250Device::i2cWriteRegister(std::vector<uint8_t> a_data)
{
  uint8_t* buffer = a_data.data();

  uint8_t status = write(m_deviceFile, buffer, a_data.size());

  if (status != a_data.size()) {
    std::cerr << "[MPU9250] Failed to write on I2C bus." << std::endl;
    return -1;
  }
  return 0;
}

int8_t MPU9250Device::i2cReadRegister(uint8_t const a_addr, uint8_t *a_data, uint8_t const a_length)
{
  uint8_t buffer[1];
  buffer[0] = a_addr;
  uint8_t statusOut = write(m_deviceFile, buffer, 1);
  uint8_t statusIn = read(m_deviceFile, a_data, a_length);
  if (statusOut != 1 || statusIn != a_length) {
    std::cerr << "[MPU9250] Failed to read I2C on bus." << std::endl;
    return -1;
  }
  return 0;
}

int8_t MPU9250Device::i2cAccessDevice(uint8_t const a_addr)
{
  if (ioctl(m_deviceFile, I2C_SLAVE, a_addr) < 0) {
    std::cerr << "[MPU9250] Failed to acquire bus access or talk to slave device. " << std::endl;
    return -1;
  }
  return 0;
}

void MPU9250Device::initialiseMpu()
{
  // wake up device
  // Clear sleep mode bit (6), enable all sensors
  uint8_t addr = MPU9250_ADDRESS;
  i2cAccessDevice(addr);
  uint8_t reg = MPU9250::PWR_MGMT_1;
  i2cWriteRegister(std::vector<uint8_t>{reg,0x00});
  usleep(100000); // Wait for all registers to reset

  // Get stable time source
  // Auto select clock source to be PLL gyroscope reference if ready else
  i2cWriteRegister(std::vector<uint8_t>{reg,0x01});
  usleep(200000);

  // Configure Gyro and Thermometer
  // Disable FSYNC and set thermometer and gyro bandwidth to 41 and 42 Hz,
  // respectively;
  // minimum delay time for this setting is 5.9 ms, which means sensor fusion
  // update rates cannot be higher than 1 / 0.0059 = 170 Hz
  // DLPF_CFG = bits 2:0 = 011; this limits the sample rate to 1000 Hz for both
  // With the MPU9250, it is possible to get gyro sample rates of 32 kHz (!),
  // 8 kHz, or 1 kHz
  reg = MPU9250::CONFIG;
  i2cWriteRegister(std::vector<uint8_t>{reg,0x03});

  // Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)
  // Use a 200 Hz rate; a rate consistent with the filter update rate
  // determined inset in CONFIG above.
  reg = MPU9250::SMPLRT_DIV;
  i2cWriteRegister(std::vector<uint8_t>{reg, 0x04});

  // Set gyroscope full scale range
  // Range selects FS_SEL and AFS_SEL are 0 - 3, so 2-bit values are
  // left-shifted into positions 4:3

  // get current GYRO_CONFIG register value
  uint8_t c;
  reg = MPU9250::GYRO_CONFIG;
  i2cReadRegister(reg, &c, 1);
  // c = c & ~0xE0; // Clear self-test bits [7:5]
  c = c & ~0x02; // Clear Fchoice bits [1:0]
  c = c & ~0x18; // Clear AFS bits [4:3]
  c = c | m_gscale << 3; // Set full scale range for the gyro
  // Set Fchoice for the gyro to 11 by writing its inverse to bits 1:0 of
  // GYRO_CONFIG
  // c =| 0x00;
  // Write new GYRO_CONFIG value to register
  i2cWriteRegister(std::vector<uint8_t>{reg, c});

  // Set accelerometer full-scale range configuration
  // Get current ACCEL_CONFIG register value
  reg = MPU9250::ACCEL_CONFIG;
  i2cReadRegister(reg, &c, 1);
  // c = c & ~0xE0; // Clear self-test bits [7:5]
  c = c & ~0x18;  // Clear AFS bits [4:3]
  c = c | m_ascale << 3; // Set full scale range for the accelerometer
  // Write new ACCEL_CONFIG register value
  i2cWriteRegister(std::vector<uint8_t>{reg, c});

  // Set accelerometer sample rate configuration
  // It is possible to get a 4 kHz sample rate from the accelerometer by
  // choosing 1 for accel_fchoice_b bit [3]; in this case the bandwidth is
  // 1.13 kHz
  // Get current ACCEL_CONFIG2 register value
  reg = MPU9250::ACCEL_CONFIG2;
  i2cReadRegister(reg, &c, 1);
  c = c & ~0x0F; // Clear accel_fchoice_b (bit 3) and A_DLPFG (bits [2:0])
  c = c | 0x03;  // Set accelerometer rate to 1 kHz and bandwidth to 41 Hz
  // Write new ACCEL_CONFIG2 register value
  i2cWriteRegister(std::vector<uint8_t>{reg, c});
  // The accelerometer, gyro, and thermometer are set to 1 kHz sample rates,
  // but all these rates are further reduced by a factor of 5 to 200 Hz because
  // of the SMPLRT_DIV setting

  // Configure Interrupts and Bypass Enable
  // Set interrupt pin active high, push-pull, hold interrupt pin level HIGH
  // until interrupt cleared, clear on read of INT_STATUS, and enable
  // I2C_BYPASS_EN so additional chips can join the I2C bus and all can be
  // controlled by the Arduino as master.
  reg = MPU9250::INT_PIN_CFG;
  i2cWriteRegister(std::vector<uint8_t>{reg, 0x22});
  // Enable data ready (bit 0) interrupt
  reg = MPU9250::INT_ENABLE;
  i2cWriteRegister(std::vector<uint8_t>{reg, 0x01});
  usleep(100000);
}


std::vector<float> MPU9250Device::calibrateMPU9250()
{
  
  std::cout << "[MPU9250] Starting calibration...\n";
  uint8_t rawData[12];

  i2cAccessDevice(MPU9250_ADDRESS);

  i2cWriteRegister(std::vector<uint8_t>{MPU9250::PWR_MGMT_1, 0x80});
  usleep(100000);
  i2cWriteRegister(std::vector<uint8_t>{MPU9250::PWR_MGMT_1, 0x01});
  i2cWriteRegister(std::vector<uint8_t>{MPU9250::PWR_MGMT_2, 0x00});
  usleep(200000);

  i2cWriteRegister(std::vector<uint8_t>{MPU9250::INT_ENABLE, 0x00});
  i2cWriteRegister(std::vector<uint8_t>{MPU9250::FIFO_EN, 0x00});
  i2cWriteRegister(std::vector<uint8_t>{MPU9250::PWR_MGMT_1, 0x00});
  i2cWriteRegister(std::vector<uint8_t>{MPU9250::I2C_MST_CTRL, 0x00});
  i2cWriteRegister(std::vector<uint8_t>{MPU9250::USER_CTRL, 0x00});
  i2cWriteRegister(std::vector<uint8_t>{MPU9250::USER_CTRL, 0x0C});
  usleep(15000);


  i2cWriteRegister(std::vector<uint8_t>{MPU9250::CONFIG, 0x01});
  i2cWriteRegister(std::vector<uint8_t>{MPU9250::SMPLRT_DIV, 0x00});
  i2cWriteRegister(std::vector<uint8_t>{MPU9250::GYRO_CONFIG, 0x00});
  i2cWriteRegister(std::vector<uint8_t>{MPU9250::ACCEL_CONFIG, 0x00});
  
  float const gyroSens  = (250.0f / 32768.0f * static_cast<float>(M_PI) / 180.0f);

  i2cWriteRegister(std::vector<uint8_t>{MPU9250::USER_CTRL, 0x40});
  i2cWriteRegister(std::vector<uint8_t>{MPU9250::FIFO_EN, 0x78});
  usleep(40000);
  i2cWriteRegister(std::vector<uint8_t>{MPU9250::FIFO_EN, 0x00});
  
  i2cReadRegister(MPU9250::FIFO_COUNTH, &rawData[0], 2);

  uint16_t fifoCount = ((uint16_t) rawData[0] <<  8) | rawData[1];
  std::cout << "[MPU9250] FIFO Count: " << fifoCount << std::endl;
  uint16_t packetCount = fifoCount/12;
  std::cout << "[MPU9250] Packet Count: " << packetCount << std::endl;

  // int32_t accelBias[3] = {0,0,0};
  // std::vector<float> gyroBias;  
  int32_t gyroBias[3] = {0,0,0};
  for (uint8_t i = 0; i < packetCount; i++) {
    int16_t gyroSampl[3] = {0,0,0};
    i2cReadRegister(MPU9250::FIFO_R_W, &rawData[0], 12);

    gyroSampl[0]  = (int16_t) (((int16_t)rawData[6] << 8) | rawData[7]  );
    gyroSampl[1]  = (int16_t) (((int16_t)rawData[8] << 8) | rawData[9]  );
    gyroSampl[2]  = (int16_t) (((int16_t)rawData[10] << 8) | rawData[11]);

    gyroBias[0] += (int32_t) gyroSampl[0];
    gyroBias[1] += (int32_t) gyroSampl[1];
    gyroBias[2] += (int32_t) gyroSampl[2];
    // std::cout << "[MPU9250] Gyro bias: " << gyroBias[0]/gyroSens << ", " << gyroBias[1]/gyroSens << ", " << gyroBias[2]/gyroSens << std::endl;
  }

  gyroBias[0] /= packetCount;
  gyroBias[1] /= packetCount;
  gyroBias[2] /= packetCount;

  
  // std::cout << "[MPU9250] Gyro bias: " << gyroBias.at(0) << ", " << gyroBias.at(1) << ", " << gyroBias.at(2) << std::endl;
  std::vector<float> gyroBiasVec;
  gyroBiasVec.push_back(gyroBias[0] * gyroSens);
  gyroBiasVec.push_back(gyroBias[1] * gyroSens);
  gyroBiasVec.push_back(gyroBias[2] * gyroSens);
  std::cout << "[MPU9250] Gyro bias: " << gyroBiasVec.at(0) << ", " << gyroBiasVec.at(1) << ", " << gyroBiasVec.at(2) << std::endl;
  return gyroBiasVec;
}

int8_t MPU9250Device::setGyroOffset(std::vector<float> const a_offset)
{
  if (a_offset.size() != 3) {
    std::cerr << "[MPU9250] setGyroOffset received a vector of a length not supported." << std::endl;
    return -1;
  }

  float const gyroSens  = (250.0f / 32768.0f * static_cast<float>(M_PI) / 180.0f);

  int32_t xOffset = std::lround(a_offset.at(0) / gyroSens);
  int32_t yOffset = std::lround(a_offset.at(1) / gyroSens);
  int32_t zOffset = std::lround(a_offset.at(2) / gyroSens);

  uint8_t xh = (-xOffset/4 >> 8);
  uint8_t xl = ((-xOffset/4) & 0xFF);
  uint8_t yh = (-yOffset/4 >> 8);
  uint8_t yl = ((-yOffset/4) & 0xFF);
  uint8_t zh = (-zOffset/4 >> 8);
  uint8_t zl = ((-zOffset/4) & 0xFF);

  i2cAccessDevice(MPU9250_ADDRESS);
  i2cWriteRegister(std::vector<uint8_t>{MPU9250::XG_OFFSET_H, xh, xl, yh, yl, zh, zl});
  
  return 0;
}

int8_t MPU9250Device::saveGyroCalibrationFile(std::vector<float> const a_offset)
{
  if (a_offset.size() != 3) {
    std::cerr << "[MPU9250] saveGyroCalibrationFile received a vector of a length not supported." << std::endl;
    return -1;
  }

  std::ofstream gyroCalibrationFile(m_calibrationFile);
  if (gyroCalibrationFile.is_open()) {
    gyroCalibrationFile << a_offset.at(0) << "\n" << a_offset.at(1) << "\n" << a_offset.at(2) << "\n";
  } else {
    std::cout << "Unable to open calibration file\n";
    return -1;
  }
  gyroCalibrationFile.flush();
  gyroCalibrationFile.close();
  return 0;
}

std::vector<float> MPU9250Device::loadGyroCalibrationFile() 
{
  std::vector<float> gyroAvg;
  std::ifstream file(m_calibrationFile, std::ifstream::in);
  if (file.is_open()){
    std::string line;
    for (uint8_t i = 0; i < 3; ++i) {
      std::getline(file, line);
      try{
        gyroAvg.push_back(std::stof(line));
      } catch (std::invalid_argument e) {
        std::cout << "[MPU9250] Invalid calibration file format." << std::endl;
        file.close();
        return {};
      }
    }

    std::cout << "[MPU9250] Loaded the calibration settings." << std::endl;
    std::cout << "\nLoaded:"
        << " Gyro: " << gyroAvg.at(0) << ", " 
        << gyroAvg.at(1) << ", " 
        << gyroAvg.at(2) << std::endl;
    file.close();
    return gyroAvg;
  } else {
    std::cout << "[MPU9250] Could not load the calibration settings." 
        << std::endl;
    file.close();
    return {};
  }
}

opendlv::proxy::AccelerationReading MPU9250Device::readAccelerometer()
{
  uint8_t addr = MPU9250_ADDRESS;
  i2cAccessDevice(addr);
  uint8_t reg = MPU9250::ACCEL_XOUT_H;
  uint8_t rawData[6];
  i2cReadRegister(reg, &rawData[0], 6);

  float const c = getAscale();

  int16_t x = (((int16_t)rawData[0] << 8) | rawData[1] );
  int16_t y = (((int16_t)rawData[2] << 8) | rawData[3] );
  int16_t z = (((int16_t)rawData[4] << 8) | rawData[5] );

  // opendlv::proxy::AccelerationReading accelerometerReading(x*c,y*c,z*c);
  // opendlv::proxy::AccelerometerReading accelerometerReading(0,0,0);
  opendlv::proxy::AccelerationReading accelerationReading;
  accelerationReading.accelerationX(x*c);
  accelerationReading.accelerationY(y*c);
  accelerationReading.accelerationZ(z*c);
  return accelerationReading;
}

opendlv::proxy::MagneticFieldReading MPU9250Device::readMagnetometer()
{
  uint8_t addr = AK8963_ADDRESS;
  i2cAccessDevice(addr);
  uint8_t reg = MPU9250::AK8963_XOUT_L;
  uint8_t rawData[6];
  i2cReadRegister(reg, &rawData[0], 6);

  int16_t x = (((int16_t)rawData[0] << 8) | rawData[1] );
  int16_t y = (((int16_t)rawData[2] << 8) | rawData[3] );
  int16_t z = (((int16_t)rawData[4] << 8) | rawData[5] );

  // opendlv::proxy::MagneticFieldReading magnetometerReading(x,y,z);
  opendlv::proxy::MagneticFieldReading magneticFieldReading;
  magneticFieldReading.magneticFieldX(x);
  magneticFieldReading.magneticFieldY(y);
  magneticFieldReading.magneticFieldX(z);
  return magneticFieldReading;
}


opendlv::proxy::AngularVelocityReading MPU9250Device::readGyroscope()
{
  uint8_t addr = MPU9250_ADDRESS;
  i2cAccessDevice(addr);
  uint8_t reg = MPU9250::GYRO_XOUT_H;
  uint8_t rawData[6];
  i2cReadRegister(reg, &rawData[0], 6);

  float const c = getGscale(true);
  
  int16_t x = (((int16_t)rawData[0] << 8) | rawData[1] );
  int16_t y = (((int16_t)rawData[2] << 8) | rawData[3] );
  int16_t z = (((int16_t)rawData[4] << 8) | rawData[5] );
  
  // opendlv::proxy::AngularVelocityReading gyroscopeReading(x*c,y*c,z*c);
  // opendlv::proxy::GyroscopeReading gyroscopeReading(0,0,0);
  opendlv::proxy::AngularVelocityReading angularVelocityReading;
  angularVelocityReading.angularVelocityX(x*c);
  angularVelocityReading.angularVelocityY(y*c);
  angularVelocityReading.angularVelocityZ(z*c);
  return angularVelocityReading;
}
opendlv::proxy::AltitudeReading MPU9250Device::readAltimeter()
{
  opendlv::proxy::AltitudeReading altitudeReading;
  altitudeReading.altitude(0);
  return altitudeReading;
}

opendlv::proxy::TemperatureReading MPU9250Device::readThermometer()
{
  uint8_t addr = MPU9250_ADDRESS;
  i2cAccessDevice(addr);
  uint8_t reg = MPU9250::TEMP_OUT_H;
  uint8_t rawData[2];
  i2cReadRegister(reg, &rawData[0], 2);

  int16_t temp = (((int16_t)rawData[0] << 8) | rawData[1]) / 1.0f;

  // opendlv::proxy::TemperatureReading temperatureReading(21.0f + temp / 333.87f);
  // opendlv::proxy::TemperatureReading temperatureReading(0);

  opendlv::proxy::TemperatureReading temperatureReading;
  temperatureReading.temperature(21.0f + temp / 333.87f);
  return temperatureReading;
}

void MPU9250Device::setAscale(A_SCALE a_scale)
{
  m_ascale = a_scale;
}

float MPU9250Device::getAscale()
{
  switch (m_ascale) {
    case AFS_2G:
      return (9.82f * 2.0f / 32768.0f);
    case AFS_4G:
      return (9.82f * 4.0f / 32768.0f);
    case AFS_8G:
      return (9.82f * 8.0f / 32768.0f);
    case AFS_16G:
      return (9.82f * 16.0f / 32768.0f);
    default:
      return 0.0f;
  }
}

void MPU9250Device::setGscale(G_SCALE a_scale)
{
  m_gscale = a_scale;
}

float MPU9250Device::getGscale(bool a_radFlag)
{
    float conversion = 1;
    if (a_radFlag) {
      conversion = static_cast<float>(M_PI) / 180.0f;
    }
    switch (m_ascale) {
    case GFS_250DPS:
      return (250.0f / 32768.0f) * conversion;
    case GFS_500DPS:
      return (500.0f / 32768.0f) * conversion;
    case GFS_1000DPS:
      return (1000.0f / 32768.0f) * conversion;
    case GFS_2000DPS:
      return (2000.0f / 32768.0f) * conversion;
    default:
      return 0.0f;   
  }
}


}
}
} // opendlv::core::system::proxy
