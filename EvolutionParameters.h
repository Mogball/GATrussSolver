#ifndef GA_TRUSSER_EVOLUTIONPARAMETERS_H
#define GA_TRUSSER_EVOLUTIONPARAMETERS_H

#include "Global.h"

class EvolutionParameters {
public:
	EvolutionParameters();

	EvolutionParameters(uint32 max_generations, uint32 max_population,
	                    uint32 num_species, uint32 tournament_size);

	uint32 max_generations;
	uint32 max_population;
	uint32 num_species;

	uint32 tournament_size;
	uint32 migration_size;
	uint32 elite_size;

	float64 crossover_rate;
	float64 float_mutation_rate;
	float64 bit_mutation_rate;

	float64 single_point_trunc;
	float64 two_point_trunc;
	float64 uniform_trunc;

	float64 average_trunc;
	float64 randomize_trunc;

	float64 float_perturb_size;
	float64 float_perturb_chance;
	float64 float_randomize_chance;

	float64 bit_mutation_chance;
};


#endif //GA_TRUSSER_HYPERPARAMETERS_H
