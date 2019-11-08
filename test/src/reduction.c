#include <iostream>
// get AVX intrinsics
#include <immintrin.h>

double indata[4] = {2.0, 3.0f, 2.0f, 5.0f};
double outdata[4] = {0.0, 0.0, 0.0, 0.0};

__m256d ymm0, ymm1, ymm2, ymm3, ymm4;

int main()
{
	ymm0 = _mm256_loadu_pd(indata);
	ymm1 = _mm256_permute_pd(ymm0, 0x05);
	ymm2 = _mm256_add_pd(ymm0, ymm1);
	ymm3 = _mm256_permute2f128_pd(ymm2, ymm2, 0x01);
	ymm4 = _mm256_add_pd(ymm2, ymm3);
	_mm256_storeu_pd(outdata, ymm4);
	std::cout << "Out vector: " << outdata[0] << " " << outdata[1] << " " << outdata[2] << " " << outdata[3] << std::endl;
}
