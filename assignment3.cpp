//Anushka Badhe
//TY-IT-A
//123B1F004

/*Problem Statement
Scenario: Emergency Relief Supply Distribution
A devastating flood has hit multiple villages in a remote area, and the government, along
with NGOs, is organizing an emergency relief operation. A rescue team has a limited-
capacity boat that can carry a maximum weight of W kilograms. The boat must transport
critical supplies, including food, medicine, and drinking water, from a relief center to the
affected villages.
Each type of relief item has:
● A weight (wi) in kilograms.
● Utility value (vi) indicating its importance (e.g., medicine has higher value than food).
● Some items can be divided into smaller portions (e.g., food and water), while others must
be taken as a whole (e.g., medical kits).
As the logistics manager, you must:
1. Implement the Fractional Knapsack algorithm to maximize the total utility value of the
supplies transported.
2. Prioritize high-value items while considering weight constraints.
3. Allow partial selection of divisible items (e.g., carrying a fraction of food packets).
4. Ensure that the boat carries the most critical supplies given its weight limit W.
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Supply
{
    string name;
    double weight;
    double value;
    bool divisible;
};

// Sort supplies by value-weight efficiency
bool compareSupply(const Supply& a, const Supply& b)
{
    return (a.value / a.weight) > (b.value / b.weight);
}

// Fractional knapsack routine
void loadSupplies(double capacity, vector<Supply>& supplies)
{
    sort(supplies.begin(), supplies.end(), compareSupply);

    double totalWeight = 0.0;
    double totalValue = 0.0;

    cout << "--------------------------------------------\n";
    cout << " Supply Loading Report\n";
    cout << "--------------------------------------------\n\n";

    cout << left << setw(14) << "Item"
         << right << setw(10) << "Used"
         << setw(12) << "Load(kg)"
         << setw(12) << "Value"
         << "\n";
    cout << string(48, '-') << "\n";

    for (const auto& s : supplies)
    {
        if (capacity <= 0)
        {
            break;
        }

        if (s.divisible)
        {
            // Take proportional amount if allowed
            double used = min(capacity, s.weight);
            double val = (used / s.weight) * s.value;

            totalWeight += used;
            totalValue += val;
            capacity -= used;

            cout << left << setw(14) << s.name
                 << right << setw(9) << fixed << setprecision(1) << (used / s.weight) * 100 << "%"
                 << setw(12) << used
                 << setw(12) << val
                 << "\n";
        }
        else
        {
            if (s.weight <= capacity)
            {
                totalWeight += s.weight;
                totalValue += s.value;
                capacity -= s.weight;

                cout << left << setw(14) << s.name
                     << right << setw(9) << "100%"
                     << setw(12) << s.weight
                     << setw(12) << s.value
                     << "\n";
            }
            else
            {
                cout << left << setw(14) << s.name
                     << right << setw(9) << "0%"
                     << setw(12) << "0"
                     << setw(12) << "0"
                     << "\n";
            }
        }
    }

    cout << "\n--------------------------------------------\n";
    cout << " Summary\n";
    cout << "--------------------------------------------\n";
    cout << " Total Weight Loaded : " << totalWeight << " kg\n";
    cout << " Total Utility Value : " << totalValue << "\n";
    cout << "--------------------------------------------\n";
}

int main()
{
    double capacity = 50;
    vector<Supply> supplies =
    {
        {"Food", 30, 45, true},
        {"Water", 20, 40, true},
        {"MedicalKit", 10, 100, false},
        {"Blankets", 15, 10, false}
    };

    cout << "Boat capacity: " << capacity << " kg\n\n";
    loadSupplies(capacity, supplies);

    return 0;
}
