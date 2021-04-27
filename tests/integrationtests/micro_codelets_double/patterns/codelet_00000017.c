for (i = 0; i <= 7; ++i) {
    for (j = 0; j <=7; ++j) {
        y[27*i + 3*j + ORIG_y] += A[8*i + j + ORIG_A] * x[27*i + 3 + ORIG_x];
     }
}
