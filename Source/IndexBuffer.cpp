/*============================================================================*/
/* IndexBuffer.cpp -- Index Buffer                                            */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
IndexBuffer* DBobjects::_pIndexBuffer(void* pAddr)
{
  IndexBuffer* pIndexBuffer = reinterpret_cast<IndexBuffer*>(pAddr);
  
  RASSERT( !pIndexBuffer || CN_INDEXBUFFER == pIndexBuffer->eClass() );
  
  return pIndexBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* IndexBuffer::operator new(size_t iSize)
{
  RASSERT(iSize);
  Buffer* pBuffer = (Buffer*)Heap::pAllocate(CN_INDEXBUFFER);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* IndexBuffer::operator new(size_t iSize, void* pAddr)
{
  RASSERT(iSize);
  
  return pAddr;
}

/*----------------------------------------------------------------------------*/
/* return a segment to the buffer pool                                        */
/*----------------------------------------------------------------------------*/
void IndexBuffer::operator delete(void* pIndexBufferAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pIndexBufferAddress);
  
  Heap::vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IndexBuffer::IndexBuffer(void)
: Buffer(CN_INDEXBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);

  m_pFreeSpace  = pDataArea();
  m_nRows       = 0;
  m_pPrevBuffer = nullptr;
  m_pNextBuffer = nullptr;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);

  _checkInitialised_(CN_INDEXBUFFER);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IndexBuffer::IndexBuffer(i8 iDataBytes)
: Buffer(CN_INDEXBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  m_pFreeSpace  = static_cast<Object*>(pvAdd(pDataArea(),iDataBytes));
  m_nRows       = 0;
  m_pPrevBuffer = nullptr;
  m_pNextBuffer = nullptr;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_INDEXBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IndexBuffer::~IndexBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* row buffer factory                                                         */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexBuffer::pNewIndexBuffer(void)
{
  if (gpBufferPool->iFreeCount() <= C_BUFFER_THRESHOLD)
    return nullptr;
  
  IndexBuffer* pIndexBuffer = reinterpret_cast<IndexBuffer*>(
                            Heap::pAllocate(CN_INDEXBUFFER));
  if (! pIndexBuffer)
    return nullptr;
  
  new(pIndexBuffer) IndexBuffer;
  
  return pIndexBuffer;
}

/*----------------------------------------------------------------------------*/
/* dump rowbuffer                                                             */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_INDEXBUFFER);
  ASSERT(pObject->eClass() == CN_INDEXBUFFER);
  
  IndexBuffer* pIndexBuffer = reinterpret_cast<IndexBuffer*>(pObject);
  
  vTrace("rows %lld freeSpace %p prevBuffer %p nextBuffer %p\n",
         pIndexBuffer->m_nRows,
         pIndexBuffer->m_pFreeSpace,
         pIndexBuffer->m_pPrevBuffer,
         pIndexBuffer->m_pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allocate a row in this segmented space                                     */
/*----------------------------------------------------------------------------*/
Row* IndexBuffer::pAllocRow(i8 iDataBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  i8 iBytes = Row::iBytesRequired(iDataBytes);

  // test  sufficient freespace
  if (iFreeSpace() < iBytes) {
    return nullptr;
  }
  
  Row* pRow = reinterpret_cast<Row*>(m_pFreeSpace);

  m_pFreeSpace = static_cast<Object*>(pvAdd(m_pFreeSpace,iBytes));
  
  new(pRow) Row(iDataBytes);
  
  m_nRows++;
    
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_,CN_INDEXBUFFER);
  
  return pRow;
}

/*----------------------------------------------------------------------------*/
/* append rows of arg buffer into this buffer                                 */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vAppendBuffer(IndexBuffer* pArgBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  // copy arg rows into this buffer
  void* pDst = pvAdd(pDataArea(),iBytesUsed());
  void* pSrc = pArgBuffer->pDataArea();
  i8 nBytes  = pArgBuffer->iBytesUsed();
  Memory::vCopy(pDst,pSrc,nBytes);
  
  m_pFreeSpace = static_cast<Object*>(pvAdd(m_pFreeSpace,nBytes));
  
  // inc the number of rows in this buffer
  i8 nArgRows = pArgBuffer->nRows();
  m_nRows += nArgRows;
}

/*----------------------------------------------------------------------------*/
/* first row                                                                  */
/*----------------------------------------------------------------------------*/
Row* IndexBuffer::pFirstRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  Row* pFirst = reinterpret_cast<Row*>(pDataArea());
  
  if ((char*)pFirst < (char*)m_pFreeSpace)
    return pFirst;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* next row                                                                   */
/*----------------------------------------------------------------------------*/
Row* IndexBuffer::pNextRow(Row* pCurr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
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
IndexBuffer* IndexBuffer::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  return m_pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vNextBuffer(IndexBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexBuffer::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  return m_pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vPrevBuffer(IndexBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* calc bytes used                                                            */
/*----------------------------------------------------------------------------*/
i8 IndexBuffer::iBytesUsed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  i8 iBytes = iSub(m_pFreeSpace,pDataArea());
  
  return iBytes;
}

/*----------------------------------------------------------------------------*/
/* get number of rows in buffer                                               */
/*----------------------------------------------------------------------------*/
i8 IndexBuffer::nRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  return m_nRows;
}

/*----------------------------------------------------------------------------*/
/* set row count                                                              */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vRowCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  m_nRows = 0;
  for (Row* pRow = pFirstRow(); pRow; pRow = pNextRow(pRow))
    m_nRows++;
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_,CN_INDEXBUFFER);
}

/*----------------------------------------------------------------------------*/
/* copy given buffer                                                          */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vCopyBuffer(IndexBuffer* pFromBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  i8 iBytes = pFromBuffer->iBytesUsed();
  
  Memory::vCopy(pDataArea(),pFromBuffer->pDataArea(),iBytes);
  
  char* pRevised = pcAdd(pDataArea(),iBytes);
  
  m_pFreeSpace = reinterpret_cast<Object*>(pRevised);

  vRowCount();
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_,CN_INDEXBUFFER);
}

/*----------------------------------------------------------------------------*/
/* insert row into this buffer                                                */
/*----------------------------------------------------------------------------*/
//bool IndexBuffer::bInsertRow(TxRef* pTxRef, StrVal* pRowStr)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
//  
//  i8 iDataBytes  = pRowStr->iSize();
//  
//  Row* pRow = pAllocRow(iDataBytes);
//  if (!pRow)
//    return false;
//  
//  i8 iTxId = pTxRef->iTxId();
//  
////  vTrace("TxId %lld\n",iTxId);
//  
//  pRow->vTxId(iTxId);
//  
//  Memory::vCopy(pRow->pData(),pRowStr,iDataBytes);
//  
//  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_,CN_INDEXBUFFER);
//
//  return true;
//}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool IndexBuffer::bInsertRow(TxRef* pTxRef, char* pRecord, i8 iLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  i8 nBytesRequired = StrVal::nBytesRequired(iLength);
  
  Row* pRow = pAllocRow(nBytesRequired);
  if (!pRow)
    return false;
  
  i8 iTxId = pTxRef->iTxId();
  
  pRow->vTxId(iTxId);
  
  char* pData = pRow->pData();
  
  StrVal* pStr = new((StrVal*)pData) StrVal;
  
  Memory::vCopy(pStr->sStr(),pRecord,iLength);
  pStr->pData()[iLength] = '\0';
  pStr->vSize(iLength);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_,CN_INDEXBUFFER);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
Object* IndexBuffer::pDataArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  Object* pStart = static_cast<Object*>(pvAdd(this,sizeof(IndexBuffer)));
  
  return pStart;
}

/*----------------------------------------------------------------------------*/
/* how much free space (in bytes) left in segment?                            */
/*----------------------------------------------------------------------------*/
i8 IndexBuffer::iFreeSpace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  i8 nBytes = iSub(m_pLastByte,m_pFreeSpace);

  nBytes -= sizeof(Footer);
  
  return nBytes;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
