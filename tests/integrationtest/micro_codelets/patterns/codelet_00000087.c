for (i = 0; i <= 1; ++i) {
    y[2*i + ORIG_y] += A[i + ORIG_A] * x[-2*i + ORIG_x];
}
