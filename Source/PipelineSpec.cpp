/*============================================================================*/
/* DynamicSpec.cpp -- Dynamic Specification                                   */
/*============================================================================*/

#include "Dynamics.h"


/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* DynamicSpec::operator new(size_t)
{
  DynamicSpec* pDynamicSpec = reinterpret_cast<DynamicSpec*>(HeapMem::pAllocate(CN_PIPELINESPEC));
  
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
: Object(CN_PIPELINESPEC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  m_sName = szName;
  
  m_pStageSpecVec = new StageSpecVec;
  
  _checkInitialised_(CN_PIPELINESPEC);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
DynamicSpec::~DynamicSpec(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  delete m_pStageSpecVec;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add stage spec                                                             */
/*----------------------------------------------------------------------------*/
void DynamicSpec::vAddStageSpec(StageSpec* pStageSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  m_pStageSpecVec->vAdd(pStageSpec);
}

/*----------------------------------------------------------------------------*/
/* add stage spec                                                             */
/*----------------------------------------------------------------------------*/
Stz* DynamicSpec::szName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  return m_sName.aStz();
}

/*----------------------------------------------------------------------------*/
/* find stage matching given name                                             */
/*----------------------------------------------------------------------------*/
StageSpec* DynamicSpec::pFindStageSpec(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  for (i8 i_Stage = 0; i_Stage < m_pStageSpecVec->nItems(); i_Stage++) {
    StageSpec* pStageSpec = m_pStageSpecVec->pGet(i_Stage);
    
    if (pStageSpec->szName()->bSame(szName))
      return pStageSpec;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
StageSpec* DynamicSpec::pStageSpec(i8 i_Stage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  StageSpec* pStageSpec = m_pStageSpecVec->pGet(i_Stage);
  
  return pStageSpec;
}

/*----------------------------------------------------------------------------*/
/* get stage spec vec                                                          */
/*----------------------------------------------------------------------------*/
StageSpecVec* DynamicSpec::pStageSpecVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  return m_pStageSpecVec;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print dynamic spec                                                         */
/*----------------------------------------------------------------------------*/
void DynamicSpec::vPrint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  vTrace("dynamic\n"_Stz);
  vTrace("\n"_Stz);
  vTrace("pipeline %s\n"_Stz,m_sName.aStz());
  vTrace("begin\n"_Stz);
  
  for (i8 i_Stage = 0; i_Stage < m_pStageSpecVec->nItems(); i_Stage++)
    m_pStageSpecVec->pGet(i_Stage)->vPrint();

  vTrace("\n"_Stz);
  vTrace("end\n"_Stz);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(DynamicSpec)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
