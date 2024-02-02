// Mtaylor
#include <bits/stdc++.h>
using namespace std;

#define pb push_back
#define fi first
#define se second
#define mp make_pair
#define endl '\n';
#define sz(x) ((int)(x).size())
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
void dbg_out() { cerr << endl; }
template <typename Head, typename... Tail>
void dbg_out(Head H, Tail... T) {
    cerr << ' ' << H;
    dbg_out(T...);
}
#define dbg(...) cerr << "(" << #__VA_ARGS__ << "):", dbg_out(__VA_ARGS__)
typedef long long ll;
const int N = 3e5 + 5;

int n, m;
char col[6][6];
vector<vector<pair<int, int> > > v;
pair<int, int> empCell = {-1, -1};
int dp[33554532];
int po[N];
bool emp[10];
int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};

void dfs(int i, int j, vector<vector<pair<int, int> > > &v,
         vector<vector<bool> > &vis, vector<pair<int, int> > &visited) {
    queue<pair<int, int> > q;

    q.push({i, j});
    vis[i][j] = 1;
    while (!q.empty()) {
        pair<int, int> p = q.front();
        int x = v[p.fi][p.se].fi;
        int y = v[p.fi][p.se].se;
        char c = col[x][y];
        q.pop();
        visited.pb(p);
        for (int k = 0; k < 4; k++) {
            int nx = p.fi + dx[k];
            int ny = p.se + dy[k];
            if (nx >= 0 && nx < n && ny >= 0 && ny < m && vis[nx][ny] == 0) {
                pair<int, int> pp = v[nx][ny];
                if (pp.fi == -1) continue;
                char cp = col[pp.fi][pp.se];
                if (c == cp) {
                    q.push({nx, ny});
                    vis[nx][ny] = 1;
                }
            }
        }
    }
}

void show(vector<vector<pair<int, int> > > &v) {
    for (auto &u : v) {
        for (auto &cell : u) {
            cout << "(" << cell.fi << "," << cell.se << ")"
                 << " ";
        }
        cout << endl;
    }
    cout << "-----------------\n";
}

void process(int mask, vector<vector<pair<int, int> > > &rs) {
    rs = v;
    for (int i = n - 1; i >= 0; i--) {
        for (int j = 0; j < m; j++) {
            int p = i * m + j;
            if (mask & po[p]) {
                rs[i][j] = {-1, -1};
            }
        }
    }

    for (int j = 0; j < m; j++) {
        emp[j] = true;
    }
    for (int j = 0; j < m; j++) {
        int lastemp = n - 1;
        for (int i = n - 1; i >= 0; i--) {
            if (rs[i][j] == empCell) {
                continue;
            }
            emp[j] = false;
            swap(rs[i][j], rs[lastemp][j]);
            lastemp--;
        }
    }
    int lastemp = 0;
    for (int j = 0; j < m; j++) {
        if (!emp[j]) {
            if (lastemp == j) {
                lastemp++;
                continue;
            }
            for (int i = 0; i < n; i++) {
                swap(rs[i][lastemp], rs[i][j]);
                rs[i][j] = empCell;
            }
            lastemp++;
        }
    }
}
int tot;

int solve(int mask) {
    if (mask == tot) {
        return 0;
    }
    int &res = dp[mask];
    if (res != -1) return res;
    res = 1e9;
    vector<vector<pair<int, int> > > rs;
    process(mask, rs);
    vector<vector<bool> > vis(n, vector<bool>(m, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (rs[i][j] != empCell && vis[i][j] == 0) {
                vector<pair<int, int> > visited;
                dfs(i, j, rs, vis, visited);
                int nmsk = 0;
                for (auto &c : visited) {
                    int x = rs[c.fi][c.se].fi;
                    int y = rs[c.fi][c.se].se;
                    int b = 1 << (x * m + y);
                    nmsk |= b;
                }
                res = min(res, 1 + solve(mask | nmsk));
            }
        }
    }
    return res;
}

void showans(int mask) {
    if (mask == tot) {
        return;
    }
    vector<vector<pair<int, int> > > rs;
    process(mask, rs);
    int res = dp[mask];
    vector<vector<bool> > vis(n, vector<bool>(m, 0));
    show(rs);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (rs[i][j] != empCell && vis[i][j] == 0) {
                vector<pair<int, int> > visited;
                dfs(i, j, rs, vis, visited);
                int nmsk = 0;
                for (auto &c : visited) {
                    int x = rs[c.fi][c.se].fi;
                    int y = rs[c.fi][c.se].se;
                    int b = 1 << (x * m + y);
                    nmsk |= b;
                }
                if (res == 1 + solve(mask | nmsk)) {
                    for (auto &c : visited) {
                        int x = rs[c.fi][c.se].fi;
                        int y = rs[c.fi][c.se].se;
                        cout << '(' << c.fi << " " << c.se << ')' << " "
                             << col[x][y] << " ";
                    }
                    cout << endl;
                    cout << i << " " << j << endl;
                    showans(mask | nmsk);
                    return;
                }
            }
        }
    }
}

void test_case() {
    memset(dp, -1, sizeof(dp));
    cin >> n >> m;
    for (int i = 0; i <= n * m; i++) {
        po[i] = 1 << (i);
    }
    tot = (1 << (n * m)) - 1;
    for (int i = 0; i < n; i++) {
        cin >> col[i];
        v.emplace_back();
        for (int j = 0; j < m; j++) {
            v[i].pb({i, j});
        }
    }
    cout << solve(0) << endl;
    //showans(0);
}

int main() {
    // freopen("input.txt", "r", stdin);
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int T = 1;
    while (T--) {
        test_case();
    }
    return 0;
}