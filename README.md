Krill+: A Compiler, Runtime System, and Hardware Accelerator for Concurrent Graph Processing
======================

Krill+ is a full-stack and high-performance graph system for processing **concurrent graph jobs**. Krill+ consists of a high-level compiler, an efficient runtime system, and a customized accelerator for concurrent graph jobs. 
* We provide an interface called **property buffer** to easily manage the property data. The property buffer descriptions are transformed into C++ header files by the compiler to share graph properties among different jobs. 
* We provide a **graph kernel fusion** technique in the runtime system to reduce redundant memory access to shared structures among different jobs.
* We provide a **high-level automatic compilation system** to efficiently design accelerators for concurrent graph processing. The system supports productive C++-based CGJ programming using APIs, and automatic end-to-end compilation from concurrent graph job implementation to FPGA accelerator. Moreover, we propose a **degree sparse row (DSR)** data structure for reducing redundant graph structure traverse. Based on the DSR data structure, we construct a streaming-architecture FPGA accelerator to reduce data transfer overheads and on-chip memory consumption.

Krill+ is an extended version of our previous work [Krill](https://github.com/chhzh123/Krill) published in [SC'21](https://dl.acm.org/doi/10.1145/3458817.3476159). The main contribution of this work is the high-level automatic compilation system for the accelerator design of concurrent graph processing, as mentioned above.

## Execution Flow
To automatically design an accelerator for concurrent graph processing, please follow the steps below.
The optional steps have been fulfilled in advance, and can be modified for customized purposes.

### (1) Prepare datasets of DSR format in the folder `datasets`. (Optional)
Datasets in DSR format contain the following components:
* Graph description
* Number of nodes `n`
* Number of edges `e`
* degrees of the `n` nodes
* The sink nodes of these `e` edges. These nodes are sorted in ascending order following the index of their corresponding source nodes.

### (2) Write the concurrent graph job algorithms. (Optional)
The algorithm can be simply implemented by customizing the function APIs in `udf.h` accordingly.
The APIs are as follows:
* `initialize`: initialize property values and bounds
* `cond`: the condition judgment of the target node
* `update`: the update operation in the graph computation kernel
* `write_results`: send back the computational results
* `finish_one_iter`: the post-processing of each iteration, such as the node update in PageRank
The modification of API `is_equal` in `test.h` is needed for the functioning test.

### (3) Backend Compilation.
```bash
$ bash run.sh
```
This script first automatically transforms the algorithm into hardware description language (HDL) by high-level synthesis.
Then, the HDL program is transformed into bitstream by logic and physical syntheses.

### (4) Bitstream deployment in FPGA.
We use Pynq to enable Python-based bitstream deployment in FPGA.
To deploy bitstream and accelerate concurrent graph job processing in FPGA, please connect the FPGA and run:
```python
$ python deploy.py [algorithm_name] [dataset_name]
```

## Requirements
* Python 3.9.7
* Numpy 1.22.4
* Vivado v2020.1.1
* pynq 3.0.1
* Avnet Ultra96-V2 SoC board
