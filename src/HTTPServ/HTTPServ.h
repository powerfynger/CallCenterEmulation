#ifndef HTTPSERV_H
#define HTTPSERV_H

#include <cstring>
#include "../utils/utils.h"
#include "../../include/easylogging++.h"
#include "../../include/mongoose.h"
#include "../jsonParser/jsonParser.h"
#include "../callProcessing/callProcessing.h"

void runHTTPServ(QueueConfig& conf, bool writeLogs);

#endif