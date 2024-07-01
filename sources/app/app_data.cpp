#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/resource.h>
#include <iostream>

#include "ak.h"

#include "rtc/rtc.hpp"
#include "task_list.h"
#include "app_config.h"
#include "app_data.h"
// #include "h26xsource.hpp"



// extern VideoCtrl videoCtrl;
// static pthread_mutex_t mtxStreamVideo = PTHREAD_MUTEX_INITIALIZER;
// static pthread_mutex_t mtxListClients = PTHREAD_MUTEX_INITIALIZER;
// optional<shared_ptr<Stream>> avStream;

ClientsGroup_t clients;