for (i = 0; i <= 180358; ++i) {
    for (j = 0; j <=7; ++j) {
        y[4*i + 1*j + ORIG_y] += A[8*i + j + ORIG_A] * x[8*i + 1 + ORIG_x];
     }
}
