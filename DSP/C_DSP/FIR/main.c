#include <dsplib.h>

#define NUM_SAMPLES 5000
int whtsamples[NUM_SAMPLES];
int samples[NUM_SAMPLES];

//wektor probek
const int filtr_dp[] = {-8, -16, -27, -42, -59, -78, -96, -110, -118, -115, -98, -63, -6, 74, 181,
314, 471, 651, 848, 1057, 1269, 1477, 1673, 1847, 1992, 2100, 2168, 2191, 2168, 2100,
1992, 1847, 1673, 1477, 1269, 1057, 848, 651, 471, 314, 181, 74, -6, -63, -98,
-115, -118, -110, -96, -78, -59, -42, -27, -16, -8};

void whtnoise(int* buffer, unsigned int buflen) { //funkcja generujaca szum bialy
	rand16init();
	rand16((DATA*)buffer, buflen);
}

void saw(int* buffer, unsigned int buflen, int krok) { //funkcja generujaca s. piloksztaltny
	static int akumulator = 0;
	 int i;
	    for(i=0; i<buflen; i++){
	        buffer[i] = akumulator;
	        akumulator = akumulator + krok;
	    }
}

void blockfir(int* input, const int* filter, int* output, int numSamples, int numFilter) {
	{
		int i,j;
		long y;
		for(i=0; i<numSamples; i++){   //zewnêtrzna pêtla iteruj¹ca po próbkach z tablicy wejsciowej, zapisuj¹ca wynik na wyjscie
			y = 0;
			for(j=0; j<numFilter; j++){  //wewnetrzna petla dokonujaca obliczenia dla jednej probki
				if (j>i)
					break;
				y = _smaci(y, input[i-j], filter[j]);
			}
			y = (int)(_sround(y) >> 15); //przesuniecie bitowe
			output[i] = y;
		}
	}
}


void main(void) {

	//saw(samples, NUM_SAMPLES, 137);
	//whtnoise(samples, NUM_SAMPLES);

	//blockfir(samples, filtr_dp, whtsamples, NUM_SAMPLES, 55);

	//int bufor[57]; //bufor niezbêdny do dzia³ania poni¿szej funkcji FIR z DSPLIB
	//fir((DATA*) samples, (DATA*) filtr_dp, (DATA*) whtsamples, (DATA*) bufor, NUM_SAMPLES, 55);

	//int i;
	//for(i=0; i<NUM_SAMPLES; i++){
		//fir((DATA*) &samples[i], (DATA*) filtr_dp, (DATA*) &whtsamples[i], (DATA*) bufor, 1, 55);
		//}

	while (1); // do not exit
}

