/**
 * Reductions
 */

#include "redux.h"

#if defined(REDUX_1D)

void init_array(red_vec(A), int n) {
  for (int i = 0; i < n; ++i) {
    A[i] = i + 0.1;
  }
}
#endif

int main(int argc, char **argv) {
  printf("name,features,flops,time,gflops\n");
  for (int n = 8; n <= 32; n *= 2) {
#if defined(REDUX_1D)
    vector1D(arr0, n);
    vector1D(arr1, n);
    vector1D(arr2, n);
    vector1D(arr3, n);
    vector1D(arr4, n);
    vector1D(arr5, n);
    vector1D(arr6, n);
    vector1D(arr7, n);
    std::string ssize = std::to_string(n);
    const char *size = ssize.c_str();

#elif defined(REDUX_2D)
    vector2D(arr, n, n);
    std::string sizestr = std::to_string(n) + "x" + std::to_string(n);
    char *size =

#endif

    int steps = 1000000;

    init_array(arr0, arr0.size());

    // Single reduction
    long flops = redux_flops(arr0) * steps;
    polybench_profile_one_function(redux(arr0), "redux", size, flops, steps);
    polybench_prevent_dce(print_array(arr0));

    // Fused reductions: 2
    flops = redux2_flops(arr0) * steps;
    polybench_profile_one_function(redux2(arr0, arr1), "redux2", size, flops,
                                   steps);
    polybench_prevent_dce(print_array(arr0));

    // Fused reductions: 4
    flops = redux4_flops(arr0) * steps;
    polybench_profile_one_function(redux4(arr0, arr1, arr2, arr3), "redux4",
                                   size, flops, steps);
    polybench_prevent_dce(print_array(arr0));

    // Fused reductions: 8
    flops = redux8_flops(arr0) * steps;
    polybench_profile_one_function(
        redux8(arr0, arr1, arr2, arr3, arr4, arr5, arr6, arr7), "redux8", size,
        flops, steps);
    polybench_prevent_dce(print_array(arr0));
  }
  return 0;
}
