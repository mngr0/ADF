/*
* SDManager.h
*
* Created: 03/05/2018 17:16:47
*  Author: COLLAUDI E SVILUPPO
*/


#ifndef SDMANAGER_H_
#define SDMANAGER_H_

#include <atmel_start.h>

typedef struct datalog_item{

	uint32_t date;
	uint8_t event;
	uint16_t value;

} dl_item_t;


void start_sd();



#endif /* SDMANAGER_H_ */