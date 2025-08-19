// kernels/sort.cl
__kernel void sort(__global int* data, int phase, int N) {
    int i = get_global_id(0);
    int idx = 2 * i + phase;

    if (idx + 1 < N) {
        if (data[idx] > data[idx + 1]) {
            int tmp = data[idx];
            data[idx] = data[idx + 1];
            data[idx + 1] = tmp;
        }
    }
}
