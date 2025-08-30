#include <omnetpp.h>

using namespace omnetpp;

class BidirectionalLeafNode : public cSimpleModule {
  protected:
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(BidirectionalLeafNode);

void BidirectionalLeafNode::handleMessage(cMessage *msg) {

    int messageId = msg->getKind();
    simtime_t delay = simTime() - msg->getTimestamp();

    EV << "LeafNode : Received message \"" << messageId << "\". Timestamp \"" << delay << "\".\n";
    delete msg;
}
