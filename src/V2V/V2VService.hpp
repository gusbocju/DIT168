#ifndef V2V_PROTOCOL_DEMO_V2VSERVICE_H
#define V2V_PROTOCOL_DEMO_V2VSERVICE_H

#include <iomanip>
#include <unistd.h>
#include <sys/time.h>
#include "cluon/OD4Session.hpp"
#include "cluon/UDPSender.hpp"
#include "cluon/UDPReceiver.hpp"
#include "cluon/Envelope.hpp"
#include "Messages.hpp"
#include "RemoteControlMessages.hpp"
#include <iostream>

/********************************************************/
/** DON'T CHANGE STUFF BELOW THIS LINE. *****************/
/********************************************************/

static const int BROADCAST_CHANNEL = 250;
static const int DEFAULT_PORT = 50001;

static const int ANNOUNCE_PRESENCE = 1001;
static const int FOLLOW_REQUEST = 1002;
static const int FOLLOW_RESPONSE = 1003;
static const int STOP_FOLLOW = 1004;
static const int LEADER_STATUS = 2001;
static const int FOLLOWER_STATUS = 3001;

std::shared_ptr<cluon::OD4Session> od4;

class V2VService {
public:
    V2VService(std::string ip, std::string id);

    void announcePresence();
    void followRequest(std::string vehicleIp);
    void followResponse();
    void stopFollow(std::string vehicleIp);
    void leaderStatus(float speed, float steeringAngle, uint8_t distanceTraveled);
    void followerStatus();

    static uint64_t getTime();

    std::string getLeader();
    std::string getFollower();

    uint64_t lastLeaderStatus;
    uint64_t lastFollowerStatus;

    std::map <std::string, std::string> presentCars;

private:
    std::string _IP;
    std::string _ID;
    
    std::string leaderIp;
    std::string followerIp;

    std::shared_ptr<cluon::OD4Session>  broadcast;
    std::shared_ptr<cluon::UDPReceiver> incoming;
    std::shared_ptr<cluon::UDPSender>   toLeader;
    std::shared_ptr<cluon::UDPSender>   toFollower;

    static std::pair<int16_t, std::string> extract(std::string data);
    template <class T>
    static std::string encode(T msg);
    template <class T>
    static T decode(std::string data);
};

#endif //V2V_PROTOCOL_DEMO_V2VSERVICE_H
