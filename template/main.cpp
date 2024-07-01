#include <ap_int.h>
#include <ap_fixed.h>
#include <ap_axi_sdata.h>
#include <hls_stream.h>
#include <hls_math.h>
#include <math.h>
#include <stdint.h>
#include <iostream>
#include "type.h"
#include "kernel.h"

bool frontier[MAX_JOBS][MAX_NODES];
bool next_frontier[MAX_JOBS][MAX_NODES];
// bool common_frontier[MAX_NODES];

int degrees[MAX_NODES];
int edges[MAX_EDGES];
#ifdef WEIGHTED
int weights[MAX_EDGES];
#endif

#include "udf.h"

void test(GraphInfo& graph, hls::stream<axis_t<int> >& inputs, hls::stream<axis_t<data_t> >& results) {
// stream ports
#pragma HLS INTERFACE s_axilite port=graph
#pragma HLS INTERFACE axis port=inputs bundle=INPUT
#pragma HLS INTERFACE axis port=results bundle=OUTPUT
// slave ports, control signals
#pragma HLS INTERFACE s_axilite register port=return bundle=CTRL

    int num_nodes = graph.num_nodes;
    int num_edges = graph.num_edges;
    int num_jobs = graph.num_jobs;
    int start = graph.start;
    axis_read<int>(inputs, degrees, num_nodes);
    axis_read<int>(inputs, edges, num_edges);
#ifdef WEIGHTED
    axis_read<int>(inputs, weights, num_edges);
#endif
    // initialization
    initialize(next_frontier, num_jobs, num_nodes, start);
    int active_vertices = 1; // pseudo
    int iter = 0;
    while (active_vertices != 0) {
    #pragma HLS loop_tripcount max=10
        // with dependency, cannot pipeline
        active_vertices = 0;
        for (int idx = 0; idx < num_nodes; ++idx) {
        #pragma HLS loop_tripcount max=10
        #pragma HLS pipeline
            for (int job_cnt = 0; job_cnt < num_jobs; ++job_cnt){
            #pragma HLS loop_tripcount max=8
                frontier[job_cnt][idx] = next_frontier[job_cnt][idx];
                next_frontier[job_cnt][idx] = 0;
            }
        }
        // traversal
        int src = -1;
        int neigh_cnt = 0;
        for (int edge_cnt = 0; edge_cnt < num_edges; ++edge_cnt) {
        #pragma HLS loop_tripcount max=10
        #pragma HLS pipeline
            while (neigh_cnt == 0) { // "while" not "if"!
            #pragma HLS loop_tripcount max=2
                src += 1;
                neigh_cnt = degrees[src];
            }
            int dst = edges[edge_cnt];
            for (int job_cnt = 0; job_cnt < num_jobs; ++job_cnt){
            #pragma HLS loop_tripcount max=8
#ifdef WEIGHTED
                int weight = weights[edge_cnt];
                if (frontier[job_cnt][src] == 1 && update(job_cnt,src,dst,weight)) {
#else
                if (frontier[job_cnt][src] == 1 && update(job_cnt,src,dst)) {
#endif
                    next_frontier[job_cnt][dst] = 1;
                    active_vertices += 1;
                }
            }
            neigh_cnt -= 1;
        }
#ifdef FINISH_ONE_ITER_FUNC
        active_vertices = finish_one_iter(iter, num_nodes, active_vertices);
#endif
        iter += 1;
    }
    write_results(results, num_jobs, num_nodes);
}