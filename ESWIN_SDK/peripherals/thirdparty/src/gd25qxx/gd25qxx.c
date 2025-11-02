#include "gd25qxx.h"

/**
 *
 * Function Name : Flash_GD25Qxx_Init
 * Description   : Initializes the GD25Q128E interface.
 *
 */
uint8_t Flash_GD25Qxx_Init(void)
{
    /* Reset GD25Qxxx */
    Flash_GD25Qxx_Reset();

    return Flash_GD25Qxx_GetStatus();
}

/**
 *
 * Function Name : Flash_GD25Qxx_Reset
 * Description   : This function reset the GD25Qxx.
 *
 */
void Flash_GD25Qxx_Reset(void)
{
    uint8_t cmd[2] = {RESET_ENABLE_CMD, RESET_MEMORY_CMD};

    SPI_DRV_MasterTransferBlocking(2, cmd, NULL, sizeof(cmd), 1000);
}

/**
 *
 * Function Name : Flash_GD25Qxx_GetStatus
 * Description   : Reads current status of the GD25Q128E.
 *
 */
uint8_t Flash_GD25Qxx_GetStatus(void)
{
    uint8_t cmd[2] = {READ_STATUS_REG1_CMD, 0};
    uint8_t statusData[2] = {0};

    SPI_DRV_MasterTransferBlocking(2, cmd, statusData, sizeof(cmd), 1000);

    /* Check the value of the register */
    if (((statusData[1] & GD25Q128E_FSR_BUSY) != 0))
    {
        return GD25Qxx_BUSY;
    }
    else
    {
        return GD25Qxx_OK;
    }
}

/**
 *
 * Function Name : Flash_GD25Qxx_Read_ID
 * Description   : Read Manufacture/Device ID.
 *
 */
uint32_t Flash_GD25Qxx_Read_ID(uint8_t *ID, uint8_t id_cmd)
{
    uint8_t cmd[] = {id_cmd, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t id_data[] = {0, 0, 0, 0, 0, 0};
    SPI_DRV_MasterTransferBlocking(2, cmd, id_data, sizeof(cmd), 1000);
    memcpy(ID, id_data, sizeof(id_data));
    return GD25Qxx_OK;
}

/**
 * @brief  Reads an amount of data from the SPI memory.
 * @param  pData: Pointer to data to be read
 * @param  ReadAddr: Read start address
 * @param  Size: Size of data to read
 * @retval SPI memory status
 */
/**
 *
 * Function Name : Flash_GD25Qxx_Read_ID
 * Description   : Reads an amount of data from the SPI memory.
 *
 */
uint8_t Flash_GD25Qxx_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
    uint8_t cmd[Size + 4];
    uint8_t data[Size + 4];

    /* Configure the command */
    cmd[0] = READ_CMD;
    cmd[1] = (uint8_t)(ReadAddr >> 16);
    cmd[2] = (uint8_t)(ReadAddr >> 8);
    cmd[3] = (uint8_t)(ReadAddr);
    for (int i = 0; i < Size; i ++)
    {
    	cmd[i + 4] = 0;
    }

    SPI_DRV_MasterTransferBlocking(2, cmd, data, sizeof(cmd), 1000);

    for (int i = 0; i < Size; i ++)
    {
    	pData[i] = data[i + 4];
    }
    return GD25Qxx_OK;
}

/**
 * @brief  This function send a Write Enable and wait it is effective.
 * @retval None
 */
uint8_t Flash_GD25Qxx_WriteEnable(void)
{
    uint8_t cmd[1] = {WRITE_ENABLE_CMD};

    SPI_DRV_MasterTransferBlocking(2, cmd, NULL, sizeof(cmd), 1000);
    /* Wait the end of Flash writing */
    while (Flash_GD25Qxx_GetStatus() == GD25Qxx_BUSY)
        ;

    return GD25Qxx_OK;
}

/**
 * @brief  Writes an amount of data to the SPI memory.
 * @param  pData: Pointer to data to be written
 * @param  WriteAddr: Write start address
 * @param  Size: Size of data to write,No more than 256byte.
 * @retval SPI memory status
 */
uint8_t Flash_GD25Qxx_Write(uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
    uint8_t cmd[Size + 4];
    uint32_t end_addr, current_size, current_addr;
//    uint32_t tickstart = HAL_GetTick();

    /* Calculation of the size between the write address and the end of the page */
    current_addr = 0;

    while (current_addr <= WriteAddr) // 判断地址属于哪一扇区开始
    {
        current_addr += GD25Q128E_PAGE_SIZE; // 0x100-> 256 bytes
    }
    current_size = current_addr - WriteAddr;

    /* Check if the size of the data is less than the remaining place in the page */
    if (current_size > Size)
    {
        current_size = Size;
    }

    /* Initialize the adress variables */ // 写入地址大小范围
    current_addr = WriteAddr;
    end_addr = WriteAddr + Size;

    /* Perform the write page by page */
    do
    {
        /* Configure the command */
        cmd[0] = PAGE_PROG_CMD;
        cmd[1] = (uint8_t)(current_addr >> 16);
        cmd[2] = (uint8_t)(current_addr >> 8);
        cmd[3] = (uint8_t)(current_addr);

        for (int i = 0; i < Size; i ++)
        {
        	cmd[i + 4] = pData[i];
        }

        /* Enable write operations */
        Flash_GD25Qxx_WriteEnable();

        /* Send the command */
        if (SPI_DRV_MasterTransferBlocking(2, cmd, NULL, sizeof(cmd), 1000)  != STATUS_SUCCESS)
        {
            return GD25Qxx_ERROR;
        }

        /* Wait the end of Flash writing */
        while (Flash_GD25Qxx_GetStatus() == GD25Qxx_BUSY)
            ;

        /* Update the address and size variables for next page programming */
        current_addr += current_size;
        pData += current_size;
        current_size = ((current_addr + GD25Q128E_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : GD25Q128E_PAGE_SIZE;
    } while (current_addr < end_addr);

    return GD25Qxx_OK;
}

/**
 * @brief  Erases the specified block of the SPI memory.
 * @param  BlockAddress: Block address to erase
 * @retval SPI memory status
 */
uint8_t Flash_GD25Qxx_Block_Erase(uint32_t Address)
{
    uint8_t cmd[4];
    cmd[0] = SECTOR_ERASE_CMD;
    cmd[1] = (uint8_t)(Address >> 16);
    cmd[2] = (uint8_t)(Address >> 8);
    cmd[3] = (uint8_t)(Address);

    /* Enable write operations */
    Flash_GD25Qxx_WriteEnable();

    SPI_DRV_MasterTransferBlocking(2, cmd, NULL, sizeof(cmd), 1000);

    /* Wait the end of Flash writing */
    while (Flash_GD25Qxx_GetStatus() == GD25Qxx_BUSY)
        ;

    return GD25Qxx_OK;
}

/**
 * @brief  Erases the entire SPI memory.This function will take a very long time.
 * @retval SPI memory status
 */
uint8_t Flash_GD25Qxx_Chip_Erase(void)
{
    uint8_t cmd[1] = {CHIP_ERASE_CMD};

    Flash_GD25Qxx_WriteEnable();

    SPI_DRV_MasterTransferBlocking(2, cmd, NULL, sizeof(cmd), 1000);

    while (Flash_GD25Qxx_GetStatus() == GD25Qxx_BUSY)
    	;
    return GD25Qxx_OK;
}
