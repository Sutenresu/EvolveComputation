#include "evolution.hpp"

// #define TESTING_EVOLUTION
// #define TESTING_TOUR
// #define TESTING_CYCLE
// #define TESTING_PMX
// #define TESTING_SURVIVAL
// #define TESTING_MUTATION
// #define TESTING_ROLL
// #define TESTING_RESPONSE


Evolution::Evolution(
    float crossoverRateGiven, float mutationRateGiven, int generationMaxNumberGiven,
    int populationSizeGiven, int numberContendersGiven)
{
    clock_t timeBegin = std::clock();

    char ch;
    
    Evolution::generationMaxNumber = generationMaxNumberGiven;
    Evolution::populationSize = populationSizeGiven;
    Evolution::numberContenders = numberContendersGiven;
    Evolution::crossoverRate = crossoverRateGiven;
    Evolution::mutationRate = mutationRateGiven;
    Evolution::generationNumber = 0;
    Evolution::population;
    std::vector<Individual> newPop;

    Evolution::finishStatus = -1;

    for ( int i = 0; i < Evolution::populationSize; i++)
    {
        Individual aux;
        Evolution::population.push_back(aux);
        if ( aux.getIndividualFitness() == 0 )
            Evolution::finishStatus = 1;
    }

    #ifdef TESTING_RESPONSE
        std::cout << "Geração " << Evolution::generationNumber << ":" << std::endl;
        Evolution::printPresentGeneration(); 
    #endif
    // #ifdef TESTING_TOUR
    //     newPop = Evolution::tournaments();
    // #endif  
    // #ifdef TESTING_CYCLE
    //     newPop = Evolution::cycleCrossover( population[0], population[1] );
    // #endif
    // #ifdef TESTING_PMX
    //     newPop = Evolution::pmxCrossover( population[0], population[1] );
    // #endif
    // #ifdef TESTING_MUTATION
    //     std::cout << std::endl;
    //     newPop = Evolution::tournaments();
    //     newPop = Evolution::permutationMutation( newPop );
    // #endif

    while( Evolution::finishStatus < 0 )
    {
        newPop = Evolution::runCrossover();
        Evolution::population = Evolution::survivalPopulation( Evolution::population, newPop );
        Evolution::generationNumber++;
        Evolution::finishStatus = Evolution::stopCriteria();
    }

    clock_t timeEnd = clock();
    Evolution::elapsedTimeSeconds = double( timeEnd - timeBegin ) / CLOCKS_PER_SEC;

    #ifdef TESTING_RESPONSE
        std::cout << "Geração " << Evolution::generationNumber << ":" << std::endl;
        Evolution::printPresentGeneration(); 
        std::cout << "Tempo gasto: " << Evolution::elapsedTimeSeconds << " segundos." << std::endl;
        std::cout << Evolution::finishStatus << std::endl;
        std::cout << "Resultado: " << ( Evolution::finishStatus ? "Sucesso" : "Falha" ) << std::endl;
    #endif

}

std::vector<Individual> Evolution::roulettes( void )
{
    std::vector<Individual> populationEdited;
    std::vector<Individual> winnerPopulation;
    //std::vector<int> contendersIndexes( Evolution::numberContenders,0 );
    //Individual backupIndividual;
    //bool repeatFlag = false;
    //int winnerIndex;
    long int worstFitness = 0;
    long int sumFitness = 0;
    populationEdited = Evolution::population;
    std::vector<int> roulette; //stores the indexes of the individuals
    #ifdef TESTING_ROLL
        char ch;

        printPopulation( populationEdited );
        ch = getchar();
    #endif
    //First step, find the worst (bigger) value of fitness, to +1 subtract from, and reorientate the fitness
    for( int i = 0; i < (int) populationEdited.size(); i++ )
    {
        if( populationEdited[i].getIndividualFitness() > worstFitness )
            worstFitness = populationEdited[i].getIndividualFitness();
    }
    #ifdef TESTING_ROLL
        std::cout << "pior: " << worstFitness << std::endl;
    #endif
    //Now reorientate and obtain the summation of the fitness
    for( int i = 0; i < (int) populationEdited.size(); i++ )
    {
        populationEdited[i].setIndividualFitness( (worstFitness + 1) - populationEdited[i].getIndividualFitness() );
        sumFitness += populationEdited[i].getIndividualFitness();
    }
    #ifdef TESTING_ROLL
        printPopulation( populationEdited );
        std::cout << "soma: " << sumFitness << std::endl;
        ch = getchar();
    #endif
    //Now normalize, setting each one to an percentual of the total
    for( int i = 0; i < (int) populationEdited.size(); i++ )
    {
        populationEdited[i].setIndividualFitness( (int) ( ( ( (float) populationEdited[i].getIndividualFitness() )/ sumFitness ) * 100 ) );
    }
    #ifdef TESTING_ROLL
        printPopulation( populationEdited );
        ch = getchar();
    #endif
    //Now initialize the roulette, setting one position to each %
    for( int i = 0; i < (int) populationEdited.size(); i++ )
    {
        for( int aux = populationEdited[i].getIndividualFitness(); aux > 0; aux-- )
            roulette.push_back( i );
    }
    #ifdef TESTING_ROLL
        printPopulation( populationEdited );
        std::cout << "Numero de roletas: " << Evolution::crossoverRate*Evolution::populationSize << std::endl;

        ch = getchar();
    #endif
    //std::srand(std::time(NULL));
    int indexPast = 0, indexDrawn = 0;
    //Roulettes
    for ( int j = 0; j < (int) ( Evolution::crossoverRate*Evolution::populationSize ); j++ )
    {   
           
        if (j % 2)
        {
        //if j is odd, verify the parent before adding. if repeated, sort again 
            indexDrawn = std::rand() % roulette.size();

            // while( roulette[ indexDrawn ] == roulette[ indexPast ] )
            // {
            //     indexDrawn = std::rand() % roulette.size();
            // }
            #ifdef TESTING_ROLL
                std::cout << "indexDrawn: " << indexDrawn << std::endl;
            #endif
            //Save the winner in the winnerPopulation
            winnerPopulation.push_back( populationEdited[ roulette[ indexDrawn ] ]  );              
       
        } else
        {
            indexDrawn = std::rand() % roulette.size();
            indexPast = indexDrawn;
            #ifdef TESTING_ROLL
                std::cout << "indexPast: " << indexDrawn << std::endl;
            #endif
            //Save the winner in the winnerPopulation
            winnerPopulation.push_back( populationEdited[ roulette[ indexDrawn ] ]  );
        }
    
    }

    return winnerPopulation;    
}

std::vector<Individual> Evolution::tournaments( void )
{
    std::vector<Individual> populationEdited;
    std::vector<Individual> winnerPopulation;
    std::vector<int> contendersIndexes( Evolution::numberContenders,0 );
    Individual backupIndividual;
    bool repeatFlag = false;
    int winnerIndex;
    
    populationEdited = Evolution::population;

    //std::srand(std::time(NULL));
    //Tournaments
    for ( int j = 0; j < (int) ( Evolution::crossoverRate*Evolution::populationSize ); j++ )
    {   
        #ifdef TESTING_TOUR
            std::cout << "\nj = " << j << std::endl;
        #endif
        //Tournament
        for ( int i = 0; i < Evolution::numberContenders; i++)
        {
            contendersIndexes[i]= std::rand() % populationEdited.size();
            //First Selected
            if ( !i )
            {
                winnerIndex = contendersIndexes[i]; //population[ contendersIndexes[i] ].getIndividualFitness() );
            }
            else
            {
                //Others
                for ( int w = i-1; w >= 0; w--)
                {
                    //Check if is repeated
                    if ( contendersIndexes[i] == contendersIndexes[w] )
                        repeatFlag = true;
                }

                if ( repeatFlag )
                {
                    //Repeated
                    //Decrement i to sort another individual
                    i--;
                    repeatFlag = false;
                }
                else
                {
                    //Not-Repeated, check if is better than the "winner until now"
                    if (populationEdited[ winnerIndex ].getIndividualFitness() > populationEdited[ contendersIndexes[i] ].getIndividualFitness())
                        winnerIndex = contendersIndexes[i];
                }
            }
        }

        #ifdef TESTING_TOUR
            for (int i = 0; i < Evolution::numberContenders; i++)
            {
                //std::cout << "indiv i: " << contendersIndexes[i] << std::endl;
                //std::cout << "Individuals: "<< std::endl;
                populationEdited[ contendersIndexes[i] ].printIndividual();
            }
            std::cout << "winnerIndex " << winnerIndex << std::endl;
            std::cout << "Individual:\n ";
            populationEdited[ winnerIndex ].printIndividual();
            //ch = getchar();
        #endif

        //Save the winner in the winnerPopulation and removes from Edited for avoid repetition
        winnerPopulation.push_back( populationEdited[ winnerIndex ] );
        if ( !(j % 2) )
        {
        //if j is even, I have the first parent for a future crossover, which I don't want
        //to repeat, so I save him, to restore him after I obtain the parent 2
            backupIndividual = populationEdited [ winnerIndex ];
            populationEdited.erase(populationEdited.begin() + winnerIndex);
        
        } else {

            populationEdited.push_back( backupIndividual );

        }
    }
    #ifdef TESTING_TOUR
        std::cout << "\nwinnerPopulation " << std::endl;
        for ( int i = 0; i < (int) winnerPopulation.size(); i++ )
        {
            winnerPopulation[ i ].printIndividual();
        }
    #endif

    #ifdef TESTING_TOUR
        std::cout << "\npopulationEdited " << std::endl;
        for ( int i = 0; i < (int) populationEdited.size(); i++ )
        {
            populationEdited[ i ].printIndividual();
        }
    #endif
    return winnerPopulation;
}

std::vector<Individual> Evolution::cycleCrossover(Individual parent1, Individual parent2)
{
    /* First, we find the cycle, starting from the first position of the parent 1, going to the first 
    * position of the parent2, and finding in the parent1 the position relative to the value found in
    * the first position of the parent2, then we go to the same position in the parent2, and we continue
    * that until reaches the number that started the cycle.
    * After that, all that numbers from the cycle we copy from the parent1, and the reaining we take
    * from the parent2, to the first child individual.
    */
    bool continueCycleFlag = true, matchFlag = false;
    std::vector<int> cycleMembers;
    int iterationIndex = 0;
    int memberCandidateIndex = 0;
    #ifdef TESTING_CYCLE
        std::cout << std::endl;
        parent1.printIndividual();
        parent2.printIndividual();
        char ch;
    #endif

    while(continueCycleFlag)
    {
        if(!iterationIndex)
        {
            //so, we are looking for the first member
            cycleMembers.push_back( parent1.individual[iterationIndex] );
            
        } else 
        {
            if (iterationIndex%2==0)
            {
                //I'm looking the position in parent1 where I have the value found in parent2
                memberCandidateIndex = parent1.findValue( cycleMembers[iterationIndex/2] );
                #ifdef TESTING_CYCLE
                    std::cout << "Position in parent 1: " << memberCandidateIndex << std::endl;
                #endif
            } else
            {
                /* I'm looking the value in parent2 in the position found in parent1
                * First I found the value, then I check if is the first member of the cycle, if is
                * I change my flag to enter in the step of crossing what I found
                */
                #ifdef TESTING_CYCLE
                    std::cout << "Value in parent 2: " << parent2.individual[memberCandidateIndex] << std::endl;
                    ch=getchar();
                #endif
                if ( parent2.individual[memberCandidateIndex]==cycleMembers[ 0 ] )
                {
                    continueCycleFlag=false;
                } else
                {
                    cycleMembers.push_back( parent2.individual[memberCandidateIndex] );
                }
            }
        }
        iterationIndex++;
    }

    #ifdef TESTING_CYCLE
        Evolution::printVectorInt( cycleMembers );
    #endif

    std::vector<int> child1,child2;
    child1.push_back( parent1.individual[0] );
    child2.push_back( parent2.individual[0] );

    #ifdef TESTING_CYCLE
        std::cout << "child1[0] = " << child1[0] << std::endl;
        std::cout << "child2[0] = " << child2[0] << std::endl;
    #endif

    for ( int i = 1; i < INDIVIDUAL_SIZE; i++ )
    {
        for ( int j = 0; ( j < (int) cycleMembers.size() ) && ( !matchFlag ); j++ ) 
        {
            //Don't need to test the first member of the cycle
            if ( cycleMembers[j]==parent1.individual[i] )
            {
                matchFlag = true;
                // #ifdef TESTING_CYCLE
                //     std::cout << "\nflag = " << matchFlag << std::endl;
                // #endif
            }
        }
        if ( matchFlag )
        {
            child1.push_back( parent1.individual[i] );
            child2.push_back( parent2.individual[i] ); 
        } else
        {
            child1.push_back( parent2.individual[i] );
            child2.push_back( parent1.individual[i] ); 
        }
        // #ifdef TESTING_CYCLE
        //     std::cout << "child1[i] = " << child1[i] << std::endl;
        //     std::cout << "child2[i] = " << child2[i] << std::endl;
        //     ch=getchar();
        // #endif
        matchFlag = false;
    }
    
    std::vector<Individual> children;
    Individual aux(child1);
    children.push_back( aux );
    aux.setIndividual(child2);
    children.push_back( aux );

    #ifdef TESTING_CYCLE

        std::cout << "Parent 1: ";
        parent1.printIndividual();  
        std::cout << "Parent 2 ";
        parent2.printIndividual();   
        std::cout << "Child 1 ";
        children[0].printIndividual();
        std::cout << "Child 2 ";
        children[1].printIndividual();

    #endif

    return children;

}

std::vector<Individual> Evolution::pmxCrossover(Individual parent1, Individual parent2)
{
    std::vector<int> indexesTable;
    int startCut,endCut;
    #ifdef TESTING_PMX
        std::cout << std::endl;
        parent1.printIndividual();
        parent2.printIndividual();        
        char ch;
    #endif
    //std::srand(std::time(NULL));
    /* First, sort at random the begin and the end of the section, as a 2-point crossover
     * The Cut will occur after the position they store.
     */
    startCut = std::rand() % ( INDIVIDUAL_SIZE - 2 );//0 to 7
    endCut = ( std::rand() % ( INDIVIDUAL_SIZE - ( startCut + 2 ) ) ) + ( startCut + 1 );

    #ifdef TESTING_PMX
        std::cout << "startCut = " << startCut << std::endl;
        std::cout << "endCut = " << endCut << std::endl;
    #endif
    //Now, we mount our table of conversion. 
    for (int i = ( startCut + 1 ); i <= endCut; i++ )
    {
        indexesTable.push_back(parent1.individual[i]);
        indexesTable.push_back(parent2.individual[i]);
    }

    #ifdef TESTING_PMX
        printVectorInt( indexesTable );
        ch = getchar();
    #endif
    //Now, we correct the situation ( X -> Y -> Z => X -> Z ) if existent 
    int positionFind;
    for (int i = 0; i < (int) indexesTable.size(); i++)
    {
        positionFind = Evolution::findAnotherValueVectorInt( indexesTable, i );
        // #ifdef TESTING_PMX
        //     std::cout << positionFind << std::endl;
        //     ch = getchar();
        // #endif
        if ( positionFind >= 0 )
        {
            if (!(i % 2))
            {
                if ( indexesTable[i+1] == indexesTable[ positionFind-1 ] )
                {
                    indexesTable.erase( indexesTable.begin() + positionFind-1 );
                    indexesTable.erase( indexesTable.begin() + positionFind-1 );
                } else 
                {
                    indexesTable[i]=indexesTable[ positionFind-1 ];
                    indexesTable.erase( indexesTable.begin() + positionFind-1 );
                    indexesTable.erase( indexesTable.begin() + positionFind-1 );
                }
            } else
            {
                //i odd
                if ( indexesTable[i-1] == indexesTable[ positionFind+1 ] )
                {
                    indexesTable.erase( indexesTable.begin() + positionFind );
                    indexesTable.erase( indexesTable.begin() + positionFind );
                } else 
                {
                    indexesTable[i]=indexesTable[ positionFind+1 ];
                    indexesTable.erase( indexesTable.begin() + positionFind );
                    indexesTable.erase( indexesTable.begin() + positionFind );
                }  
                i = i-2; // to correct the error of not seeing what you swapped
            }
            if ( positionFind < i )
                i = -1;
        }
    }   

    #ifdef TESTING_PMX
        printVectorInt( indexesTable );
        ch = getchar();
    #endif

    std::vector<int> child1,child2;
    int aux;
    for (int i = 0; i< INDIVIDUAL_SIZE;i++)
    {
        // #ifdef TESTING_PMX
        //     std::cout << "i= " << i << std::endl;
        // #endif
        if (i <= startCut || i > endCut )
        {
            //Follow the rule in the table
            aux = findPositionVectorInt( indexesTable,parent1.individual[i] );
            if( aux >= 0 )
            {
                //If I found that position, I swap as the table states
                if (!(aux % 2))
                    child1.push_back(indexesTable[aux+1]);
                else
                    child1.push_back(indexesTable[aux-1]);
            } else
            {
                //If not, I maintain from the first parent
                child1.push_back(parent1.individual[i]);
            }
            // #ifdef TESTING_PMX
            //     std::cout << "aux= " << aux << std::endl;
            //     std::cout << "parent1.individual[i]= " << parent1.individual[i] << std::endl;
            //     printVectorInt( child1 );
            //     ch = getchar();
            // #endif
            aux = findPositionVectorInt( indexesTable,parent2.individual[i] );
            if( aux >= 0 )
            {
                if (!(aux % 2))
                    child2.push_back(indexesTable[aux+1]);
                else
                    child2.push_back(indexesTable[aux-1]);
            
            } else
            {
                //If not, I maintain from the first parent
                child2.push_back(parent2.individual[i]);
            }
            // #ifdef TESTING_PMX
            //     std::cout << "aux= " << aux << std::endl;
            //     std::cout << "parent1.individual[i]= " << parent2.individual[i] << std::endl;
            //     printVectorInt( child2 );
            //     ch = getchar();
            // #endif
        } else
        {
            child1.push_back(parent2.individual[i]);
            child2.push_back(parent1.individual[i]);
        }
    }
    
    std::vector<Individual> children;
    Individual auxil(child1);
    children.push_back( auxil );
    auxil.setIndividual(child2);
    children.push_back( auxil );

    #ifdef TESTING_PMX

        std::cout << "Parent 1: ";
        parent1.printIndividual(INDIVIDUAL_SIZE);  
        std::cout << "Parent 2: ";
        parent2.printIndividual(INDIVIDUAL_SIZE);   
        std::cout << "Child 1: ";
        children[0].printIndividual(INDIVIDUAL_SIZE);
        std::cout << "Child 2: ";
        children[1].printIndividual(INDIVIDUAL_SIZE);

    #endif

    return children;
}

std::vector<Individual> Evolution::runCrossover(int crossoverStrategy, int parentSelectionStrategy)
{
    std::vector<Individual> resultPopulation, parents, offspring;  
    #ifdef TESTING_EVOLUTION
        char ch;
    #endif

    if (parentSelectionStrategy == TOUR)
        parents = Evolution::tournaments();
    else if (parentSelectionStrategy == ROLL)
        parents = Evolution::roulettes();

    #ifdef TESTING_EVOLUTION
        std::cout << "Após o torneio" << std::endl;
        Evolution::printPopulation( parents );
        ch = std::getchar();
    #endif

    int numberCrossovers = (int) ( ( Evolution::crossoverRate * populationSize )/2 );
    if( numberCrossovers % 2 )
        numberCrossovers--; 
    //Crossovers
    for( int i = 0; i < numberCrossovers; i++ )
    {
        //std::cout << "i= " << i << std::endl;
        //Crossover
        switch (crossoverStrategy)
        {
            case CYCLE_CROSSOVER:
                    offspring=cycleCrossover(parents[i],parents[i+1]);
                    resultPopulation.push_back(offspring[0]);
                    resultPopulation.push_back(offspring[1]);
                    break;
            case PMX_CROSSOVER:
                    offspring=pmxCrossover(parents[i],parents[i+1]);
                    resultPopulation.push_back(offspring[0]);
                    resultPopulation.push_back(offspring[1]);
                    break;
        }
    }

    #ifdef TESTING_EVOLUTION
        std::cout << "Após o crossover" << std::endl;
        Evolution::printPopulation( resultPopulation );
        ch = std::getchar();
    #endif

    if (!Evolution::mutationRate)
        return resultPopulation;
    else
        return Evolution::permutationMutation(resultPopulation);
}

std::vector<Individual> Evolution::permutationMutation(std::vector<Individual> populationGenerated)
{
    std::vector<Individual> mutationResult;
    int mutationPlace1,mutationPlace2,individualIndex, aux;
    //std::srand(std::time(NULL));

    #ifdef TESTING_MUTATION
        std::cout << "\n Número de Mutações: " << (int) (Evolution::mutationRate*populationGenerated.size()) << std::endl;
        for ( int i = 0; i < (int) populationGenerated.size(); i++ )
        {
            populationGenerated[i].printIndividual();
        }
        std::cout << std::endl;
    #endif
    //Mutations
    for( int i = 0; i < (int) ( Evolution::mutationRate*populationGenerated.size() ); i++)
    {
        //Mutation
        mutationPlace1 = std::rand() % INDIVIDUAL_SIZE;
        mutationPlace2 = std::rand() % INDIVIDUAL_SIZE;
        
        #ifdef TESTING_MUTATION
            std::cout << "Places: " << mutationPlace1 << "  " << mutationPlace2 << std::endl;
        #endif

        while (mutationPlace2 == mutationPlace1)
            mutationPlace2 = std::rand() % INDIVIDUAL_SIZE;

        #ifdef TESTING_MUTATION
            std::cout << mutationPlace1 << "  " << mutationPlace2 << std::endl;
        #endif

        //sorting an individual to be mutated
        individualIndex = std::rand() % populationGenerated.size();

        #ifdef TESTING_MUTATION
            std::cout << "individualIndex = " << individualIndex << std::endl;
            populationGenerated[individualIndex].printIndividual( INDIVIDUAL_SIZE );
        #endif

        aux = populationGenerated[individualIndex].individual[mutationPlace1];
        populationGenerated[individualIndex].individual[mutationPlace1] = populationGenerated[individualIndex].individual[mutationPlace2];
        populationGenerated[individualIndex].individual[mutationPlace2] = aux;
        
        #ifdef TESTING_MUTATION
            populationGenerated[individualIndex].printIndividual( INDIVIDUAL_SIZE );
        #endif

        mutationResult.push_back( populationGenerated[individualIndex] );
        populationGenerated.erase( populationGenerated.begin() + individualIndex );
    }

    //Adding the non-mutated to the output
    for( int i = 0; i < (int) populationGenerated.size(); i++)
    {
        mutationResult.push_back( populationGenerated[i] );
    }

    #ifdef TESTING_MUTATION
        std::cout << std::endl;
        for ( int i = 0; i < (int) mutationResult.size(); i++ )
        {
            mutationResult[i].printIndividual();
        }
    #endif
    return mutationResult;
}

std::vector<Individual> Evolution::survivalPopulation(
    std::vector<Individual> populationOriginal, std::vector<Individual> populationNew,
    int survivalMethod, int fitnessMethod, float elitismPercent )
{
    #ifdef TESTING_SURVIVAL
        char ch;
        std::cout << "Indivíduos originais: " << std::endl;
        Evolution::printPopulation( populationOriginal );
        ch = std::getchar();
        std::cout << "Indivíduos novos gerados: " << std::endl;
        Evolution::printPopulation( populationNew );
        ch = std::getchar();
    #endif

    std::vector<Individual> resultPopulation;
    switch( survivalMethod )
    {
        case SURVIVAL_BEST_OF_ALL:
        {
            //Join the original with the generated
            for( int i = 0; i < (int) populationOriginal.size(); i++ )
            {
                resultPopulation.push_back( populationOriginal[i] );
            }

            for( int i = 0; i < (int) populationNew.size(); i++ )
            {
                resultPopulation.push_back( populationNew[i] );
            }

            #ifdef TESTING_SURVIVAL
                std::cout << "Juntando os indivíduos: " << std::endl;
                Evolution::printPopulation( resultPopulation );
                ch = std::getchar();
            #endif
            //Recalculate the fitness
            calculateFitnessPopulation( resultPopulation );
            
            #ifdef TESTING_SURVIVAL  
                std::cout << "Após o recalculo de fitness: " << std::endl;
                Evolution::printPopulation( resultPopulation );
                ch = std::getchar();
            #endif

            Individual aux;
            //Ordenate
            for( int i = 0; i < (int) resultPopulation.size(); i++)
            {
                for( int j = 0; j < (int) resultPopulation.size(); j++)
                {
                    if( fitnessMethod == MIN_FITNESS )
                    {
                        if( resultPopulation[i].getIndividualFitness() < resultPopulation[j].getIndividualFitness() )
                        {
                            aux = resultPopulation[i];
                            resultPopulation[i] = resultPopulation[j];
                            resultPopulation[j] = aux;
                        }
                    }
                }
            }

            #ifdef TESTING_SURVIVAL  
                std::cout << "Depois de ordenar: " << std::endl;
                Evolution::printPopulation( resultPopulation );
                ch = std::getchar();
            #endif

            while( resultPopulation.size() > Evolution::populationSize )
            {
                resultPopulation.erase( resultPopulation.end() );
            }

            #ifdef TESTING_SURVIVAL  
                std::cout << "Depois de selecionar: " << std::endl;
                Evolution::printPopulation( resultPopulation );
                ch = std::getchar();
            #endif

            return resultPopulation;
            break;
        }

        case ELITISM:
        {
            // In this case, the number of individual generated will be equal to 
            // what lacks, after the selection of the elite
            int numberCrossovers = (int) ( ( Evolution::crossoverRate * populationSize )/2 );
            if( numberCrossovers % 2 )
                numberCrossovers--; 
            int elitismIndividuals = Evolution::populationSize - numberCrossovers;
            //So first I have to ordenate the populationOriginal
            //Ordenate
            #ifdef TESTING_SURVIVAL 
                char ch;
                std::cout << "número de elite: " << elitismIndividuals << std::endl; 
            #endif
            Individual aux;

            for( int i = 0; i < (int) populationOriginal.size(); i++)
            {
                for( int j = 0; j < (int) populationOriginal.size(); j++)
                {
                    if( fitnessMethod == MIN_FITNESS )
                    {
                        if( populationOriginal[i].getIndividualFitness() < populationOriginal[j].getIndividualFitness() )
                        {
                            aux = populationOriginal[i];
                            populationOriginal[i] = populationOriginal[j];
                            populationOriginal[j] = aux;
                        }
                    }
                }
            }


            for( int i = 0; i < elitismIndividuals; i++ )
            {
                resultPopulation.push_back( populationOriginal[i] );
            }
            #ifdef TESTING_SURVIVAL 
                std::cout << "população após adicionar a elite: " << std::endl; 
                Evolution::printPopulation( resultPopulation ); 
                ch = getchar();
            #endif
            calculateFitnessPopulation( populationNew );

            #ifdef TESTING_SURVIVAL 
                std::cout << "população nova após recalcular o fitness: " << std::endl; 
                Evolution::printPopulation( populationNew );
                ch = getchar();
            #endif

            for( int i = 0; i < ( populationOriginal.size() - elitismIndividuals ); i++ )
            {
                resultPopulation.push_back( populationNew[i] );
            }

            #ifdef TESTING_SURVIVAL 
                std::cout << "população após adicionar o restante: " << elitismIndividuals << std::endl; 
                Evolution::printPopulation( resultPopulation ); 
                ch = getchar();
            #endif

            return resultPopulation;
            break;

        }
    }
}

void Evolution::calculateFitnessPopulation ( std::vector<Individual> &src )
{
    for ( int i = 0; i < src.size(); i++ )
    {
        src[i].calculateFitness();
    }
}

int Evolution::stopCriteria ( void )
{
    if ( Evolution::population[ 0 ].getIndividualFitness() == 0)
    {
        return 1;
    }
    if ( Evolution::generationNumber == Evolution::generationMaxNumber )
    {
        return 0;
    }
    return -1;
}

//VECTOR METHODS

int Evolution::findPositionVectorInt( std::vector<int> src, int value )
{
    for ( int i = 0; i < (int) src.size(); i++ )
    {
        if (src[i]==value)
            return i;
    }
    return -1;
}

int Evolution::findAnotherValueVectorInt( std::vector<int> src, int positionReference )
{ 
    for ( int i = 0; i < (int) src.size(); i++ )
    {
        if (i == positionReference)
        {
            continue;
        } else
        {
            if (src[i] == src[ positionReference ])
                return i;
        }
    }
    return -1;
}

void Evolution::printPopulation( std::vector<Individual> src )
{
    for ( int i = 0; i < (int) src.size(); i++ )
    {
       src[i].printIndividual(); 
    }
}


void Evolution::printPresentGeneration( int flag )
{
    if ( flag == BEST_INDIVIDUAL )
        Evolution::population[0].printIndividual();
    else
    {
        for (int i = 0; i < Evolution::populationSize; i++)
            Evolution::population[i].printIndividual();
    }
}

void Evolution::printVectorInt( std::vector<int> src )
{
	for ( int i = 0; i < (int) src.size(); i++)
	{
		std::cout << src[i];
	}
	std::cout << std::endl;
}

//SET METHODS
void Evolution::setGenerationMaxNumber( int generationWanted )
{
    Evolution::generationMaxNumber = generationWanted;
}

void Evolution::setPopulationSize( int populationSizeWanted )
{
    Evolution::populationSize = populationSizeWanted;

}

void Evolution::setNumberContenders( int numberContendersWanted )
{
    Evolution::numberContenders = numberContendersWanted;
}

void Evolution::setMutationRate( float mutationRateWanted )
{
    Evolution::mutationRate = mutationRateWanted;
}

void Evolution::setCrossoverRate( float crossoverRateWanted )
{
    Evolution::crossoverRate = crossoverRateWanted;
}

//GET METHODS
int Evolution::getGenerationMaxNumber( void )
{
    return Evolution::generationMaxNumber;
}

int Evolution::getPopulationSize( void )
{
    return Evolution::populationSize;

}

int Evolution::getNumberContenders( void )
{
    return Evolution::numberContenders;
}

float Evolution::getMutationRate( void )
{
    return Evolution::mutationRate;
}

float Evolution::getCrossoverRate( void )
{
    return Evolution::crossoverRate;
}

double Evolution::getTime( void )
{
    return Evolution::elapsedTimeSeconds;
}