#include <gtest/gtest.h>
#include "../src/CDRJournal/CDRJournal.h"
#include "../src/callProcessing/callProcessing.h"
#include "../src/HTTPServ/HTTPServ.h"
#include "../include/easylogging++.h"

INITIALIZE_EASYLOGGINGPP

class CallCenterTest : public ::testing::Test {
protected:
    QueueConfig _conf;
    CallCenter callCenter;

    void SetUp() override {
        ::testing::Test::SetUp();
    }

    void InitializeCallCenter(unsigned int queLen, unsigned int procDur, unsigned int maxOps) {
        _conf.maxOperatorsNum = maxOps;
        _conf.maxProcessingTime = procDur;
        _conf.minProcessingTime = procDur;
        _conf.queueLength = queLen;
        CallCenter tmp(_conf, false);
        callCenter = tmp;
    }
};

TEST_F(CallCenterTest, QueueSizeInitiallyZero) {
    InitializeCallCenter(0, 0, 0);
    
    EXPECT_EQ(callCenter.getQueueSize(), 0);
}

TEST_F(CallCenterTest, AddOperatorIncreasesOperatorsSize) {
    InitializeCallCenter(0, 0, 5);
    Operator op(0, 5, false);
    
    callCenter.addOperator(op);
    
    EXPECT_EQ(callCenter.findFreeOperator(), 0);
}

TEST_F(CallCenterTest, AddToQueueIncreasesQueueSize) {
    InitializeCallCenter(5, 0, 0);
    HttpRequest request;
    
    callCenter.addToQueue(request);
    
    EXPECT_EQ(callCenter.getQueueSize(), 1);
}

TEST_F(CallCenterTest, DeleteFromQueueDecreasesQueueSize) {
    InitializeCallCenter(5, 0, 0);
    HttpRequest request;
    // Need to set number due to comparison implementation between requests
    request.number = "123321";
    
    callCenter.addToQueue(request);
    callCenter.deleteFromQueue(request);
    
    EXPECT_EQ(callCenter.getQueueSize(), 0);
}

TEST_F(CallCenterTest, FindFreeOperatorReturnsCorrectIndex) {
    InitializeCallCenter(5, 1, 2);

    EXPECT_EQ(callCenter.findFreeOperator(), 0);
    
    HttpRequest request;
    
    callCenter.addToQueue(request);
    callCenter.processQueue();
    
    EXPECT_EQ(callCenter.findFreeOperator(), 1);
}

TEST_F(CallCenterTest, FindFreeOperatorAllBusy){
    InitializeCallCenter(0, 5, 1);
    HttpRequest request;
    
    callCenter.addToQueue(request);
    callCenter.processQueue();
    
    EXPECT_EQ(callCenter.findFreeOperator(), -1);
}

TEST_F(CallCenterTest, HandleHttpRequestNormal){
    InitializeCallCenter(1, 5, 1);
    HttpRequest request;
    
    HttpStatusCode code = callCenter.handleHttpRequest(request);
    
    EXPECT_EQ(code, HttpStatusCode::OK);
}

TEST_F(CallCenterTest, HandleHttpRequestQueueFull){
    InitializeCallCenter(0, 5, 1);
    HttpRequest request;
    
    HttpStatusCode code = callCenter.handleHttpRequest(request);
    
    EXPECT_EQ(code, HttpStatusCode::SERVICE_UNAVALIABLE);
}

TEST_F(CallCenterTest, HandleHttpRequestDuplicate){
    InitializeCallCenter(2, 5, 0);
    HttpRequest request;
    // Need to set number due to comparison implementation between requests
    request.number = "123321";

    callCenter.handleHttpRequest(request);
    HttpStatusCode code = callCenter.handleHttpRequest(request);
    
    EXPECT_EQ(code, HttpStatusCode::TOO_MANY_REQUESTS);
}

TEST_F(CallCenterTest, abpba){
    InitializeCallCenter(2, 5, 1);
    HttpRequest request;
    // Need to set number due to comparison implementation between requests
    request.number = "123321";

    callCenter.handleHttpRequest(request);

    
    EXPECT_EQ(request.record.operatorId, 0);
    EXPECT_EQ(request.record.operatorResponseTime, std::chrono::system_clock::now());
    EXPECT_EQ(request.record.duration, 5);
    EXPECT_EQ(request.record.status, static_cast<int>(HttpStatusCode::OK));
}

