#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <algorithm>


#include "../utils/utils.h"
#include "../jsonParser/jsonParser.h"
// #include "../HTTPServ/HTTPServ.h"

struct HttpRequest {
    std::string number;
    std::string callId;
    std::chrono::time_point<std::chrono::steady_clock> timeOut;

    bool operator==(const HttpRequest& other) const {
        return other.callId == callId;
    }
};

enum OperatorStatus {
    FREE,
    BUSY
};

class Operator {
private:
    int operatorId;
    OperatorStatus status;

public:
    int processingTime;

    Operator(int id, int procTime) : operatorId(id), processingTime(procTime), status(FREE) {}

    void processCall(const HttpRequest& request);
    int getOperatorId();
    OperatorStatus getOperatorStatus();
};

class CallCenter {
private:
    std::vector<HttpRequest> callQueue; // Очередь вызовов
    std::vector<Operator> operators; // Список операторов
    QueueConfig conf;
    int findFreeOperator();

public:
    CallCenter(QueueConfig& conf)
        : conf(conf){
        for (int i = 1; i <= conf.maxOperatorsNum; ++i) {
            operators.push_back(Operator(i, conf.processingDuration));
        }
    }
    CallCenter() {}


    bool handleHttpRequest(HttpRequest& request);
    void addToQueue(HttpRequest& request);
    void processQueue();
    HttpRequest findHttpRequestByCallId(const std::string& targetCallId);
};