//
// Created by Jeff Niu on 7/19/2017.
//

#include <algorithm>
#include <random>
#include "Species.h"

Species::Species(EvolutionParameters &eparams, Hyperparameters &hparams, uint32 size) :
		eparams(eparams), hparams(hparams), size(size) {
	for (uint32 k = 0; k < size; k++) {
		Genotype genotype(hparams);
		genome.push_back(genotype);
	}
}

Species::Species(EvolutionParameters &eparams, Hyperparameters &hparams, genome_vector &posterity) :
		eparams(eparams), hparams(hparams), size(posterity.size()) {
	for (uint32 k = 0; k < size; k++) {
		genome.push_back(posterity[k]);
	}
}

bool descending(int i, int j) {
	return j > i;
}

struct genotype_compare {
	inline bool operator()(const Genotype &g1, const Genotype &g2) {
		return g1.fitness.aggregate > g2.fitness.aggregate;
	}
};

Genotype::uint_vector rand_ints(uint32 lower, uint32 upper, uint32 num) {
	Genotype::uint_vector ints;
	for (uint32 k = lower; k <= upper; k++) {
		ints.push_back(k);
	}
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(ints.begin(), ints.end(), g);
	Genotype::uint_vector result;
	for (uint32 k = 0; k < num; k++) {
		result.push_back(ints[k]);
	}
	return result;
}

Genotype::uint_vector Species::get_migrants() {
	Genotype::uint_vector migrants = rand_ints(0, size - 1, eparams.migration_size);
	std::sort(migrants.begin(), migrants.end(), descending);
	return migrants;
}

Genotype *Species::tournament_select() {
	Genotype::uint_vector participants = rand_ints(0, size - 1, eparams.tournament_size);
	int32 max = -1;
	for (Genotype::uint_vector::iterator it = participants.begin(); it != participants.end(); it++) {
		if (max == -1 || genome[*it].fitness.aggregate > genome[max].fitness.aggregate) {
			max = *it;
		}
	}
	return &genome[max];
}

void Species::order() {
	std::sort(genome.begin(), genome.end(), genotype_compare());
}

