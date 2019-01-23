#include<iostream>
#include<thread>
#include<future>
#include<vector>

using namespace std;

int scalar_product(int threadId, int nrThreads, vector<int> a, vector<int> b) {
	int start = (a.size()*threadId) / nrThreads;
	int end = (a.size()*(threadId + 1)) / nrThreads;
	int result = 0;

	for (int i = start; i < end; i++)
		result += a[i] * b[i];
	return result;
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

	future<int>* futures = new future<int>[nrThreads];
	for (int i = 0; i < nrThreads; i++)
		futures[i] = async(scalar_product, i, nrThreads, a, b);

	int res = 0;
	for (int i = 0; i < nrThreads; i++)
		res += futures[i].get();
	cout << res;
	system("pause");


	return 0;
}