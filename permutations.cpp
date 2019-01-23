#include<iostream>
#include<thread>
#include<future>
#include<vector>
#include<mutex>

using namespace std;

int res;
mutex mtx;
int N, nrThreads;
vector<thread> threads;
//vector<vector<int>> good_perms;

bool pred(vector<int> const &v) {
	for (int i = 1; i < v.size(); i++)
		if (v[i] - v[i - 1] < -2 || v[i] - v[i - 1]>2)
			return false;
	return true;
}

void permutations(vector<int> perm, int nr) {
	auto pos = find(perm.begin(), perm.end(), nr);
	if (pos < perm.end())
		return;
	perm.push_back(nr);
	if (perm.size() == N) {
		if (pred(perm)) {
			unique_lock<mutex> lck(mtx);
			res++;
			//good_perms.push_back(perm);
		}
	}
	else {
		for (int i = 0; i < N; i++)
			permutations(perm, i);
	}
}

void threaded_permutations(int threadId) {
	int start = (N*threadId) / nrThreads;
	int end = (N*(threadId + 1)) / nrThreads;
	vector<int> p;
	for (int i = start; i < end; i++)
		permutations(p, i);
}

int main() {
	cout << "N=";
	cin >> N;
	cout << "Nr threads: ";
	cin >> nrThreads;

	for (int i = 0; i < nrThreads; i++) {
		threads.push_back(thread(threaded_permutations, i));
	}

	for (int i = 0; i < nrThreads; i++)
		threads[i].join();

	cout << res<<"\n";

	//for (int i = 0; i < good_perms.size(); i++) {
	//	for(int j=0;j<N;j++)
	//		cout << good_perms[i][j]<<" ";
	//	cout<<'\n';
	//}

	system("pause");
	return 0;
}