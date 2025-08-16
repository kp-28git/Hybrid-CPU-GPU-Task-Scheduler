__kernel void matMul(
    const int N,
    __global const int *A,
    __global const int *B,
    __global int *C)
{
    int row = get_global_id(0);
    int col = get_global_id(1);

    if (row < N && col < N) {
        int sum = 0;
        for (int k = 0; k < N; ++k) {
            sum += A[row * N + k] * B[k * N + col];
        }
        C[row * N + col] = sum;
    }
}
