/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * semantic.c - Semantic Actions
 *
 * Created on: 31/10/2011
 * 		Authors:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 *
 */

#include "semantic_actions.h"

void dummy_semantic_action() {
	// DOES NOTHING
	printf("TODO\n");
}

void init_semantic_actions() {
	int i, j, k = 0;

	for( i = 0; i < MACHINE_TYPES; i++)
		for( j = 0; j < MAX_STATES; j++) {
			for( k = 0; k < MTOKEN_TYPES; k++)
				actions_on_state_transition[i][j][k] = dummy_semantic_action;

			for( k = 0; k < MACHINE_TYPES; k++)
				actions_on_machine_transition[i][j][k] = dummy_semantic_action;

			actions_on_machine_return[i][j] = dummy_semantic_action;
		}
}
