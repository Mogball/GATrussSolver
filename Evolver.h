#ifndef GA_TRUSSER_EVOLVER_H
#define GA_TRUSSER_EVOLVER_H


#include "Hyperparameters.h"
#include "EvolutionParameters.h"
#include "Vivarium.h"

class Evolver {
public:
	Evolver(EvolutionParameters &, Hyperparameters &);

	void launch_evolution(Vivarium *p);

	Hyperparameters hparams;
	EvolutionParameters eparams;
};


#endif //GA_TRUSSER_EVOLVER_H
