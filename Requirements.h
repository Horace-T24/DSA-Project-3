#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include <sstream>
#include <vector>
using namespace std;


string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}

vector<string> splitString(const string& input) {
    vector<string> result;
    stringstream ss(input);
    string item;
    while (getline(ss, item, ',')) {
        string trimmed = trim(item);
        if (!trimmed.empty()) {
            result.push_back(trimmed);
        }
    }
    return result;
}

int getValidatedInt(const string& prompt, int min, int max) {
    int value;
    while (true) {
        cout << prompt << endl;
        if (cin >> value) {
            if (value >= min && value <= max) {
                break;
            }
            else {
                cout << "Invalid value. Please enter a number between " << min << " and " << max << "." << endl;
            }
        } else {
            cout << "Invalid input. Please enter a valid integer value." << endl;
            cin.clear(); 
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    return value;
}

vector<string> getValidatedStringList(const string& prompt) {
    string input;
    vector<string> items;
    while (true) {
        cout << prompt << endl;
        getline(cin, input);
        items = splitString(input);

        bool allValid = true;
        for (const string& item : items) {
            bool hasNonSpace = false;
            for (char c : item) {
                if (!isalnum(c) && !isspace(c) && c != '-' && c != '\'' && c != '.' && c != '&') {
                    allValid = false;
                    break;
                }
                if (!isspace(c)) hasNonSpace = true;
            }
            if (!hasNonSpace) allValid = false;
            if (!allValid) break;
        }

        if (!items.empty() && allValid) {
            break;
        }
        else {
            cout << "Invalid input. Please enter names using letters, numbers, spaces, or symbols like - . ' &" << endl;
        }
    }
    return items;
}

vector<int> getValidatedIntList(const string& prompt, size_t expectedCount, int min, int max, int maxTotal) {
    string input;
    vector<int> numbers;

    while (true) {
        cout << prompt << endl;
        getline(cin, input);
        vector<string> parts = splitString(input);
        numbers.clear();

        if (parts.size() != expectedCount) {
            cout << "You must enter exactly " << expectedCount << " values." << endl;
            continue;
        }

        bool allValid = true;
        int total = 0;

        for (const string& str : parts) {
            try {
                int value = stoi(str);
                if (value < min || value > max) {
                    allValid = false;
                    break;
                }
                total += value;
                numbers.push_back(value);
            } catch (...) {
                allValid = false;
                break;
            }
        }

        if (!allValid) {
            cout << "All entries must be numbers between " << min << " and " << max << "." << endl;
        } else if (total > maxTotal) {
            cout << "Total requirement exceeds " << maxTotal << ". Please enter smaller values." << endl;
        } else {
            break;
        }
    }

    return numbers;
}

int Requirements() {
    int budget = getValidatedInt("What is your squad budget?", 0, numeric_limits<int>::max());
    int overallRating = getValidatedInt("What is your required squad overall rating? (0–99)", 0, 99);
    int teamChemistry = getValidatedInt("What is your required squad team chemistry? (0–33)", 0, 33);

    // Nation requirements
    vector<string> nations = getValidatedStringList("Enter required nations (e.g., England, France, Germany):");
    vector<int> nationCounts = getValidatedIntList("Enter number of players required from each nation (e.g., 1, 2, 3):", nations.size(), 1, 11, 11);

    // League requirements
    vector<string> leagues = getValidatedStringList("Enter required leagues (e.g., Premier League, Ligue 1):");
    vector<int> leagueCounts = getValidatedIntList("Enter number of players required from each league:", leagues.size(), 1, 11, 11);

    // Club requirements
    vector<string> clubs = getValidatedStringList("Enter required clubs (e.g., Manchester City, PSG):");
    vector<int> clubCounts = getValidatedIntList("Enter number of players required from each club:", clubs.size(), 1, 11, 11);

    // Summary output
    cout << "\n--- Squad Requirements Summary ---" << endl;
    cout << "Team Budget: " << budget << endl;
    cout << "Overall Rating: " << overallRating << endl;
    cout << "Team Chemistry: " << teamChemistry << endl;

    cout << "\nNation Requirements:" << endl;
    for (size_t i = 0; i < nations.size(); ++i) {
        cout << "- " << nations[i] << ": " << nationCounts[i] << " player(s)" << endl;
    }

    cout << "\nLeague Requirements:" << endl;
    for (size_t i = 0; i < leagues.size(); ++i) {
        cout << "- " << leagues[i] << ": " << leagueCounts[i] << " player(s)" << endl;
    }

    cout << "\nClub Requirements:" << endl;
    for (size_t i = 0; i < clubs.size(); ++i) {
        cout << "- " << clubs[i] << ": " << clubCounts[i] << " player(s)" << endl;
    }

    return 0;
}


