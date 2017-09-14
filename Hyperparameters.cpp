//
// Created by Jeff Niu on 7/18/2017.
//

#include "Hyperparameters.h"

Hyperparameters::Hyperparameters(uint32 num_free_nodes, uint32 num_fixed_nodes,
                                 uint32 support_A, uint32 support_B) :
		num_free_nodes(num_free_nodes), num_fixed_nodes(num_fixed_nodes),
		support_A(support_A), support_B(support_B) {
	num_nodes = num_free_nodes + num_fixed_nodes;
	num_struts = num_nodes * (num_nodes - 1) / 2;

	num_bits = num_struts;
	num_floats = num_free_nodes * 2;

	x_min = -2;
	x_max = 16;
	y_min = -16;
	y_max = 16;

	gusset_cost = 6;
	member_cost = 12;

	train_weight = 4;
	truss_length = 14;
	spacing = truss_length / (num_fixed_nodes - 1);

	float64 total_force = train_weight * truss_length;
	load_force = total_force / num_fixed_nodes;
	support_force = -total_force / 2;

	min_force = -13;
	max_force = 8;

	tol = 1e-6;

	loads = new float64[num_nodes]();
	loads[support_A] += support_force;
	loads[support_B] += support_force;
	strut_indices = new uint32 *[num_nodes];
	strut_inverse = new Pair[num_struts];
	for (uint32 k = 0; k < num_nodes; k++) {
		strut_indices[k] = new uint32[num_nodes];
	}
	for (uint32 k = 0; k < num_fixed_nodes; k++) {
		loads[k] += load_force;
	}
	uint32 strut_index = 0;
	for (uint32 i = 0; i < num_nodes - 1; i++) {
		for (uint32 j = i + 1; j < num_nodes; j++) {
			strut_indices[i][j] = strut_index;
			strut_indices[j][i] = strut_index;
			strut_inverse[strut_index].x = i;
			strut_inverse[strut_index].y = j;
			strut_index++;
		}
	}
}

Hyperparameters::Hyperparameters(uint32 num_free_nodes, uint32 num_fixed_nodes,
                                 int32 support_A, int32 support_B,
                                 float64 x_min, float64 x_max,
                                 float64 y_min, float64 y_max) :
		Hyperparameters(num_free_nodes, num_fixed_nodes, support_A, support_B) {
	this->x_min = x_min;
	this->x_max = x_max;
	this->y_min = y_min;
	this->y_max = y_max;
}
