for (i = 0; i <= 14; ++i) {
    for (j = 0; j <=2; ++j) {
        y[3*i + 2*j + ORIG_y] += A[3*i + j + ORIG_A] * x[1570*i + 0 + ORIG_x];
     }
}
