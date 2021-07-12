#include <dsplib.h>
#include <testsignal.h>
#include <hamming.h> //tablica wspolczynnikow okna Hamminga

#define NUM_SAMPLES 2048
int samples[NUM_SAMPLES];
int autocor[NUM_SAMPLES];

void amplituda(DATA *buffer, int buflen){
	int i=0;
	int y=2;

	//pêtla implementuj¹ca funkcjê okna (zad. 3)

	//int j=0;

	//while(j<buflen){
		//buffer[j] = _smpy(buffer[j], hamming[j]);
		//j++;
	//}

	//funkcja tworzaca widmo zespolone

	rfft((DATA*) buffer, NUM_SAMPLES, SCALE);

	//pêtla obliczaj¹ca modu³y z kolejnych wartoci widma zespolonego

	for (; y<buflen; y=y+2){
		short re2 = _smpy(buffer[y], buffer[y]);
		short im2 = _smpy(buffer[y+1], buffer[y+1]);
		buffer[i] = im2 + re2;
		i++;
	}

	//funkcja pierwiastkuj¹ca otrzymane wartosci

	sqrt_16((DATA*) buffer, (DATA*) buffer, 1024);
}

short maksimum(DATA *buffer, DATA* pochodne, int buflen, int podloga_szum){
	int i = 0;
	int max_amplituda = 0;
	short max_index;

	//pêtla licz¹ca pochodne i zapisujaca maksima

	for(; i<buflen; i++){

		pochodne[i] = buffer[i+1]-buffer[i];

		//sprawdzanie zmiany znaku pochodnych - czy ist. maksimum
		if((pochodne[i-1] > 0) && (pochodne[i] < 0)){
			//sprawdzenie czy znalezione maksimum jest najwiêksze
			if ((buffer[i] > max_amplituda) && (buffer[i] > podloga_szum)){
			//zapisanie maksimum i indeksu
			max_amplituda = buffer[i];
			max_index = i;
			}
		}
	}

	return max_index;
}

//funkcja obliczajaca czestotliwosc podstawowa

int max_f(short index){
	int krok = (48000 >> 11);
	int wynik = krok * index;
	return wynik;
}

//funkcja tworzaca bufor uzyty do f. autokorel.

void bufor(DATA *wejscie, DATA* wyjscie, int buflen){
	int i = 0;
	for(;i<buflen;i++){
		wyjscie[i] = (wejscie[i] >> 4);
	}
}

short autocor_maksimum(DATA *buffer, DATA *pochodne, int buflen){
	int i = 0;
	int max_amplituda = 0;
	short max_index;

	//petla liczaca pochodne i zapisujaca maksima
	for(; i<buflen; i++){

	pochodne[i] = buffer[i+1]-buffer[i];

	//sprawdzanie zmiany znaku pochodnych - czy ist. maksimum
	if((pochodne[i-2] > 0) && (pochodne[i] < 0)){
		//sprawdzenie czy znalezione maksimum jest najwiêksze
		if (buffer[i] > max_amplituda){
		//zapisanie maksimum i indeksu
		max_amplituda = buffer[i];
		max_index = i;
			}
		}
	}
	return max_index;
}


void main(void) {

	//tablica magazynujaca pochodne
	int pochodne[1024];

	//wywolanie f. tworzacej widmo amplitudowe (zad. 2)
	//amplituda((DATA*) testsignal, NUM_SAMPLES);

	//wywolanie f. znajdujacej indeks maksimum widma
	//short index = maksimum((DATA*) testsignal, (DATA*) pochodne, 1024, 500);

	//wywolanie f. liczaca czestotliwosc na podstawie znal. indeksu maksimum (zad. 4)
	//int wynik = max_f(index);

	//wywolanie f. tworzacej bufor do f. autokorelacji
	bufor((DATA*) testsignal, (DATA*) samples, NUM_SAMPLES);

	//wywolanie f. autokorelacji
	acorr((DATA*) samples, (DATA*) autocor, 1000, 1000, bias);

	//wywolanie f. znajdujacej indeks maksimum f. autokorelacji
	short max = autocor_maksimum((DATA*) autocor, (DATA*) pochodne, NUM_SAMPLES);

 	while (1); // do not exit
}
