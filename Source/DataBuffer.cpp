/*============================================================================*/
/* DataBuffer.cpp -- Data Buffer                                              */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
DataBuffer* Data::_pDataBuffer(void* aAddress)
{
  DataBuffer* pDataBuffer = reinterpret_cast<DataBuffer*>(aAddress);
  
  return pDataBuffer;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
DataBuffer::DataBuffer(eCN eClassNo)
: Object(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  m_okLock.vInitLock(LK_ROWBUFFER);
  m_iBufferNo     = *nPoolBufferNo();
  m_iPartitionNo  = C_UNKNOWN;
  m_iBufferHeight = 0;
  m_oWhenNewed.vNow();

  m_pNextBuffer   = nullptr;
  m_pPrevBuffer   = nullptr;
  m_pHigherBuffer = nullptr;
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);  
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
DataBuffer::~DataBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get age of buffer in seconds                                               */
/*----------------------------------------------------------------------------*/
i8 DataBuffer::iAgeSsec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Since oSince;
  oSince.vNow();
  
  Ivl wDiff;
  
  wDiff = oSince.wSubtract(m_oWhenNewed);
  
  i8 iAge = wDiff.iSecond();
  
  return iAge;
}

/*----------------------------------------------------------------------------*/
/* set buffer level                                                           */
/*----------------------------------------------------------------------------*/
void DataBuffer::vBufferHeight(i8 iBufferHeight)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iBufferHeight = iBufferHeight;
}

/*----------------------------------------------------------------------------*/
/* get the buffer height                                                       */
/*----------------------------------------------------------------------------*/
i8 DataBuffer::iBufferHeight(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iBufferHeight;
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void DataBuffer::vDropWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void DataBuffer::vGrabWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* get higher buffer                                                          */
/*----------------------------------------------------------------------------*/
DataBuffer* DataBuffer::pHigherBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return _pDataBuffer(m_pHigherBuffer);
}

/*----------------------------------------------------------------------------*/
/* set higher buffer                                                          */
/*----------------------------------------------------------------------------*/
void DataBuffer::vHigherBuffer(DataBuffer* pHigherBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pHigherBuffer = pHigherBuffer;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
DataBuffer* DataBuffer::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return _pDataBuffer(m_pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void DataBuffer::vNextBuffer(DataBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* set partition no                                                           */
/*----------------------------------------------------------------------------*/
void DataBuffer::vPartitionNo(i8 iPartitionNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iPartitionNo = iPartitionNo;
}

/*----------------------------------------------------------------------------*/
/* get partition no                                                           */
/*----------------------------------------------------------------------------*/
i8 DataBuffer::iPartitionNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iPartitionNo;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
DataBuffer* DataBuffer::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return _pDataBuffer(m_pPrevBuffer);
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void DataBuffer::vPrevBuffer(DataBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* when was this buffer newed?                                                */
/*----------------------------------------------------------------------------*/
Since& DataBuffer::oWhenNewed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_oWhenNewed;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
