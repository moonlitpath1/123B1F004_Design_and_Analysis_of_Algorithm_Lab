//Anushka Badhe
//TY-IT-A
//123B1F004

/*Problem Statement
Design and implement a sorting algorithm using Merge Sort to efficiently arrange customer
orders based on their timestamps. The solution should handle a large dataset (up to 1 million
orders) with minimal computational overhead. Additionally, analyze the time complexity and
compare it with traditional sorting techniques.
*/
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
using namespace std;

struct Purchase
{
    string client;
    time_t timeStamp;
};

// print log entries in a neat table
void displayList(const vector<Purchase>& logs)
{
    cout << left << setw(15) << "Client"
         << " | " << setw(28) << "Recorded Time" << endl;
    cout << string(50, '=') << endl;

    for (const auto &row : logs)
    {
        // ctime has its own newline btw
        cout << left << setw(15) << row.client
             << " | " << setw(28) << ctime(&row.timeStamp);
    }
    cout << string(50, '-') << "\n\n";
}

// combine sorted halves
void mergeData(vector<Purchase>& data, int start, int mid, int end)
{
    int leftSize = mid - start + 1;
    int rightSize = end - mid;

    vector<Purchase> leftArr(leftSize), rightArr(rightSize);

    for (int i = 0; i < leftSize; i++)
    {
        leftArr[i] = data[start + i];
    }
    for (int j = 0; j < rightSize; j++)
    {
        rightArr[j] = data[mid + 1 + j];
    }

    int i = 0, j = 0, k = start;

    // merge back in place
    while (i < leftSize && j < rightSize)
    {
        if (leftArr[i].timeStamp <= rightArr[j].timeStamp)
        {
            data[k++] = leftArr[i++];
        }
        else
        {
            data[k++] = rightArr[j++];
        }
    }

    while (i < leftSize)
    {
        data[k++] = leftArr[i++];
    }
    while (j < rightSize)
    {
        data[k++] = rightArr[j++];
    }
}

void mergeSort(vector<Purchase>& data, int start, int end)
{
    if (start < end)
    {
        int mid = start + (end - start) / 2;
        mergeSort(data, start, mid);
        mergeSort(data, mid + 1, end);
        mergeData(data, start, mid, end);
    }
}

// bubble sort — simple but slow on big data :)
void bubbleSort(vector<Purchase>& data)
{
    int n = data.size();
    for (int i = 0; i < n - 1; i++)
    {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++)
        {
            if (data[j].timeStamp > data[j + 1].timeStamp)
            {
                swap(data[j], data[j + 1]);
                swapped = true;
            }
        }
        if (!swapped)
        {
            break; // no swaps means we're done
        }
    }
}

int main()
{
    vector<Purchase> logEntries = {
        {"Ava", 1729500000},
        {"Ben", 1729400000},
        {"Clio", 1729550000},
        {"Dax", 1729300000},
        {"Faye", 1729600000}
    };

    cout << ">>> Current Transaction Log <<<\n\n";
    displayList(logEntries);

    vector<Purchase> mSorted = logEntries;
    mergeSort(mSorted, 0, mSorted.size() - 1);
    cout << "*** Sorted via Merge Algorithm ***\n";
    displayList(mSorted);

    vector<Purchase> bSorted = logEntries;
    bubbleSort(bSorted);
    cout << "*** Sorted via Bubble Pass ***\n";
    displayList(bSorted);

    cout << "Process completed.\n";
    return 0;
}
