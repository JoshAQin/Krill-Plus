#ifndef __KERNEL_H__
#define __KERNEL_H__

#include <ap_int.h>
#include <ap_fixed.h>
#include <hls_stream.h>

#define MAX_NODES 10000
#define MAX_EDGES 50000
#define MAX_JOBS 8

typedef ap_int<32> bit32;
typedef ap_uint<32> ubit32;

template<class T>
struct axis_t {
    T data;
    ap_int<1> last;
};

template<class T>
inline void axis_read(hls::stream<axis_t<T> >& in_arr, T* out_arr, std::size_t size) {
    for (int i = 0; i < size; ++i) {
    #pragma HLS loop_tripcount max=10
        axis_t<T> tmp = in_arr.read();
        out_arr[i] = tmp.data;
    }
}

template<class T>
inline void axis_write(T* in_arr, hls::stream<axis_t<T> >& out_arr, std::size_t size) {
    for (int i = 0; i < size; ++i) {
    #pragma HLS loop_tripcount max=10
        axis_t<T> tmp;
        tmp.data = in_arr[i];
        if (i == size - 1)
            tmp.last = 1;
        else
            tmp.last = 0;
        out_arr.write(tmp);
    }
}

struct GraphInfo {
    int num_nodes;
    int num_edges;
    int num_jobs;
    int start;
};

void test(GraphInfo& graph, hls::stream<axis_t<int> >& inputs, hls::stream<axis_t<data_t> >& results);

#endif