/*============================================================================*/
/* RbCursor.cpp - row buffer cursor                                           */
/*============================================================================*/

#include "DBobjects.h"

extern Heap* gpSmallHeap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RbCursor* DBobjects::_pRbCursor(void* pAddr)
{
  RbCursor* pRbCursor = reinterpret_cast<RbCursor*>(pAddr);
  
  RASSERT( !pRbCursor || CN_ROWCURSOR == pRbCursor->eClass() );
  
  return pRbCursor;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RbCursor::operator new(size_t iSize)
{
  RASSERT(iSize);

  void* pObject = Heap::pAllocate(CN_ROWCURSOR);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RbCursor::operator new(size_t iSize, RbCursor* pRbCursor)
{
  RASSERT(iSize);
  return pRbCursor;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RbCursor::operator delete(void* pRbCursorAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pRbCursorAddress);
  
  Heap::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RbCursor::RbCursor(void)
: Object(CN_ROWCURSOR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
  
  m_pTable     = nullptr;
  m_iPartition = 0;
  
  m_pPartition = nullptr;
  m_pRowStrand = nullptr;
  m_pRowBuffer = nullptr;
  
  _checkInitialised_(CN_ROWCURSOR);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RbCursor::~RbCursor(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* begin iteration                                                            */
/*----------------------------------------------------------------------------*/
void RbCursor::vBegin(i8 iPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);

  m_iPartition = iPartition;
  
  m_pPartition = nullptr;
  m_pRowStrand = nullptr;
  m_pRowBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/* end iteration                                                              */
/*----------------------------------------------------------------------------*/
void RbCursor::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
  
  m_pPartition = nullptr;
  m_pRowStrand = nullptr;
  m_pRowBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/* set to first buffer                                                        */
/*----------------------------------------------------------------------------*/
void RbCursor::vFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
  
  m_pPartition = m_pTable->pPartition(m_iPartition);
  m_pRowStrand = m_pPartition->pHeadStrand();
  
  if (m_pRowStrand)
    m_pRowBuffer = m_pRowStrand->pFirstBuffer();
  else
    m_pRowBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/* advance to next buffer                                                     */
/*----------------------------------------------------------------------------*/
void RbCursor::vNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
  
  ASSERT(m_pRowBuffer);
  
  m_pRowBuffer = m_pRowBuffer->pNextBuffer();
  if (m_pRowBuffer)
    return;
  
  m_pRowStrand = m_pRowStrand->pNextStrand();
  if (m_pRowStrand)
    m_pRowBuffer = m_pRowStrand->pFirstBuffer();
  else
    m_pRowBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/* get current row buffer                                                     */
/*----------------------------------------------------------------------------*/
RowBuffer* RbCursor::pRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
  
  return m_pRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* RbCursor::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
  
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void RbCursor::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR,_FOOTER_);
  
  m_pTable = pTable;
  
  m_pPartition = nullptr;
  m_pRowStrand = nullptr;
  m_pRowBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* RbCursor::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCURSOR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
