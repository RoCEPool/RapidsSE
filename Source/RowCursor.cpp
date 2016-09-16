/*============================================================================*/
/* RowCursor.cpp - row cursor                                                 */
/*============================================================================*/

#include "DBobjects.h"

extern Heap* gpSmallHeap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RowCursor* DBobjects::_pRowCursor(void* pAddr)
{
  RowCursor* pRowCursor = reinterpret_cast<RowCursor*>(pAddr);
  
  return pRowCursor;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowCursor::operator new(size_t iSize)
{
  ASSERT(iSize);
  Object* pObject = gpSmallHeap->pAllocObject(CN_ROWCURSOR);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowCursor::operator new(size_t iSize, RowCursor* pRowCursor)
{
  ASSERT(iSize);
  return pRowCursor;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RowCursor::operator delete(void* pRowCursorAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pRowCursorAddress);
  
  gpSmallHeap->vFreeObject(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowCursor::RowCursor(void)
: Object(CN_ROWCURSOR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
  
  m_pTable     = nullptr;
  m_iPartition = 0;
  
  m_pPartition = nullptr;
  m_pRowStrand = nullptr;
  m_pRowBuffer = nullptr;
  m_pRow       = nullptr;
  
  _checkInitialised_(sizeof(RowCursor));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowCursor::~RowCursor(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* begin iteration                                                            */
/*----------------------------------------------------------------------------*/
void RowCursor::vBegin(i8 iPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);

  m_iPartition = iPartition;
  
  m_pPartition = nullptr;
  m_pRowStrand = nullptr;
  m_pRowBuffer = nullptr;
  m_pRow       = nullptr;
}

/*----------------------------------------------------------------------------*/
/* end iteration                                                              */
/*----------------------------------------------------------------------------*/
void RowCursor::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
  
  m_pPartition = nullptr;
  m_pRowStrand = nullptr;
  m_pRowBuffer = nullptr;
  m_pRow       = nullptr;
}

/*----------------------------------------------------------------------------*/
/* set to first row                                                           */
/*----------------------------------------------------------------------------*/
void RowCursor::vFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
  
  m_pPartition = m_pTable->pPartition(m_iPartition);
  m_pRowStrand = m_pPartition->pHeadStrand();
  if (m_pRowStrand) {
    m_pRowBuffer = m_pRowStrand->pFirstBuffer();
    if (m_pRowBuffer)
      m_pRow = m_pRowBuffer->pFirstRow();
    else
      m_pRow = nullptr;
  }
  else {
    m_pRowBuffer = nullptr;
    m_pRow = nullptr;
  }
}

/*----------------------------------------------------------------------------*/
/* advance to next row                                                        */
/*----------------------------------------------------------------------------*/
void RowCursor::vNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
  
  ASSERT(m_pRow);
  
  m_pRow = m_pRowBuffer->pNextRow(m_pRow);
  if (m_pRow)
    return;
  
  m_pRowBuffer = m_pRowBuffer->pNextBuffer();
  if (m_pRowBuffer) {
    m_pRow = m_pRowBuffer->pFirstRow();
    if (m_pRow)
      return;
  }
  
  m_pRowStrand = m_pRowStrand->pNextStrand();
  if (m_pRowStrand) {
    m_pRowBuffer = m_pRowStrand->pFirstBuffer();
    if (m_pRowBuffer) {
      m_pRow = m_pRowBuffer->pFirstRow();
      if (m_pRow)
        return;
    }
  }
  
  m_pRow = nullptr;
}

/*----------------------------------------------------------------------------*/
/* get current row                                                            */
/*----------------------------------------------------------------------------*/
Row* RowCursor::pRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
  
  return m_pRow;
}

/*----------------------------------------------------------------------------*/
/* get current row (as a char*)                                               */
/*----------------------------------------------------------------------------*/
char* RowCursor::pRowAsStr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
  
  if (! m_pRow)
    return nullptr;
  
  StrVal* pStrVal = reinterpret_cast<StrVal*>(m_pRow->pData());
  
  if (pStrVal)
    return pStrVal->sStr();
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* RowCursor::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
  
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void RowCursor::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
  
  m_pTable = pTable;
  m_pRow   = nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* RowCursor::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/