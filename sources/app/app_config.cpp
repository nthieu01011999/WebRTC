#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex>
#include <string.h>

#include <iostream>
#include <cstring>

using namespace std;

#include "utils.h"

#include "app.h"
#include "app_data.h"
#include "app_config.h"

#include "app_dbg.h"
// #include "app_config.h"
#include "parser_json.h"
// #include "vvtk_system.h"
// #include "ota.h"
/******************************************************************************
 * APP CONFIGURE CLASS (BASE)
 ******************************************************************************/

#include <vector>

// int configGetRtcServers(rtcServersConfig_t *rtcServerCfg) {
// 	json cfgJs;

// 	string fileName = "RtcServersConfig";
// 	if (mtce_readConfigUsrDfaulFileToJs(cfgJs, fileName)) {
// 		return APP_CONFIG_ERROR_FILE_OPEN;
// 	}

// 	if (mtce_jsonGetRtcServers(cfgJs, rtcServerCfg)) {
// 		APP_DBG("rtc server config: %s\n", cfgJs.dump().data());
// 		return APP_CONFIG_SUCCESS;
// 	}
// 	else {
// 		APP_DBG("Can not convert: %s\n", fileName.data());
// 		return APP_CONFIG_ERROR_DATA_INVALID;
// 	}
// }

// int mtce_readConfigUsrDfaulFileToJs(json &cfgJs, string &file) {
// 	string filePath = "/usr/conf" + string("/") + file;
// 	if (!read_json_file(cfgJs, filePath)) {
// 		APP_DBG("Can not read: %s\n", filePath.data());
// 		filePath = "/app/default" + string("/") + file;
// 		cfgJs.clear();
// 		if (!read_json_file(cfgJs, filePath)) {
// 			APP_DBG("Can not read: %s\n", filePath.data());
// 			return APP_CONFIG_ERROR_FILE_OPEN;
// 		}
// 	}
// 	return APP_CONFIG_SUCCESS;
// }
 
int configGetRtcServers(rtcServersConfig_t *rtcServerCfg) {
    // Hardcoded JSON content
    std::string json_string = R"({
        "Connections": [
            {
                "Type": "http",
                "HttpUrl": "https://camera-http.ftel.vn"
            },
            {
                "Type": "signaling",
                "SignalingUrl": "ws://sig.espitek.com:8089"
            },
            {
                "Type": "stun",
                "StunUrl": ["stun.l.google.com:19302"]
            },
            {
                "Type": "turn",
                "TurnUrl": [
                    "turn:turnuser:camfptvnturn133099@stun-connect.fcam.vn:3478",
                    "turn:turnuser:camfptvnturn133099@stunp-connect.fcam.vn:3478"
                ]
            }
        ]
    })";

    // Parse the JSON string into a json object
    json cfgJs = json::parse(json_string);

    // Check if the JSON has the required "Connections" key
    if (!cfgJs.contains("Connections") || !cfgJs["Connections"].is_array()) {
        APP_DBG("Invalid JSON format: 'Connections' key missing or not an array.\n");
        return APP_CONFIG_ERROR_DATA_INVALID;
    }

    // Loop through each connection and populate the rtcServersConfig_t struct
    for (const auto& connection : cfgJs["Connections"]) {
        std::string type = connection["Type"];
        if (type == "signaling" && connection.contains("SignalingUrl")) {
            rtcServerCfg->wSocketServerCfg = connection["SignalingUrl"];
        } else if (type == "stun" && connection.contains("StunUrl")) {
            rtcServerCfg->arrStunServerUrl = connection["StunUrl"].get<std::vector<std::string>>();
        } else if (type == "turn" && connection.contains("TurnUrl")) {
            rtcServerCfg->arrTurnServerUrl = connection["TurnUrl"].get<std::vector<std::string>>();
        }
    }

    // Debugging print to show what has been loaded
    APP_DBG("rtc server config: %s\n", cfgJs.dump().data());
    return APP_CONFIG_SUCCESS;
}
