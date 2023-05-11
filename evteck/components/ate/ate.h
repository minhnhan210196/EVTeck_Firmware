/*
 * ate.h
 *
 *  Created on: May 11, 2023
 *      Author: Admin
 */

#ifndef COMPONENTS_ATE_ATE_H_
#define COMPONENTS_ATE_ATE_H_

#include "afe/afe.h"
#include "stdint.h"


typedef enum{
	ATE_ST_Init = 0,
	ATE_ST_Run,
	ATE_ST_Handle,
	ATE_ST_Maintaining
}ATE_ST_t;

typedef struct ATE_t ATE;
typedef struct ATE_Config_t ATE_Config;
struct ATE_t{
	AFE *p_afe;
	ATE_Config *p_config;
};

struct ATE_Config_t{

};

void ate_init(ATE *p_ate,ATE_Config *p_config);
void ate_poll(ATE *p_ate);

#endif /* COMPONENTS_ATE_ATE_H_ */
