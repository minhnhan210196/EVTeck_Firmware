/*******************************************************************************
 *  @FILE NAME:    template.h
 *  @DESCRIPTION:  This file for button process
 *
 *  Copyright (c) 2019 BTM Technologies.
 *  All Rights Reserved This program is the confidential and proprietary
 *  product of BTM Technologies. Any Unauthorized use, reproduction or transfer
 *  of this program is strictly prohibited.
 *
 *  @Author: ManhBT
 *  @NOTE:   No Note at the moment
 *  @BUG:    No known bugs.
 *
 *<pre>
 *  MODIFICATION HISTORY:
 *
 *  Ver   Who       Date                Changes
 *  ----- --------- ------------------  ----------------------------------------
 *  1.00  ManhBT    Jun 12, 2019        First release
 *
 *
 *</pre>
*******************************************************************************/

#ifndef __COM_FIFO_H__
#define __COM_FIFO_H__



/****************************** Include Files *********************************/
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*************************** Constant Definitions *****************************/

/***************************** Type Definitions *******************************/
typedef struct fifo_descriptor {
    /**
     * Start address on memory for item space.
     */
    void * itemspace;
    /**
     * Size in bytes for each element in the buffer.
     */
    size_t itemsize;
    /**
     * Size in bytes of the whole buffer.
     */
    size_t allocatedbytes;
    /**
     * Memory offset from which we will read data.
     */
    uint16_t readoffset;
    /**
     * Memory offset where data will be written.
     */
    uint16_t writeoffset;
    /**
    ¨* Number of bytes used currently by stored items.
     */
    size_t storedbytes;
}fifo_t;

enum fifo_side {
    E_FIFO_FRONT,
    E_FIFO_BACK,
};

#ifdef __cplusplus
extern "C" {
#endif

/****************** Macros (Inline Functions) Definitions *********************/

/*************************** Variable Definitions *****************************/

/******************************************************************************/

#ifndef ARDUINO
#ifdef __cplusplus
extern "C" {
#endif
#endif
/*************************** Function Prototypes ******************************/
/**
 * @brief      Creates a statically allocated FIFO buffer
 *
 * @param      p_fifo  The pointer to the structure holding fifo_t data
 * @param      buf     Pointer to the memory used to store actual fifo items
 * @param[in]  count   The number of items to store on the FIFO
 * @param[in]  size    The size in bytes of each item on the buffer
 *
 * @return     TRUE if success,
 *             FALSE if failed
 */
uint8_t fifo_create(fifo_t* p_fifo, void * buf, uint16_t count, size_t size);

/**
 * @brief      Add item to the FIFO buffer
 *
 * @param      p_fifo  The pointer to the structure holding fifo_t data
 * @param[in]  item    Pointer to  data to be written to the fifo  buffer
 *
 * @return    TRUE if success,
 *            FALSE if failed
 */
uint8_t fifo_add(fifo_t* p_fifo, const void * item);

/**
 * @brief      Get one item from the FIFO buffer.
 *
 * @param      p_fifo  The pointer to the structure holding fifo_t data
 * @param      item    Pointer to a location to hold the data read from the fifo buffer
 *
 * @return     TRUE if there is data available on the fifo buffer to be copied.
 *             FALSE if the buffer is empty and no data can be read
 */
uint8_t fifo_get(fifo_t* p_fifo, void * item);


/**
 * @brief      Check the FIFO is full or not
 *
 * @param      p_fifo  The pointer to the structure holding fifo_t data
 *
 * @return     TRUE if the buffer is full,
 *             FALSE otherwise
 */
uint8_t fifo_is_full(fifo_t* p_fifo);

/**
 * @brief      Check the FIFO is empty.or not
 *
 * @param      p_fifo  The pointer to the structure holding fifo_t data
 *
 * @return     TRUE if the buffer is empty
 *             FALSE otherwise.
 */
uint8_t fifo_is_empty(fifo_t* p_fifo);

/**
 * @brief      Discard data from the buffer.
 *
 * This function discards data from the back or the front side of the buffer,
 * the side and the ammount of discarded data depends on the parameters passed.
 *
 * @param      p_fifo  The pointer to the structure holding fifo_t data
 * @param[in]  count   The number of elements to discard from the buffer
 * @param[in]  side    Defines if data should be discarted from the front or back side
 * of the buffer
 *
 * @return     TRUE if the data was discarted
 *             FALSE if not.
 */
uint8_t fifo_discard(fifo_t* p_fifo, uint16_t count, enum fifo_side side);

uint16_t fifo_get_cnt(fifo_t* p_fifo);
/******************************************************************************/

#ifndef ARDUINO
#ifdef __cplusplus
}
#endif
#endif

#endif /* __COM_FIFO_H__ */
