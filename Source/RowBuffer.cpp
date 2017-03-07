/*============================================================================*/
/* RowBuffer.cpp -- Row Buffer                                                */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RowBuffer* DBobjects::_pRowBuffer(void* aAddr)
{
  RowBuffer* pRowBuffer = reinterpret_cast<RowBuffer*>(aAddr);
  
  RASSERT( !pRowBuffer || CN_ROWBUFFER == pRowBuffer->eClass() );
  
  return pRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* RowBuffer::operator new(size_t iSize)
{
  RASSERT(iSize);
  Buffer* pBuffer = (Buffer*)Heap::pAllocate(CN_ROWBUFFER);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* RowBuffer::operator new(size_t iSize, void* aAddr)
{
  RASSERT(iSize);
  
  return aAddr;
}

/*----------------------------------------------------------------------------*/
/* return a segment to the buffer pool                                        */
/*----------------------------------------------------------------------------*/
void RowBuffer::operator delete(void* pRowBufferAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pRowBufferAddress);
  
  Heap::vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowBuffer::RowBuffer(void)
: Buffer(CN_ROWBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);

  m_eDurability = RD_TEMPORARY;
  m_pFreeSpace  = pDataArea();
  m_nRows       = 0;
  m_pPrevBuffer = nullptr;
  m_pNextBuffer = nullptr;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);

  _checkInitialised_(CN_ROWBUFFER);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowBuffer::RowBuffer(i8 iDataBytes)
: Buffer(CN_ROWBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  m_pFreeSpace  = pAdd<Object*>(pDataArea(),iDataBytes);
  m_nRows       = 0;
  m_pPrevBuffer = nullptr;
  m_pNextBuffer = nullptr;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_ROWBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowBuffer::~RowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* row buffer factory                                                         */
/*----------------------------------------------------------------------------*/
RowBuffer* RowBuffer::pNewRowBuffer(void)
{
  if (gpBufferPool->iFreeCount() <= C_BUFFER_THRESHOLD)
    return nullptr;
  
  RowBuffer* pRowBuffer = reinterpret_cast<RowBuffer*>(
                            Heap::pAllocate(CN_ROWBUFFER));
  if (! pRowBuffer)
    return nullptr;
  
  new(pRowBuffer) RowBuffer;
  
  return pRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* dump rowbuffer                                                             */
/*----------------------------------------------------------------------------*/
void RowBuffer::vDump(eCN eClass, Object* pObject, Str* aTab)
{
  ASSERT(eClass == CN_ROWBUFFER);
  ASSERT(pObject->eClass() == CN_ROWBUFFER);
  
  RowBuffer* pRowBuffer = reinterpret_cast<RowBuffer*>(pObject);
  
  vTrace("rows %lld freeSpace %p prevBuffer %p nextBuffer %p\n"_Str,
         pRowBuffer->m_nRows,
         pRowBuffer->m_pFreeSpace,
         pRowBuffer->m_pPrevBuffer,
         pRowBuffer->m_pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get durability                                                             */
/*----------------------------------------------------------------------------*/
eRD RowBuffer::eDurability(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  return m_eDurability;
}

/*----------------------------------------------------------------------------*/
/* set durability                                                             */
/*----------------------------------------------------------------------------*/
void RowBuffer::vDurability(eRD eDurability)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  m_eDurability = eDurability;
}

/*----------------------------------------------------------------------------*/
/* allocate a row in this segmented space                                     */
/*----------------------------------------------------------------------------*/
Row* RowBuffer::pAllocRow(i8 iDataBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  i8 iBytes = Row::iBytesRequired(iDataBytes);

  // test  sufficient freespace
  if (iFreeSpace() < iBytes) {
    return nullptr;
  }
  
  Row* pRow = reinterpret_cast<Row*>(m_pFreeSpace);

  m_pFreeSpace = pAdd<Object*>(m_pFreeSpace,iBytes);
  
  new(pRow) Row(iDataBytes);
  
  m_nRows++;
    
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_,CN_ROWBUFFER);
  
  return pRow;
}

/*----------------------------------------------------------------------------*/
/* append rows of arg buffer into this buffer                                 */
/*----------------------------------------------------------------------------*/
void RowBuffer::vAppendBuffer(RowBuffer* pArgBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  // copy arg rows into this buffer
  void* pDst = pAdd<void*>(pDataArea(),iBytesUsed());
  void* pSrc = pArgBuffer->pDataArea();
  i8 nBytes  = pArgBuffer->iBytesUsed();
  MemLib::vCopy(pDst,pSrc,nBytes);
  
  m_pFreeSpace = pAdd<Object*>(m_pFreeSpace,nBytes);
  
  // inc the number of rows in this buffer
  i8 nArgRows = pArgBuffer->nRows();
  m_nRows += nArgRows;
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
  
  i8 nBytes = pCurr->nSize();
  
  Row* pNext = pAdd<Row*>(pCurr,nBytes);
  
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
  
  i8 nBytes = nSub(m_pFreeSpace,pDataArea());
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get number of rows in buffer                                               */
/*----------------------------------------------------------------------------*/
i8 RowBuffer::nRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  return m_nRows;
}

/*----------------------------------------------------------------------------*/
/* set row count                                                              */
/*----------------------------------------------------------------------------*/
void RowBuffer::vRowCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  m_nRows = 0;
  for (Row* pRow = pFirstRow(); pRow; pRow = pNextRow(pRow))
    m_nRows++;
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_,CN_ROWBUFFER);
}

/*----------------------------------------------------------------------------*/
/* copy given buffer                                                          */
/*----------------------------------------------------------------------------*/
void RowBuffer::vCopyBuffer(RowBuffer* pFromBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  i8 iBytes = pFromBuffer->iBytesUsed();
  
  MemLib::vCopy(pDataArea(),pFromBuffer->pDataArea(),iBytes);
  
  char* pRevised = pAdd<char*>(pDataArea(),iBytes);
  
  m_pFreeSpace = reinterpret_cast<Object*>(pRevised);

  vRowCount();
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_,CN_ROWBUFFER);
}

/*----------------------------------------------------------------------------*/
/* insert row into this buffer                                                */
/*----------------------------------------------------------------------------*/
bool RowBuffer::bInsertRow(TxRef* pTxRef, char* pRecord, i8 iLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  i8 nSizeBytes = StrVal::nAreaRequired(iLength);
  
  Row* pRow = pAllocRow(nSizeBytes);
  if (!pRow)
    return false;
  
  i8 iTxId = pTxRef->iTxId();
  
  pRow->vTxId(iTxId);
  
  char* pData = pRow->pData();
  
  StrVal* pStr = new((StrVal*)pData) StrVal((StrVal*)pData);
  
  pStr->vAssign(pRecord,iLength);  
  
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
  
  Object* pStart = pAdd<Object*>(this,sizeof(RowBuffer));
  
  return pStart;
}

/*----------------------------------------------------------------------------*/
/* how much free space (in bytes) left in segment?                            */
/*----------------------------------------------------------------------------*/
i8 RowBuffer::iFreeSpace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  i8 nBytes = nSub(m_pLastByte,m_pFreeSpace);

  nBytes -= sizeof(Footer);
  
  return nBytes;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
