#ifndef GA_TRUSSER_POPULATION_H
#define GA_TRUSSER_POPULATION_H

#include "EvolutionParameters.h"
#include "Hyperparameters.h"
#include "Genotype.h"
#include "Species.h"

class Vivarium {
public:
	typedef std::vector<Species> species_vector;

	Vivarium(EvolutionParameters &, Hyperparameters &);

	EvolutionParameters eparams;
	Hyperparameters hparams;
	species_vector species;
};


#endif //GA_TRUSSER_POPULATION_H
