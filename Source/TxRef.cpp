/*============================================================================*/
/* TxRef.cpp -- Transaction Reference                                         */
/*============================================================================*/

#include "Transactions.h"

extern TxMap*        gpTxMap;
extern TxCollection* gpTxCollection;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TxRef* Transactions::_pTxRef(void* aAddr)
{
  TxRef* pTxRef = reinterpret_cast<TxRef*>(aAddr);
  
  RASSERT( !pTxRef || CN_TXREF == pTxRef->eClass() );
  
  return pTxRef;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TxRef::operator new(size_t iSize, TxRef* pTxRef)
{
  RASSERT(iSize);
  return pTxRef;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TxRef::operator new(size_t iSize)
{
  RASSERT(iSize);
  Object* pObject = Heap::pAllocate(CN_TXREF);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TxRef::operator delete(void* pBufferAddress)
{
  RASSERT(pBufferAddress);
  //vFatal("fatal: TxMap class cannot be deleted"_Stz);
  
  TxRef* pTxRef = static_cast<TxRef*>(pBufferAddress);
  
  Heap::vFree(pTxRef);
  
  //NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TxRef::TxRef(void)
: Object(CN_TXREF)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXREF,_FOOTER_);
  
  m_pTx = nullptr;
  
  _checkInitialised_(CN_TXREF);
  
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
  
  i8 iTxId = m_pTx->iLocalTxId();
  
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
  
  if (m_pTx == nullptr)
    return false;
  
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
/* dump a tx                                                                  */
/*----------------------------------------------------------------------------*/
void TxRef::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXREF,_FOOTER_);
  
  m_pTx->vContentDump();
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
/* finalize                                                                   */
/*----------------------------------------------------------------------------*/
void TxRef::vFinalize(eCN eClass, Object* pObject)
{
  ASSERT(eClass == CN_TXREF);
  ASSERT(pObject->eClass() == CN_TXREF);

  TxRef* pTxRef = reinterpret_cast<TxRef*>(pObject);
  ASSERT(pTxRef->bWordBody());
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
Footer* TxRef::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXREF);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
