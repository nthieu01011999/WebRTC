#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <vector>

#include <algorithm>
#include <future>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include <thread>
#include <unordered_map>
#include <chrono>
#include <pthread.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <atomic>

#include "app.h"
#include "app_data.h"
#include "app_config.h"
#include "app_dbg.h"
#include "message.h"
// #include "datachannel_hdl.h"

#include "task_list.h"

// #include "dispatchqueue.hpp"
// #include "helpers.hpp"
#include "rtc/rtc.hpp"
#include "json.hpp"
// #include "stream.hpp"

#ifdef BUILD_ARM_VVTK
// #include "h26xsource.hpp"
// #include "audiosource.hpp"
#endif

// #include "mtce_audio.hpp"
// #include "parser_json.h"
#include "utils.h"

#define CLIENT_SIGNALING_MAX 20
#define CLIENT_MAX			 10

using namespace rtc;
using namespace std;
using namespace chrono_literals;
using namespace chrono;
using json = nlohmann::json;

/*****************************************************************************/
/*  task GW_TASK_WEBRTC define
 */
/*****************************************************************************/
/* define  */
q_msg_t gw_task_webrtc_mailbox;
atomic<bool> isConnected(false);
static shared_ptr<WebSocket> globalWs;

/*****************************************************************************/
/*  task GW_TASK_WEBRTC define
 */
/*****************************************************************************/
/* define websocket */
void initializeWebSocketServer(const std::string &wsUrl);
std::string loadWebSocketConfig();
void onWebSocketOpen(const shared_ptr<WebSocket>& ws);
void onWebSocketClose(const shared_ptr<WebSocket>& ws);
void onWebSocketError(const shared_ptr<WebSocket>& ws, const std::string& error);
int8_t loadWsocketSignalingServerConfigFile(string &wsUrl);



void *gw_task_webrtc_entry(void *) {
	
	// std::cout << "[STARTED] gw_task_webrtc_entry\n" << std::endl;
    APP_DBG("[STARTED] gw_task_webrtc_entry\n");
	wait_all_tasks_started();
    ak_msg_t *msg = AK_MSG_NULL;

	task_post_pure_msg(GW_TASK_WEBRTC_ID,GW_WEBRTC_INIT_WEBSOCKET);
      
    while (1) {
        /* Get message */
        msg = ak_msg_rev(GW_TASK_WEBRTC_ID);
        if (msg == AK_MSG_NULL) {
            APP_DBG_SIG("[ERROR] No message received\n");  
        } else {
            APP_DBG_SIG("[DEBUG] Message received\n");   
        }

        switch (msg->header->sig) {
        case GW_WEBRTC_INIT_WEBSOCKET: {
            APP_DBG_SIG("[GW_WEBRTC_INIT_WEBSOCKET]\n"); 
            auto wsUrl = loadWebSocketConfig();  
            initializeWebSocketServer(wsUrl);
            
            
        } break;

        case GW_WEBRTC_TRY_CONNECT_SOCKET_REQ: {
            APP_DBG_SIG("GW_WEBRTC_TRY_CONNECT_SOCKET_REQ");
            globalWs->close();
            if (globalWs->isClosed()) {
                std::string wsUrl = "ws://sig.espitek.com:8089/";
                APP_DBG("try connect to websocket: %s\n", wsUrl.data());
                globalWs->open(wsUrl);
                timer_set(GW_TASK_WEBRTC_ID, GW_WEBRTC_TRY_CONNECT_SOCKET_REQ, GW_WEBRTC_TRY_CONNECT_SOCKET_INTERVAL, TIMER_ONE_SHOT);
			
            }

        } break;

        default:
            APP_DBG_SIG("[DEBUG] Unknown message signal received: %d\n", msg->header->sig);   
            break;
        }

        /* Free message */
        ak_msg_free(msg);
    }

    return (void *)0;
}

void initializeWebSocketServer(const std::string &wsUrl) {
    // auto ws = make_shared<WebSocket>();
    // weak_ptr<WebSocket> wws = ws;
    globalWs = std::make_shared<WebSocket>();
    APP_DBG("[initializeWebSocketServer]\n");

    globalWs->onOpen([&]() {
        isConnected.store(true);
        APP_DBG("WebSocket has opened successfully.\n");
    });

    globalWs->onClosed([&]() {
        isConnected.store(false);
        APP_DBG("WebSocket has closed unexpectedly.\n");
    });

    globalWs->onError([&](const string &error) {
        isConnected.store(false);
        APP_DBG("WebSocket error: %s\n", error.c_str());
    });

    globalWs->open(wsUrl);

}



std::string loadWebSocketConfig() {
    std::string wsUrl;
    if (loadWsocketSignalingServerConfigFile(wsUrl) != APP_CONFIG_SUCCESS || wsUrl.empty()) {
        APP_DBG("Failed to load WebSocket URL configuration or URL is empty.\n");
    }
    APP_DBG("Websocket: %s\n", wsUrl.c_str());
    return wsUrl;
}

int8_t loadWsocketSignalingServerConfigFile(string &wsUrl) {
    rtcServersConfig_t rtcServerCfg;
    /*Fetch the RTC server configurations stored in a JSON file
    Then store into rtcServersConfig_t rtcServerCfg;*/
    int8_t ret = configGetRtcServers(&rtcServerCfg);
    
    // APP_DBG("[DEBUG] Loading WebSocket Signaling Server Config...\n");
    
    if (ret == APP_CONFIG_SUCCESS) {
        // APP_DBG("[DEBUG] Successfully retrieved RTC server configuration.\n");

        // Debug print to show the contents of rtcServerCfg
        // APP_DBG("[DEBUG] WebSocket Server URL: %s\n", rtcServerCfg.wSocketServerCfg.c_str());
        for (const auto& url : rtcServerCfg.arrStunServerUrl) {
            // APP_DBG("[DEBUG] STUN Server URL: %s\n", url.c_str());
        }
        for (const auto& url : rtcServerCfg.arrTurnServerUrl) {
            // APP_DBG("[DEBUG] TURN Server URL: %s\n", url.c_str());
        }
        
        wsUrl.clear();
        if (!rtcServerCfg.wSocketServerCfg.empty()) {
            wsUrl = rtcServerCfg.wSocketServerCfg + "/c02i24010000008"; 
            // APP_DBG("[DEBUG] WebSocket URL constructed: %s\n", wsUrl.c_str());
        } else {
            APP_DBG("[ERROR] WebSocket server configuration is empty.\n");
        }
    } else {
        APP_DBG("[ERROR] Failed to retrieve RTC server configuration. Error code: %d\n", ret);
    }
    
    return ret;
}

// WebSocket event handlers
void onWebSocketOpen(const shared_ptr<WebSocket>& ws) {
    // isConnected.store(true);
    // storeConnection(ws); // Assign an ID and store the connection in a map or similar structure
    APP_PRINT("WebSocket connected, signaling ready\n");
}

void onWebSocketClose(const shared_ptr<WebSocket>& ws) {
    // isConnected.store(false);
    // removeConnection(ws); // Clean up and remove the connection from management
    APP_PRINT("WebSocket closed\n");
    // timer_set(GW_TASK_WEBRTC_ID, GW_WEBRTC_TRY_CONNECT_SOCKET_REQ, GW_WEBRTC_TRY_CONNECT_SOCKET_INTERVAL, TIMER_ONE_SHOT);
}

void onWebSocketError(const shared_ptr<WebSocket>& ws, const std::string& error) {
    // isConnected.store(false);
    // logError(ws, error); // Log or handle the error appropriately
    APP_PRINT("WebSocket connection failed: %s\n", error.c_str());
    // Optionally implement reconnection or other error recovery actions here
}
