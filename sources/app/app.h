#ifndef __APP_H__
#define __APP_H__

#include <string>
#include "ak.h"


using namespace std;

/*****************************************************************************/
/* task GW_SYS define.
 */
/*****************************************************************************/
/* define timer */
#define GW_SYS_WATCH_DOG_TIMEOUT_INTERVAL		(120)	  // 120s watchdog
#define GW_SYS_WATCH_DOG_PING_TASK_REQ_INTERVAL (5000)	  // 5s reset watchdog
/* define signal */
enum {
	GW_SYS_WATCH_DOG_PING_NEXT_TASK_REQ = AK_USER_DEFINE_SIG,
	GW_SYS_WATCH_DOG_PING_NEXT_TASK_RES,
	GW_SYS_RESET_WATCH_DOG_REQ,
	GW_SYS_CLOUD_REBOOT_REQ,
	GW_SYS_REBOOT_REQ,
	GW_SYS_CMD_REBOOT_REQ,
	GW_SYS_START_WATCH_DOG_REQ,
	GW_SYS_STOP_WATCH_DOG_REQ,
	GW_SYS_SET_TELNET_REQ,
	GW_SYS_GET_TELNET_REQ,
	GW_SYS_GET_RTSP_INFO_REQ
};

/*****************************************************************************/
/*  task GW_CONSOLE define
 */
/*****************************************************************************/
/* define timer */

/* define signal */
enum {
	GW_CONSOLE_INTERNAL_LOGIN_CMD = AK_USER_DEFINE_SIG,
};

/*****************************************************************************/
/*  task GW_CLOUD define
 */
/*****************************************************************************/
/* define timer */
#define GW_WEB_MQTT_TRY_CONNECT_TIMEOUT_INTERVAL		  (5000)
#define GW_WEB_MQTT_CHECK_CONNECT_STATUS_INTERVAL		  (10000)
#define GW_WEB_MQTT_TRY_CONNECT_AFTER_DISCONNECT_INTERVAL (20000)

/* define signal */
enum {
	GW_CLOUD_MQTT_INIT_REQ	   = AK_USER_DEFINE_SIG,
	GW_CLOUD_MQTT_TRY_CONNECT_REQ,
	GW_CLOUD_MQTT_CHECK_CONNECT_STATUS_REQ,
	GW_CLOUD_DATA_COMMUNICATION,
	GW_CLOUD_SET_MQTT_CONFIG_REQ,
	GW_CLOUD_GET_MQTT_CONFIG_REQ,
	GW_CLOUD_CAMERA_STATUS_RES,
	GW_CLOUD_CAMERA_CONFIG_RES,
	GW_CLOUD_CAMERA_ALARM_RES,
	GW_CLOUD_SIGNALING_MQTT_RES,
	GW_CLOUD_MESSAGE_LENGTH_OUT_OF_RANGE_REP,
};

/*****************************************************************************/
/* task GW_AV define
 */
/*****************************************************************************/
/* define timer */
#define GW_AV_CHECK_VIDEO_STREAM_RUNNING_LONG_INTERVAL	(60000)
#define GW_AV_CHECK_VIDEO_STREAM_RUNNING_SHORT_INTERVAL (10000)
#define GW_AV_CHECK_CAPTURE_PROCESS_TIMEOUT_INTERVAL	(10000)
#define GW_AV_AUTO_CONTROL_DAYNIGHT_TIMEOUT_INTERVAL	(5000)
#define GW_AV_VIDEO_SET_ENCODE_TIMEOUT_INTERVAL			(5000)
#define GW_AV_OSD_UPDATE_TIME_TEXT_TIMEOUT_INTERVAL		(1000)

/* define signal */

enum {
	GW_AV_INIT_REQ			= AK_USER_DEFINE_SIG,
	GW_AV_SET_CAMERAPARAM_REQ,
	GW_AV_GET_CAMERAPARAM_REQ,
	GW_AV_SET_ENCODE_REQ,
	GW_AV_SET_ENCODE_REQ_TO,
	GW_AV_APPLY_NEW_ENCODE_REQ,
	GW_AV_GET_ENCODE_REQ,
	GW_AV_SET_WATERMARK_REQ,
	GW_AV_GET_WATERMARK_REQ,
	GW_AV_CHECK_CAPTURE_RUNNING_REQ,
	GW_AV_CHECK_VIDEO_STREAM_RUNNING_REQ,
	GW_AV_RELOAD_PICTURE_REQ,
	GW_AV_SET_ENCODE_STOP_VIDEO_MAIN_STREAM_RES,
	GW_AV_SET_ENCODE_STOP_VIDEO_SUB_STREAM_RES,
	GW_AV_SET_ENCODE_STOP_RTC_AUDIO_STREAM_RES,
	GW_AV_SET_ENCODE_STOP_RTMP_AUDIO_STREAM_RES,
};

/*****************************************************************************/
/* task GW_CONFIG define
 */
/*****************************************************************************/
/* define timer */
#define GW_CONFIG_SET_LIGHTING_STATUS_INTERVAL	(5000)
/* define signal */

enum {
	// SET
	GW_CONFIG_SET_CONFIG		= AK_USER_DEFINE_SIG,
	GW_CONFIG_SET_RESET_CONFIG_REQ,
	GW_CONFIG_SET_STORAGE_FORMAT_CONFIG_REQ,
	GW_CONFIG_SET_UPGRADE_REQ,
	GW_CONFIG_SET_UPGRADE_STATUS_REQ,
	GW_CONFIG_SET_REBOOT_REQ,
	GW_CONFIG_SET_RECORD_CONFIG_REQ,
	GW_CONFIG_SET_OWNER_CONFIG_REQ,
	GW_CONFIG_SET_USER_STEP_MOTOR_REQ,
	GW_CONFIG_SET_LIGHTING_STATUS,
	// GET
	GW_CONFIG_GET_RESET_CONFIG_REQ,
	GW_CONFIG_GET_STORAGE_INFO_CONFIG_REQ,
	GW_CONFIG_GET_WIFI_CONFIG_REQ,
	GW_CONFIG_GET_SYSINFO_CONFIG_REQ,
	GW_CONFIG_GET_RECORD_CONFIG_REQ,
	GW_CONFIG_GET_NETAP_CONFIG_REQ,
	GW_CONFIG_CLOUD_SET_LED_INDICATOR_REQ,
	GW_CONFIG_CLOUD_GET_LED_INDICATOR_REQ
};

/*****************************************************************************/
/* task GW_DETECT define
 */
/*****************************************************************************/
/* define timer */
#define GW_DETECT_INIT_TIMEOUT_INTERVAL				 (7000)
#define GW_DETECT_LIGHT_ALERT_BLINKING_INTERVAL		 (1000)
#define GW_DETECT_HANDLING_ALARM_INTERVAL			 (300)
/* define signal */

enum {
	GW_DETECT_INIT_REQ			= AK_USER_DEFINE_SIG,
	GW_DETECT_SET_MOTION_CONFIG_REQ,
	GW_DETECT_GET_MOTION_CONFIG_REQ,
	GW_DETECT_SET_ALARM_CONTROL_REQ,
	GW_DETECT_GET_ALARM_CONTROL_REQ,
	GW_DETECT_SNAP_JPEG_REQ,
	GW_DETECT_ALARM_HUMAN_DETECTED,
	GW_DETECT_ALARM_MOTION_DETECTED,
	GW_DETECT_HANDLING_ALARM,
	GW_DETECT_INTERVAL_ALARM_TIMEOUT,
	GW_DETECT_DURATION_ALARM_TIMEOUT,
	GW_DETECT_START_DETECT_REQ,
	GW_DETECT_RECORDING_MP4_COMPLETED,
	GW_DETECT_PERIODIC_ALERT_LIGHTING,
};

/*****************************************************************************/
/* task GW_LED define
 */
/*****************************************************************************/
/* define timer */
#define GW_LED_BLINK_1S_INTERVAL	(10000)
#define GW_LED_BLINK_500MS_INTERVAL (5000)
/* define signal */

enum {
	GW_LED_WHITE_BLINK		 = AK_USER_DEFINE_SIG,
	GW_LED_WHITE_ON,
	GW_LED_WHITE_OFF,
	GW_LED_ORANGE_BLINK,
	GW_LED_ORANGE_ON,
	GW_LED_ORANGE_OFF,
};

/*****************************************************************************/
/* task GW_RECORD define
 */
/*****************************************************************************/
/* define timer */
#define GW_RECORD_TRY_MOUNTING_INTERVAL						 (5000)
#define GW_RECORD_PERIOD_QUERY_NEXT_DAY_UPTIME_INTERVAL		 (60000)
#define GW_RECORD_CLOSE_SESSION_RECORDING_MOTION_INTERVAL	 (15000)
#define GW_RECORD_OPEN_RECORD_MP4_BUFFER_INTERVAL			 (3000)
/* define signal */

enum {
	GW_RECORD_TRY_MOUNTING		= AK_USER_DEFINE_SIG,
	GW_RECORD_GET_RECORD_REQ,
	GW_RECORD_SET_RECORD_REQ,
	GW_RECORD_OPEN_SESSION_RECORDING_FULL,
	GW_RECORD_CLOSE_SESSION_RECORDING_FULL,
	GW_RECORD_OPEN_SESSION_RECORDING_MOTION,
	GW_RECORD_CLOSE_SESSION_RECORDING_MOTION,
	GW_RECORD_PERIOD_QUERY_NEXT_DAY_UPTIME,
	GW_RECORD_FORMAT_SDCARD_REQ,
	GW_RECORD_GET_CAPACITY_SDCARD_REQ,
	GW_RECORD_HANDLE_STORAGE_FAILED,
	GW_RECORD_OPEN_RECORD_MP4_RESERVE,
	GW_RECORD_CLOSE_RECORD_MP4_RESERVE,
};

/*****************************************************************************/
/* task GW_UPLOAD define
 */
/*****************************************************************************/
/* define timer */
/* define signal */

enum {
	GW_UPLOAD_INIT_REQ			= AK_USER_DEFINE_SIG,
	GW_UPLOAD_SET_S3_CONFIG_REQ,
	GW_UPLOAD_GET_S3_CONFIG_REQ,
	GW_UPLOAD_FILE_JPEG_REQ,
	GW_UPLOAD_FILE_MP4_REQ,
};

/*****************************************************************************/
/* task GW_NETWORK define
 */
/*****************************************************************************/
/* define timer */
#define GW_NET_CHECK_CONNECT_ROUTER_INTERVAL (1000) /* 1s */
#define GW_NET_CHECK_STATE_PLUG_LAN_INTERVAL (500)	/* 500ms */

/* define signal */
enum {
	/* wifi station interface */
	GW_NET_WIFI_INIT_REQ	 = AK_USER_DEFINE_SIG,

	GW_NET_WIFI_MQTT_SET_CONF_REQ,
	GW_NET_WIFI_MQTT_GET_CONF_REQ,
	GW_NET_WIFI_MQTT_GET_LIST_AP_REQ,
	GW_NET_MQTT_GET_NETWORK_INFO_REQ,

	GW_NET_WIFI_STA_SET_CONF_REQ,
	GW_NET_WIFI_STA_START_REQ,
	GW_NET_WIFI_AP_START_REQ,

	GW_NET_WIFI_STA_CONNECT_TO_ROUTER,
	GW_NET_WIFI_STA_CONNECTED_TO_ROUTER,

	GW_NET_INTERNET_STATUS_REQ,
	GW_NET_GET_STATE_LAN_REQ,

	GW_NET_RESET_WIFI_REQ,
	GW_NET_PLAY_VOICE_NETWORK_CONNECTED_REQ,
};

/*****************************************************************************/
/* task GW_FW_TASK define
 */
/*****************************************************************************/
/* define timer */
#define GW_FW_UPGRADE_FIRMWARE_TIMEOUT_INTERVAL (300000) /* 5min */

/* define signal */
enum {
	/* wifi station interface */
	GW_FW_GET_URL_REQ		= AK_USER_DEFINE_SIG,
	GW_FW_DOWNLOAD_START_REQ,
	GW_FW_UPGRADE_START_REQ,
	GW_FW_RELEASE_OTA_STATE_REQ,
};

/*****************************************************************************/
/*  task GW_TASK_WEBRTC define
 */
/*****************************************************************************/
/* define timer */
#define GW_WEBRTC_ERASE_CLIENT_NO_ANSWER_TIMEOUT_INTERVAL (40000) /* 40s */
#define GW_WEBRTC_TRY_CONNECT_SOCKET_INTERVAL			  (10000) /* 10s */
#define GW_WEBRTC_TRY_GET_EXTERNAL_IP_INTERVAL			  (7000)  /* 7s */
#define GW_WEBRTC_WAIT_REQUEST_TIMEOUT_INTERVAL			  (20000) /* 20s */
#define GW_WEBRTC_CLIENT_SEND_PING_INTERVAL				  (10000) /* 10s */
#define GW_WEBRTC_ERASE_CLIENT_PING_PONG_TIMEOUT_INTERVAL (20000) /* 20s */
#define GW_WEBRTC_RELEASE_CLIENT_PUSH_TO_TALK_INTERVAL	  (2500)  /* 2.5s */

/* define signal */
enum {
	GW_WEBRTC_SIGNALING_MQTT_REQ = AK_USER_DEFINE_SIG,
	GW_WEBRTC_SET_SIGNLING_SERVER_REQ,
	GW_WEBRTC_GET_SIGNLING_SERVER_REQ,
	GW_WEBRTC_CHECK_CLIENT_CONNECTED_REQ,
	GW_WEBRTC_ERASE_CLIENT_REQ,
	GW_WEBRTC_DBG_IPC_SEND_MESSAGE_REQ,
	GW_WEBRTC_ON_MESSAGE_CONTROL_DATACHANNEL_REQ,
	GW_WEBRTC_DATACHANNEL_DOWNLOAD_RELEASE_REQ,
	GW_WEBRTC_RELEASE_CLIENT_PUSH_TO_TALK,
	GW_WEBRTC_TRY_GET_STUN_EXTERNAL_IP_REQ,
};

/*****************************************************************************/
/*  task GW_TASK_RTMP define
 */
/*****************************************************************************/
/* define timer */
#define GW_RTMP_RECONNECT_INTERVAL (30000) /* 30s */

/* define signal */
enum {
	GW_RTMP_START_REQ		  = AK_USER_DEFINE_SIG,
	GW_RTMP_STOP_REQ,
	GW_RTMP_SET_CONFIG_REQ,
	GW_RTMP_GET_CONFIG_REQ,
	GW_RTMP_LOAD_RTMP_CONTROL_REQ,
};

/*****************************************************************************/
/*  global define variable
 */
/*****************************************************************************/
#define APP_OK (0x00)
#define APP_NG (0x01)

#define APP_FLAG_OFF (0x00)
#define APP_FLAG_ON	 (0x01)

#endif	  // __APP_H__
