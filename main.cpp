#include <iostream>
#include "Genotype.h"
#include "Species.h"
#include "Vivarium.h"
#include "Evolver.h"

#define s(a, b) g.struts[hparams.strut_indices[a][b]] = 1

int main() {
	Global::init();

	Hyperparameters hparams(5, 6, 0, 5);
	Genotype g(hparams);
	g.points[0].x = 2.088379;
	g.points[0].y = 2.667725;
	g.points[1].x = 4.480957;
	g.points[1].y = 1.382324;
	g.points[2].x = 6.941895;
	g.points[2].y = 0.726807;
	g.points[3].x = 9.549805;
	g.points[3].y = 1.440918;
	g.points[4].x = 11.826172;
	g.points[4].y = 2.750122;
	for (uint32 k = 0; k < g.points.size(); k++) {
		printf("(%.2f, %.2f)\n", g.points[k].x, g.points[k].y);
	}
	for (uint32 k = 0; k < g.struts.size(); k++) {
		g.struts[k] = 0;
	}
	s(0, 1);
	s(1, 2);
	s(2, 3);
	s(3, 4);
	s(4, 5);
	s(6, 0);
	s(6, 1);
	s(7, 1);
	s(7, 2);
	s(8, 2);
	s(8, 3);
	s(9, 3);
	s(9, 4);
	s(10, 4);
	s(10, 5);
	s(6, 7);
	s(7, 8);
	s(8, 9);
	s(9, 10);
	g.evaluate();
	Fitness f = g.fitness;
	printf("%.2f, %.6f, %.6f\n", f.total_cost, f.excess_force, f.nonzero_sum);
	EvolutionParameters eparams(750, 1000, 1, 25);
	Vivarium p(eparams, hparams);
	Evolver e(eparams, hparams);
	e.launch_evolution(&p);
}