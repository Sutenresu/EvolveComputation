#include <vector>
#include <iostream>
#include <ctime>      
#include "individual.hpp"
#include "evolution.hpp"

#define NUMBER_OF_EXECUTIONS 100

int main(int argc, char *argv[])
{
    std::srand (std::time(NULL));
    // Individual a;
    // a.printIndividual();

    double timeTotal = 0.0;
    float result = 0.0;
    char ch;
    for( int i = 0; i < NUMBER_OF_EXECUTIONS; i++ )
    {
        Evolution geracao;
        timeTotal += geracao.getTime();
        result += geracao.finishStatus;
        // if (geracao.finishStatus == 1)
        //     geracao.printPresentGeneration( BEST_INDIVIDUAL );
        //ch = std::getchar();
    }
    //std::cout << "Sucessos: " << result << std::endl;

    result /= ( (float) NUMBER_OF_EXECUTIONS );
    //como o numero de execuções é 1000, mudando para ms, n precisa converter p ter o tempo em media
    std::cout << "\n" << result*100 << " %.\t" << timeTotal  << std::endl;
    // std::cout << "A precisão do algoritmo é de " << result*100 << " %." << std::endl;
    // std::cout << "O tempo médio gasto por execução foi de " << timeTotal << " milissegundos." << std::endl;
    // geracao.printPresentGeneration();   
    return 0;
}
