#include "V2VService.hpp"

int main(int argc, char **argv) {
    int retVal = 0;
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    if (0 == commandlineArguments.count("cid") || 0 == commandlineArguments.count("freq") ||
        0 == commandlineArguments.count("ip") || 0 == commandlineArguments.count("id")) {
        std::cerr << argv[0] << " sends and receives follower-/leader-status in accordance to the DIT168 V2V protocol."
                  << std::endl;
        std::cerr << "Usage:   " << argv[0]
                  << " --cid=<OD4Session components> --freq=<frequency> --ip=<onV2VNetwork> --id=<DIT168Group>"
                  << std::endl;
        std::cerr << "Example: " << argv[0] << " --cid=111 --freq=10 --ip=127.0.0.1 --id=5" << std::endl;
        retVal = 1;
    } else {
        const uint16_t CID = (uint16_t) std::stoi(commandlineArguments["cid"]);
        const uint16_t FREQ = (uint16_t) std::stoi(commandlineArguments["freq"]);
        const std::string IP = commandlineArguments["ip"];
        const std::string ID = commandlineArguments["id"];

        std::shared_ptr<V2VService> v2vService = std::make_shared<V2VService>(IP, ID);
        float pedalPos = 0, steeringAngle = 0;

        // Listen to steering instructions sent internally:
        od4 = std::make_shared<cluon::OD4Session>(CID,
        [&v2vService, &pedalPos, &steeringAngle](cluon::data::Envelope &&envelope) noexcept {
            switch (envelope.dataType()) {
                case 1041: {
                    opendlv::proxy::PedalPositionReading ppr =
                            cluon::extractMessage<opendlv::proxy::PedalPositionReading>(std::move(envelope));
                    pedalPos = ppr.position();
                } break;
                case 1045: {
                    opendlv::proxy::GroundSteeringReading gsr =
                            cluon::extractMessage<opendlv::proxy::GroundSteeringReading>(std::move(envelope));
                    steeringAngle = gsr.groundSteering();
                } break;
                case 9001: {
                    MARBLE::StartFollow sf = cluon::extractMessage<MARBLE::StartFollow>(std::move(envelope));
                    v2vService->followRequest(std::to_string(sf.groupId()));
                } break;
                case 9002: {
                    MARBLE::StopFollow sf = cluon::extractMessage<MARBLE::StopFollow>(std::move(envelope));
                    v2vService->stopFollow(v2vService->getLeader());
                } break;
                case 9003: {
                    MARBLE::StopLead sl = cluon::extractMessage<MARBLE::StopLead>(std::move(envelope));
                    v2vService->stopFollow(v2vService->getFollower());
                } break;
                default: break;
            }
        });

        // Repeat at FREQ:
        auto atFrequency{[&v2vService, &pedalPos, &steeringAngle]() -> bool {
            // Check for potentially lost connections:
            if (!v2vService->getLeader().empty() && V2VService::getTime() - v2vService->lastLeaderStatus >= 1000) {
                std::cout << "[V2V] issuing StopFollow to the leading vehicle..." << std::endl;
                v2vService->stopFollow(v2vService->getLeader());
            }
            if (!v2vService->getFollower().empty() && V2VService::getTime() - v2vService->lastFollowerStatus >= 1000) {
                std::cout << "[V2V] issuing StopFollow to the following vehicle..." << std::endl;
                v2vService->stopFollow(v2vService->getFollower());
            }
            // Spam announcePresence(), followerStatus() and leaderStatus():
            v2vService->announcePresence();
            v2vService->followerStatus();

            // TODO: implement acquired 'distanceTraveled' from IMU!
            v2vService->leaderStatus(pedalPos, steeringAngle, 0);
            return true;
        }};
        od4->timeTrigger(FREQ, atFrequency);
    }
}

/**
 * Implementation of the V2VService class as declared in V2VService.hpp
 */
V2VService::V2VService(std::string ip, std::string id) {
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
                /* std::cout << "[OD4] received 'AnnouncePresence' from '"
                          << ap.vehicleIp() << "', GroupID '"
                          << ap.groupId() << "'!" << std::endl; */

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
                 std::cout << "[UDP] received '" << followRequest.LongName()
                           << "' from '" << sender << "'!" << std::endl;

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
                 std::cout << "[UDP] received '" << followResponse.LongName()
                           << "' from '" << sender << "'!" << std::endl;

                 od4->send(followResponse);
                 break;
             }
             case STOP_FOLLOW: {
                 StopFollow stopFollow = decode<StopFollow>(msg.second);
                 std::cout << "[UDP] received '" << stopFollow.LongName()
                           << "' from '" << sender << "'!" << std::endl;

                 // Clear either follower or leader slot, depending on current role.
                 unsigned long len = sender.find(':');
                 if (sender.substr(0, len) == followerIp) {
                     followerIp = "";
                     toFollower.reset();
                 }
                 else if (sender.substr(0, len) == leaderIp) {
                     leaderIp = "";
                     toLeader.reset();
                 }

                 od4->send(stopFollow);
                 break;
             }
             case FOLLOWER_STATUS: {
                 FollowerStatus followerStatus = decode<FollowerStatus>(msg.second);
                 lastFollowerStatus = getTime();
                 /* std::cout << "[UDP] received '" << followerStatus.LongName()
                           << "' from '" << sender << "'!" << std::endl; */

                 od4->send(followerStatus);
                 break;
             }
             case LEADER_STATUS: {
                 LeaderStatus leaderStatus = decode<LeaderStatus>(msg.second);
                 lastLeaderStatus = getTime();
                 /* std::cout << "[UDP] received '" << leaderStatus.LongName()
                           << "' from '" << sender << "'!" << std::endl; */

                 /* TODO: implement (proper) follow logic! */
                 opendlv::proxy::PedalPositionReading ppr;
                 ppr.position(leaderStatus.speed());
                 od4->send(ppr);
                 opendlv::proxy::GroundSteeringReading gsr;
                 gsr.groundSteering(leaderStatus.steeringAngle());
                 od4->send(gsr);

                 od4->send(leaderStatus);
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
    if (!leaderIp.empty()) return;
    std::cout << "[UDP] sending FollowRequest to '" << vehicleIp << "'!" << std::endl;
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
    std::cout << "[UDP] sending FollowResponse!" << std::endl;
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
    StopFollow stopFollow;
    if (vehicleIp == leaderIp) {
        std::cout << "[UDP] sending StopFollow to '" << vehicleIp << "'!" << std::endl;
        toLeader->send(encode(stopFollow));
        leaderIp = "";
        toLeader.reset();
        od4->send(stopFollow);
    }
    if (vehicleIp == followerIp) {
        std::cout << "[UDP] sending StopFollow to '" << vehicleIp << "'!" << std::endl;
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
