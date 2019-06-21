/*============================================================================*/
/* LocalSet.cpp -- Local Set                                                  */
/*============================================================================*/

#include "Forth.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
LocalSet* Forth::_pLocalSet(void* aAddr)
{
  LocalSet* pLocalSet = reinterpret_cast<LocalSet*>(aAddr);
  
  RASSERT_TYPE(pLocalSet,CN_LOCALSET);

  return pLocalSet;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* LocalSet::operator new(size_t)
{
  void* pLocalSet = HeapMem::pAllocate(CN_LOCALSET);
  
  return pLocalSet;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void LocalSet::operator delete(void* pLocalSetAddress)
{
  LocalSet* pLocalSet = _pLocalSet(pLocalSetAddress);
  
  HeapMem::vFree(pLocalSet);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LocalSet::LocalSet(void)
: Object(CN_LOCALSET)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSET,_FOOTER_);

  m_pNext         = nullptr;
  m_aDiscardPoint = nullptr;
  //m_xLocal
  
  _checkInitialised_(CN_LOCALSET);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOCALSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LocalSet::~LocalSet(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump register frame                                                        */
/*----------------------------------------------------------------------------*/
void LocalSet::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  LocalSet* pLocalSet = _pLocalSet(pObject);
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get next set of locals                                                     */
/*----------------------------------------------------------------------------*/
LocalSet* LocalSet::pNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSET,_FOOTER_);
  
  return m_pNext;
}

/*----------------------------------------------------------------------------*/
/* set next set of locals                                                     */
/*----------------------------------------------------------------------------*/
void LocalSet::vNext(LocalSet* pNext)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSET,_FOOTER_);
  
  m_pNext = pNext;
}

/*----------------------------------------------------------------------------*/
/* get return point where this local set should be discarded                  */
/*----------------------------------------------------------------------------*/
ForthValue* LocalSet::aDiscardPoint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSET,_FOOTER_);
  
  return m_aDiscardPoint;
}

/*----------------------------------------------------------------------------*/
/* set return point where this local set should be discarded                  */
/*----------------------------------------------------------------------------*/
void LocalSet::vDiscardPoint(ForthValue* aDiscardPoint)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSET,_FOOTER_);
  
  m_aDiscardPoint = aDiscardPoint;
}

/*----------------------------------------------------------------------------*/
/* get given local                                                            */
/*----------------------------------------------------------------------------*/
Local* LocalSet::pLocal(i8 i_Local)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSET,_FOOTER_);
  
  ASSERT(i_Local >= 0 && i_Local < C_LOCALS);
  
  return &m_xLocal[i_Local];
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* LocalSet::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSET);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
