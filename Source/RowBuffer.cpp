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
/* return a row buffer to the buffer pool                                     */
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
  m_nRows       = 0;
  m_pPrevBuffer = nullptr;
  m_pNextBuffer = nullptr;
  //m_xWsp
  
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
  
  m_xWsp.vUsed(iDataBytes);

  m_nRows       = 0;
  m_pPrevBuffer = nullptr;
  m_pNextBuffer = nullptr;
  
  //m_xWsp
  
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
  
  vTrace("rows %lld used %lld free %lld prevBuffer %p nextBuffer %p\n"_Str,
         pRowBuffer->m_nRows,
         pRowBuffer->m_xWsp.nUsed(),
         pRowBuffer->m_xWsp.nFree(),
         pRowBuffer->m_pPrevBuffer,
         pRowBuffer->m_pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allocate a row in this segmented space                                     */
/*----------------------------------------------------------------------------*/
Row* RowBuffer::pAllocRow(i8 iStrLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  i8 nBytesRequired = Row::nBytesRequiredRow(iStrLength);
  
  // test  sufficient freespace
  if (nFreeBytes() < nBytesRequired) {
    return nullptr;
  }
  
  Row* pRow = reinterpret_cast<Row*>(m_xWsp.aAllot(nBytesRequired));
  new(pRow) Row(nBytesRequired);
  
  m_nRows++;
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_,CN_ROWBUFFER);
  
  return pRow;
}

/*----------------------------------------------------------------------------*/
/* append rows of arg buffer into this buffer                                 */
/*----------------------------------------------------------------------------*/
void RowBuffer::vAppendBuffer(RowBuffer* pSrcBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  // how many bytes to be copied from arg buffer
  i8 nBytes  = pSrcBuffer->nUsedBytes();
  byte* aSrc = pSrcBuffer->aDataArea();
  
  // allot space in this buffer
  void* pDst = m_xWsp.aAllot(nBytes);
  
  // copy arg rows into this buffer
  MemLib::vCopy(pDst,aSrc,nBytes);
  
  // inc the number of rows in this buffer
  i8 nSrcRows = pSrcBuffer->nRows();
  m_nRows += nSrcRows;
}

/*----------------------------------------------------------------------------*/
/* get durability                                                             */
/*----------------------------------------------------------------------------*/
eRD RowBuffer::eDurability(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  return m_eDurability;
}

/*----------------------------------------------------------------------------*/
/* copy given buffer                                                          */
/*----------------------------------------------------------------------------*/
void RowBuffer::vCopyBuffer(RowBuffer* pSrcBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  // how many bytes to be copied from src buffer
  i8 nBytes  = pSrcBuffer->nUsedBytes();
  byte* aSrc = pSrcBuffer->aDataArea();
  
  // reset and allot space in this buffer
  m_xWsp.vUsed(0);
  void* pDst = m_xWsp.aAllot(nBytes);
  
  // copy from rows into this buffer
  MemLib::vCopy(pDst,aSrc,nBytes);
  
  // set the number of rows in this buffer
  m_nRows = pSrcBuffer->nRows();
}

/*----------------------------------------------------------------------------*/
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
byte* RowBuffer::aDataArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  byte* pStart = reinterpret_cast<byte*>(m_xWsp.aArea());
  
  return pStart;
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
/* first row                                                                  */
/*----------------------------------------------------------------------------*/
Row* RowBuffer::pFirstRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  if ( ! m_xWsp.nSize() )
    return nullptr;

  Row* pFirst = reinterpret_cast<Row*>(m_xWsp.aArea());
  
  return pFirst;
}

/*----------------------------------------------------------------------------*/
/* how much free space (in bytes) left in segment?                            */
/*----------------------------------------------------------------------------*/
i8 RowBuffer::nFreeBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  i8 nBytes = m_xWsp.nFree();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* insert row into this buffer                                                */
/*----------------------------------------------------------------------------*/
bool RowBuffer::bInsertRow(TxRef* pTxRef, Str* pRecord, i8 iLength, Row** ppRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  *ppRow = nullptr;
  
  i8 nBytes = StrVal::nAreaRequired(iLength);
  
  Row* pRow = pAllocRow(nBytes);
  if (!pRow)
    return false;
  
  i8 iTxId = pTxRef->iTxId();
  
  pRow->vTxId(iTxId);
  
  char* pData = pRow->aRowData();
  
  StrVal* pStr = new((StrVal*)pData) StrVal((StrVal*)pData);
  
  pStr->vAssign(pRecord,iLength);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_,CN_ROWBUFFER);
  
  *ppRow = pRow;
  
  return true;
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
/* next row                                                                   */
/*----------------------------------------------------------------------------*/
Row* RowBuffer::pNextRow(Row* pCurr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  i8 nBytes = pCurr->nSizeRow();
  
  Row* pNext = pAdd<Row*>(pCurr,nBytes);
  
  if ((char*)pNext >= (char*)m_xWsp.aLatter())
    return nullptr;
  
  return pNext;
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
/* get bytes used                                                             */
/*----------------------------------------------------------------------------*/
i8 RowBuffer::nUsedBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  i8 nBytes = m_xWsp.nUsed();
  
  return nBytes;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
