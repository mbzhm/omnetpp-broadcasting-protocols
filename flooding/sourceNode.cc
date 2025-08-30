#include <omnetpp.h>

using namespace omnetpp;

class SourceNode : public cSimpleModule {
//  private:
//    cMessage *sendMessageEvent; // Self-message to schedule message sending
//    int messageCount;          // Counter for the number of messages sent
//    simtime_t sendInterval;    // Interval between messages

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

//  public:
//    SourceNode();
//    virtual ~SourceNode();
};

Define_Module(SourceNode);

//SourceNode::SourceNode() {
//    sendMessageEvent = nullptr;
//}
//
//SourceNode::~SourceNode() {
//    cancelAndDelete(sendMessageEvent);
//}

void SourceNode::initialize() {
   simtime_t startTime = simTime();
   simtime_t interval = 5;

   int numExperiments = 10;

   for (int i = 0; i < numExperiments; ++i) {
     cMessage *selfmsg = new cMessage("ReminderToInit");
     selfmsg->setKind(i);
     scheduleAt(startTime + i * interval, selfmsg); // Schedule each message
   }
}

void SourceNode::handleMessage(cMessage *selfmsg) {
    int id = selfmsg->getKind();

    cMessage *msg = new cMessage("FloodingMessage");
    msg->setKind(id); // Assign a unique ID for tracking
    msg->setTimestamp(simTime());
    sendDelayed(msg, 1, "out"); // Send it to the first output gate with delay 1
}
