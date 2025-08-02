#include <iostream>
#include <string>
#include <limits>
#include <cctype>

using namespace std;

// Helper: Get a validated integer input within a specific range
int getValidatedInt(const string& prompt, int min, int max) {
    int value;
    while (true) {
        cout << prompt << endl;
        if (cin >> value) {
            if (value >= min && value <= max) {
                break;
            } else {
                cout << "Invalid value. Please enter a number between " << min << " and " << max << "." << endl;
            }
        } else {
            cout << "Invalid input. Please enter a valid integer value." << endl;
            cin.clear(); // clear error flag
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear newline after valid input
    return value;
}

// Helper: Get a non-empty, alphabetic string (letters and spaces only)
string getValidatedString(const string& prompt) {
    string input;
    while (true) {
        cout << prompt << endl;
        getline(cin, input);

        bool valid = true;
        bool has_non_space = false;

        for (char c : input) {
            if (isalpha(c) || isspace(c)) {
                if (!isspace(c)) has_non_space = true;
            } else {
                valid = false;
                break;
            }
        }

        if (valid && has_non_space) {
            break;
        } else {
            cout << "Invalid input. Please enter a name using only letters and spaces." << endl;
        }
    }
    return input;
}

int Requirements() {
    int budget;
    int overallRating;
    int teamChemistry;
    string nation;
    string league;
    string club;
    int nationReq;
    int leagueReq;
    int clubReq;

    budget = getValidatedInt("What is your squad budget?", 0, numeric_limits<int>::max());

    overallRating = getValidatedInt("What is your required squad overall rating? (0-99)", 0, 99);

    teamChemistry = getValidatedInt("What is your required squad team chemistry? (0-33)", 0, 33);

    club = getValidatedString("Are there any club requirements?");
    nation = getValidatedString("Are there any nation requirements?");
    league = getValidatedString("Are there any league requirements?");

    nationReq = getValidatedInt("How many players need to be from this nation: " + nation + " (1-11)", 1, 11);
    clubReq = getValidatedInt("How many players need to be from this club: " + club + " (1-11)", 1, 11);
    leagueReq = getValidatedInt("How many players need to be from this league: " + league + " (1-11)", 1, 11);

    cout << "\n--- Squad Requirements Summary ---" << endl;
    cout << "Team Budget: " << budget << endl;
    cout << "Overall Rating: " << overallRating << endl;
    cout << "Team Chemistry: " << teamChemistry << endl;
    cout << "Nation Requirements: " << nationReq << " from " << nation << endl;
    cout << "League Requirements: " << leagueReq << " from " << league << endl;
    cout << "Club Requirements: " << clubReq << " from " << club << endl;

    return 0;
}
