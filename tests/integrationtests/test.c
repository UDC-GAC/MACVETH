
void foo(float *p) {
	int i = 0;
#pragma macveth unroll i full 
	for (i = 0; i < 32; i++){
		p[i] += 42;
	}
#pragma endmacveth
}
