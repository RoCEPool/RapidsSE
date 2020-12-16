/*============================================================================*/
/* SharedMemory.h -- Shared Memory                                            */
/*============================================================================*/
#pragma once

#include "Alignment.h"

//------------------------------------------------------------------------------

namespace SharedMemory
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "ShmLib.hpp"

/*============================================================================*/
/* Pool.h -- Buffer Pool                                                      */
/*============================================================================*/



static const u8 C_SIZE_POOL_BUFFER   = C_1MB;

static const i8 C_SIZE_TINY_OBJECT   = C_1KB;
static const i8 C_SIZE_SMALL_OBJECT  = C_4KB;
static const i8 C_SIZE_MEDIUM_OBJECT = C_64KB;
static const i8 C_SIZE_LARGE_OBJECT  = C_SIZE_POOL_BUFFER - C_SIZE_SMALL_OBJECT;

static const u8 C_MASK_TINY_OBJECT   = 0x3FF;
static const u8 C_MASK_SMALL_OBJECT  = 0xFFF;
static const u8 C_MASK_MEDIUM_OBJECT = 0xFFFF;
static const u8 C_MASK_LARGE_OBJECT  = 0xFFFFF;

static const i8 C_LOW_BUFFERS        = 512;

static const u8 C_OBJECT_TO_BUFFER_MASK = BITWISE_NOT((C_SIZE_POOL_BUFFER - 1));

static const i8 C_VIA_MALLOC         = 1;

#include "PoolBuffer.hpp"
#include "FreeBuffer.hpp"
#include "FreeChunk.hpp"
#include "BufferPool.hpp"

#include "HeapBuffer.hpp"
#include "HeapMem.hpp"
#include "Heap.hpp"
#include "ObjectsInPool.hpp"
#include "Region.hpp"
#include "Slab.hpp"
#include "SlabAllocator.hpp"


extern BufferPool* gpBufferPool;

/*---------------------------------------------------------------------------*/
/* pCheck: check address                                                     */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
static OBJECT pPoolCheck(void* aAddress)
{
#ifdef WITH_ADDRESS_CHECKING
  if (aAddress)
    gpBufferPool->vCheckAddress(aAddress);
#endif
  
  return static_cast<OBJECT>(aAddress);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
