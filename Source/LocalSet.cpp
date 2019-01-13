/*============================================================================*/
/* LocalSet.cpp -- Local Frame                                                */
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

  m_pNext     = nullptr;
  m_pStackPtr = nullptr;
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
/* get next register frame                                                    */
/*----------------------------------------------------------------------------*/
LocalSet* LocalSet::pNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSET,_FOOTER_);
  
  return m_pNext;
}

/*----------------------------------------------------------------------------*/
/* set next register frame                                                    */
/*----------------------------------------------------------------------------*/
void LocalSet::vNext(LocalSet* pNext)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSET,_FOOTER_);
  
  m_pNext = pNext;
}

/*----------------------------------------------------------------------------*/
/* get stack pointer                                                          */
/*----------------------------------------------------------------------------*/
void* LocalSet::pStackPtr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSET,_FOOTER_);
  
  return m_pStackPtr;
}

/*----------------------------------------------------------------------------*/
/* set stack pointer                                                          */
/*----------------------------------------------------------------------------*/
void LocalSet::vStackPtr(void* pStackPtr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSET,_FOOTER_);
  
  m_pStackPtr = pStackPtr;
}

/*----------------------------------------------------------------------------*/
/* get address of value register                                              */
/*----------------------------------------------------------------------------*/
Local* LocalSet::pLocal(i8 iReg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSET,_FOOTER_);
  
  ASSERT(iReg >= 0 && iReg < C_REGISTERS);
  
  return &m_xLocal[iReg];
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
