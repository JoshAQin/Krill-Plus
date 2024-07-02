from pynq import allocate
from pynq import Overlay
import numpy as np
import time
import sys
from utils import read_graph

alg_name = sys.argv[1]

if alg_name in ["homo2","heter","msssp"]:
    weighted = True
else:
    weighted = False

dataset = sys.argv[2]
if not weighted:
    dataset_name = "./datasets/{}.txt".format(dataset)
else:
    dataset_name = "./datasets/{}_w.txt".format(dataset)

num_jobs = 8
g = read_graph(dataset_name)
num_nodes = g.num_nodes
num_edges = g.num_edges
print("# nodes: {}, # edges: {}".format(g.num_nodes, g.num_edges))

buffer_size = 2048
if not weighted:
    inputs = allocate(shape=(buffer_size,),dtype=np.int32)
    np_inputs = np.concatenate((g.degrees, g.edges)).astype(np.int32)
else:
    inputs = allocate(shape=(buffer_size,),dtype=np.int32)
    np_inputs = np.concatenate((g.degrees, g.edges, g.weights)).astype(np.int32)

datatype = np.int32

res = allocate(shape=(num_nodes,),dtype=datatype)
np_res = np.zeros((num_nodes,), dtype=datatype)

overlay = Overlay("./{0}/{0}.bit".format(alg_name))
dma = overlay.axi_dma_0
print("Bitstream loaded")
exec("ip = overlay.{}_core".format(alg_name))

def xcel_func(start_node=0):
    ip.write(0x10, num_nodes)
    ip.write(0x18, num_edges)
    ip.write(0x20, num_jobs)
    ip.write(0x28, start_node)
    ip.write(0x00, 0x01)
    # transfer inputs
    for i in range(len(np_inputs) // buffer_size):
        inputs[:] = np_inputs[i*buffer_size : (i+1)*buffer_size]
        dma.sendchannel.transfer(inputs)
        dma.sendchannel.wait()
    start_idx = len(np_inputs) // buffer_size * buffer_size
    size = len(np_inputs) % buffer_size
    new_inputs = allocate((size,),dtype=np.int32,cacheable=1)
    new_inputs[:] = np_inputs[start_idx:start_idx+size]
    dma.sendchannel.transfer(new_inputs)
    dma.sendchannel.wait()
    # transfer outputs
    for i in range(num_nodes // buffer_size):
        dma.recvchannel.transfer(res)
        dma.recvchannel.wait()
        np_res[i*buffer_size:(i+1)*buffer_size] = res[:]
    start_idx = num_nodes // buffer_size * buffer_size
    size = num_nodes % buffer_size
    new_res = allocate((size,),dtype=np.int32)
    dma.recvchannel.transfer(new_res)
    dma.recvchannel.wait()
    np_res[start_idx:start_idx+size] = new_res[:]
    isready = ip.read(0x00)
    while (isready == 1):
        isready = ip.read(0x00)
    return np_res

print("Begin execution...")
start_time = time.time()
res_xcel = xcel_func(2)
print(res_xcel)
exec_time = time.time() - start_time
print("FPGA Time: {:.4f}s".format(exec_time))
