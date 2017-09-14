//
// Created by Jeff Niu on 7/18/2017.
//

#ifndef GA_TRUSSER_HYPERPARAMETERS_H
#define GA_TRUSSER_HYPERPARAMETERS_H

#include "Global.h"
#include <SFML/System/Vector2.hpp>

typedef sf::Vector2<int> Pair;

class Hyperparameters {
public:
	Hyperparameters(uint32 num_free_nodes, uint32 num_fixed_nodes, uint32 support_A, uint32 support_B);

	Hyperparameters(uint32 num_free_nodes, uint32 num_fixed_nodes, int32 support_A, int32 support_B,
	                float64 x_min, float64 x_max, float64 y_min, float64 y_max);

	uint32 num_free_nodes;
	uint32 num_fixed_nodes;
	uint32 num_nodes;
	uint32 num_struts;

	uint32 num_bits;
	uint32 num_floats;

	float64 x_min;
	float64 x_max;
	float64 y_min;
	float64 y_max;

	uint32 gusset_cost;
	uint32 member_cost;

	float64 train_weight;
	float64 truss_length;
	float64 spacing;

	float64 support_force;
	float64 load_force;

	float64 min_force;
	float64 max_force;

	float64 tol;

	uint32 support_A;
	uint32 support_B;
	uint32 **strut_indices;
	float64 *loads;
	Pair *strut_inverse;
};


#endif //GA_TRUSSER_HYPERPARAMETERS_H
