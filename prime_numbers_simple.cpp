#include<iostream>
#include<thread>
#include<future>
#include<vector>
#include<mutex>
#include<math.h>

using namespace std;

int N, nrThreads;
vector<thread> threads;
vector<int> numbers;
mutex mtx;


//o functie simpla, normala, care verifica daca numarul nr e prim
bool is_prime(int nr) {
	if (nr < 2)
		return false;
	for (int d = 2; d < nr / 2; d++)
		if (nr%d == 0)
			return false;
	return true;
}

void primes(int threadId) {
	int start = (N*threadId) / nrThreads;
	int end = (N*(threadId + 1)) / nrThreads;

	//fiecare thread verifica numerele aflate in intervalul [start,end)
	for(int i=start;i<end;i++)
		if (is_prime(i)) {
			unique_lock<mutex> lck(mtx);
			numbers.push_back(i);
		}
}

int main() {
	vector<int> result;
	cout << "N=";
	cin >> N;
	cout << "Nr threads: ";
	cin >> nrThreads;
	
	for (int i = 0; i < nrThreads; i++)
		threads.push_back(thread(primes, i));

	for (int i = 0; i < nrThreads; i++)
		threads[i].join();

	for (int i = 0; i < numbers.size(); i++)
		cout << numbers[i] << " ";

	system("pause");
	return 0;
}