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

vector<int> getInitialPrimes(int N) {
	vector<int> v;
	int last = sqrt(N),i;
	int d = 2;

	while (d < last) {
		for (int i = d; i <= last; i += d)
			if (numbers[i] != d)
				numbers[i] = 0;
		int i = d + 1;
		while (numbers[i] == 0 && i<=last)
			i++;
		d = numbers[i];
	}
	for (int i = 2; i <= last; i++)
		if (numbers[i] != 0)
			v.push_back(i);
	return v;
}

void primes(int threadId) {
	int start = (N*threadId) / nrThreads;
	int end = (N*(threadId + 1)) / nrThreads;
	int i;
	for (int d : initial_primes) {
		i = start;
		while (numbers[i] == 0 || numbers[i]%d!=0)
			i++;

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
	numbers.push_back(0);
	numbers.push_back(0);	//asta ar fi 1, dar nu e prim asa ca vreau sa sar peste el de fiecare data
	for (int i = 2; i < N; i++)
		numbers.push_back(i);

	initial_primes = getInitialPrimes(N);

	for (int i = 0; i < nrThreads; i++)
		threads.push_back(thread(primes, i));

	for (int i = 0; i < nrThreads; i++)
		threads[i].join();

	for (int i = 0; i < N; i++)
		if (numbers[i] != 0)
			result.push_back(numbers[i]);

	for (int i = 0; i < result.size(); i++)
		cout << result[i] << " ";

	system("pause");
	return 0;
}