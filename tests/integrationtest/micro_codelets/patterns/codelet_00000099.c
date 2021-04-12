for (i = 0; i <= 2047; ++i) {
    for (j = 0; j <=7; ++j) {
        y[32*i + 4*j + ORIG_y] += A[8*i + j + ORIG_A] * x[32*i + 4 + ORIG_x];
     }
}
