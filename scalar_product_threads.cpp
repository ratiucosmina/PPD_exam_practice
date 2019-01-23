#include<iostream>
#include<thread>
#include<future>
#include<vector>
#include<mutex>

using namespace std;

int res;
mutex mtx;


void scalar_product(int threadId, int nrThreads, vector<int> a, vector<int> b) {
	int start = (a.size()*threadId) / nrThreads;
	int end = (a.size()*(threadId + 1)) / nrThreads;
	int result = 0;

	//fiecare thread calculeaza suma de a[i]b[i] pentru i in [start,end)
	for (int i = start; i < end; i++)
		result += a[i] * b[i];
	
	//lock mutex si adun la rezultat ce am obtinut
	unique_lock<mutex> lock(mtx);
	res += result;
}

int main() {
	vector<int> a, b;
	int n,nrThreads,nr;
	
	cout << "n=";
	cin >> n;

	for (int i = 0; i < n; i++) {
		cout << "a[" << i << "]=";
		cin >> nr;
		a.push_back(nr);
	}

	for (int i = 0; i < n; i++) {
		cout << "b[" << i << "]=";
		cin >> nr;
		b.push_back(nr);
	}

	cout << "Number of threads:";
	cin >> nrThreads;

	thread* th = new thread[nrThreads];
	for (int i = 0; i < nrThreads; i++)
		th[i] = thread(scalar_product, i, nrThreads, a, b);

	for (int i = 0; i < nrThreads; i++)
		th[i].join();
	cout << res;
	delete(th);
	system("pause");


	return 0;
}