for (i = 0; i <= 158; ++i) {
    for (j = 0; j <=4; ++j) {
        y[10*i + 2*j + ORIG_y] += A[5*i + j + ORIG_A] * x[8*i + 0 + ORIG_x];
     }
}
