#ifndef SSSP_H
#define SSSP_H

class SSSP {
public:
    data_t len[MAX_NODES];

    inline void initialize(bool* frontier, int num_nodes, int start) {
        for (int idx = 0; idx < num_nodes; ++idx) {
        #pragma HLS loop_tripcount max=10
        #pragma HLS pipeline
            len[idx] = 0x3f3f3f3f;
            frontier[idx] = 0;
        }
        len[start] = 0;
        frontier[start] = 1;
    }

    inline bool update(int src, int dst, int weight) {
        int new_len = len[src] + weight;
        if (new_len < len[dst]) {
            len[dst] = new_len;
            return 1;
        } else {
            return 0;
        }
    }

    inline void write_results(hls::stream<axis_t<data_t> >& results, int num_nodes) {
        axis_write<data_t>(len, results, num_nodes);
    }
};

#endif // SSSP_H