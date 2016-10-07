/*============================================================================*/
/* TxMap.cpp -- Transaction Map                                               */
/*============================================================================*/

#include "Transactions.h"

extern TxMap*        gpTxMap;
extern TxCollection* gpTxCollection;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TxMap* Transactions::_pTxMap(void* pAddr)
{
  TxMap* pTxMap = reinterpret_cast<TxMap*>(pAddr);
  
  return pTxMap;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TxMap::operator new(size_t iSize, TxMap* pTxMap)
{
  RASSERT(iSize);
  return pTxMap;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TxMap::operator delete(void* pBufferAddress)
{
  RASSERT(pBufferAddress);
  vFatal("fatal: TxMap class cannot be deleted");
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TxMap::TxMap(void)
: Object(CN_TXMAP)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXMAP,_FOOTER_);
  
  m_xLock.vCheckInit();
  m_iSlot = -1;
  m_iId   = -1;
  m_pTxArray  = new TxArray;
  
  gpTxMap = this;
  
  _checkInitialised_(CN_TXMAP);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TXMAP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TxMap::~TxMap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXMAP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* begin a tx                                                                 */
/*----------------------------------------------------------------------------*/
Tx* TxMap::pBeginTx(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXMAP,_FOOTER_);
  
  // allocate a transaction object
  Tx* pTx = gpTxCollection->pAllocTx();

  // too many?
  if (!pTx)
    return nullptr;

  // map it
  m_xLock.vGrabWrite();
  
  // find free slot
  vFindSlot();
  
  // take it
  vTakeSlot(pTx);
  
  m_xLock.vDropWrite();
  
  return pTx;
}

/*----------------------------------------------------------------------------*/
/* end a tx                                                                   */
/*----------------------------------------------------------------------------*/
void TxMap::vEndTx(Tx* pTx)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXMAP,_FOOTER_);
  
  ASSERT(TP_COMMITTED == pTx->ePhase() || TP_ROLLEDBACK == pTx->ePhase());
  pTx->vPhase(TP_NONE);
  
  m_xLock.vGrabWrite();
  
  vGiveSlot(pTx);
  
  m_xLock.vDropWrite();
  
  gpTxCollection->vFreeTx(pTx);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* give slot                                                                  */
/*----------------------------------------------------------------------------*/
void TxMap::vGiveSlot(Tx* pTx)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXMAP,_FOOTER_);
  
  i8 iSlot = pTx->iTxSlot();
  
  ASSERT(pTx == m_pTxArray->m_pTx[iSlot]);
  
  m_pTxArray->m_pTx[iSlot] = nullptr;
}

/*----------------------------------------------------------------------------*/
/* advance to next slot,id (modulo slot arround the array)                    */
/*----------------------------------------------------------------------------*/
void TxMap::vFindSlot(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXMAP,_FOOTER_);
  
  m_iSlot++;
  m_iId++;
  
  while (m_pTxArray->m_pTx[m_iSlot]) {
    
    // slot wasnt free, so try next one
    m_iSlot++;
    m_iId++;
  
    // modulo arround the array
    if (m_iSlot >= C_TX_ARRAY)
      m_iSlot = 0;
  }
}

/*----------------------------------------------------------------------------*/
/* take slot                                                                  */
/*----------------------------------------------------------------------------*/
void TxMap::vTakeSlot(Tx* pTx)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXMAP,_FOOTER_);
  
  pTx->vTxSlot(m_iSlot);
  pTx->vTxId(m_iId);

  m_pTxArray->m_pTx[m_iSlot] = pTx;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* TxMap::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXMAP);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/