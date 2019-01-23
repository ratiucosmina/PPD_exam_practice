#include<iostream>
#include<thread>
#include<mutex>
using namespace std;

int nrThreads;
int n;
int* a, *b, *r;
thread* th;

void convolution(int threadId) {
	int start = (n*threadId) / nrThreads;
	int end = (n*(threadId + 1)) / nrThreads;

	//fiecare thread calculeaza o parte din rezultat
	for (int i = start; i < end; i++) {
		//acest thread e singurul care modifica acest r[i], deci nu am nevoie de mutex
		r[i] = 0;
		for (int j = 0; j < n; j++) {
			int second_index = i - j;

			//asta e partea de modulo n, daca i-j e negativ, i-j modulo n e n+i-j (-1 modulo 3 e 2)
			if (second_index < 0)
				second_index = n + second_index;
			r[i] += a[j] * b[second_index];
		}
	}
}

int main() {

	cout << "N=";
	cin >> n;

	a = new int[n];
	b = new int[n];
	r = new int[n];
	for (int i = 0; i < n; i++) {
		cout << "a[" << i << "]= ";
		cin >> a[i];
	}

	for (int i = 0; i < n; i++) {
		cout << "b[" << i << "]= ";
		cin >> b[i];
	}

	cout << "Number of threads: ";
	cin >> nrThreads;

	th = new thread[nrThreads];

	for (int i = 0; i < nrThreads; i++)
		th[i] = thread(convolution, i);

	for (int i = 0; i < nrThreads; i++)
		th[i].join();

	for (int i = 0; i < n; i++)
		cout << r[i] << " ";

	system("pause");
	return 0;
}