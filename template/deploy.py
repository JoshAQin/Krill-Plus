from pynq import allocate
from pynq import Overlay
import numpy as np
import time
import timeit
import sys
from utils import read_graph

alg_name = sys.argv[1]
# exec("from {0}.{0} import {0}".format(alg_name))

if alg_name in ["homo2","heter","msssp"]:
    weighted = True
else:
    weighted = False

if not weighted:
    dataset_name = "../datasets/testGraph"
else:
    dataset_name = "../datasets/testGraph_w"

num_jobs = 8
g = read_graph(dataset_name)
num_nodes = g.num_nodes
num_edges = g.num_edges
print("# nodes: {}, # edges: {}".format(g.num_nodes, g.num_edges))

if not weighted:
    inputs = allocate(shape=(num_nodes+num_edges,),dtype=np.int32)
    np_inputs = np.concatenate((g.degrees, g.edges))
else:
    inputs = allocate(shape=(num_nodes+num_edges+num_edges,),dtype=np.int32)
    np_inputs = np.concatenate((g.degrees, g.edges, g.weights))

# if alg_name == "pagerank":
#     datatype = np.float32
# else:
#     datatype = np.int32
datatype = np.int32

res = allocate(shape=(num_nodes,),dtype=datatype)
np_res = np.zeros((num_nodes,), dtype=datatype)
# correct_res = np.loadtxt(dataset_name+"_res.txt",dtype=np.int32)

np.copyto(inputs,np_inputs)

overlay = Overlay("{0}/{0}.bit".format(alg_name))
dma = overlay.axi_dma_0
print("Bitstream loaded")
exec("ip = overlay.{}_core".format(alg_name))

def xcel_func(start_node=0):
    ip.write(0x10, num_nodes)
    ip.write(0x18, num_edges)
    ip.write(0x20, num_jobs)
    ip.write(0x28, start_node)
    dma.sendchannel.transfer(inputs)
    dma.recvchannel.transfer(res)
    ip.write(0x00, 0x1)
    isready = ip.read(0x00)
    while (isready == 1):
        isready = ip.read(0x00)
    dma.sendchannel.wait()
    dma.recvchannel.wait()
    return res

print("Begin execution...")
start_time = time.time()
res_xcel = xcel_func(0)
exec_time = time.time() - start_time
print("FPGA Time: {:.4f}s".format(exec_time))
np.copyto(np_res, res_xcel)
# print(np.array_equal(np_res, correct_res))

# exec("cpu_func = {}()".format(alg_name))
# start_time = time.time()
# res_cpu = cpu_func(g)
# exec_time = time.time() - start_time
# print("CPU Time: {:.4f}s".format(exec_time))
# print(np.array_equal(res_cpu, correct_res))

print(res_xcel)
# print(res_cpu)
# print(np.array_equal(np_res,res_cpu))

fpga_time = timeit.timeit(stmt="xcel_func()", number=1, globals=globals())
# cpu_time = timeit.timeit(stmt="cpu_func(g)", number=10, globals=globals())
print("FPGA Mean Time: {:.4f}s".format(fpga_time))
# print("CPU Mean Time: {:.4f}s".format(cpu_time))
# print("Speedup {:.2f}x".format(cpu_time / fpga_time))
