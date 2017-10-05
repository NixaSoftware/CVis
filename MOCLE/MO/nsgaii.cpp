/*
Multiple Objective MetaHeuristics Library in C++ MOMHLib++
Copyright (C) 2001 Andrzej Jaszkiewicz, Radoslaw Ziembinski (radekz@free.alpha.net.pl)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation (www.gnu.org); 
either version 2.1 of the License, or (at your option) any later 
version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __NSGAII_CPP_
#define __NSGAII_CPP_

#include "tmotools.h"

#include "nsgaii.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

template <class TProblemSolution>
TNSGAII <TProblemSolution>::TNSGAII(fs::path dirIPop, tPartition *truePart) :
        truePartition(truePart)
// modifiquei
//TNSGAII <TProblemSolution>::TNSGAII()
{
          
   // meu codigo
    dirInitialPopulation = dirIPop;
    
    //  conta o tamanho da populacao pelo numero 
    //  de arquivos no diretorio da populacao inicial
    
   	this->m_iPopulationSize = 0; // acrescentei
	fs::directory_iterator endDir; // acrescentei
    for (fs::directory_iterator dirIt(dirInitialPopulation); dirIt != endDir; dirIt++) // acrescentei
	{
		this->m_iPopulationSize++; // acrescentei
	}

    // codigo ja existente
//    this->m_iPopulationSize = 100; // retirei
	this->m_iNumberOfGenerations = 1000;
	this->m_dMutationProbability = 0.2;
	m_bScalarize = false;

	m_iDoubledPopulationSize = this->m_iPopulationSize * 2;
	m_iGeneration = 0;

}

template <class TProblemSolution>
void TNSGAII <TProblemSolution>::SetParameters(fs::path dirIPop, // acrescentei
                                             int iPopulationSize, 
											 int iNumberOfGenerations,
											 double dMutationProbability,
											 bool bScalarize)
{
// meu codigo
    
    // directory containing the initial population
    dirInitialPopulation = dirIPop; // acrescentei
    
    // count the number of individuals of the initial population
    // ( = number of files in the directory containing the initial population - each file
    // represents an individual)
   	this->m_iPopulationSize = 0; // acrescentei
	fs::directory_iterator endDir; // acrescentei
    for (fs::directory_iterator dirIt(dirInitialPopulation); dirIt != endDir; dirIt++) // acrescentei
	{
		this->m_iPopulationSize++; // acrescentei
	}

// codigo ja existente
    
//    this->m_iPopulationSize = iPopulationSize; // retirei
	this->m_iNumberOfGenerations = iNumberOfGenerations;
	this->m_dMutationProbability = dMutationProbability;
	m_bScalarize = bScalarize;

	m_iDoubledPopulationSize = this->m_iPopulationSize * 2;
	m_iGeneration = 0;
}

// Modified to take a set of partitions previously generated as the initial population
// The partitions should be provided in a directory containing only the partitions to 
// be employed as the initial population
template <class TProblemSolution>
void TNSGAII <TProblemSolution>::Run () // modifiquei 
//void TNSGAII <TProblemSolution>::Run () 
{
vector<IntVector>						iF;
vector<double>							dParentFitness;	
vector<double>							dDistance;
vector<double>							dParentDistance;
vector<IntVector>						iSortTable;

int						i = 0;
int						j = 0;
int						k = 0;
int						iFrontSize = 0;
int						iFront = 0;
double					d = 0.0;
double					dDifference;
int						iCurrentGeneration = 0;
int						iSolution = 0;
int						iSolution1 = 0;
int						iSolution2 = 0;
TProblemSolution		*pParentA = NULL;
TProblemSolution		*pOffspring = NULL;
bool					bExit = false;

	if (this->pNondominatedSet == NULL) 
	{
		return;
	}

	this->pNondominatedSet -> DeleteAll();

	this->Start();

	dParentFitness.resize(this->m_iPopulationSize);
	dParentDistance.resize(this->m_iPopulationSize);
	dDistance.resize(m_iDoubledPopulationSize);
	
	// reserve memory for population
	m_oPPopulation.resize(this->m_iPopulationSize);
	m_oQPopulation.resize(this->m_iPopulationSize);
	m_oRPopulation.resize(m_iDoubledPopulationSize);

	// Prepare initial P and Q populations
	
	
// meu codigo
	
    // Load the initial population P
	
	//this->m_iPopulationSize = 0; // acrescentei
	fs::directory_iterator endDir; // acrescentei
	i = 0;
    for (fs::directory_iterator dirIt(dirInitialPopulation); dirIt != endDir; dirIt++) // acrescentei
	{
	    //std::cout << (*dirIt).leaf() << std::endl;
         // generate random solution
		pParentA = new TProblemSolution((*dirIt), truePartition->patterns, truePartition);  // acrescentei

//		std::cout << "carregou solucao" << std::endl;
		
        //pParentA->solutionPartition.showPartition();
                
		// store it in population
		m_oPPopulation[i] = pParentA;
//		std::cout << "colocou na populacao P" << std::endl;

		// update nondominated set
		if (this->pNondominatedSet -> Update(*pParentA)) 
		{
			this->NewNondominatedSolutionFound();
		}

//		std::cout << "executou update" << std::endl;
		
		this->NewSolutionGenerated(*(TProblemSolution *)pParentA);
		
//		std::cout << "new solution generated" << std::endl;
		
		i++;
		
        //this->m_iPopulationSize++; // acrescentei
	}
//	getchar();
//	// Generate random P population
//	for(i = 0; i < this->m_iPopulationSize; i++) 
//	{
//		// generate random solution
//		pParentA = new TProblemSolution();
//
//		// store it in population
//		m_oPPopulation[i] = pParentA;
//
//		// update nondominated set
//		if (this->pNondominatedSet -> Update(*pParentA)) 
//		{
//			NewNondominatedSolutionFound();
//		}
//
//		NewSolutionGenerated(*(TProblemSolution *)pParentA);
//	}


	// Fast nondominated sorting algorithm
	TMOTools::FastNondominatedSort(m_oPPopulation, iF);

	// clear fitness
	for(i = 0; i < this->m_iPopulationSize; i++)
	{
		dParentFitness[i] = 0.0;
	}

	// assign fitness from front list
	for(i = 0; i < iF.size(); i++) 
	{
		for(j = 0; j < iF[i].size(); j++)
		{
			iSolution = iF[i][j];
			dParentFitness[iSolution] = i + 1.0; 
		}
	}

	// debug
	for(i = 0; i < this->m_iPopulationSize; i++)
	{
		assert(dParentFitness[i] > 0.0);
	}

	// create Q population
	for(i = 0; i < this->m_iPopulationSize; i++)
	{
		iSolution1 = TMOTools::GetSolutionUsingBinaryTournamentMin(m_oPPopulation, dParentFitness);
		iSolution2 = TMOTools::GetSolutionUsingBinaryTournamentMin(m_oPPopulation, dParentFitness);
   	    
//	    TProblemSolution sol1(*(TProblemSolution *)m_oPPopulation[iSolution1]);
//	    TProblemSolution sol2(*(TProblemSolution *)m_oPPopulation[iSolution2]);
//
//   	    std::cout << std::endl << "solucao " << iSolution1 << std::endl;
//   	    sol1.solutionPartition.showPartition();
//	
//   	    std::cout << std::endl << "solucao " << iSolution2 << std::endl;
//   	    sol2.solutionPartition.showPartition();

		// recombination		
		pOffspring = new TProblemSolution(*(TProblemSolution *)m_oPPopulation[iSolution1], *(TProblemSolution *)m_oPPopulation[iSolution2]);

//   	    std::cout << std::endl << "offspring " << std::endl;
//        pOffspring->solutionPartition.showPartition();

		// mutation
		if (TMOTools::DoMutate(this->m_dMutationProbability) == true) 
		{
			pOffspring->Mutate();
		}

		// update nondominated set
		if (this->pNondominatedSet -> Update(*pOffspring) == true) 
		{
			this->NewNondominatedSolutionFound();
		}

		// store offspring on new solution pull
		m_oQPopulation[i] = pOffspring;
		pOffspring = NULL;

		this->NewSolutionGenerated(*(TProblemSolution *)m_oQPopulation[i]);
	}
    
	// main algorithm loop
	iCurrentGeneration = 0;
    m_iGeneration = iCurrentGeneration;
	do
	{
		// join population P and Q to R
		for(i = 0; i < this->m_iPopulationSize; i++)
		{
			m_oRPopulation[i] = m_oPPopulation[i];
			m_oRPopulation[i + this->m_iPopulationSize] = m_oQPopulation[i];

			m_oPPopulation[i] = NULL;
			m_oQPopulation[i] = NULL;

			// clean variables
			dDistance[i] = 0.0;
			dDistance[i + this->m_iPopulationSize] = 0.0;
		}
		
		// fast nondominated sort on R population
		TMOTools::FastNondominatedSort(m_oRPopulation, iF);
		
		// compute aproximation of ideal nadir on R population
		// needed for scalarization
		if (m_bScalarize == true) 
		{
			m_oRPopulation.UpdateIdealNadir();
		}

		for(iFront = 0; iFront < iF.size(); iFront++)
		{
			// last front is empty ommit empty fronts
			if (iF[iFront].size() == 0)
			{
				continue;
			}

			// sort solutions by objective
			// to compute crowding distance
			iSortTable.resize(0);

			iFrontSize = iF[iFront].size();
			TMOTools::QuickSortByObjectivesMax(m_oRPopulation, iF[iFront], iSortTable);

			for(i = 0; i < NumberOfObjectives; i++) 
			{	
				// ommit objectives that not active
				if (Objectives[i].bActive == false) 
				{
					continue;
				}

				iSolution = iSortTable[i][0];
				dDistance[iSolution] = -1.0; // infinity

				iSolution = iSortTable[i][iFrontSize - 1];
				dDistance[iSolution] = -1.0; // infinity		

				for(j = 1; j < iFrontSize - 2; j++) 
				{
					iSolution = iSortTable[i][j];
				
					if (dDistance[iSolution] >= 0.0) 
					{
						iSolution1 = iSortTable[i][j - 1];
						iSolution2 = iSortTable[i][j + 1];
					
						d = m_oRPopulation[iSolution1]->ObjectiveValues[i] - m_oRPopulation[iSolution2]->ObjectiveValues[i];
					
						// optional scalarization 
						if (m_bScalarize == true)
						{
							dDifference = m_oRPopulation.ApproximateIdealPoint.ObjectiveValues[i] - m_oRPopulation.ApproximateNadirPoint.ObjectiveValues[i];
							if (dDifference != 0.0)
							{
								d = d / dDifference;
							}	 
							else
							{
								d = 0;
							}
						}
					
						dDistance[iSolution] += abs(d);
					}
				}
			}

			// sort solutions in the front from greatest distance
			// to lower
			TMOTools::QuickSortByValueMax(m_oRPopulation, dDistance, iF[iFront]);
		}

//		std::cout << std::endl << "populacao P: " << std::endl;
	
  	// create new parent P population from till of R population
		for(i = 0, k = 0; (i < iF.size()) && (k < this->m_iPopulationSize); i++) 
		{
//		    std::cout << "front: " << i+1 << std::endl << "individuos: " ;
			for(j = 0; (j < iF[i].size()) && (k < this->m_iPopulationSize); j++, k++)
			{
				iSolution = iF[i][j];
				assert(m_oRPopulation[iSolution] != NULL);

				
				m_oPPopulation[k] = m_oRPopulation[iSolution];
				dParentFitness[k] = i + 1.0;
				dParentDistance[k] = dDistance[iSolution];

//				std::cout << k << " ";
    
  				m_oRPopulation[iSolution] = NULL;
			}
//			std::cout << std::endl;
		}

		// release rest of R population
		for(i = 0; i < m_iDoubledPopulationSize; i++)
		{
			if (m_oRPopulation[i] != NULL)
			{
				delete (TProblemSolution *)m_oRPopulation[i];
				m_oRPopulation[i] = NULL;
			}
		}
				
		// create Q population based on nisched operator
		for(i = 0; i < this->m_iPopulationSize; i++)
		{
			iSolution1 = TMOTools::GetSolutionUsingBinaryTournamentNischedMin(m_oPPopulation, dParentFitness, dParentDistance);
			iSolution2 = TMOTools::GetSolutionUsingBinaryTournamentNischedMin(m_oPPopulation, dParentFitness, dParentDistance);
	
			// recombination		
			pOffspring = new TProblemSolution(*(TProblemSolution *)m_oPPopulation[iSolution1], *(TProblemSolution *)m_oPPopulation[iSolution2]);

			// mutation
			if (TMOTools::DoMutate(this->m_dMutationProbability) == true) 
			{
				pOffspring->Mutate();
			}

			// update nondominated set
			if (this->pNondominatedSet -> Update(*pOffspring) == true) 
			{
				this->NewNondominatedSolutionFound();
			}

			// store offspring on new solution pull
			m_oQPopulation[i] = pOffspring;
			pOffspring = NULL;

			this->NewSolutionGenerated(*(TProblemSolution *)m_oQPopulation[i]);
		}
		
		iCurrentGeneration += 1;
        m_iGeneration = iCurrentGeneration;

	}
	while((this->bStop == false) && 
          ((this->bOnlineOfflineEvaluating == true) || (m_iGeneration < this->m_iNumberOfGenerations)));

	// end callback
	this->End();

//para salvar a pop final no "save", � preciso incluir o codigo:
//	for(i = 0; i < finalPopulation.size(); i++) 
//		if (finalPopulation[i] != NULL) 
//		{
//			delete (TProblemSolution *)finalPopulation[i];
//			finalPopulation[i] = NULL;
//		}
//	finalPopulation.resize(this->m_iPopulationSize);
//    for(i = 0; i < this->m_iPopulationSize; i++) 
//	{
//	    //if (m_oPPopulation[i] != NULL)
//    	    TProblemSolution *mySol = (TProblemSolution*)m_oPPopulation[i];
//            TProblemSolution *auxSol = new TProblemSolution(*mySol);
//        	finalPopulation[i] = auxSol;
//    } 
    
    // Cleanup object
	// Remove current population
	for(i = 0; i < this->m_iPopulationSize; i++) 
	{
		if (m_oPPopulation[i] != NULL) 
		{
			delete (TProblemSolution *)m_oPPopulation[i];
			m_oPPopulation[i] = NULL;
		}

		if (m_oQPopulation[i] != NULL) 
		{
			delete (TProblemSolution *)m_oQPopulation[i];
			m_oQPopulation[i] = NULL;
		}
	}
}

template <class TProblemSolution>
void TNSGAII <TProblemSolution>::saveSolutionPopulation(fs::path dirSolutionPopulation, fs::path resultFile) // acrescentei
{
    //char buffer[100];
    fs::path partitionFileName; 
    fs::ofstream res;
    fs::ofstream resAll;
    
    fs::path dirPopulation;
    fs::path resultFilePopulation;

//std::cout << dirSolutionPopulation.string() << std::endl << resultFile.string();
    if (exists(dirSolutionPopulation))
       remove_all(dirSolutionPopulation);
    fs::create_directory(dirSolutionPopulation);

//    dirPopulation = dirSolutionPopulation.string() + "All";
    resultFilePopulation = resultFile.branch_path().string() + "/" +
                           fs::basename(resultFile) + "All" +
                           fs::extension(resultFile);
//    std::cout << dirPopulation.string() << std::endl << resultFilePopulation.string();
//    if (exists(dirPopulation))
//       remove_all(dirPopulation);
//    fs::create_directory(dirPopulation);

    res.open(resultFile);
    if (res.bad()) 
    {
        std::cout << "\nCannot open file '" << resultFile.string() << "'\n";
        getchar();
        exit(-1);
    }
    
//        (*this->pNondominatedSet)[sol]->ObjectiveValues[obj]
//                
//        *(TProblemSolution *)

    res << "Partition\tObjectives values" << std::endl;                            
    //for(int sol = 0; sol < this->m_iPopulationSize; sol++) 
	cout << "Salvando valores dos objetivos --- numero de solucoes " << (*this->pNondominatedSet).size() << endl;
    for(int sol = 0; sol < (*this->pNondominatedSet).size(); sol++) 
	{
        TProblemSolution solution(*(TProblemSolution *)(*this->pNondominatedSet)[sol]);
        std::string fileName = "partition";
        fileName += "k" + boost::lexical_cast<string>(solution.solutionPartition.nClusters());
        fileName += "s";
        fileName += boost::lexical_cast<string>(sol + 1);
        fileName +=  ".clu";
               
//        solution.solutionPartition.showPartition();
        partitionFileName = dirSolutionPopulation / fileName;        
        solution.solutionPartition.savePartition(partitionFileName);

        res << partitionFileName.string() << "\t";                            
        for(int obj = 0; obj < NumberOfObjectives; obj++) 
		{	
  		    // ommit objectives that not active
			if (Objectives[obj].bActive == true) 
			{
   			    res << (*this->pNondominatedSet)[sol]->ObjectiveValues[obj] << "\t";
			}
        }
        res << std::endl;
    }
    res.close();

//    // salva a populacao toda
//    resAll.open(resultFilePopulation);
//    if (res.bad()) 
//    {
//        std::cout << "\nCannot open file '" << resultFilePopulation.string() << "'\n";
//        getchar();
//        exit(-1);
//    }
//    
//    resAll << "Partition\tObjectives values" << std::endl;                            
//    //for(int sol = 0; sol < this->m_iPopulationSize; sol++) 
//    for(int sol = 0; sol < finalPopulation.size(); sol++) 
//	{
//        TProblemSolution solution(*(TProblemSolution *)finalPopulation[sol]);
//        std::string fileName = "f-partition";
//        fileName += "k" + boost::lexical_cast<string>(solution.solutionPartition.nClusters());
//        fileName += "s";
//        fileName += boost::lexical_cast<string>(sol + 1);
//        fileName +=  ".clu";
//        
//        //this->pNondominatedSet
//        
////        partitionFileName = dirPopulation / fileName;        
////        solution.solutionPartition.savePartition(partitionFileName);
//
//        resAll << fileName << "\t";                            
//        for(int obj = 0; obj < NumberOfObjectives; obj++) 
//		{	
//  		    // ommit objectives that not active
//			if (Objectives[obj].bActive == true) 
//			{
//   			    resAll << finalPopulation[sol]->ObjectiveValues[obj] << "\t";
//			}
//        }
//        resAll << std::endl;
//    }
    
//    for(int i = 0; i < m_oPPopulation.size(); i++) 
//	{
//	    TProblemSolution solution(*(TProblemSolution *)(finalPopulation)[i]);
//	    //solution.truePartition.showPartition();
//        //solution.solutionPartition.showPartition();
////        solution.solutionPartition.savePartition(partitionFileName);
//        //(TProblemSolution *)m_oPPopulation[iSolution1]
//    }

//    resAll.close();
     
}






template <class TProblemSolution>
void TDisplayNSGAII <TProblemSolution>::End() 
{
	cout << "Optimization finished\n";
	cout << this->pNondominatedSet->iSetSize << '\t';
	int iobj; for (iobj = 0; iobj < NumberOfObjectives; iobj++) 
	{
		if (Objectives [iobj].bActive)
		{
			cout << this->pNondominatedSet->ApproximateIdealPoint.ObjectiveValues [iobj] << '\t';
		}

		if (Objectives [iobj].bActive) 
		{
			cout << this->pNondominatedSet->ApproximateNadirPoint.ObjectiveValues [iobj] << '\t';
		}
	}
	cout << '\n';
}

template <class TProblemSolution>
void TDisplayNSGAII <TProblemSolution>::NewNondominatedSolutionFound() 
{
	cout << this->pNondominatedSet->iSetSize << '\t';
	int iobj; for (iobj = 0; iobj < NumberOfObjectives; iobj++) 
	{
		if (Objectives [iobj].bActive) 
		{
			cout << this->pNondominatedSet->ApproximateIdealPoint.ObjectiveValues [iobj] << '\t';
		}

		if (Objectives [iobj].bActive) 
		{
			cout << this->pNondominatedSet->ApproximateNadirPoint.ObjectiveValues [iobj] << '\t';
		}
	}
	cout << '\n';
}

template <class TProblemSolution>
void TDisplayNSGAII <TProblemSolution>::NewGenerationFound() 
{
 	cout << "Generation: " << this->m_iGeneration + 1 << "\n";
}

#endif 
