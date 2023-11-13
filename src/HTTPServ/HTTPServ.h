#pragma once

#include <cstring>
#include "../../include/mongoose.h"
#include "../jsonParser/jsonParser.h"
#include "../callProcessing/callProcessing.h"
#include "../CDRJournal/CDRJournal.h"



void runHTTPServ(QueueConfig& conf);