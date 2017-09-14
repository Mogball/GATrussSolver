//
// Created by Jeff Niu on 7/18/2017.
//

#include "Evolver.h"

Evolver::Evolver(EvolutionParameters &eparams, Hyperparameters &hparams) :
		eparams(eparams), hparams(hparams) {}

void Evolver::launch_evolution(Vivarium *p) {
	for (uint32 generation = 0; generation < eparams.max_generations; generation++) {
		printf("<Generation %i>\n", generation);
		std::vector<Genotype::uint_vector> specie_migrants;
		for (uint32 specie_k = 0; specie_k < eparams.num_species; specie_k++) {
			printf("\t<Species %i>\n", specie_k);
			Species *species = &p->species[specie_k];
			float64 total_fitness = 0;
			Fitness *best_fitness = nullptr;
			for (uint32 genotype_k = 0; genotype_k < species->size; genotype_k++) {
				Genotype *genotype = &species->genome[genotype_k];
				genotype->evaluate();
				total_fitness += genotype->fitness.aggregate;
				if (!best_fitness || genotype->fitness.aggregate > best_fitness->aggregate) {
					best_fitness = &genotype->fitness;
				}
			}
			printf("\t\t<Fitness>\n");
			printf("\t\t\tAverage: %.4f\n", total_fitness / species->size);
			if (best_fitness) {
				printf("\t\t\tBest: %.4f, $%.2f, %.4f, %.4f\n", best_fitness->aggregate,
				       best_fitness->total_cost, best_fitness->excess_force, best_fitness->nonzero_sum);
			}
			Genotype::uint_vector migrants = species->get_migrants();
			specie_migrants.push_back(migrants);
			printf("\t</Species>\n");
		}
		for (uint32 receiver_j = 0; receiver_j < eparams.num_species; receiver_j++) {
			uint32 donor_j = (receiver_j + 1) % eparams.num_species;
			Species *receiver = &p->species[receiver_j];
			Species *donor = &p->species[donor_j];
			for (uint32 donor_mig = 0; donor_mig < eparams.migration_size; donor_mig++) {
				uint32 migrant = specie_migrants[donor_j][donor_mig];
				receiver->genome.push_back(donor->genome[migrant]);
			}
		}
		for (uint32 specie_k = 0; specie_k < eparams.num_species; specie_k++) {
			Species *specie = &p->species[specie_k];
			for (uint32 mig = 0; mig < eparams.migration_size; mig++) {
				uint32 migrant = specie_migrants[specie_k][mig];
				specie->genome.erase(specie->genome.begin() + migrant);
			}
			specie->order();
		}
		for (uint32 specie_k = 0; specie_k < eparams.num_species; specie_k++) {
			Species *specie = &p->species[specie_k];
			uint32 posterity_size = specie->size;
			Species::genome_vector offspring;
			for (uint32 k = 0; k < eparams.elite_size; k++) {
				Genotype *elite = &specie->genome.front();
				offspring.push_back(*elite);
				specie->genome.erase(specie->genome.begin());
			}
			posterity_size -= eparams.elite_size;
			for (uint32 k = 0; k < posterity_size; k++) {
				float64 r = rand_float(0, 1);
				if (r < eparams.crossover_rate) {
					Genotype *g1 = specie->tournament_select();
					Genotype *g2 = specie->tournament_select();
					Genotype posterity(g1, g2, eparams);
					offspring.push_back(posterity);
				} else {
					offspring.push_back(*specie->tournament_select());
				}
			}
			for (uint32 k = 0; k < offspring.size(); k++) {
				Genotype *g = &offspring[k];
				if (rand_float(0, 1) < eparams.bit_mutation_rate) {
					for (uint32 t = 0; t < hparams.num_struts; t++) {
						if (rand_float(0, 1) < eparams.bit_mutation_chance) {
							g->struts[t] = g->struts[t] ? 0 : 1;
						}
					}
				}
				if (rand_float(0, 1) < eparams.float_mutation_rate) {
					for (uint32 t = 0; t < hparams.num_free_nodes; t++) {
						if (rand_float(0, 1) < eparams.float_perturb_chance) {
							g->points[t].x += rand_float(-eparams.float_perturb_size, eparams.float_perturb_size);
						} else if (rand_float(0, 1) < eparams.float_randomize_chance) {
							g->points[t].x = rand_float(hparams.x_min, hparams.x_max);
						}
						if (rand_float(0, 1) < eparams.float_perturb_chance) {
							g->points[t].y += rand_float(-eparams.float_perturb_size, eparams.float_perturb_size);
						} else if (rand_float(0, 1) < eparams.float_randomize_chance) {
							g->points[t].y = rand_float(hparams.y_min, hparams.y_max);
						}
					}
				}
			}
			Species nextGen(eparams, hparams, offspring);
			p->species[specie_k] = nextGen;
		}
		printf("</Generation>\n");
	}
}
