/*******************************************************************************
 *  @FILE NAME:    generic_fifo.c
 *  @DESCRIPTION:  generic FIFO structure
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

/***************************** Include Files **********************************/
#include "cli_fifo.h"

#include <stdio.h>

/************************** Constant Definitions ******************************/

/**************************** Type Definitions ********************************/

/***************** Macros (Inline Functions) Definitions **********************/

/********************** Internal Function Prototypes **************************/
static void fifo_copy_from(fifo_t* p_fifo, void *);
static void fifo_copy_to(fifo_t* p_fifo, const void *);

/************************** Variable Definitions ******************************/

/********************* Exported Function Definitions **************************/

uint8_t fifo_create(fifo_t* p_fifo, void * buf, uint16_t count, size_t size)
{
    // Sanity check for memory and element sizes
    if (buf != NULL && p_fifo != NULL && count != 0) {
        p_fifo->itemspace = buf;
        p_fifo->itemsize = size;
        p_fifo->allocatedbytes = count * size;
        p_fifo->readoffset = 0;
        p_fifo->writeoffset = 0;
        p_fifo->storedbytes = 0;
        return 1;
    }
    return 0;
}

uint8_t fifo_add(fifo_t* p_fifo, const void * item)
{
    if (!fifo_is_full(p_fifo)) {
        fifo_copy_to(p_fifo, item);
        p_fifo->storedbytes += p_fifo->itemsize;
        return 1;
    } else {
        return 0;
    }
}

uint8_t fifo_get(fifo_t* p_fifo, void * item)
{
    if (!fifo_is_empty(p_fifo)) {
        fifo_copy_from(p_fifo, item);
        p_fifo->storedbytes -= p_fifo->itemsize;
        return 1;
    } else {
        return 0;
    }
}

uint16_t fifo_get_cnt(fifo_t* p_fifo)
{
    if (p_fifo == NULL) {
        return 0;
    } else {

        if (p_fifo->itemsize == 0) {
           // printf("stored: %d, alloc: %d, cnt: %d\r\n", p_fifo->storedbytes, p_fifo->allocatedbytes, 0);
            return 0;
        } else {
          //  printf("stored: %d, alloc: %d, cnt: %d\r\n", p_fifo->storedbytes, p_fifo->allocatedbytes, (p_fifo->storedbytes / p_fifo->itemsize));
            return (p_fifo->storedbytes / p_fifo->itemsize);
        }
    }

}

uint8_t fifo_is_full(fifo_t* p_fifo)
{
    if (p_fifo->storedbytes >= p_fifo->allocatedbytes)
        return 1;
    else
        return 0;
}

uint8_t fifo_is_empty(fifo_t* p_fifo)
{
    if (p_fifo->storedbytes == 0)
        return 1;
    else
        return 0;
}

uint8_t fifo_discard(fifo_t* p_fifo, uint16_t count, enum fifo_side side)
{
    uint16_t t;
    t = p_fifo->itemsize * count; // Compute byte size of elements to be deleted
    if (t <= p_fifo->storedbytes) // Check if we can remove the requested ammount of data
    {
        if (side == E_FIFO_FRONT) {
            p_fifo->readoffset = (p_fifo->readoffset + t) % p_fifo->allocatedbytes; // Increase read pointer n elements
            p_fifo->storedbytes -= t; // Decrease stored bytes number
        } else if (side == E_FIFO_BACK) {
            p_fifo->writeoffset = (p_fifo->writeoffset - t) % p_fifo->allocatedbytes; // Decrease write pointer n elements
            p_fifo->storedbytes -= t; // Decrease stored bytes number
        }
        return 1;
    }
    return 0;
}

/********************* Internal Function Definitions *************************/
static void fifo_copy_from(fifo_t* p_fifo, void * item)
{
    memcpy(item, (uint8_t*)p_fifo->itemspace + p_fifo->readoffset, p_fifo->itemsize);
    p_fifo->readoffset += p_fifo->itemsize;
    if (p_fifo->readoffset >= p_fifo->allocatedbytes) {
        p_fifo->readoffset = 0;
    }
}

static void fifo_copy_to(fifo_t* p_fifo, const void *item)
{
    memcpy((uint8_t*)p_fifo->itemspace + p_fifo->writeoffset, item, p_fifo->itemsize);
    p_fifo->writeoffset += p_fifo->itemsize;
    if (p_fifo->writeoffset >= p_fifo->allocatedbytes) {
        p_fifo->writeoffset = 0;
    }
}

/*****************************************************************************/
