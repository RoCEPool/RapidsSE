/*============================================================================*/
/* LocalChain.cpp -- Locals Frame Chain                                       */
/*============================================================================*/

#include "BASE.h"


/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
LocalChain* BASE::_pLocalSet(void* aAddr)
{
  LocalChain* pLocalSet = reinterpret_cast<LocalChain*>(aAddr);
  
  RASSERT_TYPE(pLocalSet,CN_LOCALCHAIN);

  return pLocalSet;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* LocalChain::operator new(size_t)
{
  void* pLocalSet = Heaps::pAllocate(CN_LOCALCHAIN);
  
  return pLocalSet;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void LocalChain::operator delete(void* pLocalSetAddress)
{
  LocalChain* pLocalSet = _pLocalSet(pLocalSetAddress);
  
  Heaps::vFree(pLocalSet);
}

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
  
  LocalChain* pLocalSet = _pLocalSet(pObject);
  UNUSED(pLocalSet);

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
/* get given local                                                            */
/*----------------------------------------------------------------------------*/
Local* LocalChain::pLocal(i8 i_Local)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALCHAIN,_FOOTER_);
  
  ASSERT(i_Local >= 0 && i_Local < C_LOCALS_arguments);
  
  return &m_xLocal[i_Local];
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(LocalChain)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
