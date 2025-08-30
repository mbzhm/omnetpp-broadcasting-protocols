#include <unordered_set>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>  // For random probability
#include <omnetpp.h>

using namespace omnetpp;

class BidirectionalNode : public cSimpleModule {
private:
    std::set<int> receivedMessages;
    int routingMsgCount = 0;

    // Sets for managing neighbors, parents, siblings, and children
    std::unordered_set<std::string> NeighborSet;
    std::unordered_set<std::string> ParentSet;
    std::unordered_set<std::string> SiblingSet;
    std::unordered_set<std::string> ChildSet;

    std::map<std::string, double> latestPrequired;

    // Gossip probability threshold
    double gossipThreshold = 0.8;
    double tauRel = 0.8;
    double delta = 6;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    // Process incoming messages
    void handleMessageFromNode(cMessage *msg);

    // Add a node to the appropriate set based on the parent ID
    void updateSets(const std::string& sender, const std::string& parentId);

    // Calculate the gossip probability based on number of children and neighbors
    double calculateGossipProbability();
    double calculatePrequired(double tauRel, double delta);
    double calculatePgossip();

    // Broadcast a message to neighbors based on gossip probability
    void broadcastMessage(cMessage *msg);
    virtual void finish() override;
};

Define_Module(BidirectionalNode);

void BidirectionalNode::initialize() {
    std::string myName = getName();
    NeighborSet.insert(myName);  // Insert the node into its own NeighborSet
    SiblingSet.insert(myName);   // Insert the node into its own SiblingSet

    gossipThreshold = 0.8;
}

void BidirectionalNode::handleMessage(cMessage *msg) {
    int messageId = msg->getKind();

    simtime_t delay = simTime() - msg->getTimestamp();
    EV << "Node : Received message \"" << messageId << "\". Timestamp \"" << delay << "\".\n";

    handleMessageFromNode(msg);
}

// Handle the message and update the sets accordingly
void BidirectionalNode::handleMessageFromNode(cMessage *msg) {

    int messageId = msg->getKind();
    std::string sender = msg->par("source").stringValue();  // Node sending the message
    std::string parentId = msg->par("pid").stringValue();   // Parent ID from the message
    double prequired = calculatePrequired(tauRel, delta);

    // Update sets based on sender and parent ID
    updateSets(sender, parentId);

    // Track the latest prequired from each child
    if (ChildSet.find(sender) != ChildSet.end()) {
        latestPrequired[sender] = prequired;  // Update the latest prequired for this child
    }

    // Calculate gossip probability as the maximum prequired from children
    double pgossip = calculatePgossip();
    EV << "Calculated gossip probability: " << pgossip << "\n";


    // Decide whether to forward the message
    if (pgossip > uniform(0, 1) && receivedMessages.find(messageId) == receivedMessages.end()) {

        msg->par("pid").setStringValue(msg->par("source"));
        msg->par("source").setStringValue(getName());

        receivedMessages.insert(messageId);
        broadcastMessage(msg);
    } else {
        EV << "Node " << getName() << " dropped message.\n";
        delete msg;
    }
}

double BidirectionalNode::calculatePrequired(double tauRel, double delta) {
    int K = ParentSet.size();  // Number of parents
    if (K == 0) return 1.0;    // If no parents, require full reliability

    double base = pow(1 - tauRel, 1.0 / delta);  // Compute (1 - τrel)^(1/δ)
    return 1 - pow(base, 1.0 / K);              // Final prequired calculation
}

double BidirectionalNode::calculatePgossip() {
    double maxPrequired = 0.0;
    for (const auto& [child, prequired] : latestPrequired) {
        maxPrequired = std::max(maxPrequired, prequired);
    }
    return std::max(maxPrequired, gossipThreshold);
}


// Update the sets based on the parent ID
void BidirectionalNode::updateSets(const std::string& sender, const std::string& parentId) {

    NeighborSet.insert(sender);

    if (NeighborSet.find(parentId) == NeighborSet.end()) {

        ParentSet.insert(sender);
        EV << "Node " << getName() << " added " << sender << " to ParentSet.\n";

    } else if (ParentSet.find(parentId) != ParentSet.end()) {

        SiblingSet.insert(sender);
        EV << "Node " << getName() << " added " << sender << " to SiblingSet.\n";

    } else if (SiblingSet.find(parentId) != SiblingSet.end()) {

        ChildSet.insert(sender);
        EV << "Node " << getName() << " added " << sender << " to ChildSet.\n";
    }
}


void BidirectionalNode::broadcastMessage(cMessage *msg) {
    cModule *senderNode = msg->getSenderModule();

    for (int i = 0; i < gateSize("io$o"); ++i) {
       cMessage *copy = msg->dup(); // Duplicate the message for each connection
       routingMsgCount++;
       sendDelayed(copy, 1, "io$o", i);
    }
    delete msg;
}

void BidirectionalNode::finish() {
    // Output the total number of routing messages sent by the node
    EV << "Node " << getName() << " finished with " << routingMsgCount << " routing messages.\n";

    // Output the sizes of each set
    EV << "Node " << getName() << " statistics:\n";
    EV << "  - Number of Children: " << ChildSet.size() << "\n";
    EV << "  - Number of Parents: " << ParentSet.size() << "\n";
    EV << "  - Number of Siblings: " << SiblingSet.size() << "\n";
    EV << "  - Number of Neighbors: " << NeighborSet.size() << "\n";

    // Optionally, list the members of each set
    EV << "  - Children: ";
    for (const auto& child : ChildSet) {
        EV << child << " ";
    }
    EV << "\n";

    EV << "  - Parents: ";
    for (const auto& parent : ParentSet) {
        EV << parent << " ";
    }
    EV << "\n";

    EV << "  - Siblings: ";
    for (const auto& sibling : SiblingSet) {
        EV << sibling << " ";
    }
    EV << "\n";

    EV << "  - Neighbors: ";
    for (const auto& neighbor : NeighborSet) {
        EV << neighbor << " ";
    }
    EV << "\n";
}

