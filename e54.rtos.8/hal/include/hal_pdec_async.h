/**
 * \file
 *
 * \brief Position Decoder functionality declaration.
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

#ifndef HAL_PDEC_ASYNC_H_INCLUDED
#define HAL_PDEC_ASYNC_H_INCLUDED

#include <utils.h>
#include <hpl_pdec_async.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup doc_driver_hal_pdec_async
 *
 *@{
 */

/**
 * \brief Position Decoder descriptor
 *
 * The Position Decoder descriptor forward declaration.
 */
struct pdec_async_descriptor;

typedef void (*pdec_async_position_cb_t)(const struct pdec_async_descriptor *const descr, uint8_t ch);
typedef void (*pdec_async_irq_cb_t)(const struct pdec_async_descriptor *const descr,
                                    enum pdec_async_interrupt_type type, uint8_t ch);

/**
 * \brief Position Decoder callbacks
 */
struct pdec_async_callbacks {
	/* Callback type for position changed */
	pdec_async_position_cb_t pos_changed;
	/* Callback type for other interrupts */
	pdec_async_irq_cb_t irq_handler;
};

/**
 * \brief Position Decoder descriptor
 */
struct pdec_async_descriptor {
	/* Position Decoder device */
	struct _pdec_async_device device;
	/* Position Decoder callbacks type */
	struct pdec_async_callbacks pdec_async_cb;
};

/**
 * \brief Initialize Position Decoder
 *
 * This function initializes the given Position Decoder descriptor.
 * It checks if the given hardware is not initialized and if the given hardware
 * is permitted to be initialized.
 *
 * \param[out] descr         A Position Decoder descriptor to be initialized
 * \param[in] hw             The pointer to hardware instance
 *
 * \return Initialization status.
 * \retval -1 Passed parameters were invalid
 * \retval 0 The initialization is completed successfully
 */
int32_t pdec_async_init(struct pdec_async_descriptor *const descr, void *const hw);

/**
 * \brief Deinitialize Position Decoder
 *
 * This function deinitializes the given Position Decoder descriptor.
 * It checks if the given hardware is initialized and if the given hardware is
 * permitted to be deinitialized.
 *
 * \param[in] descr A Position Decoder descriptor to deinitialize
 *
 * \return De-initialization status.
 */
int32_t pdec_async_deinit(struct pdec_async_descriptor *const descr);

/**
 * \brief Eanble Position Decoder
 *
 * This function starts the Position Decoder
 *
 * \param[in] descr The pointer to the Position Decoder descriptor
 *
 * \return Enabling status.
 */
int32_t pdec_async_enable(struct pdec_async_descriptor *const descr);

/**
 * \brief Disable Position Decoder
 *
 * This function stops of Position Decoder
 *
 * \param[in] descr The pointer to the Position Decoder descriptor
 *
 * \return Disabling status.
 */
int32_t pdec_async_disable(struct pdec_async_descriptor *const descr);

/**
 * \brief Write motor axis position
 *
 * \param[in] descr     The pointer to the Position Decoder descriptor
 * \param[in] value     The position count to write
 * \param[in] axis      The axis number to read
 *
 * \return Write operation status
 */
int32_t pdec_async_write_position(struct pdec_async_descriptor *const descr, uint32_t value, uint8_t axis);

/**
 * \brief Read motor axis position
 *
 * This function reads the position count of the motor axis. With this count,
 * the user will know if the motor axis movement is clockwise (increase) or
 * re-clockwise (decrease),  and the position of the movement.
 *
 * \param[in] descr     The pointer to the Position Decoder descriptor
 * \param[in] axis      The axis number to read
 *
 * \return The position count of the motor axis.
 */
uint32_t pdec_async_read_position(struct pdec_async_descriptor *const descr, uint8_t axis);

/**
 * \brief Set Position Decoder upper threshold
 *
 * This function sets Position Decoder upper threshold.
 *
 * \param[in] descr         The pointer to the Position Decoder descriptor
 * \param[in] up_threshold  An upper threshold to set
 *
 * \return Status of the Position Decoder upper threshold setting.
 */
int32_t pdec_async_set_up_threshold(struct pdec_async_descriptor *const descr, const uint32_t up_threshold,
                                    uint8_t axis);

/**
 * \brief Set Position Decoder lower threshold
 *
 * This function sets Position Decoder lower threshold.
 *
 * \param[in] descr          The pointer to the Position Decoder descriptor
 * \param[in] low_threshold  A lower threshold to set
 * \param[in] axis           The axis number to set
 *
 * \return Status of the Position Decoder lower threshold setting.
 */
int32_t pdec_async_set_low_threshold(struct pdec_async_descriptor *const descr, const uint32_t low_threshold,
                                     uint8_t axis);

/**
 * \brief Register Position Decoder callback
 *
 * \param[in] io_descr  A Position Decoder descriptor
 * \param[in] type      Callback type
 * \param[in] cb        A callback function, passing NULL de-registers callback
 *
 * \return The status of callback assignment.
 * \retval -1 Passed parameters were invalid
 * \retval 0 A callback is registered successfully
 */
int32_t pdec_async_register_callback(struct pdec_async_descriptor *const descr,
                                     const enum pdec_async_callback_type type, FUNC_PTR cb);

/**
 * \brief Retrieve the current driver version
 *
 * \return Current driver version.
 */
uint32_t pdec_async_get_version(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* HAL_PDEC_ASYNC_H_INCLUDED */
