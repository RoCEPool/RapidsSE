/*============================================================================*/
/* Pool.h - includes for Memory Management                        */
/*============================================================================*/

#pragma once

#include "Values.h"

static const i8 C_SIZE_LARGE_BUFFER  = C_1MB;
static const i8 C_SIZE_MEDIUM_OBJECT = C_1KB * 16;
static const i8 C_SIZE_SMALL_OBJECT  = C_1KB;

#include "bn.hpp"
#include "Buffer.hpp"
#include "BufferFree.hpp"
#include "BufferPool.hpp"
#include "BufferPoolIter.hpp"
#include "BufferPoolAnalyzer.hpp"

#include "FreeChunk.hpp"
#include "HeapBuffer.hpp"
#include "Heap.hpp"

//-----------------------------------------------------------------------------
namespace Pool
{
  void vCheckAssumptions(void);
}

using namespace Pool;

extern BufferPool* gpBufferPool;

/*---------------------------------------------------------------------------*/
/* pCheck: check address                                                     */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
static OBJECT pPoolCheck(void* pAddress)
{
#ifdef ADDRESS_CHECK
  if (pAddress)
    gpBufferPool->vCheckAddress(pAddress);
#endif
  
  return static_cast<OBJECT>(pAddress);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/