/*  Knapsack 0-1
    File: knapsack.cpp
    Author: Grisha Hatavets
    Date: May 1, 2023
*/
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

vector<int> WEIGHTS; // array of WEIGHTS

/// @brief returns the max of two integers
/// @param int a
/// @param int b 
/// @return int
int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

/// @brief creates dynamic programming table
/// @param n 
/// @param target 
/// @param values
/// @return vector<vector<int>> table
vector<vector<int> > create_table (int n, int target, vector<int> values) {
    vector<vector<int>> table;
    vector<int> row;
    for (int i = 0; i <= n; i++) {
        table.push_back(row);
        for (int w = 0; w <= target; w++) {
            // Case 1: first row and first column of each row
            if (i == 0) {
                table[i].push_back(0);
            }
            // Case 2: item's weight is greater than the column weight
            else if (WEIGHTS[i - 1] > w) {
                table[i].push_back(table[i-1][w]); 
            }
            // Case 3: item's weight is at most the column weight
            else {
                table[i].push_back(max(table[i-1][w], table[i-1][w-WEIGHTS[i-1]] + values[i - 1]));
            }
        }
    }
    return table;
}

/// @brief gathers the input information from file and calls create_table function
/// @param filename 
/// @return vector<vector<int>> table
vector<vector<int>> build_from_file(string filename) {
    int n; // number of items
    int target; // target weight
    vector<int> values; // array of values

    ifstream instream(filename);
    string word; // ignored
    instream >> word;
    instream >> n;
    instream >> word;
    instream >> target;
    instream >> word;
    int coeff;
    for (int i = 0; i < n; i++) {
        instream >> coeff;
        values.push_back(coeff);
    }
    instream >> word;
    for (int i = 0; i < n; i++) {
        instream >> coeff;
        WEIGHTS.push_back(coeff);
    }
    instream.close();
    return create_table(n, target, values); // creates table
}

/// @brief find the subset of items which will maximize the objective value
/// @param table 
/// @return vector<int> items
vector<int> find_max_subset(vector<vector<int>> table) {
    vector<int> items;
    int i = table.size() - 1;
    int j = table[0].size() - 1;
    while (table[i][j] != 0) {
        if (table[i - 1][j] == table[i][j]) {
            i--;
        }
        else {
            items.push_back(i);
            j = j - WEIGHTS[i - 1];
            i--;
        }
    }
    return items;
}

/// @brief main
/// @param argc 
/// @param argv 
/// @return 1 if feasible solution was found
///         0 otherwise
int main(int argc, char ** argv) {
    string filename = argv[1];
    vector<vector<int>> table = build_from_file(filename);
    int max_value = table[table.size() - 1][table[0].size() - 1];
    vector<int> solution = find_max_subset(table);
    
    cout << "Max value: " << max_value << endl;
    if (solution.size() == 0) {
        cout << "No feasible solution was found." << endl;
        return 0;
    }
    cout << "Items to pack: {";
    for (int i = solution.size() - 1; i > 0; i--) {
        cout << solution[i] << ", ";
    }
    cout << solution[0] << "}" << endl;
    return 1;
}
