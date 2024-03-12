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
: Operand(eClassNo)
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  //LOGGABLE ASSERT(CacheLine::bMultipleCacheLines(sizeof(DataBuffer)));

  m_iBufferNo  = iBufferNo();

#ifdef WITHOUT_BUFFERIDHASHMAP
  // TODO: Hack for performance test
  m_oBufferId.m_pvAsAddress = this;
#else
  m_oBufferId = AM::pFileMap()->oGetNewBufferId();
#endif
  AM::pHashMap()->vInsertKeyValue((void *) m_oBufferId.m_pvAsAddress, (Object *) this);

  m_tLglPtn     = C_UNKNOWN;
  
  m_iBufferHeight = 0;
  m_oWhenNewed.vNow();
  m_bIsPermanent = bIsPermanent;
  
  m_pTable_dbuff = nullptr;
  m_pStreamDef_dbuff = nullptr;

  m_tTxnNo = C_UNKNOWN;

  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);  
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
DataBuffer::~DataBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
#ifdef WITHOUT_BUFFERIDHASHMAP
// TODO: Hack for performance test
  AM::pHashMap()->vDeleteKey((void*)m_oBufferId.m_pvAsAddress);
#else
  if(m_oBufferId.m_iAsI8 != 0)
    AM::pHashMap()->vDeleteKey((void*)m_oBufferId.m_pvAsAddress);
#endif

}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Check if class inherits from DataBuffer                                    */
/*----------------------------------------------------------------------------*/
bool DataBuffer::bIsDataBuffer(eCN eClass)
{
  _static_

  if (   eClass == CN_CSVBUFFER
      || eClass == CN_CRVBUFFER
      || eClass == CN_OBJBUFFER
      || eClass == CN_CRWBUFFER
      || eClass == CN_ENTBUFFER
      || eClass == CN_PDS ) {
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* get the entry buffer that given id references                              */
/*----------------------------------------------------------------------------*/
DataBuffer* DataBuffer::pRefdDataBuffer(Rrf& xRef)
{
  _static_
  
  i8 i_PoolBuffer = xRef.iBuffer();
  
  HeapBuffer* pHeapBuffer =
  reinterpret_cast<HeapBuffer*>(FileMap::pToPoolBuffer(i_PoolBuffer));
  
  DataBuffer* pDataBuffer =
  reinterpret_cast<DataBuffer*>(pHeapBuffer->aFirstChunk());
  
  ASSERT(pDataBuffer);
  ASSERT(   pDataBuffer->eGetClassNo() == CN_CRVBUFFER
         || pDataBuffer->eGetClassNo() == CN_CRWBUFFER
         || pDataBuffer->eGetClassNo() == CN_CSVBUFFER
         || pDataBuffer->eGetClassNo() == CN_ENTBUFFER
         || pDataBuffer->eGetClassNo() == CN_PDS
         );
  
  return pDataBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void DataBuffer::vRepair(Object* pObject)
{
  _static_
  DataBuffer* pDataBuffer = _pDataBuffer(pObject);
  AM::vDeleteOperand(pDataBuffer,nullptr);
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
StreamDef* DataBuffer::pStreamDef_dbuff(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_pStreamDef_dbuff;
}

/*----------------------------------------------------------------------------*/
/* set the table                                                              */
/*----------------------------------------------------------------------------*/
void DataBuffer::vStreamDef_dbuff(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_pStreamDef_dbuff = pStreamDef;
}

/*----------------------------------------------------------------------------*/
/* get the table                                                              */
/*----------------------------------------------------------------------------*/
Table* DataBuffer::pTable_dbuff(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_pTable_dbuff;
}

/*----------------------------------------------------------------------------*/
/* set the table                                                              */
/*----------------------------------------------------------------------------*/
void DataBuffer::vTable_dbuff(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  //vTrace("DataBuffer::vTable\n"_Stz);
  m_pTable_dbuff = pTable;
  vStreamDef_dbuff(pTable == nullptr ? nullptr : pTable->pStreamDef());
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
/* drop lock for read                                                         */
/*----------------------------------------------------------------------------*/
void DataBuffer::vDropRead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_okLock.vDropRead();
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
/* grab lock for read                                                         */
/*----------------------------------------------------------------------------*/
void DataBuffer::vGrabRead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_okLock.vGrabRead();
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
/* set logical partition                                                      */
/*----------------------------------------------------------------------------*/
void DataBuffer::vLglPtn(LglPtn tLglPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  m_tLglPtn = tLglPtn;
}

/*----------------------------------------------------------------------------*/
/* get logical partition                                                      */
/*----------------------------------------------------------------------------*/
LglPtn DataBuffer::tLglPtn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  return m_tLglPtn;
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
  
  i8 i_PoolBuffer = pPoolBuffer->iBufferNo();

  return i_PoolBuffer;
}

/*----------------------------------------------------------------------------*/
/* is this buffer locked ?                                                    */
/*----------------------------------------------------------------------------*/
bool DataBuffer::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_okLock.bIsLocked();
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
/* set the buffer no                                                          */
/*----------------------------------------------------------------------------*/
void DataBuffer::vSetBufferNo(i8 iBufferNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_iBufferNo = iBufferNo;
}

/*----------------------------------------------------------------------------*/
/* get the buffer id of this row buffer                                       */
/*----------------------------------------------------------------------------*/
BufferId DataBuffer::oGetBufferId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_oBufferId;
}

/*----------------------------------------------------------------------------*/
/* set the buffer id                                                          */
/*----------------------------------------------------------------------------*/
void DataBuffer::vSetBufferId(void* pIdAsAddr)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_oBufferId.m_pvAsAddress = pIdAsAddr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
