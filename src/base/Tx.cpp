/*============================================================================*/
/* Tx.cpp - Transaction                                                       */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,Tx,CN_TX)
__aNewFunction(Tx,CN_TX)
__vDeleteFunction(Tx,CN_TX)
__aFooterFunction(Tx)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Tx::Tx(void)
: Object(CN_TX)
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);

  // m_okLock
  m_tPerspective = AM::iNextTxId();
  m_ePhase       = XP_EXECUTING;
      
  _checkInitialised_(CN_TX);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Tx::~Tx(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
  UNUSED(eClass);
  UNUSED(aTab);

  Tx* pTx = _pTx(pObject);
  
  // get string of the distributed members of this transaction
  
  vTrace("tx: %lld phase %s\n"_Stz,
         pTx->tPerspective(),
         aXP[pTx->m_ePhase]
         );
}

/*----------------------------------------------------------------------------*/
/* repair the transaction                                                     */
/*----------------------------------------------------------------------------*/
void Tx::vRepair(Object* pObject)
{
  _static_

  Tx* pTx = _pTx(pObject);

  // delete the transaction
  delete pTx;
}

/*----------------------------------------------------------------------------*/
/* outline                                                                    */
/*----------------------------------------------------------------------------*/
void Tx::vOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  sOutline.vSprint("<%s focal>"_Stz,aCN[eGetClassNo()]);
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
/* get perspective                                                            */
/*----------------------------------------------------------------------------*/
t8 Tx::tPerspective(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  return m_tPerspective;
}

/*----------------------------------------------------------------------------*/
/* set perspective                                                            */
/*----------------------------------------------------------------------------*/
void Tx::vPerspective(t8 tPerspective)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TX,_FOOTER_);
  
  m_tPerspective = tPerspective;
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
