simple SourceNode
{
    @display("i=misc/sun");
    gates:
        output out;
}

simple Node
{
    @display("i=status/green");
    gates:
        input in;
        output out[3];
}

simple LeafNode
{
    @display("i=status/checkmark");
    gates:
        input in;
}

simple LeafNode2
{
    @display("i=status/checkmark");
    gates:
        input in[2];
}

network RandomNetwork
{
    @display("bgb=828,634");
    submodules:
        source: SourceNode {
            @display("p=39,606");
        }
        node0: Node {
            @display("p=139,556");
        }
        node1: Node {
            @display("p=205,448");
        }
        node2: Node {
            @display("p=296,490");
        }
        node3: Node {
            @display("p=78,419");
        }
        node4: LeafNode {
            @display("p=29,274");
        }
        node5: Node {
            @display("p=205,313");
        }
        node6: LeafNode {
            @display("p=257,370");
        }
        node7: Node {
            @display("p=228,156");
        }
        node8: LeafNode {
            @display("p=94,196");
        }
        node9: Node;
        node10: Node {
            @display("p=358,387");
        }
        node11: LeafNode {
            @display("p=238,50");
        }
        node12: LeafNode {
            @display("p=430,139");
        }
        node13: LeafNode2 {
            @display("p=380,286");
        }
        node14: Node {
            @display("p=492,507");
        }
        node15: LeafNode {
            @display("p=329,89");
        }
        node16: LeafNode {
            @display("p=166,235");
        }
        node17: LeafNode {
            @display("p=329,221");
        }
        node18: LeafNode {
            @display("p=391,546");
        }
        node19: LeafNode2 {
            @display("p=580,387");
        }

        node20: LeafNode {
            @display("p=626,468");
        }

        node21: LeafNode {
            @display("p=638,556");
        }

        node22: LeafNode2 {
            @display("p=467,300");
        }

        node23: LeafNode2 {
            @display("p=552,326");
        }
    connections:
        source.out --> node0.in;
        node0.out++ --> node1.in;
        node0.out++ --> node2.in;
        node0.out++ --> node3.in;

        node1.out++ --> node5.in;
        node1.out++ --> node6.in;
        node1.out++ --> node10.in;

        node2.out++ --> node9.in;
        node2.out++ --> node14.in;
        node2.out++ --> node18.in;

        node3.out++ --> node4.in;
        node3.out++ --> node8.in;
        node3.out++ --> node16.in;

        node5.out++ --> node7.in;
        node5.out++ --> node17.in;
        node5.out++ --> node13.in[0];

        node7.out++ --> node11.in;
        node7.out++ --> node15.in;
        node7.out++ --> node12.in;
        
        node9.out++ --> node19.in[0];
        node9.out++ --> node22.in[0];
        node9.out++ --> node23.in[0];
        
        node10.out++ --> node13.in[1];
        node10.out++ --> node22.in[1];
        node10.out++ --> node23.in[1];

        node14.out++ --> node19.in[1];
        node14.out++ --> node20.in;
        node14.out++ --> node21.in;
}
