/*============================================================================*/
/* TxCursor.cpp -- Cursor over Tx's in TxCollection                           */
/*============================================================================*/

#include "Transactions.h"

extern Heap* gpSmallHeap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TxCursor* Transactions::_pTxCursor(void* aAddr)
{
  TxCursor* pTxCursor = reinterpret_cast<TxCursor*>(aAddr);
  
  RASSERT( !pTxCursor || CN_TXCURSOR == pTxCursor->eClass() );
  
  return pTxCursor;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TxCursor::operator new(size_t iSize)
{
  RASSERT(iSize);

  void* pObject = Heap::pAllocate(CN_TXCURSOR);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TxCursor::operator new(size_t iSize, TxCursor* pTxCursor)
{
  RASSERT(iSize);
  return pTxCursor;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TxCursor::operator delete(void* pThreadAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pThreadAddress);
  
  Heap::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TxCursor::TxCursor(Collection_t<Tx>* gpTxCollection)
: Object(CN_TXCURSOR),
  CollectionCursor_t<Tx>(gpTxCollection)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXCURSOR,_FOOTER_);
  
  _checkInitialised_(CN_TXCURSOR);
  
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
Footer* TxCursor::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXCURSOR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
