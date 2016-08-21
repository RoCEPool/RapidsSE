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
  
  m_pTable = nullptr;
  m_pRow   = nullptr;
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
void RowCursor::vBegin(void)
{
  _poolAddress_(this);
  _magic_();

  m_pRow = m_pTable->pPartition(0)->pFirstObject();
}

/*----------------------------------------------------------------------------*/
/* end iteration                                                              */
/*----------------------------------------------------------------------------*/
void RowCursor::vEnd(void)
{
  _poolAddress_(this);
  _magic_();
  
  m_pRow = nullptr;
}

/*----------------------------------------------------------------------------*/
/* set to first row                                                           */
/*----------------------------------------------------------------------------*/
void RowCursor::vFirst(void)
{
  _poolAddress_(this);
  _magic_();
  
  m_pRow = m_pTable->pPartition(0)->pFirstObject();
}

/*----------------------------------------------------------------------------*/
/* advance to next row                                                        */
/*----------------------------------------------------------------------------*/
void RowCursor::vNext(void)
{
  _poolAddress_(this);
  _magic_();
  
  StrVal* pStrVal = reinterpret_cast<StrVal*>(m_pRow);
  
  i8 iSize = pStrVal->iSize();

  m_pRow = m_pTable->pPartition(0)->pNextObject(m_pRow,iSize);
}

/*----------------------------------------------------------------------------*/
/* get current row (as a char*)                                               */
/*----------------------------------------------------------------------------*/
char* RowCursor::pRowAsStr(void)
{
  _poolAddress_(this);
  _magic_();
  
  StrVal* pStrVal = reinterpret_cast<StrVal*>(m_pRow);

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
  m_pRow   = nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/