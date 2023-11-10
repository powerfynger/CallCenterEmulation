#define CONFIG_PATH "../config/conf.json"
#include <fstream>

#include "../../include/json.hpp"

using json = nlohmann::json;

struct QueueConfig {
    int queueLength;
    int minProcessingTime;
    int maxProcessingTime;
    int processingDuration;
};

QueueConfig readConfigFromFile(const std::string& filename);
