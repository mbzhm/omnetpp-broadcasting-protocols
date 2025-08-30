simple BidirectionalSourceNode
{
    @display("i=misc/sun");
    gates:
        inout io;
}

simple BidirectionalNode
{
    @display("i=status/green");
    gates:
        inout io[4];
}

simple BidirectionalLeafNode
{
    @display("i=status/checkmark");
    gates:
        inout io;
}

network RandomNetwork2
{
    @display("bgb=972,633");
    submodules:
        source: BidirectionalSourceNode {
            @display("p=39,606");
        }
        node0: BidirectionalNode {
            @display("p=139,556");
        }
        node1: BidirectionalNode {
            @display("p=205,448");
        }
        node2: BidirectionalNode {
            @display("p=296,490");
        }
        node3: BidirectionalNode {
            @display("p=78,419");
        }
        node4: BidirectionalNode {
            @display("p=29,274");
        }
        node5: BidirectionalNode {
            @display("p=205,313");
        }
        node6: BidirectionalNode {
            @display("p=257,370");
        }
        node7: BidirectionalNode {
            @display("p=228,156");
        }
        node8: BidirectionalNode {
            @display("p=94,196");
        }
        node9: BidirectionalNode;
        node10: BidirectionalNode {
            @display("p=358,387");
        }
        node11: BidirectionalNode {
            @display("p=238,50");
        }
        node12: BidirectionalNode {
            @display("p=430,139");
        }
        node13: BidirectionalNode {
            @display("p=380,286");
        }
        node14: BidirectionalNode {
            @display("p=492,507");
        }
        node15: BidirectionalLeafNode {
            @display("p=329,89");
        }
        node16: BidirectionalNode {
            @display("p=166,235");
        }
        node17: BidirectionalNode {
            @display("p=329,221");
        }
        node18: BidirectionalLeafNode {
            @display("p=391,546");
        }
        node19: BidirectionalNode {
            @display("p=580,387");
        }

        node20: BidirectionalNode {
            @display("p=626,468");
        }

        node21: BidirectionalNode {
            @display("p=638,556");
        }

        node22: BidirectionalNode {
            @display("p=467,300");
        }

        node23: BidirectionalNode {
            @display("p=552,326");
        }

        node24: BidirectionalLeafNode {
            @display("p=710,285");
        }

        node25: BidirectionalLeafNode {
            @display("p=813,312");
        }

        node26: BidirectionalNode {
            @display("p=51,105");
        }

        node27: BidirectionalNode {
            @display("p=464,50");
        }

        node28: BidirectionalNode {
            @display("p=535,195");
        }

        node29: BidirectionalNode {
            @display("p=745,403");
        }

        node30: BidirectionalNode {
            @display("p=762,523");
        }

        node31: BidirectionalLeafNode {
            @display("p=625,174");
        }

        node32: BidirectionalLeafNode {
            @display("p=597,79");
        }

        node33: BidirectionalLeafNode {
            @display("p=813,369");
        }

        node34: BidirectionalLeafNode {
            @display("p=886,448");
        }

        node35: BidirectionalLeafNode {
            @display("p=668,252");
        }

    connections:
        source.io <--> node0.io++;
        node0.io++ <--> node1.io++;
        node0.io++ <--> node2.io++;
        node0.io++ <--> node3.io++;

        node1.io++ <--> node5.io++;
        node1.io++ <--> node6.io++;
        node1.io++ <--> node10.io++;

        node2.io++ <--> node9.io++;
        node2.io++ <--> node14.io++;
        node2.io++ <--> node18.io;

        node3.io++ <--> node4.io++;
        node3.io++ <--> node8.io++;
        node3.io++ <--> node16.io++;

        node4.io++ <--> node26.io++;
        node4.io++ <--> node8.io++;
        node4.io++ <--> node16.io++;

        node5.io++ <--> node7.io++;
        node5.io++ <--> node17.io++;
        node5.io++ <--> node13.io++;

        node6.io++ <--> node17.io++;
        node6.io++ <--> node22.io++;
        node6.io++ <--> node13.io++;

        node7.io++ <--> node11.io++;
        node7.io++ <--> node15.io;
        node7.io++ <--> node12.io++;

        node8.io++ <--> node11.io++;

        node9.io++ <--> node19.io++;
        node9.io++ <--> node22.io++;
        node9.io++ <--> node23.io++;

        node10.io++ <--> node13.io++;
        node10.io++ <--> node22.io++;
        node10.io++ <--> node23.io++;

        node11.io++ <--> node27.io++;

        node12.io++ <--> node27.io++;
        node12.io++ <--> node28.io++;
        node12.io++ <--> node17.io++;

        node13.io++ <--> node28.io++;

        node14.io++ <--> node19.io++;
        node14.io++ <--> node20.io++;
        node14.io++ <--> node21.io++;

        node16.io++ <--> node17.io++;

        node19.io++ <--> node24.io;
        node19.io++ <--> node25.io;

        node20.io++ <--> node21.io++;
        node20.io++ <--> node29.io++;
        node20.io++ <--> node30.io++;

        node21.io++ <--> node29.io++;
        node21.io++ <--> node30.io++;

        node22.io++ <--> node28.io++;

        node23.io++ <--> node31.io;
        node23.io++ <--> node35.io;

        node26.io++ <--> node11.io++;
        node26.io++ <--> node8.io++;
        node26.io++ <--> node16.io++;

        node27.io++ <--> node28.io++;
        node27.io++ <--> node32.io;

        node29.io++ <--> node30.io++;
        node29.io++ <--> node33.io;

        node30.io++ <--> node34.io;
}
