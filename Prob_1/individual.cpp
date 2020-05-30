#include "individual.hpp"
//#define TESTING_CALCULATE_FITNESS

Individual::Individual(int calculationMethod)
{
	individual.resize(INDIVIDUAL_SIZE);
	/* That vector has the purpose to retain the numbers from 0 to 9 that have not yet
	 * been set to some gene, in the same individual. In this way, it's possible to control
	 * the non-repetition of them.
	 */
	std::vector<int> valuesBase(INDIVIDUAL_SIZE,0);
	// That variable controls the random choice of the contens of valuesBase
	int sortedPosition = 0;

	//The first interation to be with i=0 is not necessary
	for(int i = 1; i < INDIVIDUAL_SIZE; i++)
		valuesBase[i]=i;

    for(int i = 0; i < INDIVIDUAL_SIZE; i++)
    {
    	//Remember that rand() % 2 returns 0 or 1
    	sortedPosition = (std::rand() % valuesBase.size() );
        Individual::individual[i] = valuesBase[sortedPosition];
        valuesBase.erase(valuesBase.begin()+sortedPosition);
    }
	Individual::calculateFitness( calculationMethod );
}

Individual::Individual(std::vector<int> individualBase, int calculationMethod)
{
	individual.resize(INDIVIDUAL_SIZE);
	/* That vector has the purpose to retain the numbers from 0 to 9 that have not yet
	 * been set to some gene, in the same individual. In this way, it's possible to control
	 * the non-repetition of them.
	 */
	for(int i = 0; i < INDIVIDUAL_SIZE; i++)
        Individual::individual[i] = individualBase[i];
	Individual::calculateFitness( calculationMethod );
}

void Individual::setIndividual(std::vector<int> individualBase)
{
	for(int i = 0; i < INDIVIDUAL_SIZE; i++)
        Individual::individual[i] = individualBase[i];
}

void Individual::calculateFitness(int calculationMethod, int problemKind )
{
	switch (calculationMethod)
	{
		case COMPARING_TOTAL_SUMS:
			
			long int firstWord, secondWord, thirdWord;
			
			switch( problemKind )
			{
				case SEND_MORE:
				{
					//SEND, MORE, MONEY. individual = SENDMORY;
					int s = Individual::individual[0], e = Individual::individual[1], n = Individual::individual[2], d = Individual::individual[3];
					int m = Individual::individual[4], o = Individual::individual[5], r = Individual::individual[6], y = Individual::individual[7];

					firstWord = 1000*s + 100*e + 10*n + d;
					secondWord = 1000*m + 100*o + 10*r + e;
					thirdWord = 10000*m + 1000*o + 100*n + 10*e + y;
					break;
				}
				case EAT_THAT:
				{
					//EAT, THAT, APPLE. individual = EATHPL;
					int e = Individual::individual[0], a = Individual::individual[1], t = Individual::individual[2], h = Individual::individual[3];
					int p = Individual::individual[4], l = Individual::individual[5];

					firstWord = 100*e + 10*a + t;
					secondWord = 1001*t + 100*h + 10*a;
					thirdWord = 10000*a + 1100*p + 10*l + e;
					break;
				}
				case CROSS_ROADS:
				{
					//CROSS, ROADS, DANGER. individual = CROSADNGE;
					int c = Individual::individual[0], r = Individual::individual[1], o = Individual::individual[2], s = Individual::individual[3];
					int a = Individual::individual[4], d = Individual::individual[5], n = Individual::individual[6], g = Individual::individual[7];
					int e = Individual::individual[8];

					firstWord = 10000*c + 1000*r + 100*o + 11*s;
					secondWord = 10000*r + 1000*o + 100*a + 10*d + s;
					thirdWord = 100000*d + 10000*a + 1000*n + 100*g + 10*e + r;
					break;
				}
				case COCA_COLA:
				{
					//COCA, COLA, OASIS. individual = COALSI;
					int c = Individual::individual[0], o = Individual::individual[1], a = Individual::individual[2], l = Individual::individual[3];
					int s = Individual::individual[4], i = Individual::individual[5];

					firstWord = 1010*c + 100*o + a;
					secondWord = 1000*c + 100*o + 10*l + a;
					thirdWord = 10000*o + 1000*a + 101*s + 10*i;
					break;
				}
				case DONALD_GERALD:
				{
					//DONALD, GERALD, ROBERT. individual = DONALGERBT;
					int d = Individual::individual[0], o = Individual::individual[1], n = Individual::individual[2], a = Individual::individual[3];
					int l = Individual::individual[4], g = Individual::individual[5], e = Individual::individual[6], r = Individual::individual[7];
					int b = Individual::individual[8], t = Individual::individual[9];

					firstWord = 100001*d + 10000*o + 1000*n + 100*a + 10*l;
					secondWord = 100000*g + 10000*e + 1000*r + 100*a + 10*l + d;
					thirdWord = 100010*r + 10000*o + 1000*b + 100*e + t;
					break;
				}
			}

			#ifdef TESTING_CALCULATE_FITNESS
				std::cout << "firstWord\t" << firstWord << std::endl;
				std::cout << "secondWord\t" << secondWord << std::endl;
				std::cout << "thirdWord\t" << thirdWord << std::endl;
			#endif

			long int value;
			value = (firstWord+secondWord)-thirdWord; 
			Individual::individualFitness = std::abs(value);

			#ifdef TESTING_CALCULATE_FITNESS
				std::cout << "value\t" << value << std::endl;
			#endif

			break;
		
		case COMPARING_EACH_BIT:
			std::vector<int> first, second, third, sum;
			int result = 0;

			switch( problemKind )
			{
				case SEND_MORE:
				{
					//SEND, MORE, MONEY. individual = SENDMORY;
					int s = Individual::individual[0], e = Individual::individual[1], n = Individual::individual[2], d = Individual::individual[3];
					int m = Individual::individual[4], o = Individual::individual[5], r = Individual::individual[6], y = Individual::individual[7];

					first.push_back( s );
					first.push_back( e );
					first.push_back( n );
					first.push_back( d );
					
					second.push_back( m );
					second.push_back( o );
					second.push_back( r );
					second.push_back( e );

					third.push_back( m );
					third.push_back( o );
					third.push_back( n );
					third.push_back( e );
					third.push_back( y );
					break;
				}
				case EAT_THAT:
				{
					//EAT, THAT, APPLE. individual = EATHPL;
					int e = Individual::individual[0], a = Individual::individual[1], t = Individual::individual[2], h = Individual::individual[3];
					int p = Individual::individual[4], l = Individual::individual[5];

					first.push_back( 0 );
					first.push_back( e );
					first.push_back( a );
					first.push_back( t );
					
					second.push_back( t );
					second.push_back( h );
					second.push_back( a );
					second.push_back( t );

					third.push_back( a );
					third.push_back( p );
					third.push_back( p );
					third.push_back( l );
					third.push_back( e );
					break;
				}
				case CROSS_ROADS:
				{
					//CROSS, ROADS, DANGER. individual = CROSADNGE;
					int c = Individual::individual[0], r = Individual::individual[1], o = Individual::individual[2], s = Individual::individual[3];
					int a = Individual::individual[4], d = Individual::individual[5], n = Individual::individual[6], g = Individual::individual[7];
					int e = Individual::individual[8];

					first.push_back( c );
					first.push_back( r );
					first.push_back( o );
					first.push_back( s );
					first.push_back( s );
					
					second.push_back( r );
					second.push_back( o );
					second.push_back( a );
					second.push_back( d );
					second.push_back( s );

					third.push_back( d );
					third.push_back( a );
					third.push_back( n );
					third.push_back( g );
					third.push_back( e );
					third.push_back( r );

					break;
				}
				case COCA_COLA:
				{
					//COCA, COLA, OASIS. individual = COALSI;
					int c = Individual::individual[0], o = Individual::individual[1], a = Individual::individual[2], l = Individual::individual[3];
					int s = Individual::individual[4], i = Individual::individual[5];

					first.push_back( c );
					first.push_back( o );
					first.push_back( c );
					first.push_back( a );
					
					second.push_back( c );
					second.push_back( o );
					second.push_back( l );
					second.push_back( a );

					third.push_back( o );
					third.push_back( a );
					third.push_back( s );
					third.push_back( i );
					third.push_back( s );
					break;
				}
				case DONALD_GERALD:
				{
					//DONALD, GERALD, ROBERT. individual = DONALGERBT;
					int d = Individual::individual[0], o = Individual::individual[1], n = Individual::individual[2], a = Individual::individual[3];
					int l = Individual::individual[4], g = Individual::individual[5], e = Individual::individual[6], r = Individual::individual[7];
					int b = Individual::individual[8], t = Individual::individual[9];

					first.push_back( d );
					first.push_back( o );
					first.push_back( n );
					first.push_back( a );
					first.push_back( l );
					first.push_back( d );
					
					second.push_back( g );
					second.push_back( e );
					second.push_back( r );
					second.push_back( a );
					second.push_back( l );
					second.push_back( d );

					third.push_back( r );
					third.push_back( o );
					third.push_back( b );
					third.push_back( e );
					third.push_back( r );
					third.push_back( t );
					break;
				}
			}

			#ifdef TESTING_CALCULATE_FITNESS
				std::cout << "first: " << std::endl;
				Individual::printVectorInt( first );
				std::cout << "second: " << std::endl;
				Individual::printVectorInt( second );
				std::cout << "third: " << std::endl;
				Individual::printVectorInt( third );
			#endif

			if ( ( first[0]+second[0] ) > 9)
			{
				sum.push_back( 1 );
				sum.push_back( ( first[0]+second[0] ) - 10 );
			} else
			{
				sum.push_back( ( first[0]+second[0] ) );
			}

			#ifdef TESTING_CALCULATE_FITNESS
				std::cout << "sum: " << std::endl;
				Individual::printVectorInt( sum );
			#endif

			for( int i = 1; i < WORDS_SIZE; i++)
			{
				if ( ( first[i]+second[i] ) > 9)
				{
					sum[ sum.size() - 1 ]++;
					sum.push_back( ( first[i]+second[i] ) - 10 );
				} else
				{
					sum.push_back( ( first[i]+second[i] ) );
				}
			}

			#ifdef TESTING_CALCULATE_FITNESS
				std::cout << "sum: " << std::endl;
				Individual::printVectorInt( sum );
			#endif
			

			if( sum.size() < ANSWER_SIZE )
				sum.emplace( sum.begin(), 0 );
			
			for( int i = 0; i < ANSWER_SIZE; i++)
			{
				result += std::abs( sum[i] - third[i] );
			}
			
			#ifdef TESTING_CALCULATE_FITNESS
				std::cout << "sum: " << std::endl;
				Individual::printVectorInt( sum );
			#endif

			Individual::individualFitness = result;

			#ifdef TESTING_CALCULATE_FITNESS
				std::cout << "individualFitness = " << Individual::individualFitness << std::endl;
			#endif
			break;
	}
}

long int Individual::getIndividualFitness( void )
{
	return Individual::individualFitness;
}

void Individual::setIndividualFitness( long int value )
{
	Individual::individualFitness = value;
}

void Individual::printIndividual( int lengthPrint )
{
	for ( int i = 0; i < lengthPrint; i++)
	{
		std::cout << Individual::individual[i];
	}
	std::cout << "\t" << Individual::individualFitness << std::endl;
}

int Individual::findValue( int valueToFind )
{
	for (int i = 0; i < INDIVIDUAL_SIZE; i++)
	{
		if ( Individual::individual[i] == valueToFind )
			return i;
	}
	return -1;
}

void Individual::printVectorInt( std::vector<int> src )
{
	for ( int i = 0; i < (int) src.size(); i++)
	{
		std::cout << src[i];
	}
	std::cout << std::endl;
}