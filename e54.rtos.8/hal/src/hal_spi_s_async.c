/**
 * \file
 *
 * \brief I/O SPI related functionality implementation.
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

#include "hal_atomic.h"
#include "hal_spi_s_async.h"

#include <utils_assert.h>
#include <utils.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Driver version
 */
#define SPI_S_DRIVER_VERSION 0x00000001u

/**
 * \brief Data transmitted callback
 */
static void spi_s_async_dev_tx(struct _spi_s_async_dev *dev)
{
	struct spi_s_async_descriptor *spi = CONTAINER_OF(dev, struct spi_s_async_descriptor, dev);

	if (dev->char_size > 1) {
		_spi_s_async_write_one(dev, ((uint16_t *)spi->txbuf)[spi->txcnt++]);
	} else {
		_spi_s_async_write_one(dev, spi->txbuf[spi->txcnt++]);
	}

	if (spi->txcnt >= spi->txsize) {
		_spi_s_async_enable_tx(dev, false);
		spi->busy = 0;
		spi->callbacks.tx(spi);
	}
}

/**
 * \brief Data received callback
 */
static void spi_s_async_dev_rx(struct _spi_s_async_dev *dev)
{
	struct spi_s_async_descriptor *spi = CONTAINER_OF(dev, struct spi_s_async_descriptor, dev);
	union {
		uint8_t  u8[2];
		uint16_t u16;
	} tmp;

	tmp.u16 = _spi_s_async_read_one(dev);

	ringbuffer_put(&spi->rx_rb, tmp.u8[0]);
	if (dev->char_size > 1) {
		ringbuffer_put(&spi->rx_rb, tmp.u8[1]);
	}

	spi->callbacks.rx(spi);
}

static void spi_s_async_dev_complete(struct _spi_s_async_dev *dev, int32_t status)
{
	struct spi_s_async_descriptor *spi = CONTAINER_OF(dev, struct spi_s_async_descriptor, dev);

	spi->callbacks.complete(spi, status);
}

/** \brief Do SPI data write
 *
 *  Register background buffer to transmit data.
 *
 *  It never blocks and return quickly, user check status or set callback to
 *  know when data is sent.
 *
 *  \param[in] io Pointer to the I/O descriptor.
 *  \param[in] buf Pointer to the buffer to store data to write.
 *  \param[in] size Size of the data in number of characters.
 *  \return Operation status.
 *  \retval 0 Success.
 *  \retval -1 Busy, transfer in progress.
 *  \retval -3 Parameter error.
 */
static int32_t _spi_s_async_io_write(struct io_descriptor *const io, const uint8_t *const buf, const uint16_t size)
{
	struct spi_s_async_descriptor *spi;
	volatile hal_atomic_t          flags;

	ASSERT(io);

	if (size == 0) {
		return ERR_NONE;
	}

	spi = CONTAINER_OF(io, struct spi_s_async_descriptor, io);

	atomic_enter_critical(&flags);
	if (spi->busy) {
		atomic_leave_critical(&flags);
		return ERR_BUSY;
	}
	spi->busy = 1;
	atomic_leave_critical(&flags);

	spi->txbuf  = (uint8_t *)buf;
	spi->txcnt  = 0;
	spi->txsize = size;

	_spi_s_async_enable_tx(&spi->dev, true);

	return ERR_NONE;
}

/** \brief Do SPI read from ring-buffer (asynchronously)
 *
 *  Read available characters from ring-buffer and return number of characters
 *  read.
 *
 *  It never blocks and return quickly, user check status or set callback to
 *  know when data is ready.
 *
 *  \param[in] io Pointer to the I/O descriptor.
 *  \param[out] buf Pointer to the buffer to store read data,
                NULL to discard data.
 *  \param[in] size Size of the data in number of characters.
 *  \return Read result.
 *  \retval n Number of characters read.
 *  \retval <0 Error.
 */
static int32_t _spi_s_async_io_read(struct io_descriptor *const io, uint8_t *const buf, const uint16_t size)
{
	struct spi_s_async_descriptor *spi;
	uint32_t                       n_bytes, i;
	uint8_t *                      p = (uint8_t *)buf;

	ASSERT(io);

	if (size == 0) {
		return 0;
	}

	spi = CONTAINER_OF(io, struct spi_s_async_descriptor, io);

	n_bytes = ringbuffer_num(&spi->rx_rb);
	if (n_bytes == 0) {
		return 0;
	} else {
		uint16_t n = size;

		switch (spi->dev.char_size) {
		case 2:
			n <<= 1;
			break;
		default:
			break;
		}
		if (n_bytes > n) {
			n_bytes = n;
		}
	}

	if (p == NULL) {
		uint8_t tmp;

		for (i = 0; i < n_bytes; i++) {
			ringbuffer_get(&spi->rx_rb, &tmp);
		}
	} else {
		for (i = 0; i < n_bytes; i++) {
			ringbuffer_get(&spi->rx_rb, p++);
		}
	}

	switch (spi->dev.char_size) {
	case 2:
		return (int32_t)(n_bytes >> 1);
	default:
		return (int32_t)n_bytes;
	}
}

/**
 * \brief A dummy function to be called when SPI callbacks are not registered
 * \param[in,out] spi Pointer to the HAL SPI instance.
 * \param[in] param Parameter.
 */
static void spi_s_async_dummy_func(struct spi_s_async_descriptor *spi, uint32_t param)
{
	(void)spi;
	(void)param;
}

int32_t spi_s_async_init(struct spi_s_async_descriptor *spi, void *const hw, uint8_t *const rxbuf, int16_t bufsize)
{
	int32_t rc;

	ASSERT(spi && hw && rxbuf && bufsize);

	rc = ringbuffer_init(&spi->rx_rb, rxbuf, bufsize);
	if (rc < 0) {
		return rc;
	}
	rc = _spi_s_async_init(&spi->dev, hw);
	if (rc < 0) {
		return rc;
	}
	_spi_s_async_register_callback(&spi->dev, SPI_DEV_CB_TX, (const FUNC_PTR)spi_s_async_dev_tx);
	_spi_s_async_register_callback(&spi->dev, SPI_DEV_CB_RX, (const FUNC_PTR)spi_s_async_dev_rx);
	_spi_s_async_register_callback(&spi->dev, SPI_DEV_CB_COMPLETE, (const FUNC_PTR)spi_s_async_dev_complete);

	spi->enabled = 0;
	spi->busy    = 0;

	spi->callbacks.complete = (spi_s_async_cb_complete_t)spi_s_async_dummy_func;
	spi->callbacks.tx       = (spi_s_async_cb_xfer_t)spi_s_async_dummy_func;
	spi->callbacks.rx       = (spi_s_async_cb_xfer_t)spi_s_async_dummy_func;

	spi->io.read  = _spi_s_async_io_read;
	spi->io.write = _spi_s_async_io_write;

	return ERR_NONE;
}

void spi_s_async_deinit(struct spi_s_async_descriptor *spi)
{
	ASSERT(spi);

	spi_s_async_disable(spi);

	_spi_s_async_enable_tx(&spi->dev, false);
	_spi_s_async_deinit(&spi->dev);
}

void spi_s_async_enable(struct spi_s_async_descriptor *spi)
{
	ASSERT(spi);

	if (spi->enabled) {
		return;
	}
	if (_spi_s_async_enable(&spi->dev) != 0) {
		return;
	}
	spi->enabled = 1;
	_spi_s_async_enable_rx(&spi->dev, true);
	_spi_s_async_enable_ss_detect(&spi->dev, true);
}

void spi_s_async_disable(struct spi_s_async_descriptor *spi)
{
	ASSERT(spi);

	if (!spi->enabled) {
		return;
	}
	_spi_s_async_disable(&spi->dev);
	_spi_s_async_enable_rx(&spi->dev, false);
	_spi_s_async_enable_ss_detect(&spi->dev, false);
	spi->enabled = 0;
}

int32_t spi_s_async_set_mode(struct spi_s_async_descriptor *spi, const enum spi_transfer_mode mode)
{
	ASSERT(spi);

	if (spi->enabled) {
		return ERR_DENIED;
	}
	return _spi_s_async_set_mode(&spi->dev, mode);
}

int32_t spi_s_async_set_char_size(struct spi_s_async_descriptor *spi, const enum spi_char_size char_size)
{
	ASSERT(spi);

	if (spi->enabled) {
		return ERR_DENIED;
	}
	return _spi_s_async_set_char_size(&spi->dev, char_size);
}

int32_t spi_s_async_set_data_order(struct spi_s_async_descriptor *spi, const enum spi_data_order dord)
{
	ASSERT(spi);

	if (spi->enabled) {
		return ERR_DENIED;
	}
	return _spi_s_async_set_data_order(&spi->dev, dord);
}

int32_t spi_s_async_get_status(struct spi_s_async_descriptor *spi, struct spi_s_async_status *stat)
{
	ASSERT(spi);

	if (stat) {
		stat->tx_busy   = spi->busy;
		stat->error     = spi->error;
		stat->txcnt     = spi->txcnt;
		stat->rxrdy_cnt = ringbuffer_num(&spi->rx_rb);
		if (spi->dev.char_size > 1) {
			stat->rxrdy_cnt >>= 1;
		}
	}

	return spi->busy ? ERR_BUSY : spi->error;
}

void spi_s_async_register_callback(struct spi_s_async_descriptor *spi, const enum spi_s_async_cb_type type,
                                   const FUNC_PTR func)
{
	FUNC_PTR f = (func == NULL) ? (FUNC_PTR)spi_s_async_dummy_func : (FUNC_PTR)func;

	ASSERT(spi);

	switch (type) {
	case SPI_S_CB_TX:
		spi->callbacks.tx = (spi_s_async_cb_xfer_t)f;
		break;
	case SPI_S_CB_RX:
		spi->callbacks.rx = (spi_s_async_cb_xfer_t)f;
		break;
	case SPI_S_CB_COMPLETE:
		spi->callbacks.complete = (spi_s_async_cb_complete_t)f;
		break;
	default:
		break;
	}
}

void spi_s_async_flush_rx_buffer(struct spi_s_async_descriptor *spi)
{
	ASSERT(spi);

	ringbuffer_flush(&spi->rx_rb);
}

void spi_s_async_abort_tx(struct spi_s_async_descriptor *spi)
{
	ASSERT(spi);

	if (!spi->busy) {
		return;
	}

	_spi_s_async_enable_tx(&spi->dev, false);
	spi->busy = 0;
}

int32_t spi_s_async_get_io_descriptor(struct spi_s_async_descriptor *spi, struct io_descriptor **io)
{
	ASSERT(spi && io);
	*io = &spi->io;
	return ERR_NONE;
}

uint32_t spi_s_async_get_version(void)
{
	return SPI_S_DRIVER_VERSION;
}

#ifdef __cplusplus
}
#endif
