for (i = 0; i <= 4; ++i) {
    for (j = 0; j <=7; ++j) {
        y[0*i + 1*j + ORIG_y] += A[8*i + j + ORIG_A] * x[1*i + 0 + ORIG_x];
     }
}
