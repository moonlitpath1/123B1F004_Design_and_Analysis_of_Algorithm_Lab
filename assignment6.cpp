#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
using namespace std;

struct SupplyItem {
    string name;
    int weight;
    int utility;
    bool isPerishable; 
};

int main() {
    int maxCapacity = 50;

    vector<SupplyItem> supplyList = {
        {"Medicine", 10, 60, true},
        {"Food", 20, 100, true},
        {"Blankets", 30, 120, false},
        {"Water", 15, 50, true},
        {"Tents", 25, 80, false}
    };

    int itemCount = supplyList.size();

    // Sort by perishability first, then utility-to-weight ratio
    sort(supplyList.begin(), supplyList.end(), [](const SupplyItem &a, const SupplyItem &b) {
        if (a.isPerishable != b.isPerishable)
            return a.isPerishable > b.isPerishable;
        return (double)a.utility / a.weight > (double)b.utility / b.weight;
    });

    // DP Table
    vector<vector<int>> dpTable(itemCount + 1, vector<int>(maxCapacity + 1, 0));

    for (int i = 1; i <= itemCount; ++i) {
        for (int capacity = 0; capacity <= maxCapacity; ++capacity) {
            if (supplyList[i-1].weight <= capacity)
                dpTable[i][capacity] = max(
                    dpTable[i-1][capacity],
                    dpTable[i-1][capacity - supplyList[i-1].weight] + supplyList[i-1].utility
                );
            else
                dpTable[i][capacity] = dpTable[i-1][capacity];
        }
    }

    // Backtracking to find chosen items
    int remainingCapacity = maxCapacity;
    vector<int> selectedItems;

    for (int i = itemCount; i >= 1; --i) {
        if (dpTable[i][remainingCapacity] != dpTable[i-1][remainingCapacity]) {
            selectedItems.push_back(i - 1);
            remainingCapacity -= supplyList[i-1].weight;
        }
    }

    // Output
    cout << "\n───────────────────────────────────────────────\n";
    cout << "     OPTIMAL RELIEF SUPPLY SELECTION REPORT\n";
    cout << "───────────────────────────────────────────────\n";
    cout << "Maximum Utility Achieved : " << dpTable[itemCount][maxCapacity] << "\n";
    cout << "Truck Capacity           : " << maxCapacity << " kg\n";
    cout << "Priority Rule            : Perishable items first\n";
    cout << "───────────────────────────────────────────────\n\n";

    cout << left << setw(15) << "Item"
         << setw(10) << "Weight"
         << setw(10) << "Utility"
         << setw(15) << "Category"
         << "\n------------------------------------------------\n";

    int usedWeight = 0;

    for (int itemIndex : selectedItems) {
        cout << left << setw(15) << supplyList[itemIndex].name
             << setw(10) << supplyList[itemIndex].weight
             << setw(10) << supplyList[itemIndex].utility
             << setw(15) << (supplyList[itemIndex].isPerishable ? "Perishable" : "Non-Perishable")
             << "\n";

        usedWeight += supplyList[itemIndex].weight;
    }

    cout << "------------------------------------------------\n";
    cout << "Total Weight Loaded     : " << usedWeight << " kg\n";
    cout << "Remaining Capacity      : " << maxCapacity - usedWeight << " kg\n";
    cout << "───────────────────────────────────────────────\n\n";

    return 0;
}
