/*
 * Formats.h
 *
 * Created: 03/05/2018 16:56:53
 *  Author: COLLAUDI E SVILUPPO
 */ 
#include <atmel_start.h>

#ifndef FORMATS_H_
#define FORMATS_H_

#define max_len 64 //len of the time array
#define SAVED_CONF 8
#define CHANNEL_NUMBER 2


typedef struct format{
	char name[16];
	uint32_t overtime;
	uint32_t undertime;
	uint32_t times[max_len];
	// uint32_t dot_pause;
	// uint32_t dot_len
	// uint32_t dot_enable
	uint32_t len;
}format_t;

typedef struct machine_configuration{
	
	uint8_t index_gun;
	uint8_t active;
	uint8_t starter;
} machine_configuration_t;

extern format_t formats[8];
extern machine_configuration_t machine_confs[2];

#endif /* FORMATS_H_ */