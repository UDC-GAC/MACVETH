/**
 * Reductions
 */

#include "redux.h"

#ifdef POLYBENCH_PAPI
struct T {
  char *name;
  char *feat;
  long flops;
  long_long counters[128];
} values_t;

struct T values[128];

long long n_func = 0;

void update_row(char *name, char *features, long flops, int t, long_long val) {
  if (t == 0) {
    struct T tmp;
    tmp.name = name;
    tmp.feat = features;
    tmp.flops = flops;
    tmp.counters[t] = val;
    values[n_func] = tmp;
    return;
  }
  values[n_func].counters[t] = val;
}
#endif

#if defined(REDUX_1D)

void init_array(red_vec(A), int n) {
  for (int i = 0; i < n; ++i) {
    A[i] = i + 0.1;
  }
}
#endif

#ifndef POLYBENCH_PAPI
#define polybench_profile_one_function(X, name, features, flops, steps, t)     \
  {                                                                            \
    polybench_prepare_instruments;                                             \
    polybench_start_instruments;                                               \
    for (int t = 0; t < steps; ++t) {                                          \
      X;                                                                       \
    }                                                                          \
    polybench_stop_instruments;                                                \
    printf("%s,%s,%ld,", name, features, flops);                               \
    polybench_print_instruments_flops(flops);                                  \
  }
#else
#define polybench_profile_one_function(X, name, features, flops, steps, t)     \
  {                                                                            \
    polybench_start_instruments_single(t);                                     \
    for (int t = 0; t < steps; ++t) {                                          \
      X;                                                                       \
    }                                                                          \
    polybench_stop_instruments_single(t);                                      \
    update_row(name, features, flops, t, polybench_get_value(t));              \
    n_func++;                                                                  \
  }
#endif

#ifdef POLYBENCH_PAPI
char *__polybench_papi_eventlist[] = {
#include "papi_counters.list"
    NULL};
#endif

int main(int argc, char **argv) {
  int i = 0;
#ifndef POLYBENCH_PAPI
  printf("name,features,flops,time,gflops\n");
#else
  printf("name,features,flops,");
  while (__polybench_papi_eventlist[i] != NULL) {
    if (__polybench_papi_eventlist[i + 1] == NULL) {
      printf("%s", __polybench_papi_eventlist[i++]);
    } else {
      printf("%s,", __polybench_papi_eventlist[i++]);
    }
  }
  printf("\n");
  i = 0;
  polybench_papi_init();
  while (__polybench_papi_eventlist[i] != NULL) {
    n_func = 0;
    polybench_prepare_instruments();
#endif
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
    char *size = new char[ssize.size() + 1];
    std::copy(ssize.begin(), ssize.end(), size);
    size[ssize.size()] = '\0';

#elif defined(REDUX_2D)
      vector2D(arr, n, n);
      std::string sizestr = std::to_string(n) + "x" + std::to_string(n);
      char *size =

#endif

    int steps = 1000000;

    init_array(arr0, arr0.size());

    // Single reduction
    long flops = redux_flops(arr0) * steps;
    polybench_profile_one_function(redux(arr0), strdup("redux"), size, flops,
                                   steps, i);
    polybench_prevent_dce(print_array(arr0));

    // Fused reductions: 2
    flops = redux2_flops(arr0) * steps;
    polybench_profile_one_function(redux2(arr0, arr1), strdup("redux2"), size,
                                   flops, steps, i);
    polybench_prevent_dce(print_array(arr0));

    // Fused reductions: 4
    flops = redux4_flops(arr0) * steps;
    polybench_profile_one_function(redux4(arr0, arr1, arr2, arr3),
                                   strdup("redux4"), size, flops, steps, i);
    polybench_prevent_dce(print_array(arr0));

    // Fused reductions: 8
    flops = redux8_flops(arr0) * steps;
    polybench_profile_one_function(
        redux8(arr0, arr1, arr2, arr3, arr4, arr5, arr6, arr7),
        strdup("redux8"), size, flops, steps, i);
    polybench_prevent_dce(print_array(arr0));
  }

#ifdef POLYBENCH_PAPI
  i++;
}
int n_count = i;
for (int n = 0; n < n_func; ++n) {
  struct T val = values[n];
  printf("%s,%s,%ld,", val.name, val.feat, val.flops);
  for (int c = 0; c < n_count; ++c) {
    printf("%llu", val.counters[c]);
    if (c + 1 < n_count) {
      printf(",");
    }
  }
  printf("\n");
}
polybench_papi_close();
#endif

return 0;
}
