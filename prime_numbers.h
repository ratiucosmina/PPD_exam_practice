#include<iostream>
#include<thread>
#include<future>
#include<vector>
#include<mutex>
#include<math.h>

using namespace std;

int N, nrThreads;
vector<thread> threads;
vector<int> numbers,initial_primes;

//generez numerele prime pana la radical din N cu ciurul lui Eratostene
vector<int> getInitialPrimes(int N) {
	vector<int> v;
	int last = sqrt(N),i;
	int d = 2;

	while (d < last) {
		
		//elimin toti multiplii lui d mergand din d in d in vector
		for (int i = d; i <= last; i += d)
			if (numbers[i] != d)
				numbers[i] = 0;		//0 nu e prim, deci il folosesc ca semn ca numarul a fost eliminat
		
		//presupun ca urmatorul numar dupa d e prim
		int i = d + 1;
		
		//daca numbers[i] e o inseamna ca nu a fost prim, asa ca merg mai departe cautand urmatorul numar prim
		while (numbers[i] == 0 && i<=last)
			i++;
		
		//cand il gasesc, il atribui lui d si ma intorc sa caut multiplii lui in vector
		d = numbers[i];
	}
	
	//pun in v numerele prime mai mici decat radical din n, adica cele care nu sunt 0 la final
	for (int i = 2; i <= last; i++)
		if (numbers[i] != 0)
			v.push_back(i);
	return v;
}

void primes(int threadId) {
	int start = (N*threadId) / nrThreads;
	int end = (N*(threadId + 1)) / nrThreads;
	int i;
	
	//fiecare thread primeste o parte din vector
	//elimina multiplii tuturor numerelor prime gasite initial (mai mici decat radical din n)
	for (int d : initial_primes) {
		
		//caut primul multiplu al lui d in vector
		i = start;
		while (numbers[i] == 0 || numbers[i]%d!=0)
			i++;

		//de acolo merg din d in d si elimin multiplii lui d
		for (int j = i; j < end; j += d) {
			if (numbers[j] != d)
				numbers[j] = 0;
		}
	}
}


int main() {
	vector<int> result;
	cout << "N=";
	cin >> N;
	cout << "Nr threads: ";
	cin >> nrThreads;
	
	//numbers e initializat cu numerele de la 0 la N
	//pun si 0 si 1 (valoarea 0 pentru amandoua pentru ca nu sunt prime) doar ca sa imi iasa frumos indicii
	numbers.push_back(0);
	numbers.push_back(0);
	for (int i = 2; i < N; i++)
		numbers.push_back(i);

	//calculez numerele prime mai mici ca radical din N
	initial_primes = getInitialPrimes(N);

	for (int i = 0; i < nrThreads; i++)
		threads.push_back(thread(primes, i));

	for (int i = 0; i < nrThreads; i++)
		threads[i].join();

	//rezultatul e format din numerele nenule ramase in vectorul numbers
	for (int i = 0; i < N; i++)
		if (numbers[i] != 0)
			result.push_back(numbers[i]);

	for (int i = 0; i < result.size(); i++)
		cout << result[i] << " ";

	system("pause");
	return 0;
}