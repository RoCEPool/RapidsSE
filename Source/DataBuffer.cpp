/*============================================================================*/
/* DataBuffer.cpp -- Rows Buffer                                              */
/*============================================================================*/

#include "DataBuffers.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
DataBuffer* DataBuffers::_pDataBuffer(void* aAddress)
{
  DataBuffer* pDataBuffer = reinterpret_cast<DataBuffer*>(aAddress);
  
  return pDataBuffer;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
DataBuffer::DataBuffer(eCN eClassNo, bool bIsPermanent)
: LinkedBuffer(eClassNo)
, m_okLock(LK_CRWBUFFER,this)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  ASSERT(CacheLine::bMultipleCacheLines(sizeof(DataBuffer)));

  m_iBufferNo     = iBufferNo();
  m_tPtnNo        = C_UNKNOWN;
  m_iBufferHeight = 0;
  m_oWhenNewed.vNow();
  m_bIsPermanent = bIsPermanent;
  
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
  
  i8 iAge = m_oWhenNewed.iElapsedSsec();
  
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
void DataBuffer::vPtnNo(i8 tPtnNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_tPtnNo = tPtnNo;
}

/*----------------------------------------------------------------------------*/
/* get partition no                                                           */
/*----------------------------------------------------------------------------*/
i8 DataBuffer::tPtnNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_tPtnNo;
}

/*----------------------------------------------------------------------------*/
/* when was this buffer newed?                                                */
/*----------------------------------------------------------------------------*/
Birth& DataBuffer::oWhenNewed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_oWhenNewed;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the buffer no of the pool buffer that the object is within             */
/*----------------------------------------------------------------------------*/
i8 DataBuffer::iBufferNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  PoolBuffer* pPoolBuffer = PoolBuffer::pWithinPoolBuffer(this);
  
  i8 i_Buffer = pPoolBuffer->iBufferNo();

  return i_Buffer;
}

/*----------------------------------------------------------------------------*/
/* is this buffer permanent ?                                                 */
/*----------------------------------------------------------------------------*/
bool DataBuffer::bIsPermanent(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_bIsPermanent;
}

/*----------------------------------------------------------------------------*/
/* mark buffer permanent/temporary                                            */
/*----------------------------------------------------------------------------*/
void DataBuffer::vIsPermanent(bool bIsPermanent)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_bIsPermanent = bIsPermanent;
}

/*----------------------------------------------------------------------------*/
/* get the entry buffer that given id references                              */
/*----------------------------------------------------------------------------*/
DataBuffer* DataBuffer::pRefdDataBuffer(Rrf& xRef)
{
  i8 i_Buffer = xRef.iBuffer();
  
  HeapBuffer* pHeapBuffer =
  reinterpret_cast<HeapBuffer*>(gpBufferPool->pToPoolBuffer(i_Buffer));
  
  DataBuffer* pDataBuffer =
  reinterpret_cast<DataBuffer*>(pHeapBuffer->aFirstChunk());
  
  ASSERT(pDataBuffer);
  ASSERT(pDataBuffer->eClass() == CN_CRWBUFFER || pDataBuffer->eClass() == CN_CRVBUFFER);
  
  return pDataBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
