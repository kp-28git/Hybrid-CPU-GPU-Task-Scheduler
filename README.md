# X-PU Heterogeneous Scheduler

A C++/OpenCL framework that detects available CPU cores and GPU compute units and **dynamically schedules work** to the device that should execute it fastest—based on **policy**, **data size**, and **task type**. Includes automated benchmarking and reproducible experiments.

---

## Highlights

- **Scheduling policies**: CPU‑only, GPU‑only, **Round‑Robin**, **Data‑size based**, and **Task‑stealing**.
- **Heterogeneous kernels** implemented on **both CPU and GPU**: Vector Add, Sorting, Matrix Multiply, and **Gaussian Blur** (with OpenCV for I/O).\
  *Observation:* OpenCV’s highly‑optimized CPU blur can outperform a naive GPU kernel; a **naive CPU blur** is outperformed by the GPU kernel.
- **OpenCL backend** for portable GPU compute; **C++17** host runtime.
- **Automated benchmarking** (Python) that runs all policies × workloads and logs to CSV/XLSX for analysis.

---

## Repository Layout

```
Hybrid-CPU-GPU-Task-Scheduler/
├─ include/              # Public headers (scheduler, task, cpu/gpu executors, policies)
├─ src/                  # C++ sources
├─ kernels/              # OpenCL kernels (vector add, matmul, sort, blur)
├─ images/input_img/     # Sample images for Gaussian blur experiments
├─ test/                 # Test drivers / small examples
├─ results.csv           # Aggregated benchmark results (machine- & build-specific)
├─ Final Results.xlsx    # Curated results workbook
├─ Project results.xlsx  # Additional result logs
├─ runScheduler_Script.py# Automation script to run experiments + log
└─ CMakeLists.txt        # Build configuration
```

##  Architecture

**Key ideas**

- A **task** is defined by an operation (e.g., MatrixMultiply, VectorAdd, Sort, GaussianBlur) and a **problem size**.
- The **scheduler** selects an execution unit (CPU or GPU) using a pluggable **policy** (below) and binds the appropriate function/lambda for that device.
- **Two device queues** are drained by worker threads. **Task stealing** allows an idle device to pull from the other queue to reduce imbalance.
- Results are **timed and logged** for later analysis (Python/XLSX/CSV).

---

## Scheduling Policies

| Policy              | What it does                                      | When it helps                           |
| ------------------- | ------------------------------------------------- | --------------------------------------- |
| **CPU‑only**        | Route everything to CPU.                          | Baseline; control‑heavy or small tasks. |
| **GPU‑only**        | Route everything to GPU.                          | Baseline; large, data‑parallel tasks.   |
| **Round‑Robin**     | Alternate CPU ↔ GPU per task.                     | Fairness when tasks are homogeneous.    |
| **Data‑size based** | **Threshold** decides: below → CPU, above → GPU.  | Good when size correlates with speedup. |
| **Task‑stealing**   | Idle device **steals** work from the other queue. | Reduces tail latency / imbalance.       |

**Tuning:** The `threshold` (problem‑size cutover) materially impacts performance. For your machine, use the provided scripts to sweep values and pick the best per‑kernel.

---

## Workloads

- **Vector Add** (memory‑bound): useful for measuring kernel launch + PCIe overheads.
- **Matrix Multiply** (compute‑heavy): benefits from GPU at larger sizes.
- **Sorting** (control‑heavy, branchy): often favors CPU, though GPU radix sort can win with tuned kernels.
- **Gaussian Blur** (image processing): includes **OpenCV** for I/O and reference CPU blur; OpenCL kernel provided for GPU.

> See `kernels/` and `src/` for the device‑specific implementations; the scheduler maps each operation to its CPU/GPU function.

---

## Quickstart

### Prerequisites

- **C++17** toolchain (GCC/Clang/MSVC)
- **CMake ≥ 3.15**
- **OpenCL** headers and runtime (NVIDIA/AMD/Intel or CPU OpenCL)
- **OpenCV** (for Gaussian blur I/O)

Build

```bash
git clone https://github.com/kp-28git/Hybrid-CPU-GPU-Task-Scheduler.git
cd Hybrid-CPU-GPU-Task-Scheduler
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

### Run (manually)

After building, run the produced demo/binary from the `build/` directory. Exact options/flags may vary—**see **``** for usage** and available operations/policies. Typical parameters include: operation (matmul/vector\_add/sort/blur), policy (cpu/gpu/rr/size/steal), problem size / threshold, and input image path for blur.

### Run (full benchmark sweep)

The repo includes a Python automation script that runs workloads across policies and logs results:

```bash
python3 runScheduler_Script.py
```

Outputs:

- `` with raw timings
- ``\*\* / \*\*`` curated tables & summaries

> Tip: Use the script to **sweep the size threshold** and identify the crossover point per workload.

---

## Typical trends

- **Matrix Multiply:** GPU wins beyond moderate sizes; **data‑size** and **task‑stealing** policies deliver the best throughput.
- **Vector Add:** Often **CPU‑only** is competitive at small sizes due to kernel‑launch and transfer overhead; GPU wins only at very large arrays.
- **Sorting:** The CPU path tends to win unless a specialized GPU radix kernel is used.
- **Gaussian Blur:** With **OpenCV** CPU primitives, CPU can beat a naive GPU kernel; with a naive CPU loop, the **GPU** implementation wins.

> For exact numbers, see the CSV/XLSX artifacts in the repo; results are hardware and driver dependent.

---

## Configuration Knobs

- **Policy**: `SOLE_CPU`, `SOLE_GPU`, `ROUND_ROBIN`, `DATA_SIZE_BASED`, `TASK_STEALING`.
- **Threshold**: size cutover for `DATA_SIZE_BASED`.
- **Queueing**: per‑device queues, with optional **work‑stealing** enabled.
- **Kernels**: tweak work‑group sizes and tile sizes in `kernels/` for your GPU.

---

---
