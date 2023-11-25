#ifndef CALLPROCESSING_H
#define CALLPROCESSING_H

#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <algorithm>
#include <iomanip>

#include "../../include/easylogging++.h"
#include "../CDRJournal/CDRJournal.h"
#include "../jsonParser/jsonParser.h"

struct HttpRequest {
    std::string number;
    std::string callId;
    std::chrono::time_point<std::chrono::steady_clock> timeOut;
    CallDetailRecord record;

    bool operator==(const HttpRequest& other) const {
        return (other.number == number);
    }
};

enum OperatorStatus {
    FREE,
    BUSY
};

enum HttpStatusCode {
    OK = 200,
    BAD_REQUEST = 400,
    REQUEST_TIMEOUT = 408,
    TOO_MANY_REQUESTS = 429,
    INTERNAL_SERVER_ERROR = 500,
    SERVICE_UNAVALIABLE = 503
};

class Operator {
private:
    int _operatorId;
    bool _writeLogs;
    OperatorStatus _status;

public:
    int processingTime;

    Operator(int id, int procTime, bool writeLogs) : _operatorId(id), processingTime(procTime), _status(FREE), _writeLogs(writeLogs) {}

    void processCall(HttpRequest& request);
    int getOperatorId();
    OperatorStatus getOperatorStatus();
};

class CallCenter {
private:
    std::vector<HttpRequest> _callQueue;
    std::vector<Operator> _operators;
    QueueConfig _conf;
    bool _writeLogs;

public:
    CallCenter(QueueConfig& conf, bool writeLogs)
        : _conf(conf), _writeLogs(writeLogs){
        for (int i = 1; i <= conf.maxOperatorsNum; ++i) {
            _operators.push_back(Operator(i, conf.processingDuration, writeLogs));
        }
    }
    CallCenter() {}

    void deleteFromQueue(HttpRequest& request);

    HttpStatusCode handleHttpRequest(HttpRequest& request);
    void addToQueue(HttpRequest& request);
    unsigned int getQueueSize();
    void processQueue();
    void addOperator(Operator& op);
    int findFreeOperator();
    HttpRequest findHttpRequestInQueue(HttpRequest& targetRequest);
};
#endif