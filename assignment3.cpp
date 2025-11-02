#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

// Learning to write professional, self-documenting C++ code
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
