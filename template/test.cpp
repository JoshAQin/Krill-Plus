#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include "type.h"
#include "kernel.h"
#include "test.h"
using namespace std;

int main() {
    int num_nodes;
    int num_edges;
    const int start = 0;
    // read graph
#ifdef WEIGHTED
    ifstream infile("testGraph_w");
#else
    ifstream infile("testGraph");
#endif
    string str;
    infile >> str >> num_nodes >> num_edges;
    cout << num_nodes << " " << num_edges << endl;
    int offset[num_nodes] = {0};
    int degrees[num_nodes] = {0};
    int edges[num_edges] = {0};
    for (int i = 0; i < num_nodes; ++i)
        infile >> offset[i];
    for (int i = 0; i < num_nodes; ++i)
        degrees[i] = (((i == num_nodes - 1) ? num_edges : offset[i+1]) - offset[i]);
    for (int i = 0; i < num_edges; ++i)
        infile >> edges[i];
#ifdef WEIGHTED
    int weights[num_edges] = {0};
    for (int i = 0; i < num_edges; ++i)
        infile >> weights[i];
#endif
    cout << "Done reading input file" << endl;

    // transfer data
    hls::stream<axis_t<int> > inputs;
    hls::stream<axis_t<data_t> > results_s;
    axis_write<int>(degrees, inputs, num_nodes);
    axis_write<int>(edges, inputs, num_edges);
#ifdef WEIGHTED
    axis_write<int>(weights, inputs, num_edges);
#endif

    // execute
    cout << "Begin execution..." << endl;
    GraphInfo g;
    g.num_nodes = num_nodes;
    g.num_edges = num_edges;
    g.num_jobs = 8;
    g.start = start;
    test(g, inputs, results_s);

    // get results
    int num_jobs = 8;
    data_t results[num_nodes] = {0};
    axis_read<data_t>(results_s, results, num_nodes);
    cout << "Done execution" << endl;
    // for (int i = 0; i < num_nodes; ++i)
    //     cout << results[i] << " ";
    // cout << endl;

    // test results
    if (is_equal(results, num_jobs, num_nodes))
        cout << "Pass!" << endl;
    else
        cout << "Wrong!" << endl;

    return 0;
}