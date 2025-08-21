__constant float kernel5x5[25] = {
    1/256.f,  4/256.f,  6/256.f,  4/256.f, 1/256.f,
    4/256.f, 16/256.f, 24/256.f, 16/256.f, 4/256.f,
    6/256.f, 24/256.f, 36/256.f, 24/256.f, 6/256.f,
    4/256.f, 16/256.f, 24/256.f, 16/256.f, 4/256.f,
    1/256.f,  4/256.f,  6/256.f,  4/256.f, 1/256.f
};

__kernel void gaussianblur(__global const uchar *input,
                                __global uchar *output,
                                const int width,
                                const int height)
{
    int x = get_global_id(0);
    int y = get_global_id(1);

    if (x >= width || y >= height) return;

    float sum = 0.0f;

    for (int dy = -2; dy <= 2; ++dy) {
        for (int dx = -2; dx <= 2; ++dx) {
            int ix = clamp(x + dx, 0, width - 1);
            int iy = clamp(y + dy, 0, height - 1);
            int kidx = (dy + 2) * 5 + (dx + 2);
            sum += kernel5x5[kidx] * input[iy * width + ix];
        }
    }

    output[y * width + x] = (uchar)sum;
}
