This project implements and simulates three broadcasting protocols in OMNeT++ on a randomly generated network with about 30 nodes:

- **Flooding (naive approach)**: Every node forwards the packet upon first reception.
- **Dynamic Source Routing (DSR - reactive approach)**: A route is discovered on-demand before packet forwarding.
- **Smart Gossip (probabilistic approach)**: Nodes forward messages based on a probability function to reduce redundancy. (https://ieeexplore.ieee.org/document/4054017)

<img width="468" height="326" alt="DSR_Omnetpp" src="https://github.com/user-attachments/assets/8902209c-f77b-4718-9b1a-91c6192ffe56" />
<img width="475" height="330" alt="SmartGossip_Omnetpp" src="https://github.com/user-attachments/assets/90905522-0a34-4426-b8ef-f4e0ad2bf3cc" />
