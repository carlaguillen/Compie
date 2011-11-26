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


#ifndef SEMANTIC_ACTIONS_H_
#define SEMANTIC_ACTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include "machines.h"

/************************************************************/
/*						AÇÔES								*/
/************************************************************/
/*
 * As ações semânticas consistem três em vetores de funções.
 *
 * - actions_on_state_transition[MACHINE_TYPES][MAX_STATES][MTOKEN_TYPES]
 * 		É o vetor de funções que é chamado quando ocorre uma transição
 * 		de estado da máquina corrente do APE. Na transição deve ser cha-
 * 		mada a função pelo índice:
 * 			actions_on_state_transition[MÁQUINA_CORRENTE][ESTADO_CORRENTE][TIPO_DE_TOKEN_DE_MÁQUINA]
 *
 * - actions_on_machine_transition[MACHINE_TYPES][MAX_STATES][MACHINE_TYPES]
 * 		É o vetor de funções que é chamado quando ocorre uma transição
 * 		entre máquinas do APE. Na transição deve ser chamada a função
 * 		pelo índice:
 * 			actions_on_machine_transition[MÁQUINA_CORRENTE][ESTADO_CORRENTE][MÁQUINA_DESTINO]
 *
 * - actions_on_state_transition[MACHINE_TYPES][MAX_STATES]
 * 		É o vetor de funções que é chamado quando ocorre o retorno de
 * 		uma máquina da pilha. Na transição deve ser chamada a função
 * 		pelo índice:
 * 			actions_on_machine_return[MÁQUINA_CORRENTE][ESTADO_CORRENTE]
 *
 */

void (*actions_on_state_transition[MACHINE_TYPES][MAX_STATES][MTOKEN_TYPES])();
void (*actions_on_machine_transition[MACHINE_TYPES][MAX_STATES][MACHINE_TYPES])();
void (*actions_on_machine_return[MACHINE_TYPES][MAX_STATES])();

void init_semantic_actions();

#endif /* SEMANTIC_ACTIONS_H_ */
