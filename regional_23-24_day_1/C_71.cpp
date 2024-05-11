#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef pair<ll, ll> pll;

#define f first
#define s second

void acceleration(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
}

void solve();

int main() {
    acceleration();
    solve();
}

void countCols(vector<ll>& csums, ll mask, vector<vector<ll>>& a, ll l, ll r, ll h) {
    for (ll i = 0; i < h; ++i) { 
        if (mask & (1ll << i)) { 
            for (ll j = l; j < r; ++j) { 
                csums[j - l] += a[i][j]; 
            }   
        }
    }
}

void calcPart(vector<vector<ll>>& a, vector<vector<ll>>& part, bool p, ll h, ll w) {
    ll l, r; //[l; r) <- colums indexes 
    (p == 0) ? (l = 0, r = w / 2) : (l = w / 2, r = w);

    for (ll mask = 0; mask < (1ll << h); ++mask) { // brute-force existing rows 
        vector<ll> csums(r - l); 
        countCols(csums, mask, a, l, r, h);
        for (ll mask1 = 0; mask1 < (1ll << (r - l)); ++mask1) { 
            for (ll rbit = 0; rbit < (r - l); ++rbit) {
                if (mask1 & (1ll << rbit)) {
                    part[mask][mask1] = part[mask][mask1 ^ (1ll << rbit)] + csums[rbit];
                    break;
                }
            }
        }
    }
}

void fillVector(vector<unordered_map<ll, ll>>& partSums, vector<vector<ll>>& part) {
    partSums.resize(part.size());
    for (ll mask = 0; mask < part.size(); ++mask) {
        for (ll mask1 = 0; mask1 < part[mask].size(); ++mask1) {
            partSums[mask][part[mask][mask1]] = mask1;
        }
    }
}

ll findSum(vector<unordered_map<ll, ll>>& partSums, ll mask, ll sum) {
   if (partSums[mask].count(sum)) 
      return partSums[mask][sum];
   return -1;
}

void printAns(ll mask, ll mask1, ll mask2, ll h, ll w) {
    cout << "YES" << "\n";
    ll cnt = 0;
    vector<pll> ans;
    
    for (ll i = 0; i < h; ++i) {
        if (!(mask & (1ll << i))) {
            ans.push_back({1, i + 1});
            ++cnt;
        }
    }
    for (ll j = 0; j < w; ++j) {
        if (j < w / 2) {
            if (!(mask1 & (1ll << j))) {
                ans.push_back({2, j + 1}); 
                ++cnt;
            }
        } else {
            if (!(mask2 & (1ll << (j - w / 2)))) {
                ans.push_back({2, j + 1});
                ++cnt;
            }
        }
    }

    cout << cnt << "\n";
    for (ll i = 0; i < ans.size(); ++i) {
        cout << ans[i].f << " " << ans[i].s << "\n";
    }
}

void printPart(vector<vector<ll>>& part) {
    for (ll mask = 0; mask < part.size(); ++mask) {
        for (ll mask1 = 0; mask1 < part[mask].size(); ++mask1) {
            cout << mask << " " << mask1 << " " << part[mask][mask1] << "\n";
        }
    }
}

void solve() {
    ll h, w, s;
    cin >> h >> w;
    vector<vector<ll>> a(h, vector<ll>(w));
    for (ll i = 0; i < h; ++i) {
        for (ll j = 0; j < w; ++j) {
            cin >> a[i][j];
        }
    }
    cin >> s;

    //calc left[mask][mask1], right[mask][mask1]
    ll lw = w / 2, rw = w - lw; // left [0; w/2), right [w/2, w) 
    vector<vector<ll>> left ((1ll << h), vector<ll>(1ll << lw, 0)); // 00000..1000 = 8 <=> (1ll << 33)
    vector<vector<ll>> right((1ll << h), vector<ll>(1ll << rw, 0));
    
    
    calcPart(a, left,  0, h, w);
    calcPart(a, right, 1, h, w);


    //right[mask][mask2] -> rightSums[mask][sum] = mask2
    vector<unordered_map<ll, ll>> rightSums;  
    fillVector(rightSums, right);
    
    //for each sum from the left part try to find (s - sum) in the right part with the same mask    
    for (ll mask = 0; mask < (1ll << h); ++mask) {
        for (ll mask1 = 0; mask1 < (1ll << (lw)); ++mask1) {
            ll s1 = left[mask][mask1];
            ll s2 = s - s1;

            ll mask2 = findSum(rightSums, mask, s2);
            if (mask2 != -1) {
                printAns(mask, mask1, mask2, h, w);
                return;
            }
        }
    }
    cout << "NO" << "\n";    
}
