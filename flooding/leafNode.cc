#include <omnetpp.h>

using namespace omnetpp;

class LeafNode : public cSimpleModule {
  private:
//    simsignal_t delaySignal;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(LeafNode);

void LeafNode::initialize(){
//    delaySignal = registerSignal("endToEndDelay");
}

void LeafNode::handleMessage(cMessage *msg) {

    int messageId = msg->getKind();
    simtime_t delay = simTime() - msg->getTimestamp();
//    emit(delaySignal, delay);

    EV << "LeafNode : Received message \"" << messageId << "\". Timestamp \"" << delay << "\".\n";
    delete msg;  // Since the leaf node doesn't forward the message, delete it to free memory.
}
