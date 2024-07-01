#ifndef CC_H
#define CC_H

class CC {
public:
    data_t arr[MAX_NODES];

    inline void initialize(bool* frontier, int num_nodes, int start) {
        for (int idx = 0; idx < num_nodes; ++idx) {
        #pragma HLS loop_tripcount max=10
        #pragma HLS pipeline
            arr[idx] = idx;
            frontier[idx] = 1;
        }
    }

    inline bool update(int src, int dst) {
        if (arr[dst] > arr[src]) {
            arr[dst] = arr[src];
            return 1;
        } else {
            return 0;
        }
    }

    inline void write_results(hls::stream<axis_t<data_t> >& results, int num_nodes) {
        axis_write<data_t>(arr, results, num_nodes);
    }
};

#endif // CC_H