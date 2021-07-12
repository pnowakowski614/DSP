#include <dsplib.h>
#include <math.h>
#include <stdbool.h>

#define NUM_SAMPLES 5000  //def. liczby próbek
#define M_PI 3.14159265358979323846 //def. liczby pi
int samples[NUM_SAMPLES];
int d = 137; //obliczony krok amplitudy

int silnia (int n) {  //funkcja realizujaca silnie uzyta przy generowaniu sinusa
if (n == 0) return 1;
else return n*silnia(n-1);
}

void saw(int* buffer, unsigned int buflen, int krok) { //funkcja generujaca s. piloksztaltny
	static int akumulator = 0;
	 int i;
	    for(i=0; i<buflen; i++){
	        buffer[i] = akumulator;
	        akumulator = akumulator + krok;
	    }
}

void rect(int* buffer, unsigned int buflen, int krok, int prog) { //funkcja gener. s. prostokatny
	static int akumulator = 0;
	 int i;
	    for(i=0; i<buflen; i++){
	    	if (akumulator > prog) //warunek decydujacy o poziomie sygnalu
	        buffer[i] = 32767;
	    	else
	    	buffer[i] = -32768;

	        akumulator = akumulator + krok;
	    }
}


void tri(int* buffer, unsigned int buflen, int krok) { // funkcja gener. s. trojkatny
	static int akumulator = 0;
	 int i;
	    for(i=0; i<buflen; i++){
	        buffer[i] = ((akumulator < 0 ? -akumulator : akumulator) << 1) - 32767;
	        akumulator = akumulator + krok;
	    }
}


void sint(int* buffer, unsigned int buflen, int krok) { // funkcja gener. sinus na podstawie szeregu Taylora
	 static int akumulator = 0;
	 int x = 0;
	 int i;
	 long y;
	 bool ujem = true; //zmienna pomagajaca okreslic czy oryginalny x byl dodatni czy ujemny

	 //definicja wspolczynnikow (mnozenie razy 4096, gdyz zapisujemy w formacie Q3.12)

	 long a1 = round(M_PI*4096);
	 long a3 = round((-pow(M_PI, 3)/silnia(3))*4096);
	 long a5 = round((pow(M_PI, 5)/silnia(5))*4096);
	 long a7 = round((-pow(M_PI, 7)/silnia(7))*4096);

	 //glowna petla

	    for(i=0; i<buflen; i++){
	    	if(x>0)
	    	ujem = false;  //zmienna bool przechowuj¹ca informacje o tym czy oryginalne x jest dodatnie

	    	if((x < 0 ? -x : x) > 16384)
	    	x = 32767 - x;

	    	//obliczanie poteg x

	    	int x2 = _smpy(x, x);
	    	int x3 = _smpy(x2, x);
	    	int x5 = _smpy(x2, x3);
	    	int x7 = _smpy(x2, x5);

	    	y = a1 * x + a3 * x3 + a5 * x5 + a7 * x7; // obliczanie szeregu taylora
	    	y = (y + (1<<11)) >> 12; //zaokr¹glenie i przesuniêcie bitowe

	    	if (ujem == true)
	    	buffer[i] = (int) -y;
	    	else
	    	buffer[i] = (int) y;

	    	akumulator = akumulator + krok;
	    	x = akumulator;
	    }
}


void main(void) {

	//int t = 16384; // wartosc progowa

	//wywolywanie funkcji

	saw(samples, NUM_SAMPLES, d);

	//rect(samples, NUM_SAMPLES, d, t);

	//tri(samples, NUM_SAMPLES, d);

	//sint(samples, NUM_SAMPLES, d);

	//sine((DATA*) samples, (DATA*) samples, NUM_SAMPLES);

	//rand16init();
	//rand16((DATA*) samples, NUM_SAMPLES);



	while (1); // do not exit
}
