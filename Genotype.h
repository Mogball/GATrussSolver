#ifndef GA_TRUSSER_GENOTYPE_H
#define GA_TRUSSER_GENOTYPE_H

#include <vector>
#include "Hyperparameters.h"
#include "Point.h"
#include "EvolutionParameters.h"

struct Fitness {
	Fitness();
	Fitness(float64 total_cost, float64 excess_force, float64 nonzero_sum);
	float64 aggregate;
	float64 total_cost;
	float64 excess_force;
	float64 nonzero_sum;

	Fitness &operator=(const Fitness &f);
};

class Genotype {
public:
	typedef std::vector<Point> point_vector;
	typedef std::vector<bool> bit_vector;
	typedef std::vector<uint32> uint_vector;

	Genotype(Hyperparameters &);
	Genotype(Genotype *g1, Genotype *g2, EvolutionParameters &);

	void evaluate();

	Hyperparameters hparams;
	Fitness fitness;
	point_vector points;
	bit_vector struts;
};


#endif //GA_TRUSSER_GENOTYPE_H
