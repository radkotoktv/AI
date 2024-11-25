#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>

int n;
std::vector<int> queens;
std::vector<int> row;
std::vector<int> diag1;
std::vector<int> diag2;

void init(int n) {
    queens.resize(n);
    row.assign(n, 0);
    diag1.assign(2 * n - 1, 0);
    diag2.assign(2 * n - 1, 0);

    for (int i = 0; i < n; i++) {
        int x = rand() % n;
        queens[i] = x;
        row[x]++;
        diag1[i - x + n - 1]++;
        diag2[i + x]++;
    }
}

int getColumnWithQueenWithMaxConflicts() {
    int maxConflicts = 0;
    std::vector<int> bestQueens;
    for (int i = 0; i < n; i++) {
        int conflicts = row[queens[i]] + diag1[i - queens[i] + n - 1] + diag2[i + queens[i]] - 3;
        if (conflicts > maxConflicts) {
            maxConflicts = conflicts;
        }
    }

    for (int i = 0; i < n; i++) {
        int conflicts = row[queens[i]] + diag1[i - queens[i] + n - 1] + diag2[i + queens[i]] - 3;
        if (conflicts == maxConflicts) {
            bestQueens.push_back(i);
        }
    }
    return bestQueens.empty() ? -1 : bestQueens[rand() % bestQueens.size()];
}

int getRowWithMinConflicts(int col) {
    int minConflicts = n;
    std::vector<int> bestRows;

    for (int i = 0; i < n; i++) {
        int conflicts = row[i] + diag1[col - i + n - 1] + diag2[i + col];
        if (conflicts < minConflicts) {
            minConflicts = conflicts;
        }
    }

    for (int i = 0; i < n; i++) {
        int conflicts = row[i] + diag1[col - i + n - 1] + diag2[i + col];
        if (conflicts == minConflicts) {
            bestRows.push_back(i);
        }
    }
    return bestRows.empty() ? -1 : bestRows[rand() % bestRows.size()];
}

bool hasConflicts() {
    for (int i = 0; i < n; i++) {
        if (row[queens[i]] > 1 || diag1[i - queens[i] + n - 1] > 1 || diag2[i + queens[i]] > 1) {
            return true;
        }
    }
    return false;
}

void solve(int n) {
    int iter = 0;
    while (iter++ < n * n) {
        int bestCol = getColumnWithQueenWithMaxConflicts();
        if (bestCol == -1 || (row[queens[bestCol]] == 1 && diag1[bestCol - queens[bestCol] + n - 1] == 1 && diag2[bestCol + queens[bestCol]] == 1) ) {
            break;
        }

        int bestRow = getRowWithMinConflicts(bestCol);

        if (bestRow != queens[bestCol]) {
            row[queens[bestCol]]--;
            diag1[bestCol - queens[bestCol] + n - 1]--;
            diag2[bestCol + queens[bestCol]]--;
            
            queens[bestCol] = bestRow;

            row[bestRow]++;
            diag1[bestCol - bestRow + n - 1]++;
            diag2[bestCol + bestRow]++;
        }
    }

    if (hasConflicts()) {
        solve(n);
    }
}

int main() {
    std::cin >> n;
    auto start = std::chrono::high_resolution_clock::now();
    if (n == 2 || n == 3) {
        std::cout << "-1" << std::endl;
        return 0;
    }
    
    init(n);
    solve(n);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Solution found for " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds!" << std::endl;
    
    if (n <= 40) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                std::cout << (queens[j] == i ? "* " : "- ");
            }
            std::cout << std::endl;
        }
    }
}
