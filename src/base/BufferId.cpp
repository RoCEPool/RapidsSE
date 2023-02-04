/*============================================================================*/
/* BufferId.cpp -- BufferId                                                   */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/* Get the address that this BufferId represents                              */
/*----------------------------------------------------------------------------*/
// We will update this to use the HashMap, and page in from disk
// What happens if page out receives a buffer not in memory?
// Potentially page it in, just to page it back out.
void* BufferId::vAddressFromId(BufferId oBufferId)
{
  _static_

  ASSERT(oBufferId.m_bIdFlag == 1);
/*
  oBufferId.m_bIdFlag = 0;

  // Convert BufferId to BufferNo to BufferAddress
  PoolBuffer* pPoolBuffer =
      FileMap::pToPoolBuffer(oBufferId.m_iAsI8);

  HeapBuffer* pHeapBuffer =
      reinterpret_cast<HeapBuffer*>(pPoolBuffer);

  DataBuffer* pDataBuffer =
      reinterpret_cast<DataBuffer*>(pHeapBuffer->aFirstChunk());

  return (void*) pDataBuffer;
*/
  return AM::pHashMap()->pValueAt(oBufferId.m_pvAsAddress);
}

/*----------------------------------------------------------------------------*/
/* Calculate the DataBuffer BufferId from the PoolBufferNo                    */
/*----------------------------------------------------------------------------*/
BufferId BufferId::vBufferIdFromNo(i8 iPoolBufferNo)
{
  _static_

  BufferId oBufferId;

  oBufferId.m_iAsI8 =  iPoolBufferNo;

  oBufferId.m_bIdFlag = 1;

  return oBufferId;

}

/*----------------------------------------------------------------------------*/
/* Check if the given BufferId is an address or an Id by checking the flag bit*/
/*----------------------------------------------------------------------------*/
bool BufferId::bIsAddress(BufferId oBufferId)
{
  _static_

  return oBufferId.m_bIdFlag == 0;

}

/*----------------------------------------------------------------------------*/
/* Check if the given BufferId is an address or an Id by checking the flag bit*/
/*----------------------------------------------------------------------------*/
bool BufferId::bIsBufferId(BufferId oBufferId)
{
  _static_

  return oBufferId.m_bIdFlag == 1;

}

/*----------------------------------------------------------------------------*/
/* Set the Buffer Subscript                                                   */
/*----------------------------------------------------------------------------*/
//TODO Arjun: Check overflow before setting below members
void BufferId::vSetBuffSubscript(u8 uBuffSubscript)
{
  m_uSeekOffset = uBuffSubscript;
}

/*----------------------------------------------------------------------------*/
/* Set the Page File Number                                                   */
/*----------------------------------------------------------------------------*/
void BufferId::vSetPageFile(u8 uPageFile)
{
  m_uPageFile = uPageFile;
}

/*----------------------------------------------------------------------------*/
/* Set the Id flag bit                                                        */
/*----------------------------------------------------------------------------*/
void BufferId::vSetIdFlag(bool bFlag)
{
  m_bIdFlag = bFlag;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/