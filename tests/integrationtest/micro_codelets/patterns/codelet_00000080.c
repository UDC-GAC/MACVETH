for (i = 0; i <= 4; ++i) {
    for (j = 0; j <=7; ++j) {
        y[16*i + 2*j + ORIG_y] += A[8*i + j + ORIG_A] * x[16*i + 2 + ORIG_x];
     }
}
