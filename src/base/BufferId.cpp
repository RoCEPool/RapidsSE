/*============================================================================*/
/* BufferId.cpp -- BufferId                                                   */
/*============================================================================*/

#include "BASE.h"

// We will update this to use the HashMap, and page in from disk
void* BufferId::vAddressFromId(BufferId oBufferId)
{
  _static_

  ASSERT(oBufferId.m_bIdFlag == 1);

  oBufferId.m_bIdFlag = 0;

  // Convert BufferId to BufferNo to BufferAddress
  PoolBuffer* pPoolBuffer =
      gpBufferPool->pToPoolBuffer(oBufferId.m_iAsI8);

  HeapBuffer* pHeapBuffer =
      reinterpret_cast<HeapBuffer*>(pPoolBuffer);

  DataBuffer* pDataBuffer =
      reinterpret_cast<DataBuffer*>(pHeapBuffer->aFirstChunk());

  return (void*) pDataBuffer;

}

BufferId BufferId::vBufferIdFromNo(i8 iBufferNo)
{
  _static_

  BufferId oBufferId;

  oBufferId.m_iAsI8 =  iBufferNo;

  oBufferId.m_bIdFlag = 1;

  return oBufferId;

}