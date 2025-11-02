#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

struct Film
{
    string name;
    double score;
    int year;
    double trend; // general popularity indicator
};

// read movies from CSV file
vector<Film> importFilmsCSV(const string &path)
{
    vector<Film> items;
    ifstream inFile(path);
    string row;
    getline(inFile, row); // skip header

    while (getline(inFile, row))
    {
        istringstream ss(row);
        Film f;
        string tmp;

        getline(ss, f.name, ',');
        getline(ss, tmp, ',');  f.score = stod(tmp);
        getline(ss, tmp, ',');  f.year = stoi(tmp);
        getline(ss, tmp, ',');  f.trend = stod(tmp);

        items.push_back(f);
    }
    return items;
}

// partition for quicksort
template <typename T>
int doPartition(vector<Film> &list, int low, int high, T Film::*field, bool desc = false)
{
    T pivot = list[high].*field;
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        bool check = desc ? (list[j].*field > pivot) : (list[j].*field < pivot);
        if (check)
        {
            i++;
            swap(list[i], list[j]);
        }
    }
    swap(list[i + 1], list[high]);
    return i + 1;
}

// quicksort
template <typename T>
void quickOrder(vector<Film> &list, int low, int high, T Film::*field, bool desc = false)
{
    if (low < high)
    {
        int p = doPartition(list, low, high, field, desc);
        quickOrder(list, low, p - 1, field, desc);
        quickOrder(list, p + 1, high, field, desc);
    }
}

// show first few records
void showFilms(const vector<Film>& list, int maxRows = 10)
{
    cout << left << setw(35) << "Title"
         << setw(12) << "Score"
         << setw(12) << "Year"
         << setw(12) << "Trend" << endl;

    cout << string(72, '=') << endl;

    for (int i = 0; i < min(maxRows, (int)list.size()); i++)
    {
        cout << left << setw(35) << list[i].name
             << setw(12) << list[i].score
             << setw(12) << list[i].year
             << setw(12) << list[i].trend << endl;
    }

    if (list.size() > maxRows)
    {
        cout << "... only first " << maxRows << " of " << list.size() << " entries shown\n";
    }
}

int main()
{
    string filePath = "movies.csv";
    vector<Film> films = importFilmsCSV(filePath);

    if (films.empty())
    {
        cout << "No records found or file missing. Please verify the CSV file.\n";
        return 1;
    }

    bool running = true;

    cout << "\n=== Film Sorting Utility ===\n";

    while (running)
    {
        cout << "\nChoose sorting category:\n";
        cout << "1) Rating\n";
        cout << "2) Year Released\n";
        cout << "3) Popularity Score\n";
        cout << "4) Exit Program\n";
        cout << "Enter option: ";

        int opt;
        cin >> opt;

        bool highFirst = true; // sort highest to lowest for all

        switch (opt)
        {
            case 1:
                quickOrder(films, 0, films.size() - 1, &Film::score, highFirst);
                cout << "\n--- Sorted by Rating (highest first) ---\n\n";
                showFilms(films);
                break;

            case 2:
                quickOrder(films, 0, films.size() - 1, &Film::year, highFirst);
                cout << "\n--- Sorted by Release Year (newest first) ---\n\n";
                showFilms(films);
                break;

            case 3:
                quickOrder(films, 0, films.size() - 1, &Film::trend, highFirst);
                cout << "\n--- Sorted by Popularity Value ---\n\n";
                showFilms(films);
                break;

            case 4:
                running = false;
                cout << "\nProgram terminated. Goodbye.\n";
                break;

            default:
                cout << "Invalid selection. Please pick a number from 1–4.\n";
        }
    }

    return 0;
}
