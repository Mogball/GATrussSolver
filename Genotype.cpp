#include <Eigen>
#include <time.h>
#include "Genotype.h"

typedef Eigen::SparseMatrix<double> sparse_matrix;
typedef Eigen::VectorXd algebra_vector;
typedef Eigen::LeastSquaresConjugateGradient<sparse_matrix> linear_solver;

Fitness::Fitness() : total_cost(-1), excess_force(-1), nonzero_sum(-1), aggregate(-1) {}

Fitness::Fitness(float64 total_cost, float64 excess_force, float64 nonzero_sum) :
		total_cost(total_cost), excess_force(excess_force), nonzero_sum(nonzero_sum) {
	aggregate = 30000 / total_cost - 5 * excess_force - 75 * nonzero_sum;
}

Fitness &Fitness::operator=(const Fitness &f) {
	this->aggregate = f.aggregate;
	this->total_cost = f.total_cost;
	this->excess_force = f.excess_force;
	this->nonzero_sum = f.nonzero_sum;
	return *this;
}

Genotype::Genotype(Hyperparameters &hparams) : hparams(hparams) {
	for (uint32 k = 0; k < hparams.num_free_nodes; k++) {
		float64 x = rand_float(hparams.x_min, hparams.x_max);
		float64 y = rand_float(hparams.y_min, hparams.y_max);
		Point p(x, y);
		points.push_back(p);
	}
	for (uint32 k = 0; k < hparams.num_struts; k++) {
		bool strut = rand_bit();
		struts.push_back(strut);
	}
}

Genotype::Genotype(Genotype *g1, Genotype *g2, EvolutionParameters &eparams) :
		hparams(g1->hparams) {
	float64 chance_b = rand_float(0, 1);
	float64 chance_f = rand_float(0, 1);
	if (chance_b < eparams.single_point_trunc) {
		int32 point = rand_int(0, g1->struts.size());
		uint32 k = 0;
		for (; k < point; k++) {
			struts.push_back(g1->struts[k]);
		}
		for (; k < g1->hparams.num_struts; k++) {
			struts.push_back(g2->struts[k]);
		}
	} else if (chance_b < eparams.two_point_trunc) {
		int32 point1 = rand_int(0, g1->struts.size());
		int32 point2 = rand_int(0, g2->struts.size());
		if (point2 < point1) {
			int32 t = point1;
			point1 = point2;
			point2 = t;
		}
		uint32 k = 0;
		for (; k < point1; k++) {
			struts.push_back(g1->struts[k]);
		}
		for (; k < point2; k++) {
			struts.push_back(g2->struts[k]);
		}
		for (; k < hparams.num_struts; k++) {
			struts.push_back(g1->struts[k]);
		}
	} else if (chance_b < eparams.uniform_trunc) {
		for (uint32 k = 0; k < hparams.num_struts; k++) {
			bool bit = rand_float(0, 1) < 0.5 ? g1->struts[k] : g2->struts[k];
			struts.push_back(bit);
		}
	} else {
		for (uint32 k = 0; k < hparams.num_struts; k++) {
			struts.push_back(g1->struts[k]);
		}
	}
	if (chance_f < eparams.average_trunc) {
		for (uint32 k = 0; k < hparams.num_free_nodes; k++) {
			Point *p1 = &g1->points[k];
			Point *p2 = &g2->points[k];
			Point p(0.5 * (p1->x + p2->x), 0.5 * (p1->y + p2->y));
			points.push_back(p);
		}
	} else if (chance_f < eparams.randomize_trunc) {
		for (uint32 k = 0; k < hparams.num_free_nodes; k++) {
			float64 x = rand_float(0, 1) < 0.5 ? g1->points[k].x : g2->points[k].y;
			float64 y = rand_float(0, 1) < 0.5 ? g1->points[k].y : g2->points[k].y;
			points.push_back(Point(x, y));
		}
	} else {
		for (uint32 k = 0; k < hparams.num_free_nodes; k++) {
			points.push_back(g1->points[k]);
		}
	}
}

void Genotype::evaluate() {
	Point nodes[hparams.num_nodes];
	uint32 node_k = 0;
	for (; node_k < hparams.num_fixed_nodes; node_k++) {
		nodes[node_k].x = node_k * hparams.spacing;
		nodes[node_k].y = 0;
	}
	for (; node_k < hparams.num_nodes; node_k++) {
		nodes[node_k].x = points[node_k - hparams.num_fixed_nodes].x;
		nodes[node_k].y = points[node_k - hparams.num_fixed_nodes].y;
	}
	bool node_active[hparams.num_nodes]{0};
	uint_vector active_nodes;
	uint_vector active_struts;
	for (uint32 k = 0; k < hparams.num_struts; k++) {
		if (struts[k]) {
			node_active[hparams.strut_inverse[k].x] = 1;
			node_active[hparams.strut_inverse[k].y] = 1;
			active_struts.push_back(k);
		}
	}
	for (uint32 k = 0; k < hparams.num_nodes; k++) {
		if (node_active[k]) {
			active_nodes.push_back(k);
		}
	}
	uint32 num_equations = 2 * active_nodes.size();
	uint32 num_variables = active_struts.size();
	sparse_matrix coefficients(num_equations, num_variables);
	algebra_vector constants(num_equations);
	algebra_vector variables(num_variables);
	for (uint32 k = 0; k < num_equations; k++) {
		constants[k] = 0;
	}
	for (uint32 k = 0; k < hparams.num_fixed_nodes; k++) {
		constants[2 * k + 1] = -hparams.loads[k];
	}
	uint32 strut_norm_i = 0;
	for (uint32 i = 0; i < active_nodes.size() - 1; i++) {
		for (uint32 j = i + 1; j < active_nodes.size(); j++) {
			uint32 node_i = active_nodes[i];
			uint32 node_j = active_nodes[j];
			uint32 strut_index = hparams.strut_indices[node_i][node_j];
			if (struts[strut_index]) {
				Point u_ij = unit_vector(nodes, node_i, node_j);
				Point u_ji = u_ij * -1;
				coefficients.insert(2 * i, strut_norm_i) = u_ij.x;
				coefficients.insert(2 * i + 1, strut_norm_i) = u_ij.y;
				coefficients.insert(2 * j, strut_norm_i) = u_ji.x;
				coefficients.insert(2 * j + 1, strut_norm_i) = u_ji.y;
				strut_norm_i++;
			}
		}
	}
	coefficients.makeCompressed();

	linear_solver solver;
	solver.analyzePattern(coefficients);
	solver.factorize(coefficients);
	variables = solver.solve(constants);

	float64 excess_force = 0;
	float64 total_cost = active_nodes.size() * hparams.gusset_cost;
	Point force_sum[hparams.num_nodes];
	for (uint32 k = 0; k < active_nodes.size(); k++) {
		uint32 node_index = active_nodes[k];
		force_sum[node_index].x = 0;
		force_sum[node_index].y = hparams.loads[node_index];
	}
	for (uint32 k = 0; k < active_struts.size(); k++) {
		uint32 strut_k = active_struts[k];
		uint32 node_i = (uint32) hparams.strut_inverse[strut_k].x;
		uint32 node_j = (uint32) hparams.strut_inverse[strut_k].y;
		float64 strut_length = node_length(nodes, node_i, node_j);
		Point f_ij = unit_vector(nodes, node_i, node_j) * variables[k];
		force_sum[node_i].x += f_ij.x;
		force_sum[node_i].y += f_ij.y;
		force_sum[node_j].x -= f_ij.x;
		force_sum[node_j].y -= f_ij.y;
		float64 member_cost = strut_length * hparams.member_cost;
		total_cost += member_cost;
		if (variables[k] < hparams.min_force * 2) {
			excess_force += 2 * hparams.min_force - variables[k];
		} else if (variables[k] < hparams.min_force) {
			total_cost += member_cost;
		}
		if (variables[k] > hparams.max_force * 2) {
			excess_force += variables[k] - 2 * hparams.max_force;
		} else if (variables[k] > hparams.max_force) {
			total_cost += member_cost;
		}
	}
	float64 nonzero_sum = 0;
	for (uint32 k = 0; k < hparams.num_nodes; k++) {
		nonzero_sum += fabs(force_sum[k].x);
		nonzero_sum += fabs(force_sum[k].y);
	}
	Fitness fitness(total_cost, excess_force, nonzero_sum);
	this->fitness = fitness;
}
