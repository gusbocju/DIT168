#include <queue>
#include "V2VService.hpp"

int main(int argc, char **argv) {
    int retVal = 0;
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    if (0 == commandlineArguments.count("cid") || 0 == commandlineArguments.count("freq") ||
        0 == commandlineArguments.count("ip") || 0 == commandlineArguments.count("id") || 0 == commandlineArguments.count("safety-distance")) {
        std::cerr << argv[0] << " sends and receives follower-/leader-status in accordance to the DIT168 V2V protocol."
                  << std::endl;
        std::cerr << "Usage:   " << argv[0]
                  << " --cid=<OD4Session components> --freq=<frequency> --ip=<onV2VNetwork> --id=<DIT168Group> --safety-distance=<cm> --turn-delay=<ms (optional)> --steering-correction=<float (optional)> --speed-correction=<float (optional)>"
                  << std::endl;
        std::cerr << "Example: " << argv[0] << " --cid=111 --freq=10 --ip=127.0.0.1 --id=5 --safety-distance=20 --turn-delay=2000 --steering-correction=0.0 --speed-correction=0.0" << std::endl;
        retVal = 1;
    } else {
        const uint16_t CID = (uint16_t) std::stoi(commandlineArguments["cid"]);
        const uint16_t FREQ = (uint16_t) std::stoi(commandlineArguments["freq"]);
        const std::string IP = commandlineArguments["ip"];
        const std::string ID = commandlineArguments["id"];
        const float SAFETY_DISTANCE = std::stoi(commandlineArguments["safety-distance"]) /100.f;

        int TURN_DELAY = 0;
        float SPEED_CORRECTION = 0, STEERING_CORRECTION = 0;
        if (0 != commandlineArguments.count("turn-delay")) TURN_DELAY = std::stoi(commandlineArguments["turn-delay"]);
        if (0 != commandlineArguments.count("speed-correction")) SPEED_CORRECTION = std::stof(commandlineArguments["speed-correction"]);
        if (0 != commandlineArguments.count("steering-correction")) STEERING_CORRECTION = std::stof(commandlineArguments["steering-correction"]);

        std::cout << "TURN_DELAY " << TURN_DELAY << std::endl;
        std::cout << "SPEED_CORRECTION " << SPEED_CORRECTION << std::endl;
        std::cout << "STEERING_CORRECTION " << STEERING_CORRECTION << std::endl;

        std::shared_ptr<V2VService> v2vService = std::make_shared<V2VService>(IP, ID, SAFETY_DISTANCE);
        float pedalPos = 0, steeringAngle = 0, distance = 0;

        // Listen to steering instructions sent internally:
        od4 = std::make_shared<cluon::OD4Session>(CID,
        [&v2vService, &pedalPos, &steeringAngle, &distance](cluon::data::Envelope &&envelope) noexcept {
            switch (envelope.dataType()) {
                case 1039: {
                    opendlv::proxy::DistanceReading dr =
                            cluon::extractMessage<opendlv::proxy::DistanceReading>(std::move(envelope));
                    v2vService->_CURRENT_DISTANCE = dr.distance();
                } break;
                case 9010: {
                    MARBLE::Steering::Instruction::GroundSteering groundSteering =
                            cluon::extractMessage<MARBLE::Steering::Instruction::GroundSteering>(std::move(envelope));
                    steeringAngle = groundSteering.groundSteering();
                } break;
                case 9011: {
                    MARBLE::Steering::Instruction::PedalPosition pedalPosition =
                            cluon::extractMessage<MARBLE::Steering::Instruction::PedalPosition>(std::move(envelope));
                    pedalPos = pedalPosition.pedalPosition();
                } break;
                case 9001: {
                    MARBLE::DS4::StartFollow sf = cluon::extractMessage<MARBLE::DS4::StartFollow>(std::move(envelope));
                    v2vService->followRequest(v2vService->presentCars[std::to_string(sf.groupId())]);
                } break;
                case 9002: {
                    MARBLE::DS4::StopFollow sf = cluon::extractMessage<MARBLE::DS4::StopFollow>(std::move(envelope));
                    v2vService->stopFollow(v2vService->getLeader());
                } break;
                case 9003: {
                    MARBLE::DS4::StopLead sl = cluon::extractMessage<MARBLE::DS4::StopLead>(std::move(envelope));
                    v2vService->stopFollow(v2vService->getFollower());
                } break;
                default: break;
            }
        });

        LeaderStatus lastCmd;
        uint64_t cmdQueuedSince, cmdProcessed = v2vService->getTime();

        // Repeat at FREQ:
        auto atFrequency{[&v2vService, &pedalPos, &steeringAngle, &lastCmd, &cmdQueuedSince, &cmdProcessed, &TURN_DELAY, &SPEED_CORRECTION, &STEERING_CORRECTION]() -> bool {
            // Check for potentially lost connections:
            if (!v2vService->getLeader().empty() && V2VService::getTime() - v2vService->lastLeaderStatus >= 1000) {
                std::cout << "[V2V] StopFollow --> Leader" << std::endl;
                v2vService->stopFollow(v2vService->getLeader());
            }
            if (!v2vService->getFollower().empty() && V2VService::getTime() - v2vService->lastFollowerStatus >= 1000) {
                std::cout << "[V2V] StopFollow --> Follower" << std::endl;
                v2vService->stopFollow(v2vService->getFollower());
            }
            // Spam announcePresence(), followerStatus() and leaderStatus():
            // TODO: implement acquired 'distanceTraveled' from IMU!
            v2vService->announcePresence();
            v2vService->followerStatus();
            v2vService->leaderStatus(pedalPos, steeringAngle, 0);

            if (!v2vService->cmdQueue.empty()) {
                if (cmdQueuedSince != 0 && v2vService->getTime() -cmdQueuedSince >= TURN_DELAY) {
                    lastCmd = v2vService->cmdQueue.front();
                    v2vService->cmdQueue.pop();
                    cmdQueuedSince = 0;
                }
                else if (cmdQueuedSince == 0) {
                    if (lastCmd.steeringAngle() == 0 && v2vService->cmdQueue.front().steeringAngle() != 0) {
                        cmdQueuedSince = v2vService->getTime();
                    }
                    else {
                        lastCmd = v2vService->cmdQueue.front();
                        v2vService->cmdQueue.pop();
                    }
                }
            }
            if (lastCmd.timestamp() != cmdProcessed) {
                opendlv::proxy::GroundSteeringReading steeringReading;
                steeringReading.groundSteering(lastCmd.steeringAngle() == 0 ? STEERING_CORRECTION : lastCmd.steeringAngle());
                od4->send(steeringReading);
                opendlv::proxy::PedalPositionReading pedalPositionReading;
                pedalPositionReading.position(lastCmd.speed() == 0 ? lastCmd.speed() : lastCmd.speed() +SPEED_CORRECTION);
                od4->send(pedalPositionReading);
                lastCmd.timestamp(cmdProcessed);
            }
            std::cout << v2vService->cmdQueue.size() << std::endl;
            return true;
        }};
        od4->timeTrigger(FREQ, atFrequency);
    }
}

/**
 * Implementation of the V2VService class as declared in V2VService.hpp
 */
V2VService::V2VService(std::string ip, std::string id, float sd) {
    _SAFETY_DISTANCE = sd;
    _IP = ip;
    _ID = id;

    /*
     * The broadcast field contains a reference to the broadcast channel which is an OD4Session. This is where
     * AnnouncePresence messages will be received.
     */
    broadcast =
    std::make_shared<cluon::OD4Session>(BROADCAST_CHANNEL,
    [this](cluon::data::Envelope &&envelope) noexcept {
        switch (envelope.dataType()) {
            case ANNOUNCE_PRESENCE: {
                AnnouncePresence ap = cluon::extractMessage<AnnouncePresence>(std::move(envelope));
                presentCars[ap.groupId()] = ap.vehicleIp();

                od4->send(ap);
                break;
            }
            default: std::cout << "[OD4] ¯\\_(ツ)_/¯" << std::endl;
        }
    });

    /*
     * Each car declares an incoming UDPReceiver for messages directed at them specifically. This is where messages
     * such as FollowRequest, FollowResponse, StopFollow, etc. are received.
     */
    incoming =
    std::make_shared<cluon::UDPReceiver>("0.0.0.0", DEFAULT_PORT,
    [this](std::string &&data, std::string &&sender, std::chrono::system_clock::time_point &&ts) noexcept {
         std::pair<int16_t, std::string> msg = extract(data);

         switch (msg.first) {
             case FOLLOW_REQUEST: {
                 FollowRequest followRequest = decode<FollowRequest>(msg.second);
                 std::cout << "[UDP] " << sender << " --> FollowRequest" << std::endl;

                 // After receiving a FollowRequest, check first if there is currently no car already following.
                 if (followerIp.empty()) {
                     unsigned long len = sender.find(':');    // If no, add the requester to known follower slot
                     followerIp = sender.substr(0, len);      // and establish a sending channel.
                     toFollower = std::make_shared<cluon::UDPSender>(followerIp, DEFAULT_PORT);
                     followResponse();
                 }

                 od4->send(followRequest);
                 break;
             }
             case FOLLOW_RESPONSE: {
                 FollowResponse followResponse = decode<FollowResponse>(msg.second);
                 std::cout << "[UDP] " << sender << " --> FollowResponse" << std::endl;

                 od4->send(followResponse);
                 break;
             }
             case STOP_FOLLOW: {
                 StopFollow stopFollow = decode<StopFollow>(msg.second);
                 std::cout << "[UDP] " << sender << " --> StopFollow ";

                 // Clear either follower or leader slot, depending on current role.
                 unsigned long len = sender.find(':');
                 if (sender.substr(0, len) == followerIp) {
                     std::cout << "(Follower)" << std::endl;
                     followerIp = "";
                     toFollower.reset();
                 }
                 else if (sender.substr(0, len) == leaderIp) {
                     std::cout << "(Leader)" << std::endl;
                     leaderIp = "";
                     toLeader.reset();
                 }
                 else std::cout << "(Unknown)" << std::endl;

                 od4->send(stopFollow);
                 break;
             }
             case FOLLOWER_STATUS: {
                 FollowerStatus followerStatus = decode<FollowerStatus>(msg.second);
                 lastFollowerStatus = getTime();

                 od4->send(followerStatus);
                 break;
             }
             case LEADER_STATUS: {
                 LeaderStatus leaderStatus = decode<LeaderStatus>(msg.second);
                 cmdQueue.push(leaderStatus);
                 lastLeaderStatus = getTime();
                 od4->send(leaderStatus);

                 /* TODO: implement (proper) follow logic! */
                 /*
                 MARBLE::Steering::Instruction::GroundSteering gsi;
                 gsi.groundSteering(leaderStatus.steeringAngle());
                 od4->send(gsi);
                 MARBLE::Steering::Instruction::PedalPosition ppi;
                 ppi.pedalPosition(leaderStatus.speed() < 0 || _CURRENT_DISTANCE > _SAFETY_DISTANCE ? leaderStatus.speed() : 0);
                 od4->send(ppi); */
                 break;
             }
             default: std::cout << "[UDP] ¯\\_(ツ)_/¯" << std::endl;
         }
    });
}

/**
 * This function sends an AnnouncePresence (id = 1001) message on the broadcast channel. It will contain information
 * about the sending vehicle, including: IP, port and the group identifier.
 */
void V2VService::announcePresence() {
    if (!followerIp.empty()) return;
    AnnouncePresence announcePresence;
    announcePresence.vehicleIp(_IP);
    announcePresence.groupId(_ID);
    broadcast->send(announcePresence);
}

/**
 * This function sends a FollowRequest (id = 1002) message to the IP address specified by the parameter vehicleIp. And
 * sets the current leaderIp field of the sending vehicle to that of the target of the request.
 *
 * @param vehicleIp - IP of the target for the FollowRequest
 */
void V2VService::followRequest(std::string vehicleIp) {
    if (!leaderIp.empty() || vehicleIp.empty()) return;
    while (!cmdQueue.empty()) cmdQueue.pop();
    std::cout << "[UDP] FollowRequest --> " << vehicleIp << std::endl;
    leaderIp = vehicleIp;
    toLeader = std::make_shared<cluon::UDPSender>(leaderIp, DEFAULT_PORT);
    FollowRequest followRequest;
    lastLeaderStatus = getTime();
    toLeader->send(encode(followRequest));
    od4->send(followRequest);
}

/**
 * This function send a FollowResponse (id = 1003) message and is sent in response to a FollowRequest (id = 1002).
 * This message will contain the NTP server IP for time synchronization between the target and the sender.
 */
void V2VService::followResponse() {
    if (followerIp.empty()) return;
    std::cout << "[UDP] FollowResponse --> " << followerIp << std::endl;
    FollowResponse followResponse;
    lastFollowerStatus = getTime();
    toFollower->send(encode(followResponse));
    od4->send(followResponse);
}

/**
 * This function sends a StopFollow (id = 1004) request on the ip address of the parameter vehicleIp. If the IP address is neither
 * that of the follower nor the leader, this function ends without sending the request message.
 *
 * @param vehicleIp - IP of the target for the request
 */
void V2VService::stopFollow(std::string vehicleIp) {
    if (vehicleIp.empty()) return;
    StopFollow stopFollow;
    if (vehicleIp == leaderIp) {
        std::cout << "[UDP] StopFollow --> " << vehicleIp << std::endl;
        toLeader->send(encode(stopFollow));
        leaderIp = "";
        toLeader.reset();
        od4->send(stopFollow);
    }
    if (vehicleIp == followerIp) {
        std::cout << "[UDP] StopFollow --> " << vehicleIp << std::endl;
        toFollower->send(encode(stopFollow));
        followerIp = "";
        toFollower.reset();
        od4->send(stopFollow);
    }
}

/**
 * This function sends a FollowerStatus (id = 3001) message on the leader channel.
 *
 * @param speed - current velocity
 * @param steeringAngle - current steering angle
 * @param distanceFront - distance to nearest object in front of the car sending the status message
 * @param distanceTraveled - distance traveled since last reading
 */
void V2VService::followerStatus() {
    if (leaderIp.empty()) return;
    FollowerStatus followerStatus;
    toLeader->send(encode(followerStatus));
    od4->send(followerStatus);
}

/**
 * This function sends a LeaderStatus (id = 2001) message on the follower channel.
 *
 * @param speed - current velocity
 * @param steeringAngle - current steering angle
 * @param distanceTraveled - distance traveled since last reading
 */
void V2VService::leaderStatus(float speed, float steeringAngle, uint8_t distanceTraveled) {
    if (followerIp.empty()) return;
    LeaderStatus leaderStatus;
    leaderStatus.timestamp(getTime());
    leaderStatus.speed(speed);
    leaderStatus.steeringAngle(steeringAngle);
    leaderStatus.distanceTraveled(distanceTraveled);
    toFollower->send(encode(leaderStatus));
    od4->send(leaderStatus);
}

/**
 * Gets the current time.
 *
 * @return current time in milliseconds
 */
uint64_t V2VService::getTime() {
    return (uint64_t ) std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::system_clock::now().time_since_epoch()).count();
}

/**
 * The extraction function is used to extract the message ID and message data into a pair.
 *
 * @param data - message data to extract header and data from
 * @return pair consisting of the message ID (extracted from the header) and the message data
 */
std::pair<int16_t, std::string> V2VService::extract(std::string data) {
    if (data.length() < 10) return std::pair<int16_t, std::string>(-1, "");
    int id, len;
    std::stringstream ssId(data.substr(0, 4));
    std::stringstream ssLen(data.substr(4, 10));
    ssId >> std::hex >> id;
    ssLen >> std::hex >> len;
    return std::pair<int16_t, std::string> (
            data.length() -10 == len ? id : -1,
            data.substr(10, data.length() -10)
    );
};

/**
 * Generic encode function used to encode a message before it is sent.
 *
 * @tparam T - generic message type
 * @param msg - message to encode
 * @return encoded message
 */
template <class T>
std::string V2VService::encode(T msg) {
    cluon::ToProtoVisitor v;
    msg.accept(v);
    std::stringstream buff;
    buff << std::hex << std::setfill('0')
         << std::setw(4) << msg.ID()
         << std::setw(6) << v.encodedData().length()
         << v.encodedData();
    return buff.str();
}

/**
 * Generic decode function used to decode an incoming message.
 *
 * @tparam T - generic message type
 * @param data - encoded message data
 * @return decoded message
 */
template <class T>
T V2VService::decode(std::string data) {
    std::stringstream buff(data);
    cluon::FromProtoVisitor v;
    v.decodeFrom(buff);
    T tmp = T();
    tmp.accept(v);
    return tmp;
}

std::string V2VService::getLeader() {
    return this->leaderIp;
}

std::string V2VService::getFollower() {
    return this->followerIp;
}
