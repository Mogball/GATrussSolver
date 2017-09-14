//
// Created by Jeff Niu on 7/18/2017.
//

#include "EvolutionParameters.h"

EvolutionParameters::EvolutionParameters(uint32 max_generations, uint32 max_population,
                                         uint32 num_species, uint32 tournament_size) :
		max_generations(max_generations), max_population(max_population),
		num_species(num_species), tournament_size(tournament_size) {
	migration_size = 15;
	elite_size = 50;

	crossover_rate = 0.8;
	float_mutation_rate = 0.01;
	bit_mutation_rate = 0.005;

	single_point_trunc = 0.3;
	two_point_trunc = 0.95;
	uniform_trunc = 1.0;

	average_trunc = 0.8;
	randomize_trunc = 1.0;

	float_perturb_size = 0.8;
	float_perturb_chance = 0.05;
	float_randomize_chance = 0.005;

	bit_mutation_chance = 0.005;
}

EvolutionParameters::EvolutionParameters() :
		EvolutionParameters(250, 2500, 1, 3) {}
