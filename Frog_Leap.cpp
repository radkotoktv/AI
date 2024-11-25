#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <utility>

using namespace std;

int n;
int size;
string goalState;
vector<string> fullPath;

string makeStartState() {
    string startState = string(n, '<') + "_" + string(n, '>');
    return startState;
}

string makeGoalState() {
    string goalState = string(n, '>') + "_" + string(n, '<');
    return goalState;
}

bool isGoalState(const string &board) {
    return board == goalState;
}

bool dfs(string &board, int zeroPos) {
    if (isGoalState(board)) {
        fullPath.push_back(board);
        return true;
    }

    if (zeroPos - 1 >= 0 && board[zeroPos - 1] == '<') {
        swap(board[zeroPos], board[zeroPos - 1]);
        if(dfs(board, zeroPos - 1)) {
            swap(board[zeroPos], board[zeroPos - 1]);
            fullPath.push_back(board);
            return true;
        }
        swap(board[zeroPos], board[zeroPos - 1]);
    }

    if (zeroPos - 2 >= 0 && board[zeroPos - 2] == '<') {
        swap(board[zeroPos], board[zeroPos - 2]);
        if(dfs(board, zeroPos - 2)) {
            swap(board[zeroPos], board[zeroPos - 2]);
            fullPath.push_back(board);
            return true;
        }
        swap(board[zeroPos], board[zeroPos - 2]);
    }


    if (zeroPos + 1 < size && board[zeroPos + 1] == '>') {
        swap(board[zeroPos], board[zeroPos + 1]);
        if(dfs(board, zeroPos + 1)) {
            swap(board[zeroPos], board[zeroPos + 1]);
            fullPath.push_back(board);
            return true;
        }
        swap(board[zeroPos], board[zeroPos + 1]);
    }
    if (zeroPos + 2 < size && board[zeroPos + 2] == '>') {
        swap(board[zeroPos], board[zeroPos + 2]);
        if(dfs(board, zeroPos + 2)) {
            swap(board[zeroPos], board[zeroPos + 2]);
            fullPath.push_back(board);
            return true;
        }
        swap(board[zeroPos], board[zeroPos + 2]);
    }

    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
 
    cin >> n;

    size = (n * 2) + 1;
    string startState = makeStartState();
    goalState = makeGoalState();
    int zeroPos = n;

    
    auto start = chrono::system_clock::now();
    dfs(startState, zeroPos);
    auto end = chrono::system_clock::now();

    chrono::duration<double> elapsed = end - start;

    for(string s : fullPath) {
        cout << s << endl;
    }

    cout << "The frog leap puzzle was solved in " << elapsed.count() << " seconds!" << endl;

    return 0;
}