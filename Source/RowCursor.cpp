/*============================================================================*/
/* RowCursor.cpp - row cursor                                                 */
/*============================================================================*/

#include "DBobjects.h"

extern Heap* gpSmallHeap;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowCursor::operator new(size_t)
{
  Object* pObject = gpSmallHeap->pAllocObject();
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowCursor::operator new(size_t, void* pRowCursorAddress)
{
  return pRowCursorAddress;
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
  _poolAddress_(this);
  _magic_();
  
  m_pTable     = nullptr;
  m_pPartition = nullptr;
  m_pRowStrand = nullptr;
  m_pRowBuffer = nullptr;
  m_pObject    = nullptr;
  m_pRow       = nullptr;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowCursor::~RowCursor(void)
{
  _poolAddress_(this);
  _magic_();
}

/*----------------------------------------------------------------------------*/
/* begin iteration                                                            */
/*----------------------------------------------------------------------------*/
void RowCursor::vBegin(i8 iPartition)
{
  _poolAddress_(this);
  _magic_();

  m_iPartition = iPartition;
  
  m_pPartition = nullptr;
  m_pRowStrand = nullptr;
  m_pRowBuffer = nullptr;
  m_pObject    = nullptr;
  m_pRow       = nullptr;
}

/*----------------------------------------------------------------------------*/
/* end iteration                                                              */
/*----------------------------------------------------------------------------*/
void RowCursor::vEnd(void)
{
  _poolAddress_(this);
  _magic_();
  
  m_pPartition = nullptr;
  m_pRowStrand = nullptr;
  m_pRowBuffer = nullptr;
  m_pObject    = nullptr;
  m_pRow       = nullptr;
}

/*----------------------------------------------------------------------------*/
/* set to first row                                                           */
/*----------------------------------------------------------------------------*/
void RowCursor::vFirst(void)
{
  _poolAddress_(this);
  _magic_();
  
  m_pPartition = m_pTable->pPartition(m_iPartition);
  m_pRowStrand = m_pPartition->pHeadStrand();
  if (m_pRowStrand) {
    m_pRowBuffer = m_pRowStrand->pFirstBuffer();
    if (m_pRowBuffer)
      m_pObject = m_pRowBuffer->pFirstObject();
    else
      m_pObject = nullptr;
  }
  else {
    m_pRowBuffer = nullptr;
    m_pObject = nullptr;
  }
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void RowCursor::vFirst2(void)
{
  _poolAddress_(this);
  _magic_();
  
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
  _poolAddress_(this);
  _magic_();
  
  StrVal* pStrVal = reinterpret_cast<StrVal*>(m_pObject);
  
  i8 iSize = pStrVal->iSize();
  
  m_pObject = m_pRowBuffer->pNextObject(m_pObject,iSize);
  if (m_pObject)
    return;
  
  m_pRowBuffer = m_pRowBuffer->pNextBuffer();
  if (m_pRowBuffer) {
    m_pObject = m_pRowBuffer->pFirstObject();
    if (m_pObject)
      return;
  }
  
  m_pRowStrand = m_pRowStrand->pNextStrand();
  if (m_pRowStrand) {
    m_pRowBuffer = m_pRowStrand->pFirstBuffer();
    if (m_pRowBuffer) {
      m_pObject = m_pRowBuffer->pFirstObject();
      if (m_pObject)
        return;
    }
  }
  
  m_pObject = nullptr;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void RowCursor::vNext2(void)
{
  _poolAddress_(this);
  _magic_();
  
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
/* get current object                                                         */
/*----------------------------------------------------------------------------*/
Object* RowCursor::pObject(void)
{
  _poolAddress_(this);
  _magic_();
  
  return m_pObject;
}

/*----------------------------------------------------------------------------*/
/* get current row                                                            */
/*----------------------------------------------------------------------------*/
Row* RowCursor::pRow(void)
{
  _poolAddress_(this);
  _magic_();
  
  return m_pRow;
}

/*----------------------------------------------------------------------------*/
/* get current row (as a char*)                                               */
/*----------------------------------------------------------------------------*/
char* RowCursor::pRowAsStr(void)
{
  _poolAddress_(this);
  _magic_();
  
  StrVal* pStrVal = reinterpret_cast<StrVal*>(m_pObject);

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
  _poolAddress_(this);
  _magic_();
  
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void RowCursor::vTable(Table* pTable)
{
  _poolAddress_(this);
  _magic_();
  
  m_pTable = pTable;
  m_pObject   = nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/