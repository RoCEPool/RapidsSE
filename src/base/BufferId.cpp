/*============================================================================*/
/* BufferId.cpp -- BufferId                                                   */
/*============================================================================*/

#include "BASE.h"
#include "DataBuffer.hpp"

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
BufferId::BufferId(void)
{
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
BufferId::BufferId(void *pAddr)
{
  this->m_pvAsAddress = pAddr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
BufferId::BufferId(i8 iVal)
{
  this->m_iAsI8 = iVal;
}

/*----------------------------------------------------------------------------*/
/* Get the address that this BufferId represents                              */
/*----------------------------------------------------------------------------*/
// We will update this to use the HashMap, and page in from disk
// What happens if page out receives a buffer not in memory?
// Potentially page it in, just to page it back out.
void* BufferId::aAddressFromId(BufferId oBufferId)
{
  _static_
#ifdef WITHOUT_BUFFERIDHASHMAP
// TODO: Hack for performance test
  return oBufferId.m_pvAsAddress;
#else
  if(oBufferId.m_iAsI8 == 0)
    return nullptr;

  ASSERT(oBufferId.m_bIdFlag == 1);

  return AM::pHashMap()->pValueAt(oBufferId.m_pvAsAddress);
#endif
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
#ifdef WITHOUT_BUFFERIDHASHMAP
  // TODO: Hack for performance test
  if (oBufferId.m_pvAsAddress == nullptr)
    return false;
  Object* pAddr = (Object*) oBufferId.m_pvAsAddress;
  if(DataBuffer::bIsDataBuffer(pAddr->eGetClassNo()) )
    return true;
  return false;
#else
  return oBufferId.m_bIdFlag == 1;
#endif
}

/*----------------------------------------------------------------------------*/
/* Check if the given address is a Buffer Id by checking the flag bit         */
/*----------------------------------------------------------------------------*/
bool BufferId::bIsBufferId(void* aAddr)
{
  _static_
#ifdef WITHOUT_BUFFERIDHASHMAP
  // TODO: Hack for performance test
  if (aAddr == nullptr)
    return false;
  Object* pAddr = (Object*) aAddr;
  if(DataBuffer::bIsDataBuffer(pAddr->eGetClassNo()) )
    return true;
  return false;
#else
  BufferId oBufferId(aAddr);
  return oBufferId.m_bIdFlag == 1;
#endif
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