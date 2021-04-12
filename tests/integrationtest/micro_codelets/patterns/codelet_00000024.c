for (i = 0; i <= 162178; ++i) {
    for (j = 0; j <=3; ++j) {
        y[4*i + 1*j + ORIG_y] += A[4*i + j + ORIG_A] * x[8*i + -1 + ORIG_x];
     }
}
