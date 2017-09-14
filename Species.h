//
// Created by Jeff Niu on 7/19/2017.
//

#ifndef GA_TRUSSER_SPECIES_H
#define GA_TRUSSER_SPECIES_H


#include "Hyperparameters.h"
#include "EvolutionParameters.h"
#include "Genotype.h"

class Species {
public:
	typedef std::vector<Genotype> genome_vector;

	Species(EvolutionParameters &, Hyperparameters &, uint32 size);
	Species(EvolutionParameters &, Hyperparameters &, genome_vector &posterity);

	Genotype::uint_vector get_migrants();
	Genotype *tournament_select();

	void order();

	EvolutionParameters eparams;
	Hyperparameters hparams;
	genome_vector genome;
	uint32 size;
};


#endif //GA_TRUSSER_SPECIES_H
