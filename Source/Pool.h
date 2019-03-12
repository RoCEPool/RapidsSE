/*============================================================================*/
/* Pool.h -- Buffer Pool                                                      */
/*============================================================================*/
#pragma once

#include "System.h"

//-----------------------------------------------------------------------------
namespace Pool
{
  void vCheckAssumptions(void);
}

using namespace Pool;

//-----------------------------------------------------------------------------

static const u8 C_SIZE_POOL_BUFFER      = C_1MB;
static const u8 C_OBJECT_TO_BUFFER_MASK = BITWISE_NOT((C_SIZE_POOL_BUFFER - 1));
static const u8 C_JUMBO_COEFFICIENT     = 2;
static const u8 C_JUMBO_RATIO           = 16;

static const i8 C_SIZE_HEAP_HEADER      = C_4KB;
static const i8 C_SIZE_SMALL_OBJECT     = C_4KB;
static const i8 C_SIZE_MEDIUM_OBJECT    = C_64KB;
static const i8 C_SIZE_LARGE_OBJECT     = C_SIZE_POOL_BUFFER - C_SIZE_HEAP_HEADER;
static const i8 C_SIZE_JUMBO_OBJECT     = C_SIZE_POOL_BUFFER * C_JUMBO_RATIO;

static const i8 C_LOW_BUFFERS           = 512;

#include "bn.hpp"
#include "PoolBuffer.hpp"
#include "FreeBuffer.hpp"
#include "BufferPool.hpp"
#include "BPA.hpp"

#include "FreeChunk.hpp"
#include "HeapBuffer.hpp"
#include "HeapMem.hpp"
#include "Heap.hpp"
#include "Slab.hpp"
#include "SlabAllocator.hpp"

#include "Region.hpp"

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
