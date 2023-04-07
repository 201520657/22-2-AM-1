#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <map>
using namespace std;

struct info {
	int r, c, d, s, g;

	info(int aa, int bb, int cc, int dd, int ee) {
		r = aa;
		c = bb;
		d = cc;
		s = dd;
		g = ee;
	}
};

int n, m, k;
int score[30];
int dr[] = { -1, 0, 1, 0 };
int dc[] = { 0, 1, 0, -1 };
int mp_g[20][20], mp_p[20][20];
vector<int> multi_gun[400];
vector<info> person;

pair<int, int> person_move(int idx) {
	int r = person[idx].r;
	int c = person[idx].c;

	int nr = r + dr[person[idx].d];
	int nc = c + dc[person[idx].d];

	if (nr < 0 || nr >= n || nc < 0 || nc >= n) {
		person[idx].d = (person[idx].d + 2) % 4;

		nr = r + dr[person[idx].d];
		nc = c + dc[person[idx].d];
	}

	mp_p[nr][nc]++;
	mp_p[r][c]--;

	person[idx].r = nr;
	person[idx].c = nc;

	return make_pair(nr, nc);
}

void gun_check(int r, int c, int idx) {
	int maxx = 0;

	if (mp_g[r][c] > 0 && person[idx].g == 0) {
		person[idx].g = mp_g[r][c];

		for (int i = 0; i < multi_gun[n * r + c].size(); i++) {
			if (multi_gun[n * r + c][i] == mp_g[r][c]) {
				multi_gun[n * r + c].erase(multi_gun[n * r + c].begin() + i);
				break;
			}
		}

		for (int i = 0; i < multi_gun[n * r + c].size(); i++) {
			maxx = max(maxx, multi_gun[n * r + c][i]);
		}

		mp_g[r][c] = maxx;
	}

	else if (mp_g[r][c] > 0 && person[idx].g > 0) {
		if (mp_g[r][c] > person[idx].g) {
			int temp = person[idx].g;

			person[idx].g = mp_g[r][c];

			for (int i = 0; i < multi_gun[n * r + c].size(); i++) {
				if (multi_gun[n * r + c][i] == mp_g[r][c]) {
					multi_gun[n * r + c][i] = temp;
					break;
				}
			}

			for (int i = 0; i < multi_gun[n * r + c].size(); i++) {
				maxx = max(maxx, multi_gun[n * r + c][i]);
			}

			mp_g[r][c] = maxx;
		}
	}
}

void after_fight(int win, int lose, int num) {
	score[win] += num;

	int maxx = 0;
	int r = person[lose].r;
	int c = person[lose].c;

	multi_gun[n * r + c].push_back(person[lose].g);
	person[lose].g = 0;

	for (int i = 0; i < multi_gun[n * r + c].size(); i++) {
		maxx = max(maxx, multi_gun[n * r + c][i]);
	}

	mp_g[r][c] = maxx;


	int nr = r + dr[person[lose].d];
	int nc = c + dc[person[lose].d];

	if (nr < 0 || nr >= n || nc < 0 || nc >= n || mp_p[nr][nc] != 0) {
		while (1) {
			if (nr >= 0 && nr < n && nc >= 0 && nc < n && mp_p[nr][nc] == 0) {
				break;
			}

			else {
				person[lose].d = (person[lose].d + 1) % 4;

				nr = r + dr[person[lose].d];
				nc = c + dc[person[lose].d];
			}
		}
	}


	mp_p[nr][nc]++;
	mp_p[r][c]--;

	person[lose].r = nr;
	person[lose].c = nc;


	if (mp_g[nr][nc] > 0) {
		maxx = 0;

		person[lose].g = mp_g[nr][nc];

		for (int i = 0; i < multi_gun[n * nr + nc].size(); i++) {
			if (multi_gun[n * nr + nc][i] == mp_g[nr][nc]) {
				multi_gun[n * nr + nc].erase(multi_gun[n * nr + nc].begin() + i);
				break;
			}
		}

		for (int i = 0; i < multi_gun[n * nr + nc].size(); i++) {
			maxx = max(maxx, multi_gun[n * nr + nc][i]);
		}

		mp_g[nr][nc] = maxx;
	}

	maxx = 0;
	r = person[win].r;
	c = person[win].c;
	
	if (mp_g[r][c] > person[win].g) {
		int temp = person[win].g;

		person[win].g = mp_g[r][c];

		for (int i = 0; i < multi_gun[n * r + c].size(); i++) {
			if (multi_gun[n * r + c][i] == mp_g[r][c]) {
				multi_gun[n * r + c][i] = temp;
				break;
			}
		}

		for (int i = 0; i < multi_gun[n * r + c].size(); i++) {
			maxx = max(maxx, multi_gun[n * r + c][i]);
		}

		mp_g[r][c] = maxx;
	}
}

void fight(int r, int c, int idx) {
	int verse_idx = 0;

	for (int j = 0; j < m; j++) {
		if (person[j].r == r && person[j].c == c && j != idx) {
			verse_idx = j;
			break;
		}
	}

	int num_1 = person[idx].s + person[idx].g;
	int num_2 = person[verse_idx].s + person[verse_idx].g;
	int res = 0;
	
	if (num_1 > num_2) {
		res = 1;
	}

	else if (num_1 == num_2) {
		if (person[idx].s >= person[verse_idx].s) {
			res = 1;
		}

		else {
			res = 2;
		}
	}

	else {
		res = 2;
	}


	if (res == 1) {
		after_fight(idx, verse_idx, abs(num_1 - num_2));
	}

	else if (res == 2) {
		after_fight(verse_idx, idx, abs(num_1 - num_2));
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	freopen("input.txt", "rt", stdin);

	cin >> n >> m >> k;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> mp_g[i][j];

			if (mp_g[i][j] > 0) {
				multi_gun[n * i + j].push_back(mp_g[i][j]);
			}
		}
	}

	for (int i = 0; i < m; i++) {
		int temp1, temp2, temp3, temp4;
		cin >> temp1 >> temp2 >> temp3 >> temp4;

		mp_p[temp1 - 1][temp2 - 1]++;
		person.push_back(info(temp1 - 1, temp2 - 1, temp3, temp4, 0));
	}


	for (int z = 0; z < k; z++) {
		for (int i = 0; i < m; i++) {
			int nr, nc;
			tie(nr, nc) = person_move(i);

			if (mp_p[nr][nc] == 1) {
				gun_check(nr, nc, i);
			}

			else if (mp_p[nr][nc] == 2) {
				fight(nr, nc, i);
			}
		}
	}

	for (int i = 0; i < m; i++) {
		cout << score[i] << " ";
	}
	cout << "\n";

	return 0;
}