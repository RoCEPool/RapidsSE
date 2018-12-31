/*============================================================================*/
/* RowCursor.cpp - row cursor                                                 */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RowCursor* DBobjects::_pRowCursor(void* aAddr)
{
  RowCursor* pRowCursor = reinterpret_cast<RowCursor*>(aAddr);
  
  RASSERT_TYPE(pRowCursor,CN_ROWCURSOR);
  
  return pRowCursor;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowCursor::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_ROWCURSOR);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowCursor::operator new(size_t, RowCursor* pRowCursor)
{
  return pRowCursor;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RowCursor::operator delete(void* pRowCursorAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pRowCursorAddress);
  
  HeapMem::vFree(pObject);
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
  m_pRowBuffer = nullptr;
  m_pRow       = nullptr;
  
  _checkInitialised_(CN_ROWCURSOR);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowCursor::~RowCursor(void) noexcept(false)
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
  m_pRowBuffer = nullptr;
  m_pRow       = nullptr;
}

/*----------------------------------------------------------------------------*/
/* set to first row                                                           */
/*----------------------------------------------------------------------------*/
void RowCursor::vFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
  
  if (m_pRowBuffer)
    m_pRow = m_pRowBuffer->pFirstEntry();
  else
    m_pRow = nullptr;
}

/*----------------------------------------------------------------------------*/
/* advance to next row                                                        */
/*----------------------------------------------------------------------------*/
void RowCursor::vNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
  
  ASSERT(m_pRow);
  
  if (m_pRowBuffer)
    m_pRow = m_pRowBuffer->pNextEntry(m_pRow, 0);
  else
    m_pRow = nullptr;
}

/*----------------------------------------------------------------------------*/
/* get current row                                                            */
/*----------------------------------------------------------------------------*/
Entry* RowCursor::pRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
  
  return m_pRow;
}

/*----------------------------------------------------------------------------*/
/* get current row (as a Stz*)                                                */
/*----------------------------------------------------------------------------*/
Stz* RowCursor::aRowAsStr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
  
  if (! m_pRow)
    return nullptr;
  
//  StrVal* pStrVal = reinterpret_cast<StrVal*>(m_pRow->aRow());
//  
//  if (pStrVal)
//    return pStrVal->aStz();
//  else
//    return nullptr;
  
  s48K sRow;
  ColumnVec* pColVec = m_pTable->pColumnVec();
  Row* aRow = m_pRow->aRow();
  aRow->vAsBinaryStringVLQ(sRow,pColVec);
  
  return sRow;
}

/*----------------------------------------------------------------------------*/
/* get current row buffer                                                     */
/*----------------------------------------------------------------------------*/
EntryBuffer* RowCursor::pRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
  
  return m_pRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* set current row buffer                                                     */
/*----------------------------------------------------------------------------*/
void RowCursor::vRowBuffer(EntryBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
  
  m_pRowBuffer = pRowBuffer;
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
Footer* RowCursor::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
