/*
 * afe.h
 *
 *  Created on: Jan 6, 2023
 *      Author: MinhNhan
 */

#ifndef COMPONENTS_AFE_AFE_H_
#define COMPONENTS_AFE_AFE_H_

typedef struct AFE_t AFE;

struct AFE_t{
	void *p_driver;
	uint32_t num_channel;

};

#endif /* COMPONENTS_AFE_AFE_H_ */
