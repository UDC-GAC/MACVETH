for (i = 0; i <= 162177; ++i) {
    for (j = 0; j <=1; ++j) {
        y[4*i + 0*j + ORIG_y] += A[2*i + j + ORIG_A] * x[8*i + 1 + ORIG_x];
     }
}
