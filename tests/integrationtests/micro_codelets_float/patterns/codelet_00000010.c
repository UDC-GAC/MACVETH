for (i = 0; i <= 9; ++i) {
    for (j = 0; j <=7; ++j) {
        y[1*i + 0*j + ORIG_y] += A[8*i + j + ORIG_A] * x[0*i + 1 + ORIG_x];
     }
}
