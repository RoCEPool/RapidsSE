/*============================================================================*/
/* ResultBuffer.cpp -- Result Buffer                                          */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ResultBuffer* DBobjects::_pResultBuffer(void* aAddr)
{
  ResultBuffer* pResultBuffer = reinterpret_cast<ResultBuffer*>(aAddr);
  
  RASSERT( !pResultBuffer || CN_RESULTBUFFER == pResultBuffer->eClass());
  
  return pResultBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new result buffer from buffer pool                                 */
/*----------------------------------------------------------------------------*/
void* ResultBuffer::operator new(size_t iSize)
{
  RASSERT(iSize);
  Buffer* pBuffer = (Buffer*)Heap::pAllocate(CN_RESULTBUFFER);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* ResultBuffer::operator new(size_t iSize, void* aAddr)
{
  RASSERT(iSize);
  
  return aAddr;
}

/*----------------------------------------------------------------------------*/
/* return a result buffer to the buffer pool                                  */
/*----------------------------------------------------------------------------*/
void ResultBuffer::operator delete(void* pBufferAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pBufferAddress);
  
  Heap::vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ResultBuffer::ResultBuffer(Table* pTable)
: ItemBuffer(CN_RESULTBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESULTBUFFER,_FOOTER_);
  
  m_bIsLast = false;
  m_pTable  = pTable;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_RESULTBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_RESULTBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ResultBuffer::~ResultBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESULTBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append row                                                                 */
/*----------------------------------------------------------------------------*/
bool ResultBuffer::bAppendRow(Row* aRow, i8 nRowSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESULTBUFFER,_FOOTER_);
  
  i8 nResultRowSize = ResultRow::nBytesRequired(nRowSize);
  
  i8 nFreeSpace = nFreeBytes();
  if (nFreeSpace < nResultRowSize)
    return false;
  
  ResultRow* aResultRow = reinterpret_cast<ResultRow*>(pAllotItem(nResultRowSize));

  new(aResultRow) ResultRow(aRow,nResultRowSize,nRowSize);
  
//  Row* aAllotedRow = aResultRow->aRow();
//  
//  MemLib::vCopy(aAllotedRow,aRow,nRowSize,CN_RESULTBUFFER);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* first result row                                                           */
/*----------------------------------------------------------------------------*/
ResultRow* ResultBuffer::pFirstRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  ResultRow* pFirst = reinterpret_cast<ResultRow*>(pFirstItem());
  
  return pFirst;
}

/*----------------------------------------------------------------------------*/
/* set buffer is last                                                         */
/*----------------------------------------------------------------------------*/
void ResultBuffer::vIsLast(bool bIsLast)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESULTBUFFER,_FOOTER_);
  
  m_bIsLast = bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get if buffer is last                                                      */
/*----------------------------------------------------------------------------*/
bool ResultBuffer::bIsLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESULTBUFFER,_FOOTER_);
  
  return m_bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get next row                                                               */
/*----------------------------------------------------------------------------*/
ResultRow* ResultBuffer::pNextRow(ResultRow* pCurrentRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESULTBUFFER,_FOOTER_);
  
  ResultRow* pNext = reinterpret_cast<ResultRow*>(pNextItem(pCurrentRow));
  
  return pNext;
}

/*----------------------------------------------------------------------------*/
/* show the contents of the buffer                                            */
/*----------------------------------------------------------------------------*/
void ResultBuffer::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESULTBUFFER,_FOOTER_);

  ColumnVec* pColVec = m_pTable->pColumnVec();
  
  ResultRow* pResultRow;
  for (pResultRow = pFirstRow();
       pResultRow;
       pResultRow = pNextRow(pResultRow)) {

    pResultRow->vShow(pColVec);
    vTrace("\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* ResultBuffer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESULTBUFFER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
