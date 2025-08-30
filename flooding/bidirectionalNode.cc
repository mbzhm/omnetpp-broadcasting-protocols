#include <omnetpp.h>

using namespace omnetpp;

class BidirectionalNode : public cSimpleModule
{
  private:
    std::set<int> receivedMessages; // Keep track of received message IDs to avoid duplicate flooding
    int routingMsgCount;  // Count of forwarded messages

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};

Define_Module(BidirectionalNode);

void BidirectionalNode::initialize()
{
    routingMsgCount = 0;
    routingSignal = registerSignal("routingOverhead");
}

void BidirectionalNode::handleMessage(cMessage *msg)
{
    int messageId = msg->getKind();

    // If this message ID has already been processed, drop it
    if (receivedMessages.find(messageId) != receivedMessages.end()) {
        delete msg;
        return;
    }

    receivedMessages.insert(messageId);

    cModule *senderNode = msg->getSenderModule();

    for (int i = 0; i < gateSize("io$o"); ++i) {

        cGate *outGate = gate("io$o", i); // Get the current output gate
        cGate *connectedGate = outGate->getNextGate(); // Get the gate on the connected module
        cModule *connectedNode = connectedGate->getOwnerModule(); // Get the module owning the connected gate

        if (connectedNode == senderNode) {
           continue; // Skip the node that sent the message
        }


        cMessage *copy = msg->dup(); // Duplicate the message for each connection
        routingMsgCount++;
        sendDelayed(copy, 1, "io$o", i);
    }

    simtime_t delay = simTime() - msg->getTimestamp();

    EV << "Node : Received message \"" << messageId << "\". Timestamp \"" << delay << "\".\n";
    delete msg;
}

void BidirectionalNode::finish(){
    EV << "Node : Transmissions \"" << routingMsgCount << "\".\n";
    emit(routingSignal, routingMsgCount);
}
