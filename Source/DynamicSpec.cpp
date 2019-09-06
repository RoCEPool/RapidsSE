/*============================================================================*/
/* DynamicSpec.cpp -- Dynamic Specification                                   */
/*============================================================================*/

#include "Dynamics.h"


/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* DynamicSpec::operator new(size_t)
{
  DynamicSpec* pDynamicSpec = reinterpret_cast<DynamicSpec*>(HeapMem::pAllocate(CN_DYNAMICSPEC));
  
  return pDynamicSpec;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void DynamicSpec::operator delete(void* aAddr)
{
  DynamicSpec* pDynamicSpec = reinterpret_cast<DynamicSpec*>(aAddr);
  
  HeapMem::vFree(pDynamicSpec);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
DynamicSpec::DynamicSpec(Stz* szName)
: Object(CN_DYNAMICSPEC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMICSPEC,_FOOTER_);
  
  m_sName = szName;
  
  m_pStageSpecVec = new StageSpecVec;
  
  _checkInitialised_(CN_DYNAMICSPEC);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DYNAMICSPEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
DynamicSpec::~DynamicSpec(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMICSPEC,_FOOTER_);
  
  delete m_pStageSpecVec;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add op spec                                                                */
/*----------------------------------------------------------------------------*/
void DynamicSpec::vAddStageSpec(StageSpec* pStageSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  m_pStageSpecVec->vAdd(pStageSpec);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(DynamicSpec)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
