/*============================================================================*/
/* RbCursor.cpp - row buffer cursor                                           */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RbCursor* DBobjects::_pRbCursor(void* aAddr)
{
  RbCursor* pRbCursor = reinterpret_cast<RbCursor*>(aAddr);
  
  RASSERT_TYPE(pRbCursor,CN_RBCURSOR);
  
  return pRbCursor;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RbCursor::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_RBCURSOR);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RbCursor::operator new(size_t, RbCursor* pRbCursor)
{
  return pRbCursor;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RbCursor::operator delete(void* pRbCursorAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pRbCursorAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RbCursor::RbCursor(void)
: Object(CN_RBCURSOR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RBCURSOR,_FOOTER_);
  
  m_pTable     = nullptr;
  m_iPartition = 0;
  
  m_pPartition = nullptr;
  m_pRowBuffer = nullptr;
  
  _checkInitialised_(CN_RBCURSOR);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_RBCURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RbCursor::~RbCursor(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RBCURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* begin iteration                                                            */
/*----------------------------------------------------------------------------*/
void RbCursor::vBegin(i8 iPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RBCURSOR,_FOOTER_);

  m_iPartition = iPartition;
  
  m_pPartition = nullptr;
  m_pRowBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/* end iteration                                                              */
/*----------------------------------------------------------------------------*/
void RbCursor::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RBCURSOR,_FOOTER_);
  
  m_pPartition = nullptr;
  m_pRowBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/* set to first buffer                                                        */
/*----------------------------------------------------------------------------*/
void RbCursor::vFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RBCURSOR,_FOOTER_);
  
  m_pPartition = m_pTable->pPartition(m_iPartition);
  IndexTree* pHiddenIndexTree = m_pPartition->pHiddenIndexTree();
  
  m_pRowBuffer = pHiddenIndexTree->pFirstL0Buffer();
}

/*----------------------------------------------------------------------------*/
/* advance to next buffer                                                     */
/*----------------------------------------------------------------------------*/
void RbCursor::vNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RBCURSOR,_FOOTER_);
  
  ASSERT(m_pRowBuffer);
  
  m_pRowBuffer = (EntryBuffer*)m_pRowBuffer->pNextBuffer();
  if (m_pRowBuffer)
    return;
}

/*----------------------------------------------------------------------------*/
/* get current row buffer                                                     */
/*----------------------------------------------------------------------------*/
EntryBuffer* RbCursor::pRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RBCURSOR,_FOOTER_);
  
  return m_pRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* RbCursor::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RBCURSOR,_FOOTER_);
  
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void RbCursor::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RBCURSOR,_FOOTER_);
  
  m_pTable = pTable;
  
  m_pPartition = nullptr;
  m_pRowBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* RbCursor::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RBCURSOR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
