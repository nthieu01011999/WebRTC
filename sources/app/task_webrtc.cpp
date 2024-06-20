#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "ak.h"
#include "timer.h"

#include "app.h"
#include "app_dbg.h"
#include "app_data.h"

#include "task_list.h"
#include "task_webrtc.h"

q_msg_t gw_task_webrtc_mailbox;
void printout() {
    std::cout << "[after] gw_task_webrtc_entry\n" << std::endl;
}
void *gw_task_webrtc_entry(void *) {
	
	// std::cout << "[STARTED] gw_task_webrtc_entry\n" << std::endl;
    APP_DBG("[STARTED] gw_task_webrtc_entry\n");
	wait_all_tasks_started();
    ak_msg_t *msg = AK_MSG_NULL;

	
     printout();
     
    while (1) {
        /* Get message */
        msg = ak_msg_rev(GW_TASK_WEBRTC_ID);
        if (msg == AK_MSG_NULL) {
            APP_DBG_SIG("[ERROR] No message received\n");  
        } else {
            APP_DBG_SIG("[DEBUG] Message received\n");   
        }

        switch (msg->header->sig) {

        default:
            APP_DBG_SIG("[DEBUG] Unknown message signal received: %d\n", msg->header->sig);   
            break;
        }

        /* Free message */
        ak_msg_free(msg);
    }

    return (void *)0;
}
