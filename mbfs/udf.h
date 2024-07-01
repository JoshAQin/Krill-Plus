#ifndef UDF_H
#define UDF_H

#include "../bfs.h"

BFS bfs[8];

inline void initialize(bool frontier[MAX_JOBS][MAX_NODES], int num_jobs, int num_nodes, int start) {
    for (int i = 0; i < num_jobs; ++i) {
    #pragma HLS unroll
    #pragma HLS loop_tripcount max=8
        bfs[i].initialize(frontier[i], num_nodes, start);
    }
}

inline bool update(int job_id, int src, int dst) {
    return bfs[job_id].update(src, dst);
}

inline void write_results(hls::stream<axis_t<data_t> >& results, int num_jobs, int num_nodes) {
    bfs[0].write_results(results, num_nodes);
}

#endif // UDF_H