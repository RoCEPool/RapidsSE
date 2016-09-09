/*============================================================================*/
/* TxCursor.cpp -- Cursor over Tx's in TxCollection                           */
/*============================================================================*/

#include "Transactions.h"

extern Heap* gpSmallHeap;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TxCursor::operator new(size_t iSize)
{
  ASSERT(iSize);
  Object* pObject = gpSmallHeap->pAllocObject(CN_TXCURSOR);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TxCursor::operator new(size_t iSize, TxCursor* pTxCursor)
{
  ASSERT(iSize);
  return pTxCursor;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TxCursor::operator delete(void* pThreadAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pThreadAddress);
  
  gpSmallHeap->vFreeObject(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TxCursor::TxCursor(Collection_t<Tx>* gpTxCollection)
: Object(CN_TXCURSOR),
  CollectionCursor_t<Tx>(gpTxCollection)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXCURSOR,_FOOTER_);
  
  Memory::vCheckInitialised(this,sizeof(TxCursor));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TXCURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TxCursor::~TxCursor(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXCURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void TxCursor::vInit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXCURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* TxCursor::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXCURSOR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/