/*============================================================================*/
/* BufferId.cpp -- BufferId                                                   */
/*============================================================================*/

#include "BASE.h"

// We will update this to use the HashMap, and page in from disk
// What happens if page out receives a buffer not in memory?
// Potentially page it in, just to page it back out.
void* BufferId::vAddressFromId(BufferId oBufferId)
{
  _static_

  ASSERT(oBufferId.m_bIdFlag == 1);

  oBufferId.m_bIdFlag = 0;

  // Convert BufferId to BufferNo to BufferAddress
  PoolBuffer* pPoolBuffer =
      FileMap::pToPoolBuffer(oBufferId.m_iAsI8);

  HeapBuffer* pHeapBuffer =
      reinterpret_cast<HeapBuffer*>(pPoolBuffer);

  DataBuffer* pDataBuffer =
      reinterpret_cast<DataBuffer*>(pHeapBuffer->aFirstChunk());

  return (void*) pDataBuffer;

}

bool BufferId::bIsAddress(BufferId oBufferId)
{
  _static_

  return oBufferId.m_bIdFlag == 0;

}
//TODO Arjun: Check overflow before setting below members
void BufferId::vSetBuffSubscript(u8 uBuffSubscript)
{
  m_uBuffSubscript = uBuffSubscript;
}
void BufferId::vSetPageFile(u8 uPageFile)
{
  m_uPageFile = uPageFile;
}
void BufferId::vSetIdFlag(bool bFlag)
{
  m_bIdFlag = bFlag;
}