/*============================================================================*/
/* Tx.cpp - Transaction                                                       */
/*============================================================================*/

#include "Transactions.h"
#include "AM.hpp"

Atomic gixTx;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Tx* Transactions::_pTx(void* aAddr)
{
  Tx* pTx = reinterpret_cast<Tx*>(aAddr);
  
  RASSERT_TYPE(pTx,CN_TX);
  
  return pTx;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Tx::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_TXSET);

  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Tx::operator new(size_t, Tx* pTx)
{
  return pTx;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Tx::operator delete(void* pTxAddress)
{
  Tx* pTx = reinterpret_cast<Tx*>(pTxAddress);
  
  HeapMem::vFree(pTx);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Tx::Tx(bool bAddToSet)
: Focal(CN_TX,AM::iNextTxId(),C_UNKNOWN)
, m_okLock(LK_TX_SET,this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);

  // m_okLock
  m_tPerspective = ++ gixTx;
  m_ePhase       = XP_EXECUTING;
  
  m_iLocalTxId = -1;
  m_iDistTxId  = -1;
  m_ixCounter  = AM::nSlotsPerNode();
  
  for (i8 i_Node = 0; i_Node < C_MAX_NODES; i_Node++)
    m_xNode[i_Node].m_bDistMember = false;
  
  _checkInitialised_(CN_TX);

  if (bAddToSet)
    AM::pTxSet()->vAddLocalTx(this);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Tx::Tx(i8 iDistTxId)
: Focal(CN_TX,AM::iNextTxId(),C_UNKNOWN)
, m_okLock(LK_TX_SET,this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  // m_okLock
  m_tPerspective = ++ gixTx;
  m_ePhase       = XP_EXECUTING;
  
  m_iLocalTxId = -1;
  m_iDistTxId  = iDistTxId;
  m_ixCounter  = AM::nSlotsPerNode();

  for (i8 i_Node = 0; i_Node < C_MAX_NODES; i_Node++)
    m_xNode[i_Node].m_bDistMember = false;
  
  _checkInitialised_(CN_TX);
  
  AM::pTxSet()->vAddDistTx_NL(this);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Tx::~Tx(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  if (-1 != m_iLocalTxId)
    AM::pTxSet()->vRemoveTx(this);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* attempt to find a tx by dist tx id, create if not found                    */
/*----------------------------------------------------------------------------*/
Tx* Tx::pTestDistTx(i8 iDistTxId)
{
  Tx* pDistTx = nullptr;
  
  TxSet* pTxSet = AM::pTxSet();
  
  pTxSet->vGrabWrite();
  
  Tx* pTxFound = pTxSet->pFindTx_NL(iDistTxId);
  
  if (pTxFound) {
    pTxSet->vDropWrite();
    return pTxFound;
  }
  
  pDistTx = new Tx(iDistTxId);
  
  pTxSet->vDropWrite();
  
  return pDistTx;
}

/*----------------------------------------------------------------------------*/
/* get tx no of given tx id                                                   */
/*----------------------------------------------------------------------------*/
i8 Tx::iTxNo(i8 iTxId)
{
  i8 iMaskOffUpper16Bits = 0x0000FFFFFFFFFFFF;
  
  i8 iTxNo = iTxId & iMaskOffUpper16Bits;
  
  return iTxNo;
}

/*----------------------------------------------------------------------------*/
/* get tx node no of given tx id                                              */
/*----------------------------------------------------------------------------*/
i8 Tx::iTxNode(i8 iTxId)
{
  i8 iShiftRight48Bits = 48;
  
  i8 iTxNode = iTxId >> iShiftRight48Bits;
  
  return iTxNode;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* focal outline                                                              */
/*----------------------------------------------------------------------------*/
void Tx::vFocalOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  sOutline.vSprint("<%s focal>"_Stz,aCN[eClass()]);
}

/*----------------------------------------------------------------------------*/
/* decrement atmoic counter                                                   */
/*----------------------------------------------------------------------------*/
i8 Tx::iCounterDec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  i8 iCounter = --m_ixCounter;
  
  return iCounter;
}

/*----------------------------------------------------------------------------*/
/* get general counter                                                        */
/*----------------------------------------------------------------------------*/
i8 Tx::iCounterGet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  return *m_ixCounter;
}

/*----------------------------------------------------------------------------*/
/* set general counter                                                        */
/*----------------------------------------------------------------------------*/
void Tx::vCounterSet(i8 iCounter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  m_ixCounter = iCounter;
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
/* get local tx no                                                            */
/*----------------------------------------------------------------------------*/
i8 Tx::iLocalTxNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  i8 iMaskOffUpper16Bits = 0x0000FFFFFFFFFFFF;
  
  i8 iTxNo = m_iLocalTxId & iMaskOffUpper16Bits;
  
  return iTxNo;
}

/*----------------------------------------------------------------------------*/
/* get local tx node no                                                       */
/*----------------------------------------------------------------------------*/
i8 Tx::iLocalTxNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  i8 iMaskOffLower48Bits = 0xFFFF000000000000;
  
  i8 iTxNode = m_iLocalTxId & iMaskOffLower48Bits;
  iTxNode >>= 48;
  
  return iTxNode;
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
/* get dist tx no                                                             */
/*----------------------------------------------------------------------------*/
i8 Tx::iDistTxNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  i8 iMaskOffUpper16Bits = 0x0000FFFFFFFFFFFF;
  
  i8 iTxNo = m_iDistTxId & iMaskOffUpper16Bits;
  
  return iTxNo;
}

/*----------------------------------------------------------------------------*/
/* get dist tx node no                                                        */
/*----------------------------------------------------------------------------*/
i8 Tx::iDistTxNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  i8 iMaskOffLower48Bits = 0xFFFF000000000000;
  
  i8 iTxNode = m_iDistTxId & iMaskOffLower48Bits;
  iTxNode >>= 48;
  
  return iTxNode;
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
/* get perspective                                                            */
/*----------------------------------------------------------------------------*/
t8 Tx::tPerspective(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  return m_tPerspective;
}

/*----------------------------------------------------------------------------*/
/* get phase                                                                  */
/*----------------------------------------------------------------------------*/
eXP Tx::ePhase(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  return m_ePhase;
}

/*----------------------------------------------------------------------------*/
/* set phase                                                                  */
/*----------------------------------------------------------------------------*/
void Tx::vPhase(eXP ePhase)
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
  
  vDump(CN_TX, this, Stz::szTab(1));
}

/*----------------------------------------------------------------------------*/
/* dump transaction                                                           */
/*----------------------------------------------------------------------------*/
void Tx::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Tx* pTx = _pTx(pObject);
  
  // get string of the distributed members of this transaction
  char caMembers[C_64KB] = "";
  if (pTx->bLocalTx()) {
    StrLib::vConcat(caMembers,"{");
    for (i8 i_Node = 0; i_Node < C_MAX_NODES; i_Node++)
      if (pTx->bDistMember(i_Node))
        StrLib::vSprintCat((Stz*)caMembers," %lld"_Stz,i_Node);
    StrLib::vConcat(caMembers," }");
  }
  
  vTrace("tx: %lld.%lld-%lld.%lld phase %s counter %lld %s\n"_Stz,
         pTx->iLocalTxNode(), pTx->iLocalTxNo(),
         pTx->iDistTxNode(), pTx->iDistTxNo(),  
         aXP[pTx->m_ePhase],
         pTx->iCounterGet(),
         caMembers);
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
/* is this an active transaction?                                             */
/*----------------------------------------------------------------------------*/
bool Tx::bIsActive(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  if (XP_EXECUTING   == m_ePhase ||
      XP_COMMITTING  == m_ePhase ||
      XP_PREPARING   == m_ePhase ||
      XP_ROLLINGBACK == m_ePhase)
    return true;
  else
    return false;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Tx);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
