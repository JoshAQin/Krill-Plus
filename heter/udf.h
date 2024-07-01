#ifndef UDF_H
#define UDF_H

#include "../bfs.h"
#include "../cc.h"
#include "../pr.h"
#include "../sssp.h"

BFS bfs[2];
CC cc[2];
PR pr[2];
SSSP sssp[2];

inline void initialize(bool frontier[MAX_JOBS][MAX_NODES], int num_jobs, int num_nodes, int start) {
    for (int i = 0; i < num_jobs; ++i) {
    #pragma HLS unroll
    #pragma HLS loop_tripcount max=8
        if (i < 2)
            bfs[i].initialize(frontier[i], num_nodes, start);
        else if (i < 4)
            cc[i-2].initialize(frontier[i], num_nodes, start);
        else if (i < 6)
            pr[i-4].initialize(frontier[i], num_nodes, start);
        else
            sssp[i-6].initialize(frontier[i], num_nodes, start);
    }
}

inline bool update(int job_id, int src, int dst, int weight) {
    if (job_id < 2)
        return bfs[job_id].update(src, dst);
    else if (job_id < 4)
        return cc[job_id-2].update(src, dst);
    else if (job_id < 6)
        return pr[job_id-4].update(src, dst);
    else
        return sssp[job_id-6].update(src, dst, weight);
}

inline int finish_one_iter(int iter, int num_nodes, int active_vertices) {
    int ret = 0;
    for (int i = 0; i < 2; ++i)
        ret = pr[i].finish_one_iter(iter, num_nodes, active_vertices);
    return ret;
}

inline void write_results(hls::stream<axis_t<data_t> >& results, int num_jobs, int num_nodes) {
    bfs[0].write_results(results, num_nodes);
    // for (int i = 0; i < num_jobs; ++i) { // cannot pipeline
    // #pragma HLS loop_tripcount max=8
    //     if (i < 2)
    //         bfs[i].write_results(results, num_nodes);
    //     else if (i < 4)
    //         cc[i-2].write_results(results, num_nodes);
    //     else if (i < 6)
    //         pr[i-4].write_results(results, num_nodes);
    //     else
    //         sssp[i-6].write_results(results, num_nodes);
    // }
}

#endif // UDF_H