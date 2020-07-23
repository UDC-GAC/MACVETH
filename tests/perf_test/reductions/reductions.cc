/**
 * Reductions
 */

#include "redux.h"

int main(int argc, char **argv) {
  printf("name,features,flops,time,gflops\n");
  for (int n = 8; n <= 32; n *= 2) {
#if defined(REDUX_1D)
    vector1D(arr, n);
    std::string ssize = std::to_string(n);
    const char *size = ssize.c_str();

#elif defined(REDUX_2D)
    vector2D(arr, n, n);
    std::string sizestr = std::to_string(n) + "x" + std::to_string(n);
    char *size =

#endif

    int steps = 1000000;

    // Single reduction
    long flops = redux_flops(arr) * steps;
    polybench_profile_one_function(redux(arr), "redux", size, flops, steps);
    polybench_prevent_dce(print_array(arr));

    // Fused reductions: 2
    flops = redux2_flops(arr) * steps;
    polybench_profile_one_function(redux2(arr), "redux2", size, flops, steps);
    polybench_prevent_dce(print_array(arr));

    // Fused reductions: 4
    flops = redux4_flops(arr) * steps;
    polybench_profile_one_function(redux4(arr), "redux4", size, flops, steps);
    polybench_prevent_dce(print_array(arr));

    // Fused reductions: 8
    flops = redux8_flops(arr) * steps;
    polybench_profile_one_function(redux8(arr), "redux8", size, flops, steps);
    polybench_prevent_dce(print_array(arr));
  }
  return 0;
}
