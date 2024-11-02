#include <iostream>
#include <math.h>
#include <vector>
#include <chrono>

int n, l;
std::vector<std::string> moves;

std::pair<int, int> getIndexes(int num) {
    if (num == 0) {
        if (l == -1) return {n - 1, n - 1};
        if (l == 0) return {0, 0};
        return getIndexes(l);
    }
    return {(num - 1) / n, (num - 1) % n};
}

int calculateManhattan(std::vector<std::vector<int>>& matrix) {
    int result = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            std::pair<int, int> indexes = getIndexes(matrix[i][j]);
            result += abs(indexes.first - i) + abs(indexes.second - j);
        }
    }
    return result;
}

bool ida_star(std::vector<std::vector<int>>& matrix, int g, int bound) {
    int manh = calculateManhattan(matrix);
    int f = g + manh;
    if(f > bound) return false;
    if(manh == 0) return true;
    int min = 1000000;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(matrix[i][j] == l + 1) {
                if(i > 0) {
                    std::swap(matrix[i][j], matrix[i - 1][j]);
                    if(ida_star(matrix, g + 1, bound)) {
                        moves.push_back("down");
                        return true;
                    }
                    std::swap(matrix[i][j], matrix[i - 1][j]);
                }
                if(i < n - 1) {
                    std::swap(matrix[i][j], matrix[i + 1][j]);
                    if(ida_star(matrix, g + 1, bound)) {
                        moves.push_back("up");
                        return true;
                    }
                    std::swap(matrix[i][j], matrix[i + 1][j]);
                }
                if(j > 0) {
                    std::swap(matrix[i][j], matrix[i][j - 1]);
                    if(ida_star(matrix, g + 1, bound)) {
                        moves.push_back("right");
                        return true;
                    }
                    std::swap(matrix[i][j], matrix[i][j - 1]);
                }
                if(j < n - 1) {
                    std::swap(matrix[i][j], matrix[i][j + 1]);
                    if(ida_star(matrix, g + 1, bound)) {
                        moves.push_back("left");
                        return true;
                    }
                    std::swap(matrix[i][j], matrix[i][j + 1]);
                }
            }
        }
    }
    return false;
}

bool isSolvable(std::vector<std::vector<int>>& matrix) {
    int inversions = 0;
    std::vector<int> flatMatrix(n * n);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            flatMatrix[i * n + j] = matrix[i][j];
        }
    }
    for(int i = 0; i < n * n; i++) {
        for(int j = i + 1; j < n * n; j++) {
            if(flatMatrix[i] > flatMatrix[j] && flatMatrix[i] != 0 && flatMatrix[j] != 0) {
                inversions++;
            }
        }
    }
    if(n % 2 == 1) {
        return inversions % 2 == 0;
    } else {
        std::pair<int, int> zeroIndexes = getIndexes(0);
        return (inversions + zeroIndexes.first) % 2 == 1;
    }
}


int main() {
    std::cin >> n >> l;
    if(l == -1) {
        l = n;
    }
    n = sqrt(n + 1);
    
    std::vector<std::vector<int>> matrix(n, std::vector<int>(n, 0));

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            std::cin >> matrix[i][j];
        }
    }

    auto start = std::chrono::high_resolution_clock::now();
    if(!isSolvable(matrix)) {
        std::cout << "-1";
        return 0;
    }

    if(l != n) {
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                if(matrix[i][j] == 0) {
                    matrix[i][j] = l + 1;
                }
                else if(matrix[i][j] > l) {
                    matrix[i][j]++;
                }
            }
        }
    }

    int bound = calculateManhattan(matrix);
    if(bound == 0) {
        std::cout << "0";
        return 0;
    }

    while(true) {
        if(ida_star(matrix, 0, bound)) {
            break;
        }
        bound++;
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << moves.size() << std::endl;
    for(int i = moves.size() - 1; i >= 0; i--) {
        std::cout << moves[i] << std::endl;
    }
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds!" << std::endl;
}
