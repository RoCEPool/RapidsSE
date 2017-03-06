/*============================================================================*/
/* RowCursor.cpp - row cursor                                                 */
/*============================================================================*/

#include "DBobjects.h"

extern Heap* gpSmallHeap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RowCursor* DBobjects::_pRowCursor(void* aAddr)
{
  RowCursor* pRowCursor = reinterpret_cast<RowCursor*>(aAddr);
  
  RASSERT( !pRowCursor || CN_ROWCURSOR == pRowCursor->eClass() );
  
  return pRowCursor;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowCursor::operator new(size_t iSize)
{
  RASSERT(iSize);

  void* pObject = Heap::pAllocate(CN_ROWCURSOR);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowCursor::operator new(size_t iSize, RowCursor* pRowCursor)
{
  RASSERT(iSize);
  return pRowCursor;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RowCursor::operator delete(void* pRowCursorAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pRowCursorAddress);
  
  Heap::vFree(pObject);
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
  
  _checkInitialised_(CN_ROWCURSOR);
  
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
  
  if (m_pRowBuffer)
    m_pRow = m_pRowBuffer->pFirstRow();
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
    m_pRow = m_pRowBuffer->pNextRow(m_pRow);
  else
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
  
  StrVbl* pStrVal = reinterpret_cast<StrVbl*>(m_pRow->pData());
  
  if (pStrVal)
    return pStrVal->aStr();
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get current row buffer                                                     */
/*----------------------------------------------------------------------------*/
RowBuffer* RowCursor::pRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
  
  return m_pRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* set current row buffer                                                     */
/*----------------------------------------------------------------------------*/
void RowCursor::vRowBuffer(RowBuffer* pRowBuffer)
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
Footer* RowCursor::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
