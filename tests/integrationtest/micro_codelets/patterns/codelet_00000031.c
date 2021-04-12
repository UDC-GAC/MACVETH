for (i = 0; i <= 1986; ++i) {
    for (j = 0; j <=7; ++j) {
        y[40*i + 1*j + ORIG_y] += A[8*i + j + ORIG_A] * x[40*i + 1 + ORIG_x];
     }
}
