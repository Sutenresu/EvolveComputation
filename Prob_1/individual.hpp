#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <vector>
#include <iostream>
#include <cstdlib>
//#include <ctime>
//#include <stdlib.h>
#include <cmath>

#define COMPARING_TOTAL_SUMS 1
#define COMPARING_EACH_BIT 2

#define SEND_MORE 1
#define EAT_THAT 2
#define CROSS_ROADS 3
#define COCA_COLA 4
#define DONALD_GERALD 5
#define INDIVIDUAL_SIZE 10

#define WORDS_SIZE 4 // 4 4 5 4 6
#define NUMBER_DIFFERENT_LETTERS 8 // 8 6 9 6 10
#define ANSWER_SIZE 5 // 5 5 6 5 6
#define PROBLEM SEND_MORE
#define CALC_METHOD COMPARING_EACH_BIT
/*
 * In this class, it will be defined and represented an individual cromossome, adapted for cryptarithmetic
 * problems with integer values for the positions, no values repetitions, range from 0 to 9.
 * For instance, and mainly, that code should solve the following famous problem of that area:
 * 	 SEND
 * + MORE
 *  MONEY
 */

class Individual
{
public:
    Individual(int calculationMethod = CALC_METHOD); 
    //Initialize the individual with unique values from 0 to 9 for each gene
	Individual(std::vector<int> individualBase, int calculationMethod = CALC_METHOD);

    //int individualSize = INDIVIDUAL_SIZE;
    //Size in genes of the individual. Is statical because all the individuals really have the same size

    //int wordsSize;
	//int answerSize;
	//To define the problem specific characteristics

    std::vector<int> individual;
    //Stores the genes data of the individual
	long int getIndividualFitness( void );
	void setIndividualFitness( long int value );


	void setIndividual(std::vector<int> individualBase);
	//Replace an individual for other

	void printIndividual( int lengthPrint = NUMBER_DIFFERENT_LETTERS );
	int findValue( int valueToFind );

	void calculateFitness(int calculationMethod = CALC_METHOD, int problemKind = PROBLEM);
    /* Responsible to calculate the fitness of the individual for that kind of problems.
     * Case 1
     * An approach that we can use for developing an formula to calculate the fitness for
	 * cryptaritmetic problems is assigning the number for each letter in the sequence of 
	 * they appearance in the sum. For example, for SEND + MORE = MONEY, we have the
	 * individual given by SENDMORY. In that way, the simpliest formula is to calculate
	 * SEND and MORE and sum its results, and afterwards compute the absolute difference
	 * between that sum and the value found for MONEY. The bigger the worse.
     *
	 * Case 2
	 * As the case described before induces the evolution to consider different importances for each 
	 * letter, this approach compare each match, without clustering all the number for a same word in
	 * a single big number, where there are a most significant digit.
	 * So we take the mean of the differences in each digit position.
	 */
private:

	long int individualFitness;
	void printVectorInt( std::vector<int> src );
	//Keep the fitness of the individual, which has fundamental importance to the evolution that follows
};

#endif // INDIVIDUAL_H
