/*============================================================================*/
/* TxRef.cpp -- Transaction Reference                                         */
/*============================================================================*/

#include "Transactions.h"

extern TxMap*        gpTxMap;
extern TxCollection* gpTxCollection;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TxRef* Transactions::_pTxRef(void* pAddr)
{
  TxRef* pTxRef = reinterpret_cast<TxRef*>(pAddr);
  
  return pTxRef;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TxRef::operator new(size_t iSize, TxRef* pTxRef)
{
  ASSERT(iSize);
  return pTxRef;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TxRef::operator delete(void* pBufferAddress)
{
  ASSERT(pBufferAddress);
  vFatal("fatal: TxMap class cannot be deleted");
  
  ASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TxRef::TxRef(void)
: Object(CN_TXREF)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXREF,_FOOTER_);
  
  m_pTx = nullptr;
  
  _checkInitialised_(sizeof(TxRef));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TXREF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TxRef::~TxRef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXREF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get the id of the referenced tx                                            */
/*----------------------------------------------------------------------------*/
i8 TxRef::iTxId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXREF,_FOOTER_);
  
  i8 iTxId = m_pTx->iTxId();
  
  return iTxId;
}

/*----------------------------------------------------------------------------*/
/* begin a tx                                                                 */
/*----------------------------------------------------------------------------*/
bool TxRef::bBeginTx(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXREF,_FOOTER_);
  
  ASSERT( ! m_pTx );
  
  m_pTx = gpTxMap->pBeginTx();
  
  m_pTx->vPhase(TP_EXECUTING);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* commit a tx                                                                */
/*----------------------------------------------------------------------------*/
bool TxRef::bCommitTx(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXREF,_FOOTER_);
  
  m_pTx->vPhase(TP_COMMITTING);
  m_pTx->vPhase(TP_COMMITTED);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* end a tx                                                                   */
/*----------------------------------------------------------------------------*/
bool TxRef::bEndTx(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXREF,_FOOTER_);
  
  gpTxMap->vEndTx(m_pTx);
  m_pTx = nullptr;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* rollback a tx                                                              */
/*----------------------------------------------------------------------------*/
bool TxRef::bRollbackTx(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXREF,_FOOTER_);
  
  m_pTx->vPhase(TP_ROLLINGBACK);
  m_pTx->vPhase(TP_ROLLEDBACK);
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* TxRef::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXREF);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/