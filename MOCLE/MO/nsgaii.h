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

#ifndef __NSGAII_H_
#define __NSGAII_H_

#include "global.h"
#include "weights.h"
#include "momhsolution.h"
#include "nondominatedset.h"
#include "momethod.h"
#include "paretobase.h"

#include "../tPartition.h" // acrescentei
#include "../tPatterns.h" // acrescentei

// para incluir as bibliotecas Boost
/*#include <boost/filesystem/operations.hpp> // includes boost/filesystem/path.hpp
#include <boost/filesystem/fstream.hpp>    // ditto
#include <boost/filesystem/convenience.hpp>    // ditto
#include <boost/lexical_cast.hpp>
*/
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

namespace fs = boost::filesystem; // acrescentei

/** Implements Nondominated Sorting Genetic Algorithm II. See publications K. Deb for details.
 **/
 
// This class was modified to take a set of partitions previously generated as the initial population
 
template <class TProblemSolution> class TNSGAII : public TParetoBase <TProblemSolution>
{
protected:
    /** Size of population multipled by two */
	int					m_iDoubledPopulationSize;

    /** Scalarization enabled or disabled **/
	bool				m_bScalarize;

    /** Q population - child population **/
	TSolutionsSet		m_oQPopulation;

    /** P population - parent population **/
	TSolutionsSet		m_oPPopulation;

    /** R population - merged population **/
	TSolutionsSet		m_oRPopulation;
	
	// all individuals in the final population - used to save the final population, not only the solution population
	TSolutionsSet finalPopulation;

public:

    /** Current iteration - generation **/
	int					m_iGeneration;
	
	// directory containing the initial informed population
    fs::path dirInitialPopulation; // acrescentei
    
    // file containing the true partition
    tPartition *truePartition; // acrescentei
	
    TNSGAII(fs::path dirIPop, tPartition *truePart); // modifiquei
    void saveSolutionPopulation(fs::path dirSolutionPopulation, fs::path resultFile); // acrescentei


//	TNSGAII(); // retirei

	/** Set NSGAII algorithm parameters. 
     *
     * @param iPopulationSize population size
     * @param iNumberOfGenerations number of main iterations of algorithm  
     * @param dMutationProbability probability of mutation after recombination
     * @param bScalarize computation of distance will be done on scalarized objective values
     **/
     
   	void SetParameters(fs::path dirIPop, // acrescentei
                       int iPopulationSize, 
					   int iNumberOfGenerations,
					   double dMutationProbability,
					   bool bScalarize);

//
//	void SetParameters(int iPopulationSize, 
//					   int iNumberOfGenerations,
//					   double dMutationProbability,
//					   bool bScalarize);

    /** Call to start computations
     **/
	void Run(); 
};

/** Visualize computation process
 **/
template <class TProblemSolution> class TDisplayNSGAII : public TNSGAII <TProblemSolution> 
{
protected:
	virtual void End ();
	virtual void NewNondominatedSolutionFound ();
	virtual void NewGenerationFound ();
	virtual ~TDisplayNSGAII(){};//acrescentei
};

#include "nsgaii.cpp"

#endif 
