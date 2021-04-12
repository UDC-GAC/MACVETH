for (i = 0; i <= 6079; ++i) {
    for (j = 0; j <=1; ++j) {
        y[10*i + 1*j + ORIG_y] += A[2*i + j + ORIG_A] * x[8*i + 1 + ORIG_x];
     }
}
