/**
 * \file
 *
 * \brief SPI Slave functionality declaration.
 *
 * Copyright (C) 2015 Atmel Corporation. All rights reserved.
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

#ifndef _HAL_SPI_S_ASYNC_H_INCLUDED
#define _HAL_SPI_S_ASYNC_H_INCLUDED

#include "hpl_spi_s_async.h"
#include "utils.h"
#include "utils_ringbuffer.h"
#include "hal_io.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup doc_driver_hal_spi_slave_async
 *
 * @{
 */

/** \brief SPI slave status
 */
struct spi_s_async_status {
	/** Number of characters to send */
	uint32_t txcnt;
	/** Number of characters ready in buffer */
	uint16_t rxrdy_cnt;
	/** Last error code. */
	int8_t error;
	/** TX busy. */
	uint8_t tx_busy;
};

/** Forward declaration or SPI Slave Descriptor. */
struct spi_s_async_descriptor;

/** The callback types */
enum spi_s_async_cb_type {
	/** Callback type for TX finish, see \ref spi_s_cb_xfer_t. */
	SPI_S_CB_TX,
	/** Callback type for RX notification, see \ref spi_s_cb_xfer_t. */
	SPI_S_CB_RX,
	/** Callback type for CS deactivate, error or abort, see
     *  \ref spi_s_cb_complete_t. */
	SPI_S_CB_COMPLETE
};

/** \brief Prototype of callback on SPI transfer completion
 *
 *  Invoked on intended transfer completion by deactivating the CS, error case, or
 *  abort.
 *  Transfer status has been passed on for quick check (0 success, -1 error).
 *  For details on a transfer report such as the transfer counts,
 *  invoke \ref spi_s_get_status to check status of \ref spi_s_status_t.
 */
typedef void (*spi_s_async_cb_complete_t)(struct spi_s_async_descriptor *, const int32_t status);

/** \brief Prototype of callback on SPI transfer notification
 *
 *  Invoked on notification threshold achieved, which means the number of
 *  expected bytes received, or remaining number of bytes lower than a
 *  threshold.
 */
typedef void (*spi_s_async_cb_xfer_t)(struct spi_s_async_descriptor *);

/** \brief SPI Slave HAL callbacks
 */
struct spi_s_async_callbacks {
	/** Callback invoked when the CS deactivates, goes wrong, or aborts. */
	spi_s_async_cb_complete_t complete;
	/** Callback invoked when transmitting has been down. */
	spi_s_async_cb_xfer_t tx;
	/** Callback invoked when each character received. */
	spi_s_async_cb_xfer_t rx;
};

/** \brief SPI Slave HAL driver struct for asynchronous access with ring buffer
 */
struct spi_s_async_descriptor {
	/** SPI device instance */
	struct _spi_s_async_dev dev;
	/** I/O read/write */
	struct io_descriptor io;

	/** Callbacks for asynchronous transfer */
	struct spi_s_async_callbacks callbacks;

	/** Transmit buffer. */
	uint8_t *txbuf;
	/** Transmit size in number of characters. */
	uint16_t txsize;
	/** Transmit count in number of characters. */
	uint16_t txcnt;

	/** ring buffer for RX. */
	struct ringbuffer rx_rb;

	/** Last error code. */
	int8_t error;
	/** TX busy. */
	uint8_t busy : 1;
	/** Enabled. */
	uint8_t enabled : 1;
};

/** \brief Initialize the SPI Slave HAL instance and hardware for callback mode
 *
 *  Initialize SPI Slave HAL with interrupt mode (uses callbacks).
 *
 *  \param[in, out] spi Pointer to the SPI Slave HAL instance.
 *  \param[in] hw Pointer to the hardware base.
 *  \param[in] rxbuf Pointer to the buffer for receiving ring buffer.
 *  \param[in] bufsize The receiving ring buffer size.
 *
 *  \return Operation status.
 *  \retval 0 Success.
 *  \retval -1 Error.
 */
int32_t spi_s_async_init(struct spi_s_async_descriptor *spi, void *const hw, uint8_t *const rxbuf, int16_t bufsize);

/** \brief Deinitialize the SPI HAL instance

 *  Abort transfer, flush buffers, disable and reset SPI, de-init software.
 *
 *  \param[in,out] spi Pointer to the SPI Slave HAL instance.
 */
void spi_s_async_deinit(struct spi_s_async_descriptor *spi);

/** \brief Enable SPI and start background RX

 *  Enable the hardware and start RX with buffer in background.
 *
 *  \param[in,out] spi Pointer to the SPI Slave HAL instance.
 */
void spi_s_async_enable(struct spi_s_async_descriptor *spi);
/** \brief Disable SPI and abort any pending transfer in progress and flush buffers
 *
 * Disable SPI and flush buffers. If there is pending transfer, the complete
 * callback is invoked with \c SPI_ERR_ABORT status.
 *
 *  \param[in,out] spi Pointer to the SPI Slave HAL instance.
 */
void spi_s_async_disable(struct spi_s_async_descriptor *spi);

/** \brief Set SPI mode

 *  Set the SPI transfer mode (\ref spi_transfer_mode_t),
 *  which controls the clock polarity and clock phase:
 *  - Mode 0: leading edge is rising edge, data sample on leading edge.
 *  - Mode 1: leading edge is rising edge, data sample on trailing edge.
 *  - Mode 2: leading edge is falling edge, data sample on leading edge.
 *  - Mode 3: leading edge is falling edge, data sample on trailing edge.
 *  Note that SPI must be disabled to change mode.
 *
 *  \param[in,out] spi Pointer to the HAL SPI instance.
 *  \param[in] mode The mode (\ref spi_transfer_mode_t).
 *
 *  \return Operation status.
 *  \retval ERR_NONE Success.
 *  \retval ERR_DENIED Not Ready
 *  \retval ERR_BUSY Busy, CS activated.
 */
int32_t spi_s_async_set_mode(struct spi_s_async_descriptor *spi, const enum spi_transfer_mode mode);

/** \brief Set SPI transfer character size in number of bits
 *
 *  The character size (\ref spi_char_size_t) influence the way the data is
 *  sent/received.
 *  For char size <= 8-bit, data is stored byte by byte.
 *  For char size between 9-bit ~ 16-bit, data is stored in 2-byte length.
 *  Note that the default and recommended char size is 8-bit since it's
 *  supported by all system.
 *  Note that the SPI must be disabled to change character size. Also it affects
 *  buffer accessing, the ring buffer should be flushed before changing it to
 *  avoid conflicts.
 *
 *  \param[in,out] spi Pointer to the HAL SPI instance.
 *  \param[in] char_size The char size (~32, recommended 8).
 *
 *  \return Operation status.
 *  \retval ERR_NONE Success.
 *  \retval ERR_DENIED Not Ready
 *  \retval ERR_BUSY Busy, CS activated.
 */
int32_t spi_s_async_set_char_size(struct spi_s_async_descriptor *spi, const enum spi_char_size char_size);

/** \brief Set SPI transfer data order
 *
 *  Note that the SPI must be disabled to change data order.
 *
 *  \param[in,out] spi Pointer to the HAL SPI instance.
 *  \param[in] dord The data order: send LSB/MSB first.
 *
 *  \return Operation status.
 *  \retval ERR_NONE Success.
 *  \retval ERR_DENIED Not Ready
 *  \retval ERR_BUSY Busy, CS activated.
 */
int32_t spi_s_async_set_data_order(struct spi_s_async_descriptor *spi, const enum spi_data_order dord);

/** \brief Get SPI transfer status
 *
 *  Get transfer status, buffers statuses in a structured way.
 *
 *  \param[in,out] spi Pointer to the HAL SPI instance.
 *  \param[out] stat Pointer to the detailed status descriptor, set to NULL
 *                to not return details.
 *
 *  \return Status.
 *  \retval 0  Not busy.
 *  \retval ERR_BUSY  Busy.
 *  \retval -1 Error.
 */
int32_t spi_s_async_get_status(struct spi_s_async_descriptor *spi, struct spi_s_async_status *stat);

/** \brief Register a function as SPI transfer completion callback
 *
 *  Register callback function specified by its \c type.
 *  - SPI_S_CB_COMPLETE: set the function that will be called on SPI transfer
 *    completion including deactivating the CS.
 *  - SPI_S_CB_TX: set the function that will be called on the
 *    TX threshold notification.
 *  - SPI_S_CB_RX: set the function that will be called on the
 *    RX threshold notification.
 *  Register a NULL function to not use the callback.
 *
 *  \param[in,out] spi Pointer to the HAL SPI instance.
 *  \param[in] type Callback type (\ref spi_s_cb_type).
 *  \param[in] func Pointer to callback function.
 */
void spi_s_async_register_callback(struct spi_s_async_descriptor *spi, const enum spi_s_async_cb_type type,
                                   const FUNC_PTR func);

/**
 * \brief Flush the RX ring buffer
 *
 * \param[in,out] spi Pointer to the HAL SPI instance.
 */
void spi_s_async_flush_rx_buffer(struct spi_s_async_descriptor *spi);

/**
 * \brief Abort pending transmission
 *
 * \param[in,out] spi Pointer to the HAL SPI instance.
 */
void spi_s_async_abort_tx(struct spi_s_async_descriptor *spi);

/**
 * \brief Return I/O descriptor for this SPI instance
 *
 * This function will return an I/O instance for this SPI driver instance
 *
 * \param[in] spi An spi slave descriptor which is used to communicate through
 *                SPI
 * \param[in] io A pointer to an I/O descriptor pointer type
 *
 * \return Error code
 * \retval 0 No error detected
 * \retval <0 Error code
 */
int32_t spi_s_async_get_io_descriptor(struct spi_s_async_descriptor *spi, struct io_descriptor **io);

/** \brief Retrieve the current driver version
 *
 *  \return Current driver version.
 */
uint32_t spi_s_async_get_version(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* _HAL_SPI_S_ASYNC_H_INCLUDED */
