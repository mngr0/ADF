/*
 * SPIManager.c
 *
 * Created: 03/05/2018 16:37:57
 *  Author: COLLAUDI E SVILUPPO
 */ 
#include "SPIManager.h"
#include "Formats.h"
#include <atmel_start.h>

static uint8_t ans[512];
static uint8_t rec[512];
static uint16_t rec_index;

static void SPI_buffering(const struct spi_s_async_descriptor *const desc)
{
	struct io_descriptor *io;
	spi_s_async_get_io_descriptor(&SPI_0, &io);
	io_read(io, &rec[rec_index++], 1);
	
}



static void complete_cb_SPI_0(const struct spi_s_async_descriptor *const desc)
{
	
	struct io_descriptor *io;
	spi_s_async_get_io_descriptor(&SPI_0, &io);
	uint8_t x,y,z;
	x=y=z=0;
	x=rec[0];
	y=rec[1];

	//if set send message to datalog routine, to log change


	//add read/edit machine conf
	//add read datalog
	
	if(x==1){
		//read active
		ans[0]=0xFF;
		ans[1]=machine_confs[y].active;
		io_write(io, ans, 2);
	}
	else if(x==3){
		//edit active
		machine_confs[y].active=rec[2];
	}
	else if (x==2)	{
		//read conf
		int j;
		ans[0]=0xFF;
		uint8_t* copier = &formats[y];
		for (j=0;j<sizeof(format_t);j++){
			ans[j+1]=copier[j];
		}
		io_write(io, ans, j+1);
	}
	else if (x==4)	{
		//edit conf
		int j;
		uint8_t* copier = &formats[y];
		for (j=0;j<sizeof(format_t);j++){
			copier[j]=rec[j+2];
		}
	}
	rec_index=0;
}

void SPI_0_example(void)
{
	spi_s_async_register_callback(&SPI_0, SPI_S_CB_COMPLETE, (FUNC_PTR)complete_cb_SPI_0);
	spi_s_async_register_callback(&SPI_0, SPI_S_CB_RX, (FUNC_PTR)SPI_buffering);
	spi_s_async_enable(&SPI_0);
	rec_index=0;
}