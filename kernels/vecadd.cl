__kernel void vectoradd(__global const int* A,
                         __global const int* B,
                         __global int* C,
                         const int N) {
    int i = get_global_id(0);
    if (i < N) {
        C[i] = A[i] + B[i];
    }
}
