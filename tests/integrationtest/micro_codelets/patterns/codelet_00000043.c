for (i = 0; i <= 3; ++i) {
    for (j = 0; j <=7; ++j) {
        y[80*i + 1*j + ORIG_y] += A[8*i + j + ORIG_A] * x[0*i + 0 + ORIG_x];
     }
}
