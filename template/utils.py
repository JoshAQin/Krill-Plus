import numpy as np

class Graph:

    def __init__(self, offsets, edges, weights=None):
        self.num_nodes = len(offsets)
        self.num_edges = len(edges)
        self.offsets = np.array(offsets)
        self.edges = np.array(edges)
        tmp_offset = offsets[1:] + [self.num_nodes]
        self.degrees = np.array(tmp_offset) - np.array(offsets)
        self.weights = weights

def read_graph(dataset_name):
    with open(dataset_name,"r") as infile:
        lines = infile.read().splitlines()
        if lines[0] == "AdjacencyGraph":
            num_nodes = int(lines[1])
            num_edges = int(lines[2])
            offsets = list(map(int,lines[3:3+num_nodes]))
            edges = list(map(int,lines[3+num_nodes:3+num_nodes+num_edges]))
            g = Graph(offsets, edges)
        elif lines[0] == "WeightedAdjacencyGraph":
            num_nodes = int(lines[1])
            num_edges = int(lines[2])
            offsets = list(map(int,lines[3:3+num_nodes]))
            edges = list(map(int,lines[3+num_nodes:3+num_nodes+num_edges]))
            weights = list(map(int,lines[3+num_nodes+num_edges:]))
            g = Graph(offsets, edges, weights)
        else:
            raise RuntimeError("Input file error")
    return g

def traversal(alg, g, start_node=0):
    alg.init_func(g.num_nodes, start_node)
    active_vertices = 1 # pseudo
    if g.weights == None:
        iteration = 0
        while (active_vertices != 0):
            active_vertices = 0
            alg.frontier = [alg.new_frontier[idx] for idx in range(g.num_nodes)]
            alg.new_frontier = [False] * g.num_nodes
            # traversal
            for src in range(g.num_nodes):
                beg = g.offsets[src]
                end = g.num_edges if src == g.num_nodes - 1 else g.offsets[src+1]
                for idx in range(beg, end):
                    dst = g.edges[idx]
                    if (alg.frontier[src] and alg.cond(dst) and alg.update(src, dst)):
                        alg.new_frontier[dst] = 1
                        active_vertices += 1
            try:
                active_vertices = alg.finish_one_iter(iteration)
            except:
                pass
            iteration += 1
    else:
        iteration = 0
        while (active_vertices != 0):
            active_vertices = 0
            alg.frontier = [alg.new_frontier[idx] for idx in range(g.num_nodes)]
            alg.new_frontier = [False] * g.num_nodes
            # traversal
            for src in range(g.num_nodes):
                beg = g.offsets[src]
                end = g.num_edges if src == g.num_nodes - 1 else g.offsets[src+1]
                for idx in range(beg, end):
                    dst = g.edges[idx]
                    weight = g.weights[idx]
                    if (alg.frontier[src] and alg.cond(dst) and alg.update(src, dst, weight)):
                        alg.new_frontier[dst] = 1
                        active_vertices += 1
            try:
                active_vertices = alg.finish_one_iter(iteration)
            except:
                pass
            iteration += 1