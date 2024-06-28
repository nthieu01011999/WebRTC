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
int8_t loadWsocketSignalingServerConfigFile(string &wsUrl);

/*****************************************************************************/
/*  task GW_TASK_WEBRTC define
 */
/*****************************************************************************/
/* message handling */
void handleWebSocketMessage(const std::string& message, std::shared_ptr<WebSocket> ws);
void handleClientRequest(const std::string& clientId, std::shared_ptr<WebSocket> ws);

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


// void attemptReconnect(const std::string &wsUrl, int attempt = 1) {
//     if (attempt > 3) {
//         APP_DBG("Max reconnection attempts reached.\n");
//         return;
//     }
//     std::this_thread::sleep_for(std::chrono::seconds(attempt * 2));  // Exponential back-off
//     APP_DBG("Attempting to reconnect. Attempt: %d\n", attempt);
//     initializeWebSocketServer(wsUrl);  // Reinitialize the WebSocket connection
// }

// // Modify the onClosed and onError to use attemptReconnect
// ws->onClosed([wsUrl]() {
//     isConnected.store(false);
//     APP_DBG("WebSocket closed unexpectedly. URL: %s\n", wsUrl.c_str());
//     attemptReconnect(wsUrl);
// });

// ws->onError([wsUrl](const string &error) {
//     isConnected.store(false);
//     APP_DBG("WebSocket error at URL %s: %s\n", wsUrl.c_str(), error.c_str());
//     attemptReconnect(wsUrl);
// });




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
        timer_remove_attr(GW_TASK_WEBRTC_ID, GW_WEBRTC_TRY_CONNECT_SOCKET_REQ);
    });

    globalWs->onClosed([&]() {
        isConnected.store(false);
        APP_DBG("[WEBSOCKET_ONCLOSED] has closed unexpectedly.\n");
        timer_set(GW_TASK_WEBRTC_ID, GW_WEBRTC_TRY_CONNECT_SOCKET_REQ, GW_WEBRTC_TRY_CONNECT_SOCKET_INTERVAL, TIMER_ONE_SHOT);
    });

    globalWs->onError([&](const string &error) {
        isConnected.store(false);
        APP_DBG("[WEBSOCKET_ERROR] : %s\n", error.c_str());
    });

	globalWs->onMessage([&](variant<binary, string> data) {
		APP_PRINT("WebSocket connection @ ws->onMessage\n");
		if (holds_alternative<string>(data)) {
			string msg = get<string>(data);
			APP_DBG("%s\n", msg.data());
			handleWebSocketMessage(msg, globalWs);  // Pass the message to the handler
			// If needed, you can post the message here as well, depending on your application's architecture.
		}
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

void handleWebSocketMessage(const std::string& message, std::shared_ptr<WebSocket> ws) {

    APP_DBG("[WebSocket Message Received]: %s\n", message.c_str());
    if (ws && ws ->isOpen()) {
        APP_DBG("safe to use\n");
    } else {
        APP_DBG("WebSocket is not open or not available.\n");
    }

    try {
        json messageJson = json::parse(message);
        APP_DBG("Parser: %s\n", messageJson.dump(4).c_str());

        auto typeIt = messageJson.find("Type");
        if (typeIt != messageJson.end()) {
            std::string type = typeIt->get<string>();

            if (type == "request") {
                auto clientIdIt = messageJson.find("ClientId");
                if (clientIdIt != messageJson.end()) {
                    std::string clientId = clientIdIt->get<string>();
                    APP_DBG("Handling request for Client ID: %s\n", clientId.c_str());
                    handleClientRequest(clientId, ws);
                } else {
                    APP_DBG("Error: ClientId not found in request message\n");
                }
            } else if (type == "candidate") {

            } else if (type == "answer") {

            } else {

            }
        } else {
            APP_DBG("Error: Message type not specified\n");
        }

    } catch (const json::exception& e) {
        APP_DBG("[JSON Parsing Error] %s\n", e.what());
    }
}

void handleClientRequest(const std::string& clientId, std::shared_ptr<WebSocket> ws) {
    APP_DBG("Initiating peer connection for Client ID: %s\n", clientId.c_str());
}