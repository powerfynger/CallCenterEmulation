#include "jsonParser.h"

QueueConfig readConfigFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        LOG(FATAL) << "Failed to open file: " << filename;
    }

    json jsonData;
    file >> jsonData;

    QueueConfig config;

    try {
        config.queueLength = jsonData["queueLength"];
        config.minProcessingTime = jsonData["timeRange"]["min"];
        config.maxProcessingTime = jsonData["timeRange"]["max"];
        config.processingDuration = jsonData["processingDuration"];
        config.maxOperatorsNum = jsonData["maxOperators"];
    } catch (const json::exception& e) {
        LOG(FATAL) << "Error parsing config file.";
    }

    return config;
}
