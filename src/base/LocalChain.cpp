/*============================================================================*/
/* LocalChain.cpp -- Locals Frame Chain                                       */
/*============================================================================*/

#include "BASE.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,LocalChain,CN_LOCALCHAIN)
__aNewFunction(LocalChain,CN_LOCALCHAIN)
__vDeleteFunction(LocalChain,CN_LOCALCHAIN)
__aFooterFunction(LocalChain)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LocalChain::LocalChain(void)
: Object(CN_LOCALCHAIN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALCHAIN,_FOOTER_);

  m_pNext         = nullptr;
  m_aDiscardPoint = nullptr;
  //m_xLocal
  
  _checkInitialised_(CN_LOCALCHAIN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOCALCHAIN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LocalChain::~LocalChain(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALCHAIN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump register frame                                                        */
/*----------------------------------------------------------------------------*/
void LocalChain::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  UNUSED(eClass);
  UNUSED(aTab);
  
  LocalChain* pLocalSet = _pLocalChain(pObject);
  UNUSED(pLocalSet);

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get return point where this local set should be discarded                  */
/*----------------------------------------------------------------------------*/
TypedCell* LocalChain::aDiscardPoint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALCHAIN,_FOOTER_);
  
  return m_aDiscardPoint;
}

/*----------------------------------------------------------------------------*/
/* set return point where this local set should be discarded                  */
/*----------------------------------------------------------------------------*/
void LocalChain::vDiscardPoint(TypedCell* aDiscardPoint)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALCHAIN,_FOOTER_);
  
  m_aDiscardPoint = aDiscardPoint;
}

/*----------------------------------------------------------------------------*/
/* dump the local chain                                                       */
/*----------------------------------------------------------------------------*/
void LocalChain::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALCHAIN,_FOOTER_);
  
  vTrace("LocalChain: %p discardPoint %p"_Stz,
         this,
         m_aDiscardPoint);
  
  if (m_pNext)
    m_pNext->vDump();
}

/*----------------------------------------------------------------------------*/
/* get given local                                                            */
/*----------------------------------------------------------------------------*/
Local* LocalChain::pLocal(i8 i_Local)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALCHAIN,_FOOTER_);
  
  ASSERT(i_Local >= 0 && i_Local < C_LOCALS_arguments);
  
  return &m_xLocal[i_Local];
}

/*----------------------------------------------------------------------------*/
/* get next set of locals                                                     */
/*----------------------------------------------------------------------------*/
LocalChain* LocalChain::pNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALCHAIN,_FOOTER_);
  
  return m_pNext;
}

/*----------------------------------------------------------------------------*/
/* set next set of locals                                                     */
/*----------------------------------------------------------------------------*/
void LocalChain::vNext(LocalChain* pNext)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALCHAIN,_FOOTER_);
  
  m_pNext = pNext;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
