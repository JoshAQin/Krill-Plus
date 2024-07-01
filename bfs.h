#ifndef BFS_H
#define BFS_H

class BFS {
public:
    data_t parents[MAX_NODES];

    inline void initialize(bool* frontier, int num_nodes, int start) {
        for (int idx = 0; idx < num_nodes; ++idx) {
        #pragma HLS loop_tripcount max=10
        #pragma HLS pipeline
            parents[idx] = -1;
            frontier[idx] = 0;
        }
        parents[start] = start;
        frontier[start] = 1;
    }

    inline bool update(int src, int dst) {
        if (parents[dst] == -1) {
            parents[dst] = src;
            return 1;
        } else {
            return 0;
        }
    }

    inline void write_results(hls::stream<axis_t<data_t> >& results, int num_nodes) {
        axis_write<data_t>(parents, results, num_nodes);
    }
};

#endif // BFS_H