//Anushka Badhe
//TY-IT-A
//123B1F004

/*Problem Statement
Scenario: Disaster Relief Resource Allocation
A massive earthquake has struck a remote region, and a relief organization is transporting
essential supplies to the affected area. The organization has a limited-capacity relief truck that
can carry a maximum weight of W kg. They have N different types of essential items, each
with a specific weight and an associated utility value (importance in saving lives and meeting
urgent needs).
Since the truck has limited capacity, you must decide which items to include to maximize the
total utility value while ensuring the total weight does not exceed the truck's limit.
Your Task as a Logistics Coordinator:
1. Model this problem using the 0/1 Knapsack approach, where each item can either be
included in the truck (1) or not (0).
2. Implement an algorithm to find the optimal set of items that maximizes utility while
staying within the weight constraint.
3. Analyze the performance of different approaches (e.g., Brute Force, Dynamic
Programming, and Greedy Algorithms) for solving this problem efficiently.
4. Optimize for real-world constraints, such as perishable items (medicines, food) having
priority over less critical supplies.
Extend the model to consider multiple trucks or real-time decision-making for dynamic supply
chain management
*/

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
