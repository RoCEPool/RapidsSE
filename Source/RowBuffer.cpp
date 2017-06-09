/*============================================================================*/
/* RowBuffer.cpp -- Entry Buffer                                                */
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
/* dump rowbuffer                                                             */
/*----------------------------------------------------------------------------*/
void RowBuffer::vDump(eCN eClass, Object* pObject, Str* aTab)
{
  ASSERT(eClass == CN_ROWBUFFER);
  ASSERT(pObject->eClass() == CN_ROWBUFFER);
  
  RowBuffer* pRowBuffer = reinterpret_cast<RowBuffer*>(pObject);
  
  vTrace("rows %lld used %lld free %lld prevBuffer %p nextBuffer %p\n"_Str,
         pRowBuffer->m_nEntries,
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
Entry* RowBuffer::pAllocRow(i8 iStrLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  i8 nBytesRequired = Entry::nBytesRequiredRow(iStrLength);
  
  // test  sufficient freespace
  if (nFreeBytes() < nBytesRequired) {
    return nullptr;
  }
  
  Entry* pRow = reinterpret_cast<Entry*>(m_xWsp.aAllot(nBytesRequired));
  new(pRow) Entry(CN_ROW,nBytesRequired);
  
  m_nEntries++;
  
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
  i8 nSrcRows = pSrcBuffer->m_nEntries;
  m_nEntries += nSrcRows;
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
  m_nEntries = pSrcBuffer->nEntries();
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
/* first row                                                                  */
/*----------------------------------------------------------------------------*/
Entry* RowBuffer::pFirstRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  if ( ! m_xWsp.nSize() )
    return nullptr;

  Entry* pFirst = reinterpret_cast<Entry*>(m_xWsp.aArea());
  
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
bool RowBuffer::bInsertRow(TxRef* pTxRef, Str* pRecord, i8 iLength, Entry** ppRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  *ppRow = nullptr;
  
  i8 nBytes = StrVal::nAreaRequired(iLength);
  
  Entry* pRow = pAllocRow(nBytes);
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
/* next row                                                                   */
/*----------------------------------------------------------------------------*/
Entry* RowBuffer::pNextRow(Entry* pCurr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  i8 nBytes = pCurr->nSizeRow();
  
  Entry* pNext = pAdd<Entry*>(pCurr,nBytes);
  
  if ((char*)pNext >= (char*)m_xWsp.aLatter())
    return nullptr;
  
  return pNext;
}

/*----------------------------------------------------------------------------*/
/* set row count                                                              */
/*----------------------------------------------------------------------------*/
void RowBuffer::vRowCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  m_nEntries = 0;
  for (Entry* pRow = pFirstRow(); pRow; pRow = pNextRow(pRow))
    m_nEntries++;
  
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
