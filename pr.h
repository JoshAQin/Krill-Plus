#ifndef PR_H
#define PR_H

const float damping = 0.85;

class PR {
public:
    float pr_val[MAX_NODES];
    float pr_next[MAX_NODES];

    inline void initialize(bool* frontier, int num_nodes, int start) {
        for (int idx = 0; idx < num_nodes; ++idx) {
        #pragma HLS loop_tripcount max=10
        #pragma HLS pipeline
            pr_val[idx] = 1/(float)num_nodes;
            pr_next[idx] = 0.0;
            frontier[idx] = 1;
        }
    }

    inline bool cond(int dst) {
        return 1;
    }

    inline bool update(int src, int dst) {
        pr_next[dst] += pr_val[src] / degrees[src];
        return 1;
    }

    #define FINISH_ONE_ITER_FUNC
    inline int finish_one_iter(int iter, int num_nodes, int active_vertices) {
        for (int i = 0; i < num_nodes; ++i) {
        #pragma HLS loop_tripcount max=10
        #pragma HLS pipeline
            pr_val[i] = damping * pr_next[i] + ((1 - damping) * (1 / (float) num_nodes));
            pr_next[i] = 0.0;
        }
        if (iter >= 10)
            active_vertices = 0;
        return active_vertices;
    }

    inline void write_results(hls::stream<axis_t<data_t> >& results, int num_nodes) { // fake
        for (int i = 0; i < num_nodes; ++i) {
        #pragma HLS loop_tripcount max=10
            axis_t<data_t> tmp;
            tmp.data = 0;
            if (i == num_nodes - 1)
                tmp.last = 1;
            else
                tmp.last = 0;
            results.write(tmp);
        }
    }
};

#endif // PR_H