/*============================================================================*/
/* LocalChain.cpp -- Locals Frame Chain                                       */
/*============================================================================*/

#include "SenVM.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
LocalChain* SenVM::_pLocalSet(void* aAddr)
{
  LocalChain* pLocalSet = reinterpret_cast<LocalChain*>(aAddr);
  
  RASSERT_TYPE(pLocalSet,CN_LOCALSCHAIN);

  return pLocalSet;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* LocalChain::operator new(size_t)
{
  void* pLocalSet = HeapMem::pAllocate(CN_LOCALSCHAIN);
  
  return pLocalSet;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void LocalChain::operator delete(void* pLocalSetAddress)
{
  LocalChain* pLocalSet = _pLocalSet(pLocalSetAddress);
  
  HeapMem::vFree(pLocalSet);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LocalChain::LocalChain(void)
: Object(CN_LOCALSCHAIN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSCHAIN,_FOOTER_);

  m_pNext         = nullptr;
  m_aDiscardPoint = nullptr;
  //m_xLocal
  
  _checkInitialised_(CN_LOCALSCHAIN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOCALSCHAIN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LocalChain::~LocalChain(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSCHAIN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump register frame                                                        */
/*----------------------------------------------------------------------------*/
void LocalChain::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  LocalChain* pLocalSet = _pLocalSet(pObject);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSCHAIN,_FOOTER_);
  
  return m_pNext;
}

/*----------------------------------------------------------------------------*/
/* set next set of locals                                                     */
/*----------------------------------------------------------------------------*/
void LocalChain::vNext(LocalChain* pNext)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSCHAIN,_FOOTER_);
  
  m_pNext = pNext;
}

/*----------------------------------------------------------------------------*/
/* get return point where this local set should be discarded                  */
/*----------------------------------------------------------------------------*/
TypedCell* LocalChain::aDiscardPoint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSCHAIN,_FOOTER_);
  
  return m_aDiscardPoint;
}

/*----------------------------------------------------------------------------*/
/* set return point where this local set should be discarded                  */
/*----------------------------------------------------------------------------*/
void LocalChain::vDiscardPoint(TypedCell* aDiscardPoint)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSCHAIN,_FOOTER_);
  
  m_aDiscardPoint = aDiscardPoint;
}

/*----------------------------------------------------------------------------*/
/* get given local                                                            */
/*----------------------------------------------------------------------------*/
Local* LocalChain::pLocal(i8 i_Local)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSCHAIN,_FOOTER_);
  
  ASSERT(i_Local >= 0 && i_Local < C_LOCALS);
  
  return &m_xLocal[i_Local];
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(LocalChain)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
