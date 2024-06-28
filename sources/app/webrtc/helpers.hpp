/**
 * libdatachannel streamer example
 * Copyright (c) 2020 Filip Klembara (in2core)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

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

typedef struct t_fileDownloadInfo {
	string type;
	string path;
	uint32_t size;
} fileDownloadInfo_t;

struct ClientTrackData {
	std::shared_ptr<rtc::Track> track;
	std::shared_ptr<rtc::RtcpSrReporter> sender;

	ClientTrackData(std::shared_ptr<rtc::Track> track, std::shared_ptr<rtc::RtcpSrReporter> sender) {
		this->track	 = track;
		this->sender = sender;
	}
};

struct ClientTrack {
	std::string id;
	std::shared_ptr<ClientTrackData> trackData;

	ClientTrack(std::string id, std::shared_ptr<ClientTrackData> trackData) {
		this->id		= id;
		this->trackData = trackData;
	}
};

class Client {
public:
	enum class State {
		Waiting,
		WaitingForVideo,
		WaitingForAudio,
		Ready
	};

	enum class eOptions {
		Idle,
		LiveStream,
		Playback,
	};

	enum class ePushToTalkREQStatus {
		Begin,
		Stop
	};

	Client(std::shared_ptr<rtc::PeerConnection> pc);
	~Client();

	static void setSignalingStatus(bool value);

	/* Media Stream Methods (Live & PLayBack) */




public:

	static std::atomic<int> totalClientsConnectSuccess;
	static std::string currentClientPushToTalk;
	std::atomic<bool> mIsSignalingOk;
	
private:

	pthread_mutex_t mPOSIXMutex, mDownloadMutex;
	State mState = State::Waiting;
	std::string mId;
	std::shared_ptr<rtc::PeerConnection> _peerConnection;

	static pthread_mutex_t mtxSignaling;
	static bool isSignalingRunning;
};

typedef std::unordered_map<std::string, std::shared_ptr<Client>> ClientsGroup_t;

#endif /* helpers_hpp */
