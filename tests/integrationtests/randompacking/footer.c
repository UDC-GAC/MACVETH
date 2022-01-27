(*S) = tmp;
}

int main(int argc, char **argv) {
  /* Retrieve problem size. */
  int n = N;

  /* Variable declaration/allocation. */
  POLYBENCH_1D_ARRAY_DECL(x, DATA_TYPE, N, n);

  /* Initialize array(s). */
  init_1darray(n, POLYBENCH_ARRAY(x));

  float S = 0.0;

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_template(n, &S, POLYBENCH_ARRAY(x));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  // polybench_prevent_dce(print_1darray(n, POLYBENCH_ARRAY(x)));
  polybench_prevent_dce(print_value(S));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(x);

  return 0;
}
