import random

def generate_dag(num_nodes, num_input_gates=1, num_output_gates=2):
    """
    Generate a random directed acyclic graph (DAG) with fixed number of input and output gates per node.
    
    Args:
        num_nodes (int): Number of nodes in the DAG.
        num_input_gates (int): Number of input gates for each node.
        num_output_gates (int): Number of output gates for each node.
    
    Returns:
        list: List of edges in the DAG with gate indices.
    """
    random.seed(12345)

    edges = []
    
    # Create edges between nodes
    for i in range(num_nodes):
        for j in range(i + 1, num_nodes):  # Acyclic: i -> j where i < j
            if random.random() < 0.3:  # 30% chance of creating an edge
                # Randomly choose input and output gates
                src_out_gate = random.randint(0, num_output_gates - 1)
                dest_in_gate = random.randint(0, num_input_gates - 1)

                # Add edge with gate indices
                edges.append((i, src_out_gate, j, dest_in_gate))

    return edges

def save_to_ned(edges, num_nodes, filename="RandomNetwork.ned"):
    """
    Save the generated DAG to a NED file.
    
    Args:
        edges (list): List of edges in the DAG with gate indices.
        num_nodes (int): Number of nodes.
        filename (str): Output NED filename.
    """
    with open(filename, "w") as f:
        f.write("simple Node {\n")
        f.write("    gates:\n")
        f.write("        input in[1];  // 1 input gate for each node\n")
        f.write("        output out[2]; // 2 output gates for each node\n")
        f.write("}\n\n")
        f.write("network RandomNetwork {\n")
        f.write("    submodules:\n")
        for i in range(num_nodes):
            f.write(f"        node{i}: Node;\n")
        f.write("    connections:\n")
        for edge in edges:
            src, src_gate, dest, dest_gate = edge
            f.write(f"        node{src}.out[{src_gate}] --> node{dest}.in[{dest_gate}];\n")
        f.write("}\n")

if __name__ == "__main__":
    num_nodes = 20  # Number of nodes
    edges = generate_dag(num_nodes)
    save_to_ned(edges, num_nodes)
    print(f"Random DAG with {num_nodes} nodes and {len(edges)} edges generated!")
