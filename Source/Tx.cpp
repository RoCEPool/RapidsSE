/*============================================================================*/
/* Tx.cpp - Transaction                                                       */
/*============================================================================*/

#include "StorageEngine.h"

extern TxCollection*  gpTxCollection;
extern Arguments      gxArguments;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Tx* Transactions::_pTx(void* aAddr)
{
  Tx* pTx = reinterpret_cast<Tx*>(aAddr);
  
  RASSERT( !pTx || CN_TX == pTx->eClass() );
  
  return pTx;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Tx::operator new(size_t iSize)
{
  RASSERT(iSize);
  ASSERT(gpTxCollection);
  
  Tx* pTx = gpTxCollection->pAllocTx();
  
  return pTx;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Tx::operator new(size_t iSize, Tx* pTx)
{
  RASSERT(iSize);
  return pTx;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Tx::operator delete(void* pTxAddress)
{
  Tx* pTx = reinterpret_cast<Tx*>(pTxAddress);
  
  gpTxCollection->vFreeTx(pTx);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Tx::Tx(void)
: Object(CN_TX)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);

  m_iTxSlot    = -1;
  m_iLocalTxId = -1;
  m_iDistTxId  = -1;
  m_ePhase     = TP_NONE;
  m_zCounter   = gxArguments.m_nSlots;
  
  for (i8 i_Node = 0; i_Node < C_MAX_NODES; i_Node++)
    m_xNode[i_Node].m_bDistMember = false;
  
  _checkInitialised_(CN_TX);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get tx array slot                                                          */
/*----------------------------------------------------------------------------*/
i8 Tx::iTxSlot(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  return m_iTxSlot;
}

/*----------------------------------------------------------------------------*/
/* set tx array slot                                                          */
/*----------------------------------------------------------------------------*/
void Tx::vTxSlot(i8 iTxSlot)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  m_iTxSlot = iTxSlot;
}

/*----------------------------------------------------------------------------*/
/* decrement atmoic counter                                                   */
/*----------------------------------------------------------------------------*/
i8 Tx::iCounterDec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  i8 iCounter = --m_zCounter;
  
  return iCounter;
}

/*----------------------------------------------------------------------------*/
/* get general counter                                                        */
/*----------------------------------------------------------------------------*/
i8 Tx::iCounterGet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  return *m_zCounter;
}

/*----------------------------------------------------------------------------*/
/* set general counter                                                        */
/*----------------------------------------------------------------------------*/
void Tx::vCounterSet(i8 iCounter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  m_zCounter = iCounter;
}

/*----------------------------------------------------------------------------*/
/* get local tx id                                                            */
/*----------------------------------------------------------------------------*/
i8 Tx::iLocalTxId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  return m_iLocalTxId;
}

/*----------------------------------------------------------------------------*/
/* set local tx id                                                            */
/*----------------------------------------------------------------------------*/
void Tx::vLocalTxId(i8 iLocalTxId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  m_iLocalTxId = iLocalTxId;
}

/*----------------------------------------------------------------------------*/
/* get dist tx id                                                             */
/*----------------------------------------------------------------------------*/
i8 Tx::iDistTxId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  return m_iDistTxId;
}

/*----------------------------------------------------------------------------*/
/* set dist tx id                                                             */
/*----------------------------------------------------------------------------*/
void Tx::vDistTxId(i8 iDistTxId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  m_iDistTxId = iDistTxId;
}

/*----------------------------------------------------------------------------*/
/* get phase                                                                  */
/*----------------------------------------------------------------------------*/
eTP Tx::ePhase(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  return m_ePhase;
}

/*----------------------------------------------------------------------------*/
/* set phase                                                                  */
/*----------------------------------------------------------------------------*/
void Tx::vPhase(eTP ePhase)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  m_ePhase = ePhase;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Tx::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  vDump(CN_TX, this, aSingleTab());
}

/*----------------------------------------------------------------------------*/
/* dump dictionary                                                            */
/*----------------------------------------------------------------------------*/
void Tx::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  ASSERT(eClass == CN_TX);
  ASSERT(pObject->eClass() == CN_TX);
  
  Tx* pTx = reinterpret_cast<Tx*>(pObject);
  
  vTrace("tx: %lld-%lld phase %s counter %lld\n"_Stz,
         pTx->m_iLocalTxId,
         pTx->m_iDistTxId,
         pTP[pTx->m_ePhase],
         pTx->iCounterGet());
}

/*----------------------------------------------------------------------------*/
/* is this a local tx?                                                        */
/*----------------------------------------------------------------------------*/
bool Tx::bLocalTx(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  if (m_iDistTxId == m_iLocalTxId)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this a distributed tx?                                                  */
/*----------------------------------------------------------------------------*/
bool Tx::bDistTx(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  if (m_iDistTxId != m_iLocalTxId)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* decode whether this is a local or distributed transaction                  */
/*----------------------------------------------------------------------------*/
Stz* Tx::aDecode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  if (bLocalTx())
    return "local"_Stz;
  else
    return "dist"_Stz;
}

/*----------------------------------------------------------------------------*/
/* get if distributed tx member                                               */
/*----------------------------------------------------------------------------*/
bool Tx::bDistMember(i8 i_Node)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  bool bDistMember = m_xNode[i_Node].m_bDistMember;
  
  return bDistMember;
}

/*----------------------------------------------------------------------------*/
/* set if distributed tx member                                               */
/*----------------------------------------------------------------------------*/
void Tx::vDistMember(i8 i_Node, bool bDistMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  m_xNode[i_Node].m_bDistMember = bDistMember;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Tx::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
