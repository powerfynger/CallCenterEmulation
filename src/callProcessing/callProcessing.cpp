#include "callProcessing.h"

int Operator::getOperatorId()
{
    return this->_operatorId;
}
OperatorStatus Operator::getOperatorStatus()
{
    return this->_status;
}

void Operator::processCall(HttpRequest &request)
{
    _status = BUSY;
    // std::cout << "Operator " << operatorId << " is processing call with CallID " << request.callId << std::endl;
    LOG_IF(_writeLogs, INFO) << "Operator " << _operatorId << " is processing call with CallID " << request.callId;

    auto sharedRequest = std::make_shared<HttpRequest>(request);

    std::thread([this, sharedRequest]()
                {
        std::this_thread::sleep_for(std::chrono::seconds(processingTime));
        // std::cout << "Operator " << operatorId << " finished processing call with CallID " << sharedRequest->callId << std::endl;
        LOG_IF(_writeLogs, INFO) << "Operator " << _operatorId << " finished processing call with CallID " << sharedRequest->callId;
        sharedRequest->record.operatorId = _operatorId;
        sharedRequest->record.operatorResponseTime = std::chrono::system_clock::now();
        sharedRequest->record.duration = processingTime;
        sharedRequest->record.setStatus(static_cast<int>(OK));
        sharedRequest->record.writeCDRToFile();

        _status = FREE; })
        .detach();
}

void CallCenter::addOperator(Operator &op)
{
    _operators.push_back(op);
}

HttpRequest CallCenter::findHttpRequestInQueue(HttpRequest &targetRequest)
{
    for (const auto &request : _callQueue)
    {
        if (request == targetRequest)
        {
            return request;
        }
    }
    return HttpRequest{};
}

unsigned int CallCenter::getQueueSize()
{
    return _callQueue.size();
}

int CallCenter::findFreeOperator()
{
    for (size_t i = 0; i < _operators.size(); ++i)
    {
        if (_operators[i].getOperatorStatus() == FREE)
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void CallCenter::addToQueue(HttpRequest &request)
{
    auto now = std::chrono::steady_clock::now();
    int holdTime = std::rand() % (_conf.maxProcessingTime - _conf.minProcessingTime + 1) + _conf.minProcessingTime;
    auto endTime = now + std::chrono::seconds(holdTime);

    request.timeOut = endTime;

    _callQueue.push_back(request);
}

void CallCenter::deleteFromQueue(HttpRequest& request)
{
    HttpRequest requestToDelete = findHttpRequestInQueue(request);
    if (!requestToDelete.number.empty())
    {
        auto it = std::find(_callQueue.begin(), _callQueue.end(), request);
        _callQueue.erase(it);
    }
}


void CallCenter::processQueue()
{
    auto now = std::chrono::steady_clock::now();
    bool canProcessCall = false;
    for (auto &request : _callQueue)
    {
        if (now > request.timeOut)
        {
            auto it = std::find(_callQueue.begin(), _callQueue.end(), request);
            _callQueue.erase(it);

            // std::cout << "Call with CallID " << request.callId << " ended with timeout" << std::endl;
            LOG_IF(_writeLogs, INFO) << "Call with CallID " << request.callId << " ended with timeout";

            // CDR
            request.record.number = request.number;
            request.record.setStatus(static_cast<int>(REQUEST_TIMEOUT));
            request.record.writeCDRToFile();
            continue;
        }
        for (auto &op : _operators)
        {
            if (op.getOperatorStatus() == BUSY)
                continue;
            canProcessCall = true;
            auto it = std::find(_callQueue.begin(), _callQueue.end(), request);
            _callQueue.erase(it);
            op.processCall(request);
            break;
        }
        if (canProcessCall == false)
            break;
    }
}

HttpStatusCode CallCenter::handleHttpRequest(HttpRequest &request)
{
    request.record.endTime = std::chrono::system_clock::now();
    HttpRequest duplicate = findHttpRequestInQueue(request);
    if (!duplicate.number.empty())
    {
        request = duplicate;
        return TOO_MANY_REQUESTS;
    }
    if (_callQueue.size() < _conf.queueLength)
    {

        // std::cout << "HTTP response sent for CallID " << request.callId << std::endl;
        LOG_IF(_writeLogs, INFO) << "HTTP response sent for CallID " << request.callId;

        int freeOperatorIndex = findFreeOperator();
        if (freeOperatorIndex != -1)
        {
            addToQueue(request);
            processQueue();
            return OK;
        }
        addToQueue(request);
        return OK;
    }
    else
    {
        // std::cout << "HTTP response sent for CallID " << request.callId << " (Queue is full)" << std::endl;
        LOG_IF(_writeLogs, INFO) << "HTTP response sent for CallID " << request.callId << " (Queue is full)";

        return SERVICE_UNAVALIABLE;
    }
}