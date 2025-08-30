#include <omnetpp.h>

using namespace omnetpp;

class BidirectionalSourceNode : public cSimpleModule {
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(BidirectionalSourceNode);


void BidirectionalSourceNode::initialize() {
    simtime_t startTime = simTime();
    simtime_t interval = 20;

    int numExperiments = 10;

    for (int i = 0; i < numExperiments; ++i) {
        cMessage *selfmsg = new cMessage("ReminderToInit");
        selfmsg->setKind(i);
        scheduleAt(startTime + i * interval, selfmsg); // Schedule each message
    }
}

void BidirectionalSourceNode::handleMessage(cMessage *msg) {
    if (std::string(msg->getName()) == "ReminderToInit") {
        // Create a new DSR message
        cMessage *dsrMsg = new cMessage("DSRmessage");
        dsrMsg->setKind(2); // Kind for DSR messages
        dsrMsg->setTimestamp(simTime());
        dsrMsg->addPar("source") = getName();
        dsrMsg->addPar("destination") = "node32";
        dsrMsg->addPar("path") = getName(); // Initialize the path with the current node
        dsrMsg->addPar("payload") = "Hello!";

        // Send the DSR message
        sendDelayed(dsrMsg, 1, "io$o");
    }

    delete msg;
}
