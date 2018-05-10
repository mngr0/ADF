/**
 * \file
 *
 * \brief Position Decoder related functionality declaration.
 *
 * Copyright (C) 2016 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#ifndef _HPL_PDEC_ASYNC_H_INCLUDED
#define _HPL_PDEC_ASYNC_H_INCLUDED

/**
 * \addtogroup HPL Position Decoder
 *
 * \section hpl_async_pdec_rev Revision History
 * - v1.0.0 Initial Release
 *
 *@{
 */

#include <hpl_irq.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Position Decoder device structure
 *
 * The Position Decoder device structure forward declaration.
 */
struct _pdec_async_device;

/**
 * \brief Position Decoder callback types
 */
enum pdec_async_callback_type {
	/** Position changed */
	PDEC_ASYNC_POS_CHANGED_CB,
	/** IRQ events */
	PDEC_ASYNC_IRQ_CB
};

/**
 * \brief Position Decoder interrupt types
 */
enum pdec_async_interrupt_type {
	/** Interrupt type for direction changed */
	PDEC_IRQ_DIRECTION,
	/** Interrupt type for position counter overflow */
	PDEC_IRQ_OVERFLOW,
	/** Interrupt type for position counter underflow */
	PDEC_IRQ_UNDERFLOW,
	/** Interrupt type for errors */
	PDEC_IRQ_ERROR
};

/**
 * \brief Position Decoder interrupt callbacks
 */
struct _pdec_async_callbacks {
	void (*pos_changed)(const struct _pdec_async_device *const device, uint8_t ch);
	void (*irq_handler)(const struct _pdec_async_device *dev, enum pdec_async_interrupt_type type, uint8_t ch);
};

/**
 * \brief Position Decoder descriptor device structure
 */
struct _pdec_async_device {
	struct _pdec_async_callbacks pdec_async_cb;
	struct _irq_descriptor       irq;
	void *                       hw;
};

/**
 * \brief Initialize Position Decoder
 *
 * This function does low level Position Decoder configuration.
 *
 * param[in] device The pointer to Position Decoder device instance
 * param[in] hw     The pointer to hardware instance
 *
 * \return Initialization status
 */
int32_t _pdec_async_init(struct _pdec_async_device *const device, void *const hw);

/**
 * \brief Deinitialize Position Decoder
 *
 * \param[in] device The pointer to Position Decoder device instance
 */
void _pdec_async_deinit(struct _pdec_async_device *const device);

/**
 * \brief Enable Position Decoder
 *
 * \param[in] device The pointer to Position Decoder device instance
 */
void _pdec_async_enable(struct _pdec_async_device *const device);

/**
 * \brief Disable Position Decoder
 *
 * \param[in] device The pointer to Position Decoder device instance
 */
void _pdec_async_disable(struct _pdec_async_device *const device);

/**
 * \brief Write motor axis position
 *
 * \param[in] device     The pointer to Position Decoder device instance
 * \param[in] value      The position count to write
 * \param[in] axis       The axis number to write
 */
void _pdec_async_write_position(struct _pdec_async_device *const device, uint32_t value, uint8_t axis);

/**
 * \brief Read motor axis position
 *
 * \param[in] device     The pointer to Position Decoder device instance
 * \param[in] axis       The axis number to read
 *
 * \return The position count of motor axis
 */
uint32_t _pdec_async_read_position(struct _pdec_async_device *const device, uint8_t axis);

/**
 * \brief Set Position Decoder upper threshold
 *
 * This function sets Position Decoder upper threshold.
 *
 * \param[in] device         The pointer to Position Decoder device instance
 * \param[in] up_threshold   An upper threshold to set
 * \param[in] axis           The axis number to set
 *
 * \return Status of Position Decoder threshold setting.
 */
int32_t _pdec_async_set_up_threshold(struct _pdec_async_device *const device, const uint32_t up_threshold,
                                     uint8_t axis);

/**
 * \brief Set Position Decoder lower threshold
 *
 * This function sets Position Decoder lower threshold.
 *
 * \param[in] device         The pointer to Position Decoder device instance
 * \param[in] low_threshold  An lower threshold to set
 * \param[in] axis           The axis number to set
 *
 * \return Status of Position Decoder threshold setting.
 */
int32_t _pdec_async_set_low_threshold(struct _pdec_async_device *const device, const uint32_t low_threshold,
                                      uint8_t axis);

/**
 * \brief Enable/disable Position Decoder interrupt
 *
 * \param[in] device   The pointer to Position Decoder device instance
 * \param[in] type     The type of interrupt to disable/enable if applicable
 * \param[in] state    Enable or disable
 */
void _pdec_async_set_irq_state(struct _pdec_async_device *const device, const enum pdec_async_callback_type type,
                               const bool state);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* _HPL_PDEC_ASYNC_H_INCLUDED */
