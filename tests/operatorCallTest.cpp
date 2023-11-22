#include <gtest/gtest.h>
// #include <gmock/gmock.h>
#include "../src/callProcessing/callProcessing.h"
#include "../src/HTTPServ/HTTPServ.h"
#include "../include/easylogging++.h"

INITIALIZE_EASYLOGGINGPP

class CallCenterTest : public ::testing::Test {
private:
    QueueConfig _conf;
public:
    CallCenter& callCenter;
    CallCenterTest();
    void setUp(unsigned int queLen, unsigned int procDur, unsigned int maxOps){
        _conf.maxOperatorsNum = maxOps;
        _conf.maxProcessingTime = procDur;
        _conf.minProcessingTime = procDur;
        _conf.queueLength = queLen;
        CallCenter tmp(_conf, true);
        callCenter = tmp;
    }

};

TEST_F(CallCenterTest, QueueSizeInitiallyZero) {
    setUp(0, 0, 0);
    EXPECT_EQ(callCenter.getQueueSize(), 0);
}

TEST_F(CallCenterTest, AddOperatorIncreasesOperatorsSize) {
    setUp(0, 0, 5);
    Operator op(0, 5, true);
    callCenter.addOperator(op);
    EXPECT_EQ(callCenter.findFreeOperator(), 0);
}

TEST_F(CallCenterTest, AddToQueueIncreasesQueueSize) {
    setUp(5, 0, 0);
    HttpRequest request;
    callCenter.addToQueue(request);
    EXPECT_EQ(callCenter.getQueueSize(), 1);
}

TEST_F(CallCenterTest, FindFreeOperatorReturnsCorrectIndex) {
    setUp(5, 1, 2);
    Operator opFirst(0, 5, true), opSecond(1, 5, true);

    EXPECT_EQ(callCenter.findFreeOperator(), 0);
    HttpRequest request;
    callCenter.addToQueue(request);
    callCenter.processQueue();
    EXPECT_EQ(callCenter.findFreeOperator(), 1);
}

