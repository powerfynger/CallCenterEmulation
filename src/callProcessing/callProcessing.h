#pragma once
#include <iostream>
#include <queue>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <poll.h>



#include "../jsonParser/jsonParser.h"
// #include "../HTTPServ/HTTPServ.h"

struct HttpRequest {
    std::string number;
    std::string callId; 
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
    std::queue<HttpRequest> callQueue; // Очередь вызовов
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

    void handleHttpRequest(const HttpRequest& request);
};