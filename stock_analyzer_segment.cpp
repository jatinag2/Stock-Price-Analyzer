#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

class SegmentTree {
private:
    vector<int> tree;
    int n;

    void build(vector<int>& prices, int node, int start, int end) {
        if (start == end) {
            tree[node] = prices[start];
        } else {
            int mid = (start + end) / 2;
            build(prices, 2 * node, start, mid);
            build(prices, 2 * node + 1, mid + 1, end);
            tree[node] = tree[2 * node] + tree[2 * node + 1];
        }
    }

    int query(int node, int start, int end, int l, int r) {
        if (r < start || end < l) return 0;            // no overlap
        if (l <= start && end <= r) return tree[node]; // total overlap
        int mid = (start + end) / 2;                   // partial overlap
        return query(2 * node, start, mid, l, r) +
               query(2 * node + 1, mid + 1, end, l, r);
    }

    void update(int node, int start, int end, int idx, int val) {
        if (start == end) {
            tree[node] = val;
        } else {
            int mid = (start + end) / 2;
            if (idx <= mid)
                update(2 * node, start, mid, idx, val);
            else
                update(2 * node + 1, mid + 1, end, idx, val);
            tree[node] = tree[2 * node] + tree[2 * node + 1];
        }
    }

public:
    SegmentTree(vector<int>& prices) {
        n = prices.size() - 1; // prices are 1-indexed
        tree.resize(4 * n);
        build(prices, 1, 1, n);
    }

    int rangeQuery(int l, int r) {
        return query(1, 1, n, l, r);
    }

    void updatePrice(int index, int new_val) {
        update(1, 1, n, index, new_val);
    }
};

void displayMenu() {
    cout << "\n📊 Stock Price Analyzer Menu\n";
    cout << "1. Query sum & average in a date range\n";
    cout << "2. Update stock price for a day\n";
    cout << "3. Display all stock prices\n";
    cout << "4. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    srand(time(0));
    int days = 30;
    vector<int> prices(days + 1); // 1-indexed

    cout << "📈 Generated Stock Prices (Day 1 to " << days << "):\n";
    for (int i = 1; i <= days; ++i) {
        prices[i] = 100 + rand() % 401; // Random prices: 100–500
        cout << prices[i] << (i < days ? ", " : "\n");
    }

    SegmentTree seg(prices);

    while (true) {
        displayMenu();
        int choice;
        cin >> choice;

        if (choice == 1) {
            int l, r;
            cout << "Enter range (start day and end day): ";
            cin >> l >> r;
            if (l < 1 || r > days || l > r) {
                cout << "❌ Invalid range.\n";
                continue;
            }
            int sum = seg.rangeQuery(l, r);
            double avg = static_cast<double>(sum) / (r - l + 1);
            cout << "✅ Sum from Day " << l << " to Day " << r << ": " << sum << "\n";
            cout << fixed << setprecision(2);
            cout << "📉 Average price: " << avg << "\n";
        }

        else if (choice == 2) {
            int day, new_price;
            cout << "Enter the day and new price: ";
            cin >> day >> new_price;
            if (day < 1 || day > days) {
                cout << "❌ Invalid day.\n";
                continue;
            }
            seg.updatePrice(day, new_price);
            prices[day] = new_price;
            cout << "✅ Price updated for Day " << day << "\n";
        }

        else if (choice == 3) {
            cout << "📅 Current Stock Prices:\n";
            for (int i = 1; i <= days; ++i) {
                cout << "Day " << i << ": " << prices[i] << "\n";
            }
        }

        else if (choice == 4) {
            cout << "👋 Exiting Stock Price Analyzer.\n";
            break;
        }

        else {
            cout << "❌ Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
