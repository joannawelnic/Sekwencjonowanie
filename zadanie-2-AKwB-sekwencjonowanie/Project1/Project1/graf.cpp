#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <set>
using namespace std;
vector<int> G[40];
vector<int> H[80];
int iloscW = 0;
int maxIndeks = 1;
int maxIndeksH = 1;
vector<pair<int, int>> zbiorLukow;


void StworzGraf(fstream& file) {
	string linia;
	while (getline(file, linia)) {
		;
		stringstream strumien;
		strumien << linia;

		int nastepnik = 1;
		int wierzcholek = 1;
		char separator;
		strumien >> wierzcholek >> separator;
		while (strumien >> nastepnik) {
			G[wierzcholek].push_back(nastepnik);
		}
		iloscW += 1;
		if (wierzcholek > maxIndeks) {
			maxIndeks = wierzcholek;
		}
		maxIndeksH = maxIndeks * 2;
	}
}

void WypiszNastepnikowG() {
	for (int i = 1; i <= maxIndeks; i++) {
		cout << "Nastepnicy wierzcholka " << i << " dla grafu G" << ": ";
		for (int j = 0; j < G[i].size(); j++) {

			cout << G[i][j] << " ";
		}
		cout << endl;
	}
}

int CzySprzezony() {
	for (int i = 1; i <= maxIndeks; i++) { 
		if (G[i].size() == 0 || G[i].size() == 1) {
			continue;
		}
		int duplikat[50] = { 0 };
		for (int j = 0; j < G[i].size(); j++) {
			if (duplikat[G[i][j]] == 0) {
				duplikat[G[i][j]] = 1;
			}
			else {
				return 0;
			}
		}
	}


	for (int i = 1; i <= maxIndeks; i++)
	{
		for (int j = 1; j <= maxIndeks; j++)
		{
			if (i == j) {
				continue;
			}
			set<int> zbior(G[i].begin(), G[i].end());
			int length = zbior.size();
			for (int k = 0; k < G[j].size(); k++) {
				zbior.insert(G[j][k]);
			}
			if (!(zbior.size() == length || zbior.size() == length + G[j].size()))
				return 0;
		}
	}

	return 1;
}


void PrzeksztalcGraf() {

	for (int i = 1; i <= maxIndeks; i++) {
		zbiorLukow.push_back(make_pair(i * 2 - 1, i * 2));
	}

	for (int i = 0; i < maxIndeks; i++) {
		cout << zbiorLukow[i].first << " " << zbiorLukow[i].second << endl;
	}

	for (int i = 1; i <= maxIndeks; i++) {
		if (!G[i].empty()) {
			for (int j = 0; j < G[i].size(); j++) {
				if (zbiorLukow[i - 1].second < zbiorLukow[G[i][j] - 1].first) { 
					zbiorLukow[G[i][j] - 1].first = zbiorLukow[i - 1].second;
				}
				else {
					zbiorLukow[i - 1].second = zbiorLukow[G[i][j] - 1].first;
				}
			}
		}
	}

}


void PrzeindeksujH() {
	set<int> unikalne;

	int najmniejszy = 1;
	for (int i = 0; i < maxIndeks; i++) { 
		unikalne.insert(zbiorLukow[i].first);
		unikalne.insert(zbiorLukow[i].second);
	}


	int indeks = 1;
	for (int unikalna : unikalne) {
		for (int i = 0; i < maxIndeks; i++) {
			if (zbiorLukow[i].first == unikalna) { 
				zbiorLukow[i].first = indeks;
			}
			if (zbiorLukow[i].second == unikalna) {
				zbiorLukow[i].second = indeks;
			}
		}
		indeks++;
	}

	cout << endl;
	for (int i = 0; i < maxIndeks; i++) {
		cout << zbiorLukow[i].first << " " << zbiorLukow[i].second << endl;
	}

	for (int i = 0; i < maxIndeks; i++) {
		H[zbiorLukow[i].first].push_back(zbiorLukow[i].second);

	}
}

int CzyLiniowy() {
	for (int i = 1; i <= iloscW * 2; i++) {
		if (H[i].size() == 0 || H[i].size() == 1) {
			continue;
		}
		int duplikat[50] = { 0 };
		for (int j = 0; j < H[i].size(); j++) {
			if (duplikat[H[i][j]] == 0) {
				duplikat[H[i][j]] = 1;
			}
			else {
				return 0;
			}
		}
	}
	return 1;
}

void WypiszNastepnikowH() {
	set<int> beznastepnika;
	for (int i = 1; i <= iloscW * 2; i++) {
		if (H[i].empty()) {
			beznastepnika.insert(i);
			continue;
		}
		cout << "Nastepnicy wierzcholka " << i << " dla grafu H" << ": ";
		for (int j = 0; j < H[i].size(); j++) {
			cout << H[i][j] << " ";
		}
		cout << endl;
	}


	for (int i = 1; i <= iloscW * 2; i++) {
		for (int j = 0; j < H[i].size(); j++) {
			if (!H[i].empty()) {   
				int szukany = H[i][j];
				auto it = beznastepnika.find(szukany);
				if (it != beznastepnika.end()) { 
					cout << "Nastepnicy wierzcholka " << H[i][j] << " dla grafu H" << ": ";
					cout << endl;
				}
			}
		}
	}
}



void ZapiszDoPliku() {
	ofstream plik("graforyginalny.txt");

	if (!plik.is_open()) {
		cout << "Nie mo¿na otworzyæ pliku do zapisu!" << endl;
		return;
	}

	set<int> beznastepnika;
	for (int i = 1; i <= iloscW * 2; i++) {
		if (H[i].empty()) {
			beznastepnika.insert(i);
			continue;
		}
		plik << i << ": ";
		for (int j = 0; j < H[i].size(); j++) {
			plik << H[i][j] << " ";
		}
		plik << endl;
	}

	for (int i = 1; i <= iloscW * 2; i++) {
		for (int j = 0; j < H[i].size(); j++) {
			if (!H[i].empty()) {
				int szukany = H[i][j];
				auto it = beznastepnika.find(szukany);
				if (it != beznastepnika.end()) {
					plik << H[i][j] << ":" << endl;
				}
			}
		}
	}

	plik.close();
	cout << "Zapisano dane do pliku 'graforyginalny.txt'." << endl;
}


int main() {

	fstream file;
	file.open("graf.txt", ios::in);
	if (!file) {
		cout << "Nie mozna otworzyc pliku!";
		return 1;
	}


	StworzGraf(file);

	file.close();

	WypiszNastepnikowG();

	if (CzySprzezony() == 1) {
		cout << "\n" << "Graf jest sprzezony" << "\n";

		PrzeksztalcGraf();
		PrzeindeksujH();

		if (CzyLiniowy() == 1) {
			cout << "\n" << "Graf jest liniowy" << "\n\n";
		}
		if (CzyLiniowy() == 0) {
			cout << "\n" << "Graf nie jest liniowy" << "\n\n";
		}

		WypiszNastepnikowH();
		ZapiszDoPliku();

	}
	else {

		cout << "\n" << "Graf nie jest sprzezony" << "\n";

	}


	return 0;
}