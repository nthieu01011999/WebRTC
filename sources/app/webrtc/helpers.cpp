 

#include "helpers.hpp"
#include "utils.h"

#include <ctime>
#include <arpa/inet.h>
#include <sys/time.h>
#include <chrono>

#include "app.h"
#include "app_data.h"
#include "task_list.h"

#include <chrono>

#include "app.h"
#include "app_data.h"
#include "task_list.h"

using namespace rtc;
using namespace std::chrono;

pthread_mutex_t Client::mtxSignaling				= PTHREAD_MUTEX_INITIALIZER;
bool Client::isSignalingRunning						= false;
std::atomic<int> Client::totalClientsConnectSuccess = 0;

Client::Client(std::shared_ptr<rtc::PeerConnection> pc) {
	_peerConnection = pc;
	mPOSIXMutex		= PTHREAD_MUTEX_INITIALIZER;
	mDownloadMutex	= PTHREAD_MUTEX_INITIALIZER;
	mIsSignalingOk	= false;
}

Client::~Client() {
	APP_DBG("~Client id: %s\n", mId.c_str());
	pthread_mutex_destroy(&mPOSIXMutex);
	pthread_mutex_destroy(&mDownloadMutex);
	if (mIsSignalingOk) {
		if (--Client::totalClientsConnectSuccess < 0) {
			APP_DBG("Client::totalClientsConnectSuccess < 0\n");
			FATAL("RTC", 0x02);
		}
		APP_DBG("total client: %d\n", Client::totalClientsConnectSuccess.load());
	}
}

void Client::setSignalingStatus(bool value) {
	pthread_mutex_lock(&mtxSignaling);
	isSignalingRunning = value;
	pthread_mutex_unlock(&mtxSignaling);
}

void Client::setId(const string &newId) {
	mId = newId;
}