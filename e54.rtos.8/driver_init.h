/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef DRIVER_INIT_INCLUDED
#define DRIVER_INIT_INCLUDED

#include "atmel_start_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>

#include <hal_ext_irq.h>

#include <hal_pdec_async.h>

#include <hal_calendar.h>

#include <hal_spi_s_async.h>

#include <hal_mci_sync.h>
#include <hal_pwm.h>
#include <hpl_tc_base.h>
#include <hal_pwm.h>
#include <hpl_tc_base.h>

#include "hal_usb_device.h"

extern struct pdec_async_descriptor POSITION_DECODER_0;

extern struct calendar_descriptor CALENDER_INTERFACE;

extern struct spi_s_async_descriptor SPI_0;

extern struct mci_sync_desc IO_BUS;

extern struct pwm_descriptor PWM_0;

extern struct pwm_descriptor PWM_1;

void POSITION_DECODER_0_PORT_init(void);
void POSITION_DECODER_0_CLOCK_init(void);
void POSITION_DECODER_0_init(void);

void CALENDER_INTERFACE_CLOCK_init(void);
void CALENDER_INTERFACE_init(void);

void SPI_0_PORT_init(void);
void SPI_0_CLOCK_init(void);
void SPI_0_init(void);

void IO_BUS_PORT_init(void);
void IO_BUS_CLOCK_init(void);
void IO_BUS_init(void);

void PWM_0_PORT_init(void);
void PWM_0_CLOCK_init(void);
void PWM_0_init(void);

void PWM_1_PORT_init(void);
void PWM_1_CLOCK_init(void);
void PWM_1_init(void);

void USB_DEVICE_INSTANCE_CLOCK_init(void);
void USB_DEVICE_INSTANCE_init(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_INCLUDED
