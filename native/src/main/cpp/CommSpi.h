//
// Created by raffy on 7/4/18.
//

#ifndef PIDISP_SPI_H
#define PIDISP_SPI_H

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

#define SPI_DEV_MODE_0 0
#define SPI_DEV_MODE_1 1
#define SPI_DEV_MODE_2 2
#define SPI_DEV_MODE_3 3

/**
 * Get the current file descriptor for the specified SPI Channel
 * @param channel The spi channel (0 or 1)
 * @return The file descriptor
 */
int spi_get_fd(int channel);

/**
 * Writes a single byte to the SPI bus. This is a half-duplex operation.
 *
 * @param channel
 * @param data
 * @return
 */
int spi_write_byte(int channel, uint8_t data);

/**
 * Half-Duplex write operation. Chip-select is deactivated in this operation and you need to manually set/unset it yourself.
 *
 * @param channel
 * @param buffer
 * @param len
 * @return Returns 0 for successfull operation otherwise negative if an error occurs
 *
 * @see <a href="https://www.kernel.org/doc/Documentation/spi/spidev">SPI kernel documentation</a>
 */
int spi_write(int channel, uint8_t *buffer, unsigned int len);

/**
 * Half-Duplex read operation. Chip-select is deactivated in this operation and you need to manually set/unset it yourself.
 *
 * @param channel
 * @param buffer
 * @param len
 *
 * @return Returns 0 for successfull operation otherwise negative if an error occurs
 *
 * @see <a href="https://www.kernel.org/doc/Documentation/spi/spidev">SPI kernel documentation</a>
 */
int spi_read(int channel, uint8_t *buffer, unsigned int len);

/**
 * Full Duplex read/write operation. Received data overrides
 *
 * @param channel
 * @param buffer
 * @param len
 * @return
 */
int spi_transfer(int channel, uint8_t *buffer, int len);

/**
 * Initialize SPI Device
 *
 * @param channel
 * @param speed
 * @param mode
 * @return
 */
int spi_setup(int channel, int speed, int mode = 0);

#ifdef __cplusplus
}
#endif


#endif //PIDISP_SPI_H
