#ifndef UDF_H
#define UDF_H

#include "../bfs.h"
#include "../cc.h"

BFS bfs[4];
CC cc[4];

inline int getid(int i) {
    return (i < 4 ? i : (i - 4));
}

inline void initialize(bool frontier[MAX_JOBS][MAX_NODES], int num_jobs, int num_nodes, int start) {
    for (int i = 0; i < num_jobs; ++i) {
    #pragma HLS unroll
    #pragma HLS loop_tripcount max=8
        if (i < 4)
            bfs[i].initialize(frontier[i], num_nodes, start);
        else
            cc[getid(i)].initialize(frontier[i], num_nodes, start); // be careful of id
    }
}

inline bool update(int job_id, int src, int dst) {
    if (job_id < 4)
        return bfs[job_id].update(src, dst);
    else
        return cc[getid(job_id)].update(src, dst);
}

inline void write_results(hls::stream<axis_t<data_t> >& results, int num_jobs, int num_nodes) {
    bfs[0].write_results(results, num_nodes);
    // for (int i = 0; i < num_jobs; ++i) {// cannot pipeline
    // #pragma HLS loop_tripcount max=8
    //     if (i < 4)
    //         bfs[i].write_results(results, num_nodes);
    //     else
    //         cc[getid(i)].write_results(results, num_nodes);
    // }
}

#endif // UDF_H