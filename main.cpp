/*
    Author: Gary Travis
    Date:   08/10/2025
    Course: CS210

    File:   main.cpp
    Purpose:
    Entry point for the Corner Grocer program. Creates a CornerGrocer
    object, loads data from file, writes backup, and runs the user menu.
*/

#include <iostream>
#include <limits>
#include "ItemTracker.h"

using namespace std;

static void pauseForEnter() {
    cout << "\nPress ENTER to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    const string inputFile = "inventory.txt";
    ItemTracker tracker(inputFile);

    if (!tracker.loaded()) {
        cerr << "ERROR: Could not open \"" << inputFile << "\".\n"
            << "Make sure the file is in the program's working directory "
            << "(for Visual Studio, usually the Debug or x64\\Debug folder)." << endl;
        return 1;
    }

    int choice = -1;
    do {
        cout << "\n================ Corner Grocer ================\n"
            << "1) Look up item frequency\n"
            << "2) Print all frequencies (alphabetical)\n"
            << "3) Print all frequencies (highest count first)\n"
            << "4) Print histogram (alphabetical)\n"
            << "5) Print histogram (highest count first)\n"
            << "6) Exit\n"
            << "Choose an option: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number 1-6.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear rest of line

        switch (choice) {
        case 1: {
            cout << "Enter item name to search: ";
            string query;
            getline(cin, query);
            cout << query << " " << tracker.getFrequency(query) << "\n";
            pauseForEnter();
            break;
        }
        case 2: {
            cout << "\nItem Frequencies (A->Z):\n";
            tracker.printFrequenciesSortedAlpha(cout);
            pauseForEnter();
            break;
        }
        case 3: {
            cout << "\nItem Frequencies (highest count first):\n";
            tracker.printFrequenciesSortedByCount(cout);
            pauseForEnter();
            break;
        }
        case 4: {
            cout << "\nHistogram (A->Z):\n";
            tracker.printHistogramSortedAlpha(cout, '*'); 
            pauseForEnter();
            break;
        }
        case 5: {
            cout << "\nHistogram (highest count first):\n";
            tracker.printHistogramSortedByCount(cout, '*'); 
            pauseForEnter();
            break;
        }
        case 6:
            cout << "Exiting and writing frequency.dat...\n";
            break;

        default:
            cout << "Please choose a number 1-6.\n";
        }

    } while (choice != 6);

    // Always write frequency.dat on exit
    if (tracker.exportFrequencies("frequency.dat")) {
        cout << "Exported to frequency.dat\n";
    }
    else {
        cout << "Failed to write frequency.dat\n";
    }

    return 0;
}