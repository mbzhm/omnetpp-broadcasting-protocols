#include <omnetpp.h>

using namespace omnetpp;

class Node : public cSimpleModule
{
  private:
    std::set<int> receivedMessages; // Keep track of received message IDs to avoid duplicate flooding
    int routingMsgCount;  // Count of forwarded messages
    simsignal_t routingSignal;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};

Define_Module(Node);

void Node::initialize()
{
    routingMsgCount = 0;
    routingSignal = registerSignal("routingOverhead");
}

void Node::handleMessage(cMessage *msg)
{
    int messageId = msg->getKind();

    // If this message ID has already been processed, drop it
    if (receivedMessages.find(messageId) != receivedMessages.end()) {
        delete msg;
        return;
    }

    receivedMessages.insert(messageId);

    for (int i = 0; i < gateSize("out"); ++i) {
        cMessage *copy = msg->dup(); // Duplicate the message for each connection
        routingMsgCount++;
        sendDelayed(copy, 1, "out", i);
    }

    simtime_t delay = simTime() - msg->getTimestamp();

    EV << "Node : Received message \"" << messageId << "\". Timestamp \"" << delay << "\".\n";
    delete msg;
}

void Node::finish(){
    EV << "Node : Transmissions \"" << routingMsgCount << "\".\n";
    emit(routingSignal, routingMsgCount);
}
