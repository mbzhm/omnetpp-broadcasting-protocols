#include <omnetpp.h>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <algorithm>

using namespace omnetpp;

class BidirectionalNode : public cSimpleModule {
private:
    std::map<std::string, std::vector<std::string>> routingTable; // Routing table: destination -> route
    std::set<std::string> knownMessages;                         // Avoid processing duplicate RREQs
    int routingMsgCount = 0;                                     // Count of routing messages processed

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    // DSR-specific methods
    void sendRouteRequest(const std::string &destination);
    void handleRouteRequest(cMessage *rreq);
    void sendRouteReply(const std::string &destination, const std::string &source, const std::string &path);
    void handleRouteReply(cMessage *rrep);
    void forwardMessage(cMessage *msg);
    void handleDataMessage(cMessage *msg);

    // Helper functions
    int findGateToNode(const std::string &nodeName);
    std::vector<std::string> parsePath(const std::string &path);
};

Define_Module(BidirectionalNode);

void BidirectionalNode::initialize() {
    routingMsgCount = 0;
}

void BidirectionalNode::handleMessage(cMessage *msg) {
    int kind = msg->getKind();

    if (kind == 0) { // RREQ
        handleRouteRequest(msg);
    } else if (kind == 1) { // RREP
        handleRouteReply(msg);
    } else if (kind == 2) { // Data message
        handleDataMessage(msg);
    } else {
        EV << "Unknown message kind: " << kind << "\n";
        delete msg;
    }
}

void BidirectionalNode::sendRouteRequest(const std::string &destination) {
    cMessage *rreq = new cMessage("RREQ");
    rreq->setKind(0); // 0 for RREQ
    rreq->addPar("source") = getName();
    rreq->addPar("destination") = destination.c_str();
    rreq->addPar("path") = getName();

    // Broadcast the RREQ to all neighbors
    for (int i = 0; i < gateSize("io$o"); ++i) {
        routingMsgCount++;
        sendDelayed(rreq->dup(), 1, "io$o", i);
    }
    delete rreq;
}

void BidirectionalNode::handleRouteRequest(cMessage *rreq) {
    std::string source = rreq->par("source").stringValue();
    std::string destination = rreq->par("destination").stringValue();
    std::string path = rreq->par("path").stringValue();

    std::string messageId = source + "->" + destination; // Unique message ID
    if (knownMessages.find(messageId) != knownMessages.end()) {
        delete rreq;
        return;
    }
    knownMessages.insert(messageId);

    if (destination == getName()) {
        sendRouteReply(source, destination, path + "-" + getName());
        delete rreq;
        return;
    }

    path += std::string("-") + getName();
    rreq->par("path") = path.c_str();

    cGate *arrivalGate = rreq->getArrivalGate(); // Gate where the message arrived
    cGate *connectedGate = arrivalGate->getPreviousGate(); // Gate of the sender
    cModule *senderNode = connectedGate->getOwnerModule(); // Module of the sender

    // Forward the RREQ to all neighbors
    for (int i = 0; i < gateSize("io$o"); ++i) {

        cGate *outGate = gate("io$o", i); // Get the current output gate
        cGate *connectedGate = outGate->getNextGate(); // Get the gate on the connected module
        cModule *connectedNode = connectedGate->getOwnerModule(); // Get the module owning the connected gate

        if (connectedNode == senderNode) {
           continue; // Skip the node that sent the message
        }

        routingMsgCount++;
        sendDelayed(rreq->dup(), 1, "io$o", i);
    }
    delete rreq;
}

void BidirectionalNode::sendRouteReply(const std::string &destination, const std::string &source, const std::string &path) {
    cMessage *rrep = new cMessage("RREP");
    rrep->setKind(1); // 1 for RREP
    rrep->addPar("source") = getName();
    rrep->addPar("destination") = destination.c_str();
    rrep->addPar("path") = path.c_str();

    std::vector<std::string> route = parsePath(path);

    route.pop_back();
    std::string nextHop = route.back();

    if (nextHop != getName()) {
        sendDelayed(rrep, 1, "io$o", findGateToNode(nextHop));
    } else {
        delete rrep; // Already reached the destination
    }
}

void BidirectionalNode::handleRouteReply(cMessage *rrep) {
    std::string destination = rrep->par("destination").stringValue();
    std::string source = rrep->par("source").stringValue();
    std::string path = rrep->par("path").stringValue();

    std::vector<std::string> route = parsePath(path);
    std::vector<std::string> frontroute;
    std::vector<std::string> backroute;

    auto currentIt = std::find(route.begin(), route.end(), getName());
    if (currentIt == route.end()) {
        EV << "Current node not found in the route: " << getName() << "\n";
        delete rrep;
        return;
    }

    frontroute.assign(currentIt+1, route.end());
    routingTable[source] = frontroute;

    backroute.assign(route.begin(), currentIt);

    if (!backroute.empty()) {
        std::string nextHop = backroute.back();
        sendDelayed(rrep, 1, "io$o", findGateToNode(nextHop));
        return;
    }

    delete rrep;
}

void BidirectionalNode::handleDataMessage(cMessage *msg) {
    std::string destination = msg->par("destination").stringValue();

        simtime_t delay = simTime() - msg->getTimestamp();

        if (destination == getName()) {
            EV << "Message received at destination: " << msg->par("payload").stringValue() << "\n";
            EV << "End-to-end delay: " << delay << "\n";
            delete msg;
        } else {
            forwardMessage(msg);
        }
}

void BidirectionalNode::forwardMessage(cMessage *msg) {
    std::string destination = msg->par("destination").stringValue();

    if (routingTable.find(destination) == routingTable.end()) {
        sendRouteRequest(destination);
        return;
    }

    std::vector<std::string> route = routingTable[destination];

    std::string nextHop = route.front();
    sendDelayed(msg, 1, "io$o", findGateToNode(nextHop));
}

int BidirectionalNode::findGateToNode(const std::string &nodeName) {
    for (int i = 0; i < gateSize("io$o"); ++i) {
        cGate *outGate = gate("io$o", i);
        cGate *connectedGate = outGate->getNextGate();
        cModule *connectedNode = connectedGate->getOwnerModule();

        if (connectedNode->getName() == nodeName) {
            return i;
        }
    }
    EV << "Error: No gate to node " << nodeName << " found.\n";
    return -1;
}

std::vector<std::string> BidirectionalNode::parsePath(const std::string &path) {
    std::vector<std::string> nodes;
    std::stringstream ss(path);
    std::string node;
    while (std::getline(ss, node, '-')) {
        nodes.push_back(node);
    }
    return nodes;
}

void BidirectionalNode::finish() {
    EV << "Node " << getName() << " finished with " << routingMsgCount << " routing messages.\n";
    EV << "Routing table:\n";
    for (const auto &entry : routingTable) {
        EV << "  Destination: " << entry.first << ", Path: ";
        for (const std::string &node : entry.second) {
            EV << node << " ";
        }
        EV << "\n";
    }
}
