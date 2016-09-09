/*============================================================================*/
/* RowBuffer.cpp -- Row Buffer                                                */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* RowBuffer::operator new(size_t iSize)
{
  ASSERT(iSize);
  Buffer* pBuffer = gpBufferPool->pAllocBuffer();
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* return a segment to the buffer pool                                        */
/*----------------------------------------------------------------------------*/
void RowBuffer::operator delete(void* pRowBufferAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pRowBufferAddress);
  
  gpBufferPool->vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowBuffer::RowBuffer(void)
: Buffer(CN_ROWBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);

  m_bHasSpace   = true;
  m_pFreeSpace  = pDataArea();
  m_pPrevBuffer = nullptr;
  m_pNextBuffer = nullptr;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);

  Memory::vCheckInitialised(this,sizeof(RowBuffer));
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowBuffer::~RowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* allocate a row in this segmented space                                     */
/*----------------------------------------------------------------------------*/
Row* RowBuffer::pAllocRow(i8 iDataBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  i8 iBytes = Row::iBytesRequired(iDataBytes);

  // was exhausted on last call?
  if (bIsExhausted())
    return nullptr;
  
  // test  sufficient freespace
  if (iFreeSpace() < iBytes) {
    vSetExhausted();
    return nullptr;
  }
  
  Row* pRow = reinterpret_cast<Row*>(m_pFreeSpace);

  m_pFreeSpace = static_cast<Object*>(pvAdd(m_pFreeSpace,iBytes));
  
  new(pRow) Row(iDataBytes);
    
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_,CN_ROWBUFFER);
  
  return pRow;
}

/*----------------------------------------------------------------------------*/
/* dump rowbuffer                                                             */
/*----------------------------------------------------------------------------*/
void RowBuffer::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_ROWBUFFER);
  ASSERT(pObject->eClass() == CN_ROWBUFFER);
  
  RowBuffer* pRowBuffer = reinterpret_cast<RowBuffer*>(pObject);
  
  vTrace("%sRowBuffer %p:\n", pTab, pRowBuffer);
  vTrace("%s  hasSpace %s \n", pTab, pRowBuffer->m_bHasSpace ? "true" : "false");
  vTrace("%s  freeSpace %p\n", pTab, pRowBuffer->m_pFreeSpace);
  vTrace("%s  prevBuffer %p\n", pTab, pRowBuffer->m_pPrevBuffer);
  vTrace("%s  nextBuffer %p\n\n", pTab, pRowBuffer->m_pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/* first row                                                                  */
/*----------------------------------------------------------------------------*/
Row* RowBuffer::pFirstRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  Row* pFirst = reinterpret_cast<Row*>(pDataArea());
  
  if ((char*)pFirst < (char*)m_pFreeSpace)
    return pFirst;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* next row                                                                   */
/*----------------------------------------------------------------------------*/
Row* RowBuffer::pNextRow(Row* pCurr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  i8 iSize = pCurr->iSize();
  
  Row* pNext = static_cast<Row*>(pvAdd(pCurr,iSize));
  
  if ((char*)pNext < (char*)m_pFreeSpace)
    return pNext;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
RowBuffer* RowBuffer::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  return m_pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void RowBuffer::vNextBuffer(RowBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
RowBuffer* RowBuffer::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  return m_pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void RowBuffer::vPrevBuffer(RowBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* calc bytes used                                                            */
/*----------------------------------------------------------------------------*/
i8 RowBuffer::iBytesUsed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  i8 iBytes = iSub(m_pFreeSpace,pDataArea());
  
  return iBytes;
}

/*----------------------------------------------------------------------------*/
/* copy given buffer                                                          */
/*----------------------------------------------------------------------------*/
void RowBuffer::vCopyBuffer(RowBuffer* pFromBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  m_bHasSpace = pFromBuffer->bHasSpace();
  
  i8 iBytes = pFromBuffer->iBytesUsed();
  
  Memory::vCopy(pDataArea(),pFromBuffer->pDataArea(),iBytes);
  
  char* pRevised = pcAdd(pDataArea(),iBytes);
  
  m_pFreeSpace = reinterpret_cast<Object*>(pRevised);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_,CN_ROWBUFFER);
}

/*----------------------------------------------------------------------------*/
/* has space?                                                                 */
/*----------------------------------------------------------------------------*/
bool RowBuffer::bHasSpace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  return m_bHasSpace;
}

/*----------------------------------------------------------------------------*/
/* insert row into this buffer                                                */
/*----------------------------------------------------------------------------*/
bool RowBuffer::bInsertRow(TxRef* pTxRef, StrVal* pRowStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  i8 iDataBytes  = pRowStr->iSize();
  
  Row* pRow = pAllocRow(iDataBytes);
  if (!pRow)
    return false;
  
  i8 iTxId = pTxRef->iTxId();
  
//  vTrace("TxId %lld\n",iTxId);
  
  pRow->vTxId(iTxId);
  
  Memory::vCopy(pRow->pData(),pRowStr,iDataBytes);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_,CN_ROWBUFFER);

  return true;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
Object* RowBuffer::pDataArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  Object* pStart = static_cast<Object*>(pvAdd(this,sizeof(RowBuffer)));
  
  return pStart;
}

/*----------------------------------------------------------------------------*/
/* how much free space (in bytes) left in segment?                            */
/*----------------------------------------------------------------------------*/
i8 RowBuffer::iFreeSpace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  i8 nBytes = iSub(pLastUsableByte(),m_pFreeSpace);

  nBytes -= sizeof(Footer);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* has this buffer exhausted its space?                                       */
/*----------------------------------------------------------------------------*/
bool RowBuffer::bIsExhausted(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  return m_bHasSpace ? false : true;
}

/*----------------------------------------------------------------------------*/
/* this buffer has exhausted its space                                        */
/*----------------------------------------------------------------------------*/
void RowBuffer::vSetExhausted(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  m_bHasSpace = false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/