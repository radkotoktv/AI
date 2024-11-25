#include <iostream>
#include <math.h>
#include <vector>
#include <chrono>

#define FOUND -10

int n, zeroIndex;
std::vector<std::string> moves;

std::pair<int, int> getIndexes(int num) {
    if (num == 0) {
        if (zeroIndex == -1) return {sqrt(n + 1) - 1, sqrt(n + 1) - 1};
        if(zeroIndex == 0) return {0, 0};
        
        std::pair<int, int> newIndexes = getIndexes(zeroIndex);
        if(newIndexes.second < sqrt(n + 1) - 1) return {newIndexes.first, newIndexes.second + 1};
        return {newIndexes.first + 1, 0};
    }
    if(zeroIndex != -1 && num > zeroIndex) {
        return {num / sqrt(n + 1), num % (int)sqrt(n + 1)};
    }
    return {(num - 1) / sqrt(n + 1), (num - 1) % (int)sqrt(n + 1)};
}

int calculateManhattan(std::vector<std::vector<int>>& matrix) {
    int size = sqrt(n + 1);
    int result = 0;
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            if(matrix[i][j] == 0) continue;
            std::pair<int, int> indexes = getIndexes(matrix[i][j]);
            result += abs(indexes.first - i) + abs(indexes.second - j);
        }
    }
    return result;
}

int ida_star(std::vector<std::vector<int>>& matrix, int g, int bound, std::string lastMove) {
    int manh = calculateManhattan(matrix);
    int f = g + manh;
    if(f > bound) return f;
    if(manh == 0) return FOUND;
    int min = 1000000;
    for(int i = 0; i < sqrt(n + 1); i++) {
        for(int j = 0; j < sqrt(n + 1); j++) {
            if(matrix[i][j] == 0) {
                if(i < sqrt(n + 1) - 1) {
                    std::swap(matrix[i][j], matrix[i + 1][j]);
                    if(lastMove != "down") {
                        int t = ida_star(matrix, g + 1, bound, "up");
                        if(t == FOUND) {
                            moves.push_back("up");
                            return FOUND;
                        }
                        if(t < min) min = t;
                    }
                    std::swap(matrix[i][j], matrix[i + 1][j]);
                }
                if(i > 0) {
                    std::swap(matrix[i][j], matrix[i - 1][j]);
                    if(lastMove != "up") {
                        int t = ida_star(matrix, g + 1, bound, "down");
                        if(t == FOUND) {
                            moves.push_back("down");
                            return FOUND;
                        }
                        if(t < min) min = t;
                    }
                    std::swap(matrix[i][j], matrix[i - 1][j]);
                }
                if(j > 0) {
                    std::swap(matrix[i][j], matrix[i][j - 1]);
                    if(lastMove != "left") {
                        int t = ida_star(matrix, g + 1, bound, "right");
                        if(t == FOUND) {
                            moves.push_back("right");
                            return FOUND;
                        }
                        if(t < min) min = t;
                    }
                    std::swap(matrix[i][j], matrix[i][j - 1]);
                }
                if(j < sqrt(n + 1) - 1) {
                    std::swap(matrix[i][j], matrix[i][j + 1]);
                    if(lastMove != "right") {
                        int t = ida_star(matrix, g + 1, bound, "left");
                        if(t == FOUND) {
                            moves.push_back("left");
                            return FOUND;
                        }
                        if(t < min) min = t;
                    }
                    std::swap(matrix[i][j], matrix[i][j + 1]);
                }
            }
        }
    }
    return min;
}

int calculateInversions(std::vector<std::vector<int>>& matrix) {
    int size = sqrt(n + 1);
    std::vector<int> flatMatrix;
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            flatMatrix.push_back(matrix[i][j]);
        }
    }

    int counter = 0;
    for(int i = 0; i <= n; i++) {
        for(int j = i + 1; j <= n; j++) {
            if(flatMatrix[i] > flatMatrix[j] && flatMatrix[i] != 0 && flatMatrix[j] != 0) {
                counter++;
            }
        }
    }
    return counter;
}

bool isSolvable(std::vector<std::vector<int>>& matrix) {
    int size = sqrt(n + 1);
    int inversions = calculateInversions(matrix);
    int blankRow = getIndexes(0).first;

    if (size % 2 != 0)
    {
        return inversions % 2 == 0;
    }
    else
    {
        return (inversions + blankRow) % 2 != 0;
    }
}

int main() {
    std::cin >> n >> zeroIndex;
    
    int size = sqrt(n + 1);
    std::vector<std::vector<int>> matrix(size, std::vector<int>(size, 0));

    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            std::cin >> matrix[i][j];
        }
    }

    auto start = std::chrono::high_resolution_clock::now();
    if(!isSolvable(matrix)) {
        std::cout << "-1";
        return 0;
    }

    int bound = calculateManhattan(matrix);
    if(bound == 0) {
        std::cout << "0";
        return 0;
    }

    while(true) {
        int t = ida_star(matrix, 0, bound, "");
        if(t == FOUND) {
            break;
        }
        bound = t;
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << moves.size() << std::endl;
    for(int i = moves.size() - 1; i >= 0; i--) {
            std::cout << moves[i] << std::endl;
    }

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds!" << std::endl;
}

//15 -1
// 9 5 1 12
// 10 0 11 13
// 3 7 14 6
// 2 8 15 4
// < 30s