/*============================================================================*/
/* DevIXT.hpp -- IXT Device                                                   */
/*============================================================================*/

#include "Expressions.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Expressions,DevIXT,CN_DEVIXT)
__aNewFunction(DevIXT,CN_DEVIXT)
__vDeleteFunction(DevIXT,CN_DEVIXT)
__aFooterFunction(DevIXT)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
DevIXT::DevIXT(void)
: Device(CN_DEVIXT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DEVIXT,_FOOTER_);
  
  m_pIdxTree   = nullptr;
  
  m_aEnt       = nullptr;
  m_aUpperEnt  = nullptr;
  
  m_pEntBuffer = nullptr;

  m_aCrw       = nullptr;
  m_pCrwBuffer = nullptr;
  
  m_i_Row      = C_UNKNOWN;
  m_nRows      = C_UNKNOWN;

  _checkInitialised_(CN_DEVIXT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DEVIXT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
DevIXT::~DevIXT(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DEVIXT,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compare ent key with entwrk key                                            */
/*----------------------------------------------------------------------------*/
eKC DevIXT::e_ENT_COMPARE(LogicUnit* pLogicUnit)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DEVIXT,_FOOTER_);
  
  Key* aEntKey = m_aEnt->aKey();
  Key* aWrkKey = pLogicUnit->pEntWrk()->aKey();
  
  eKC eCmp = Key::eCompare(aEntKey,aWrkKey);
  
  return eCmp;
}

/*----------------------------------------------------------------------------*/
/* dump index tree                                                            */
/*----------------------------------------------------------------------------*/
void DevIXT::v_IXT_DUMP(LogicUnit* pLogicUnit)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DEVIXT,_FOOTER_);
  
  m_pIdxTree->vDump(false);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
