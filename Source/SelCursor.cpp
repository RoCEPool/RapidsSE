/*============================================================================*/
/* SelCursor.cpp - select all cursor                                          */
/*============================================================================*/

#include "DBobjects.h"

extern Heap* gpSmallHeap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SelCursor* DBobjects::_pSelCursor(void* aAddr)
{
  SelCursor* pSelCursor = reinterpret_cast<SelCursor*>(aAddr);
  
  RASSERT( !pSelCursor || CN_SELCURSOR == pSelCursor->eClass() );
  
  return pSelCursor;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SelCursor::operator new(size_t iSize)
{
  RASSERT(iSize);

  void* pObject = Heap::pAllocate(CN_SELCURSOR);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SelCursor::operator new(size_t iSize, SelCursor* pSelCursor)
{
  RASSERT(iSize);
  return pSelCursor;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SelCursor::operator delete(void* pSelCursorAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pSelCursorAddress);
  
  Heap::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SelCursor::SelCursor(void)
: Object(CN_SELCURSOR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SELCURSOR,_FOOTER_);
  
  m_pTable     = nullptr;
  m_iPartition = 0;
  
  m_pPartition = nullptr;
  m_pRowBuffer = nullptr;
  m_pRow       = nullptr;
  
  _checkInitialised_(CN_SELCURSOR);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SELCURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SelCursor::~SelCursor(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SELCURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* finalize                                                                   */
/*----------------------------------------------------------------------------*/
void SelCursor::vFinalize(eCN eClass, Object* pObject)
{
  ASSERT(eClass == CN_SELCURSOR);
  ASSERT(pObject->eClass() == CN_SELCURSOR);
  
  SelCursor* pSelCursor = reinterpret_cast<SelCursor*>(pObject);
  ASSERT(pSelCursor->bWordBody());
}

/*----------------------------------------------------------------------------*/
/* begin iteration                                                            */
/*----------------------------------------------------------------------------*/
void SelCursor::vBegin(i8 iPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SELCURSOR,_FOOTER_);

  m_iPartition = iPartition;
  
  m_pPartition = nullptr;
  m_pRowBuffer = nullptr;
  m_pRow       = nullptr;
}

/*----------------------------------------------------------------------------*/
/* end iteration                                                              */
/*----------------------------------------------------------------------------*/
void SelCursor::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SELCURSOR,_FOOTER_);
  
  m_pPartition = nullptr;
  m_pRowBuffer = nullptr;
  m_pRow       = nullptr;
}

/*----------------------------------------------------------------------------*/
/* set to first row                                                           */
/*----------------------------------------------------------------------------*/
void SelCursor::vFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SELCURSOR,_FOOTER_);
  
  m_pPartition = m_pTable->pPartition(m_iPartition);
  IndexData* pHiddenIndexData = m_pPartition->pHiddenIndexData();
  
  m_pRowBuffer = pHiddenIndexData->pFirstBuffer();
  if (m_pRowBuffer) {
    m_pRow = m_pRowBuffer->pNextRow(m_pRowBuffer->pFirstRow());
  }
  else
    m_pRow = nullptr;
}

/*----------------------------------------------------------------------------*/
/* advance to next row                                                        */
/*----------------------------------------------------------------------------*/
void SelCursor::vNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SELCURSOR,_FOOTER_);
  
  ASSERT(m_pRow);
  ASSERT(m_pRowBuffer);
  
  m_pRow = m_pRowBuffer->pNextRow(m_pRow);
  if (m_pRow)
    return;
  
  m_pRowBuffer = (EntryBuffer*)m_pRowBuffer->pNextBuffer();
  if (m_pRowBuffer) {
    m_pRow = m_pRowBuffer->pFirstRow();
    if (m_pRow)
      return;
  }
  
  m_pRow = nullptr;
}

/*----------------------------------------------------------------------------*/
/* get current row                                                            */
/*----------------------------------------------------------------------------*/
Entry* SelCursor::pRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SELCURSOR,_FOOTER_);
  
  return m_pRow;
}

/*----------------------------------------------------------------------------*/
/* get current row (as a Stz*)                                                */
/*----------------------------------------------------------------------------*/
Stz* SelCursor::aRowAsStr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SELCURSOR,_FOOTER_);
  
  if (! m_pRow)
    return nullptr;
  
  StrVal* pStrVal = reinterpret_cast<StrVal*>(m_pRow->aRow());
  
  if (pStrVal)
    return pStrVal->aStr();
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get current row buffer                                                     */
/*----------------------------------------------------------------------------*/
EntryBuffer* SelCursor::pRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SELCURSOR,_FOOTER_);
  
  return m_pRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* SelCursor::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SELCURSOR,_FOOTER_);
  
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void SelCursor::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SELCURSOR,_FOOTER_);
  
  m_pTable = pTable;
  m_pRow   = nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SelCursor::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SELCURSOR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
