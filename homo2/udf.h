#ifndef UDF_H
#define UDF_H

#include "../sssp.h"
#include "../pr.h"

SSSP sssp[4];
PR pr[4];

inline void initialize(bool frontier[MAX_JOBS][MAX_NODES], int num_jobs, int num_nodes, int start) {
    for (int i = 0; i < num_jobs; ++i) {
    #pragma HLS unroll
    #pragma HLS loop_tripcount max=8
        if (i < 4)
            sssp[i].initialize(frontier[i], num_nodes, start);
        else
            pr[i-4].initialize(frontier[i], num_nodes, start);
    }
}

inline bool update(int job_id, int src, int dst, int weight) {
    if (job_id < 4)
        return sssp[job_id].update(src, dst, weight);
    else
        return pr[job_id-4].update(src, dst);
}

inline int finish_one_iter(int iter, int num_nodes, int active_vertices) {
    int ret = 0;
    for (int i = 0; i < 4; ++i)
        ret = pr[i].finish_one_iter(iter, num_nodes, active_vertices);
    return ret;
}

inline void write_results(hls::stream<axis_t<data_t> >& results, int num_jobs, int num_nodes) {
    sssp[0].write_results(results, num_nodes);
    // for (int i = 0; i < num_jobs; ++i) { // cannot pipeline
    // #pragma HLS loop_tripcount max=8
    //     if (i < 4)
    //         sssp[i].write_results(results, num_nodes);
    //     else
    //         pr[i-4].write_results(results, num_nodes);
    // }
}

#endif // UDF_H