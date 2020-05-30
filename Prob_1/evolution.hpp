#ifndef EVOLUTION_H
#define EVOLUTION_H

#include "individual.hpp"
//#include "simulation.h"
#include <vector>
#include <ctime>
#include <iostream>
#include <cstdio>
#include <ctime>
//#include <chrono>
//#include <thread>
//#include <limits>

#define CYCLE_CROSSOVER 1
#define PMX_CROSSOVER 2

#define SURVIVAL_BEST_OF_ALL 1
#define ELITISM 2

#define MAX_FITNESS 1
#define MIN_FITNESS 2

#define TOUR 1
#define ROLL 2
#define BEST_INDIVIDUAL 1

#define SURVIVAL ELITISM
#define CROSSOVER PMX_CROSSOVER
#define PARENT_SELECTION TOUR
#define ELITISM_RATE 0.02
#define CROSSOVER_RATE (1.0 - ELITISM_RATE)
#define MUTATION_RATE 0.35

#define MAX_GENERATIONS 100
#define POPULATION_SIZE 200
#define TOUR_CONTEND 2

/* DISCUSSION OF ALL ELEMENTS OF THE CODE
 * PURPOSE:
 * That class is responsible for the genetic evolution implemention. For that reason, it contains
 * variables to control the number of the generations, the rates of the occurance of mutation and
 * crossover in the populaTion.
 * 
 * ---PUBLIC-----
 * 
 * VARIABLES:
 * generationMaxNumber -> maximum generations, if not achieves success rate before
 * populationSize -> fixed population size
 * crossoverRate -> percent of population that will be generated at each generation
 * mutationRate -> percent of generated population that will suffer mutation
 * 
 * FUNCTIONS:
 * crossover -> realizes all the crossovers, receives a strategy ( 1 Cycle, 2 PMX ), a number of
 *              contenders for each Tour, and the population. outputs all the individual resultants.
 *              
 * ---PRIVATE---
 * FUNCTIONS
 * printIntVector -> Cout's a vector of ints, by each element.
 * cycleCrossover -> Receives 2 parents, output 2 children
 * pmxCrossover -> Receives 2 parents, output 2 children
 * tournaments -> Realize all the tournaments needed, output the winners in a vector of individuals
 *                For a same tournament, don't choose the same parent.
 *                For each pair of tournaments, the winner of the first is withdrawn from the population
 */

class Evolution
{
public:
    Evolution(
        float crossoverRateGiven = CROSSOVER_RATE, float mutationRateGiven = MUTATION_RATE, 
        int generationMaxNumberGiven = MAX_GENERATIONS, int populationSizeGiven = POPULATION_SIZE, 
        int numberContendersGiven = TOUR_CONTEND);

    int getGenerationMaxNumber( void );
    int getPopulationSize( void );
    int getNumberContenders( void );
    float getCrossoverRate( void );
    float getMutationRate( void );
    double getTime( void );

    void setGenerationMaxNumber( int generationWanted );
    void setPopulationSize( int populationSizeWanted );
    void setNumberContenders( int numberContendersWanted );
    void setMutationRate( float mutationRateWanted );
    void setCrossoverRate( float crossoverRateWanted );
    
    void printPresentGeneration( int flag = 0 );
    void printPopulation( std::vector<Individual> src );
    void printVectorInt( std::vector<int> src );
    int findAnotherValueVectorInt( std::vector<int> src, int positionReference );

    std::vector<Individual> survivalPopulation(
        std::vector<Individual> populationOriginal, std::vector<Individual> populationNew,
        int survivalMethod = SURVIVAL, int fitnessMethod = MIN_FITNESS,
        float elitismPercent = ELITISM_RATE);

    void calculateFitnessPopulation ( std::vector<Individual> &src );
    int stopCriteria ( void );
    int finishStatus;
    // 1 = success, 0 = fail

private:

    std::vector<Individual> population;
    int generationNumber;
    int generationMaxNumber;
    int populationSize;
    int numberContenders;
    float crossoverRate;
    // Represents the number of children individuals as a fraction of the populationSize
    float mutationRate;
    // Represents the fraction of children individuals that will suffer mutation
    double elapsedTimeSeconds;
    //storing to be external acessible, to means calculation purpose

    std::vector<Individual> runCrossover(int crossoverStrategy = CROSSOVER, int parentSelectionStrategy = PARENT_SELECTION);
    // crossoverStrategy = 1 - Cycle, 2 - PMX. Realizes the tournament to select the parents.
    std::vector<Individual> permutationMutation(std::vector<Individual> populationGenerated);
    std::vector<Individual> cycleCrossover(Individual parent1, Individual parent2);
    std::vector<Individual> pmxCrossover(Individual parent1, Individual parent2);
    std::vector<Individual> tournaments( void );
    std::vector<Individual> roulettes( void );


    int findPositionVectorInt(std::vector<int> src, int value);
    //int presentGeneration;
    //double lowerGenerationCost;
    //Individual bestIndividual;
    //std::vector<Individual> mutation;
    //std::vector<Individual> crossover;
    //std::vector<Individual> newGeneration;
   
};

#endif // EVOLUTION_H
