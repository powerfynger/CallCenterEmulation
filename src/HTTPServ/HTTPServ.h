#pragma once

#include "../jsonParser/jsonParser.h"
#include "../../include/mongoose.h"
#include "../callProcessing/callProcessing.h"
#include <cstring>



void runHTTPServ(QueueConfig& conf);