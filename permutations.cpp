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

//primiti predicatul, nu trebuie sa il creati
//eu aici am pus unul doar de test
bool pred(vector<int> const &v) {
	for (int i = 1; i < v.size(); i++)
		if (v[i] - v[i - 1] < -2 || v[i] - v[i - 1]>2)
			return false;
	return true;
}

//calculez toate permutarile, recursiv (backtracking)
void permutations(vector<int> perm, int nr) {
	
	//daca nr e deja in permutare, nu ma intereseaza ramura asta
	auto pos = find(perm.begin(), perm.end(), nr);
	if (pos < perm.end())
		return;
	
	//daca nu, il pun in permutare
	perm.push_back(nr);
	
	//daca am ajuns la size N, am o permutare completa, verific daca respecta predicatul
	if (perm.size() == N) {
		if (pred(perm)) {
			unique_lock<mutex> lck(mtx);
			res++;
		}
	}
	//altfel, continui sa adaug elemente
	else {
		for (int i = 0; i < N; i++)
			permutations(perm, i);
	}
}

void threaded_permutations(int threadId) {
	int start = (N*threadId) / nrThreads;
	int end = (N*(threadId + 1)) / nrThreads;
	vector<int> p;
	
	//fiecare thread genereaza permutarile care incep cu numerele de la start la end
	//ex: th1 calculeaza permutarile care incep cu 1 sau cu 2, th2 pe cele care incep cu 3 sau cu 4
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

	system("pause");
	return 0;
}