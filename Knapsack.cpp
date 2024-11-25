#include <iostream>
#include <vector>
#include <algorithm>

int m;
int n;
std::vector<std::pair<int, int>> items;;

static bool cmp(int a, int b) {
    return a > b;
}

std::vector<std::vector<int>> initIndividuals() {
    std::vector<std::vector<int>> individuals;
    for(int i = 0; i < m; i++) {
        std::vector<int> individual;
        for(int j = 0; j < n; j++) {
            individual.push_back(rand() % 2);
        }
        individuals.push_back(individual);
    }
    return individuals;
}

int calculateFitness(std::vector<int> individual) {
    int fitness = 0;
    int weight = 0;
    for(int i = 0; i < n; i++) {
        fitness += individual[i] * items[i].second;
        weight += individual[i] * items[i].first;
    }
    
    if(weight > m) {
        fitness = -1;
    }
    return fitness;
}

void selection(std::vector<std::vector<int>>& individuals) {
    std::vector<std::pair<int, int>> fitnessSort;
    int amountToPop = 0;
    for(int i = 0; i < individuals.size(); i++) {
        int calculatedFitness = calculateFitness(individuals[i]);
        fitnessSort.push_back({i, calculatedFitness});
        if(calculatedFitness == -1) {
            amountToPop++;
        }
    }

    sort(fitnessSort.begin(), fitnessSort.end(), [](std::pair<int, int> a, std::pair<int, int> b) {
        return a.second > b.second;
    });

    for(int i = 0; i < individuals.size(); i++) {
        std::swap(individuals[i], individuals[fitnessSort[i].first]);
    }


    // while(individuals.size() > 0 && calculateFitness(individuals.back()) == -1) {
    //     individuals.pop_back();
    // }
}

void crossover(std::vector<std::vector<int>>& individuals) {
    int initialSize = individuals.size();
    while(individuals.size() < m - 1) {
        std::vector<int> individual1 = individuals[rand() % initialSize];
        std::vector<int> individual2 = individuals[rand() % initialSize];
        int crossoverPoint = rand() % n;
        for(int j = crossoverPoint; j < n; j++) {
            std::swap(individual1[j], individual2[j]);
        }
        individuals.push_back(individual1);
        individuals.push_back(individual2);
    }
}

void mutate(std::vector<std::vector<int>>& individuals) {
    if(individuals.size() == m) {
        return;
    }

    int mutationIndex = rand() % n;
    int mutationIndividual = rand() % m;
    std::vector<int> newIndividual = individuals[mutationIndividual];
    newIndividual[mutationIndex] = 1 - newIndividual[mutationIndex];
    individuals.push_back(newIndividual);
}

int geneticAlgorithm() {
    std::vector<std::vector<int>> individuals = initIndividuals();
    int generation = 0;
    int maxFitness = -1;
    while(generation < 1000) {
        selection(individuals);
        crossover(individuals);
        mutate(individuals);

        generation++;

        if(generation % 200 == 0) {
            std::cout << "The max fitness for generation " << generation << " is: " << maxFitness << std::endl;
        }

        for(int i = 0; i < individuals.size(); i++) {
            int fitness = calculateFitness(individuals[i]);
            if(fitness != -1) {
                maxFitness = std::max(maxFitness, fitness);
            }
        }
    }

    return maxFitness;
}

int main() {
    std::cin >> m >> n;
    for(int i = 0; i < n; i++) { 
        int a, b;
        std::cin >> a >> b;
        items.push_back({a, b});
    }

    std::cout << geneticAlgorithm();
}