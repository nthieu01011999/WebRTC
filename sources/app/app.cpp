#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <iostream>

#include "ak.h"

#include "app.h"
#include "app_dbg.h"
#include "app_config.h"
#include "app_data.h"
#include "task_list.h"

CppTime::Timer systemTimer;

void exit_app(int ret);

void task_init() {
	signal(SIGINT, exit_app);
	signal(SIGQUIT, exit_app);
	signal(SIGTERM, exit_app);
	signal(SIGKILL, exit_app);
	
	std::cout << "Hello world" << std::endl;
}

void exit_app(int ret) {
	
	exit(ret);
}
