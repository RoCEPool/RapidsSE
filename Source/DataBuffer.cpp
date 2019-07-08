/*============================================================================*/
/* DataBuffer.cpp -- Rows Buffer                                              */
/*============================================================================*/

#include "MetaData.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
DataBuffer* MetaData::_pDataBuffer(void* aAddress)
{
  DataBuffer* pDataBuffer = reinterpret_cast<DataBuffer*>(aAddress);
  
  return pDataBuffer;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
DataBuffer::DataBuffer(eCN eClassNo)
: LinkedBuffer(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  m_okLock.vInitLock(LK_CRWBUFFER);
  m_iBufferNo     = *nPoolBufferNo();
  m_iPartitionNo  = C_UNKNOWN;
  m_iBufferHeight = 0;
  m_oWhenNewed.vNow();
  
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
i8 DataBuffer::iAgeSec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 iAge = m_oWhenNewed.iSecondsElapsed();
  
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
/* get the buffer height                                                      */
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
/* when was this buffer newed?                                                */
/*----------------------------------------------------------------------------*/
Birth& DataBuffer::oWhenNewed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_oWhenNewed;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
