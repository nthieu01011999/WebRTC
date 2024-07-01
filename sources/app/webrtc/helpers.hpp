 

#ifndef helpers_hpp
#define helpers_hpp

#include <vector>
#include <shared_mutex>
#include <string>

#include "app_dbg.h"
#include "rtc/rtc.hpp"
// #include "stream.hpp"
// #include "streamsource.hpp"

using namespace std;



class Client {
public:

	Client(std::shared_ptr<rtc::PeerConnection> pc);
	~Client();

	static std::atomic<int> totalClientsConnectSuccess;
	std::atomic<bool> mIsSignalingOk;

	std::string getId();
	void setId(const std::string &newId);
	static void setSignalingStatus(bool value);
	const std::shared_ptr<rtc::PeerConnection> &peerConnection = _peerConnection;

	/* Media Stream Methods (Live & PLayBack) */

private:

	std::string mId;
	std::shared_ptr<rtc::PeerConnection> _peerConnection;
	pthread_mutex_t mPOSIXMutex, mDownloadMutex;
	
	static pthread_mutex_t mtxSignaling;
	static bool isSignalingRunning;
};

typedef std::unordered_map<std::string, std::shared_ptr<Client>> ClientsGroup_t;

#endif /* helpers_hpp */
