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

void BidirectionalSourceNode::handleMessage(cMessage *selfmsg) {
    int id = selfmsg->getKind();

    cMessage *msg = new cMessage("FloodingMessage");
    msg->setKind(id); // Assign a unique ID for tracking
    msg->setTimestamp(simTime());
    sendDelayed(msg, 1, "io$o"); // Send it to the first output gate with delay 1
}
