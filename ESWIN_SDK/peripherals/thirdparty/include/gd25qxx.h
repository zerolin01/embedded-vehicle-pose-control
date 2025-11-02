#ifndef __GD25Qxx_H
#define __GD25Qxx_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/

#include <stdbool.h>
#include <stdint.h>
#include "spi_master_driver.h"

/**
 * @brief  GD25Q128E Configuration
 */
#define GD25Q128E_FLASH_SIZE                     0x1000000      /* 128 MBits => 16MBytes */
#define GD25Q128E_BLOCK_SIZE                     0x10000           /* 256 sectors of 64KBytes */
#define GD25Q128E_SECTOR_SIZE                    0x1000         /* 4096 sectors of 4kBytes */
#define GD25Q128E_PAGE_SIZE                      0x100           /* 65536 pages of 256 bytes */

#define GD25Q128E_DUMMY_CYCLES_READ              4
#define GD25Q128E_DUMMY_CYCLES_READ_QUAD         10

#define GD25Q128E_BULK_ERASE_MAX_TIME            250000
#define GD25Q128E_SECTOR_ERASE_MAX_TIME          3000
#define GD25Q128E_SUBSECTOR_ERASE_MAX_TIME       800
#define GD25Qxx_TIMEOUT_VALUE                    1000

/* Write Operations */
#define WRITE_ENABLE_CMD                        0x06
#define WRITE_DISABLE_CMD                       0x04

/* Register Read Operations */
#define READ_STATUS_REG1_CMD                    0x05
#define READ_STATUS_REG2_CMD                    0x35
#define READ_STATUS_REG3_CMD                    0x15

/* Register Write Operations */
#define WRITE_STATUS_REG1_CMD                   0x01
#define WRITE_STATUS_REG2_CMD                   0x31
#define WRITE_STATUS_REG3_CMD                   0x11

/* Volatile SR Write Enable */
#define VOLATILE_SR_WRITE_ENABLE_CMD            0x50

    /* Read Operations */
#define READ_CMD                                0x03
#define FAST_READ_CMD                           0x0B
#define DUAL_OUT_FAST_READ_CMD                  0x3B
#define DUAL_INOUT_FAST_READ_CMD                0xBB
#define QUAD_OUT_FAST_READ_CMD                  0x6B
#define QUAD_INOUT_FAST_READ_CMD                0xEB

/* Set Burst with Wrap */
#define SET_BURST_WITH_WRAP                     0x77

    /* Program Operations */
#define PAGE_PROG_CMD                           0x02
#define QUAD_INPUT_PAGE_PROG_CMD                0x32

    /* Erase Operations */
#define SECTOR_ERASE_CMD                        0x20
#define BLOCK__ERASE_32K_CMD                    0x52
#define BLOCK__ERASE_64K_CMD                    0xd8
#define CHIP_ERASE_CMD                          0x60

    /* Identification Operations */
#define READ_DEVICE_ID_CMD                      0x90
#define READ_ID_CMD                             0x9F
#define READ_UNI_ID_CMD                         0x4B

    /* Security Registers */
#define ERASE_SECURITY_REG_CMD                  0x44
#define PROG_SECURITY_REG_CMD                   0x42
#define READ_SECURITY_REG_CMD                   0x48

/* Reset Operations */
#define RESET_ENABLE_CMD                        0x66
#define RESET_MEMORY_CMD                        0x99

#define PROG_ERASE_SUSPEND_CMD                  0x75
#define PROG_ERASE_RESUME_CMD                   0x7A

#define ENTER_QPI_MODE_CMD                      0x38
#define EXIT_QPI_MODE_CMD                       0xFF

/* Flag Status Register */
#define GD25Q128E_FSR_BUSY                      0x01
#define GD25Q128E_FSR_WREN                      0x02
#define GD25Q128E_FSR_QE                        0x02

#define GD25Qxx_OK                              0x00
#define GD25Qxx_ERROR                           0x01
#define GD25Qxx_BUSY                            0x02
#define GD25Qxx_TIMEOUT                         0x03

uint8_t Flash_GD25Qxx_Init(void);
void Flash_GD25Qxx_Reset(void);
uint8_t Flash_GD25Qxx_GetStatus(void);
uint8_t Flash_GD25Qxx_WriteEnable(void);
uint32_t Flash_GD25Qxx_Read_ID(uint8_t *ID, uint8_t id_cmd);
uint8_t Flash_GD25Qxx_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t Size);
uint8_t Flash_GD25Qxx_Write(uint8_t *pData, uint32_t WriteAddr, uint32_t Size);
uint8_t Flash_GD25Qxx_Block_Erase(uint32_t Address);
uint8_t Flash_GD25Qxx_Chip_Erase(void);

#ifdef __cplusplus
}
#endif

#endif /* __GD25Qxx_H */

