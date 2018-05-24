#include <RemoteControlMessages.hpp>

/*
 * THIS IS AN AUTO-GENERATED FILE. DO NOT MODIFY AS CHANGES MIGHT BE OVERWRITTEN!
 */


int32_t SteeringInstruction::ID() {
    return 2001;
}

const std::string SteeringInstruction::ShortName() {
    return "SteeringInstruction";
}
const std::string SteeringInstruction::LongName() {
    return "SteeringInstruction";
}

SteeringInstruction& SteeringInstruction::pedalPosition(const float &v) noexcept {
    m_pedalPosition = v;
    return *this;
}
float SteeringInstruction::pedalPosition() const noexcept {
    return m_pedalPosition;
}

SteeringInstruction& SteeringInstruction::steeringAngle(const float &v) noexcept {
    m_steeringAngle = v;
    return *this;
}
float SteeringInstruction::steeringAngle() const noexcept {
    return m_steeringAngle;
}



#include <RemoteControlMessages.hpp>

/*
 * THIS IS AN AUTO-GENERATED FILE. DO NOT MODIFY AS CHANGES MIGHT BE OVERWRITTEN!
 */
namespace opendlv { namespace proxy {

int32_t PedalPositionReading::ID() {
    return 1041;
}

const std::string PedalPositionReading::ShortName() {
    return "PedalPositionReading";
}
const std::string PedalPositionReading::LongName() {
    return "opendlv.proxy.PedalPositionReading";
}

PedalPositionReading& PedalPositionReading::position(const float &v) noexcept {
    m_position = v;
    return *this;
}
float PedalPositionReading::position() const noexcept {
    return m_position;
}

}}

#include <RemoteControlMessages.hpp>

/*
 * THIS IS AN AUTO-GENERATED FILE. DO NOT MODIFY AS CHANGES MIGHT BE OVERWRITTEN!
 */
namespace opendlv { namespace proxy {

int32_t GroundSteeringReading::ID() {
    return 1045;
}

const std::string GroundSteeringReading::ShortName() {
    return "GroundSteeringReading";
}
const std::string GroundSteeringReading::LongName() {
    return "opendlv.proxy.GroundSteeringReading";
}

GroundSteeringReading& GroundSteeringReading::groundSteering(const float &v) noexcept {
    m_groundSteering = v;
    return *this;
}
float GroundSteeringReading::groundSteering() const noexcept {
    return m_groundSteering;
}

}}

