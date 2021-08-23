/*============================================================================*/
/* DataBuffer.cpp -- Rows Buffer                                              */
/*============================================================================*/

#include "BASE.h"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "RowOnce.hpp"
#include "IndexStream.hpp"
#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
DataBuffer* BASE::_pDataBuffer(void* aAddress)
{
  DataBuffer* pDataBuffer = reinterpret_cast<DataBuffer*>(aAddress);
  
  return pDataBuffer;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
DataBuffer::DataBuffer(eCN eClassNo, bool bIsPermanent)
: LinkedBuffer(eClassNo)
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  //LOGGABLE ASSERT(CacheLine::bMultipleCacheLines(sizeof(DataBuffer)));

  m_iBufferNo  = iBufferNo();
  m_iBucketNo  = C_UNKNOWN;
  m_tPtnNo     = C_UNKNOWN;
  
  m_iBufferHeight = 0;
  m_oWhenNewed.vNow();
  m_bIsPermanent = bIsPermanent;
  
  m_pTable = nullptr;
  m_pStreamDef = nullptr;

  m_tTxnNo = C_UNKNOWN;
    
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
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the entry buffer that given id references                              */
/*----------------------------------------------------------------------------*/
DataBuffer* DataBuffer::pRefdDataBuffer(Rrf& xRef)
{
  _static_
  
  i8 i_Buffer = xRef.iBuffer();
  
  HeapBuffer* pHeapBuffer =
  reinterpret_cast<HeapBuffer*>(gpBufferPool->pToPoolBuffer(i_Buffer));
  
  DataBuffer* pDataBuffer =
  reinterpret_cast<DataBuffer*>(pHeapBuffer->aFirstChunk());
  
  ASSERT(pDataBuffer);
  ASSERT(   pDataBuffer->eGetClassNo() == CN_CRVBUFFER
         || pDataBuffer->eGetClassNo() == CN_CRWBUFFER
         || pDataBuffer->eGetClassNo() == CN_CSVBUFFER
         || pDataBuffer->eGetClassNo() == CN_ENTBUFFER
         || pDataBuffer->eGetClassNo() == CN_ROWBUFFER
         || pDataBuffer->eGetClassNo() == CN_EACHROWBUFFER
         );
  
  return pDataBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void DataBuffer::vRepairList(Object* pObject, eRF eRepair)
{
  _static_
  DataBuffer* pDataBuffer = _pDataBuffer(pObject);
  DataBuffer* pNextDataBuffer;

  while (pDataBuffer) {
    pNextDataBuffer = _pDataBuffer(pDataBuffer->pNextLinkedBuffer());
    AM::vDeleteOperand(pDataBuffer);
    pDataBuffer = pNextDataBuffer;
  }
}

/*----------------------------------------------------------------------------*/
/* get the data buffer that contains the given address                        */
/*----------------------------------------------------------------------------*/
DataBuffer* DataBuffer::pToDataBuffer(void* aAddr)
{
  _static_
  
  PoolBuffer* pPoolBuffer =
    PoolBuffer::pWithinPoolBuffer((Object*)aAddr);
  
  HeapBuffer* pHeapBuffer =
    reinterpret_cast<HeapBuffer*>(pPoolBuffer);

  DataBuffer* pDataBuffer =
    reinterpret_cast<DataBuffer*>(pHeapBuffer->aFirstChunk());
    
  return pDataBuffer;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the stream def                                                         */
/*----------------------------------------------------------------------------*/
StreamDef* DataBuffer::pStreamDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_pStreamDef;
}

/*----------------------------------------------------------------------------*/
/* set the table                                                              */
/*----------------------------------------------------------------------------*/
void DataBuffer::vStreamDef(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_pStreamDef = pStreamDef;
}

/*----------------------------------------------------------------------------*/
/* get the table                                                              */
/*----------------------------------------------------------------------------*/
Table* DataBuffer::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set the table                                                              */
/*----------------------------------------------------------------------------*/
void DataBuffer::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pTable = pTable;
  vStreamDef(pTable->pStreamDef());
}

/*----------------------------------------------------------------------------*/
/* get transaction number                                                     */
/*----------------------------------------------------------------------------*/
tXN DataBuffer::tTxnNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_tTxnNo;
}

/*----------------------------------------------------------------------------*/
/* set transaction number                                                     */
/*----------------------------------------------------------------------------*/
void DataBuffer::vTxnNo(tXN tTxnNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_tTxnNo = tTxnNo;
}

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
/* get bucket no                                                              */
/*----------------------------------------------------------------------------*/
i8 DataBuffer::iBucketNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iBucketNo;
}

/*----------------------------------------------------------------------------*/
/* set bucket no                                                              */
/*----------------------------------------------------------------------------*/
void DataBuffer::vBucketNo(i8 iBucketNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iBucketNo = iBucketNo;
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
