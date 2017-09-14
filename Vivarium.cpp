//
// Created by Jeff Niu on 7/18/2017.
//

#include "Vivarium.h"

Vivarium::Vivarium(EvolutionParameters &eparams, Hyperparameters &hparams) :
		eparams(eparams), hparams(hparams) {
	uint32 size = eparams.max_population;
	uint32 specie_size = size / eparams.num_species;
	for (uint32 k = 0; k < eparams.num_species - 1; k++) {
		Species specie(eparams, hparams, specie_size);
		size -= specie_size;
		species.push_back(specie);
	}
	Species specie(eparams, hparams, size);
	species.push_back(specie);
}
