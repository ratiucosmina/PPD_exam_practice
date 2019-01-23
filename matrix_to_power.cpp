#include<iostream>
#include<thread>
#include<mutex>
using namespace std;

int nrThreads;
int n;
thread* th;

//inmultesc matricile x1 si x2, salvez rezultatul in rez (partea unul thread)
void multiply_thread(int threadId, int** x1, int** x2, int** rez) {
	int start = (n*threadId) / nrThreads;
	int end = (n*(threadId + 1)) / nrThreads;

	//impart liniile la threaduri, fiecare thread va calcula un numar aprox egal de linii din rezultat
	for (int i = start; i < end; i++)
		for (int j = 0; j < n; j++) {

			//rez[i][j]  e suma produselor de forma x1[i][k]*x2[k][j], pentru orice k
			rez[i][j] = 0;
			for (int k = 0; k < n; k++) {
				rez[i][j] += x1[i][k] * x2[k][j];
			}
		}
}

//wrapper pentru inmultirea a doua matrici
int** multiply(int** x1, int** x2) {

	//aloc spatiu pentru rezultat
	int** rez = new int*[n];
	for (int i = 0; i < n; i++)
		rez[i] = new int[n];

	//pornesc threadurile, fiecare calculeaza o parte din rezultat (cateva linii)
	for (int i = 0; i < nrThreads; i++)
		th[i] = thread(multiply_thread, i, x1, x2, rez);

	//join
	for (int i = 0; i < nrThreads; i++)
		th[i].join();

	return rez;
}

//functia care calculeaza x la puterea power
int** powerX(int**x, int power) {
	//daca power e 1, returnez x
	if (power == 1)
		return x;

	//daca power e 2, returnez rezultatul produsului x*x (produsul se face pe threaduri)
	if (power == 2) {
		return multiply(x, x);
	}

	//daca power e par, calculez recursiv x la puterea power/2
	if (power % 2 == 0) {
		int** rez = powerX(x, power / 2);

		//rezultatul va fi x^(n/2) * x^(n/2) 
		int** rez1 = multiply(rez, rez);

		//eliberez memoria ocupara de rezultatul intermediar, adica x^(n/2)
		free(rez);

		return rez1;
	}

	//daca power e impar, calculez x^(n/2) si x^((n+1)/2) recursiv
	//asta inseamna ca, de exemplu, pentru power=3 calculez x si x^2
	int** rez = powerX(x, power / 2);
	int** rez1 = powerX(x, (power + 1) / 2);

	//rezultatul final va fi produsul celor doua produse intermediare
	int** rez_final = multiply(rez, rez1);

	//eliberez memoria ocupara de produsele intemediare
	free(rez1);
	free(rez);
	return rez_final;
}

int main() {
	thread* th;
	int**x, n, nrThreads, **rez;

	//citiri si initializari
	cout << "Size of the matrix: ";
	cin >> n;

	x = new int*[n];
	for (int i = 0; i < n; i++) {
		x[i] = new int[n];
		for (int j = 0; j < n; j++) {
			cout << "x[" << i << "][" << j << "]=";
			cin >> x[i][j];
		}
	}

	cout << "Number of threads: ";
	cin >> nrThreads;
	//aloc spatiu pentru threaduri
	th = new thread[nrThreads];
	int power;
	cout << "Power:";
	cin >> power;

	//apelez functia recursiva care va calcula rezultatul ridicarii la putere
	rez = powerX(x, power);

	//printez rezultatul pentru verificare
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			cout << rez[i][j] << " ";
		cout << '\n';
	}

	system("pause");
	return 0;
}