#ifndef __SPI_H
#define __SPI_H

#include <inttypes.h>
#include <seos.h>
#include <stdlib.h>

struct SpiDevice;

typedef uint8_t spi_cs_t;
typedef uint32_t SpiSpeed;

typedef void (*SpiCbkF)(void *cookie, int err);

struct SpiMode {
    enum {
        SPI_CPOL_IDLE_LO,
        SPI_CPOL_IDLE_HI,
    } cpol;

    enum {
        SPI_CPHA_LEADING_EDGE,
        SPI_CPHA_TRAILING_EDGE,
    } cpha;

    uint8_t bitsPerWord;
    enum {
        SPI_FORMAT_LSB_FIRST,
        SPI_FORMAT_MSB_FIRST,
    } format;

    uint16_t txWord;

    SpiSpeed speed;

    bool nssChange;
};

struct SpiPacket {
    void *rxBuf;
    const void *txBuf;
    size_t size;
};

/**
 * NOTE:
 *
 * To avoid copies, spiMasterRxTx() and spiSlaveRxTx() transfer ownership of
 * packets[] to the SPI driver.  The SPI driver returns ownership when the
 * callback is called.
 *
 * The caller MUST NOT pass packets[] allocated on the stack, and MUST NOT
 * deallocate or otherwise mutate packets[] in the meantime.
 */

int spiMasterRxTx(uint8_t busId, spi_cs_t cs,
        const struct SpiPacket packets[], size_t n,
        const struct SpiMode *mode, SpiCbkF callback,
        void *cookie);

int spiSlaveRequest(uint8_t busId, const struct SpiMode *mode,
        struct SpiDevice **dev);

int spiSlaveRxTx(struct SpiDevice *dev,
        const struct SpiPacket packets[], size_t n,
        SpiCbkF callback, void *cookie);

int spiSlaveWaitForInactive(struct SpiDevice *dev, SpiCbkF callback,
        void *cookie);

int spiSlaveRelease(struct SpiDevice *dev);
#endif /* __SPI_H */
