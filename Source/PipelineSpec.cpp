/*============================================================================*/
/* PipelineSpec.cpp -- Dynamic Specification                                   */
/*============================================================================*/

#include "Pipelines.h"


/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* PipelineSpec::operator new(size_t)
{
  PipelineSpec* pPipelineSpec = reinterpret_cast<PipelineSpec*>(HeapMem::pAllocate(CN_PIPELINESPEC));
  
  return pPipelineSpec;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void PipelineSpec::operator delete(void* aAddr)
{
  PipelineSpec* pPipelineSpec = reinterpret_cast<PipelineSpec*>(aAddr);
  
  HeapMem::vFree(pPipelineSpec);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PipelineSpec::PipelineSpec(Stz* szName)
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
PipelineSpec::~PipelineSpec(void) noexcept(false)
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
void PipelineSpec::vAddStageSpec(StageSpec* pStageSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  m_pStageSpecVec->vAdd(pStageSpec);
}

/*----------------------------------------------------------------------------*/
/* add stage spec                                                             */
/*----------------------------------------------------------------------------*/
Stz* PipelineSpec::szName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  return m_sName.aStz();
}

/*----------------------------------------------------------------------------*/
/* find stage matching given name                                             */
/*----------------------------------------------------------------------------*/
StageSpec* PipelineSpec::pFindStageSpec(Stz* szName)
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
StageSpec* PipelineSpec::pStageSpec(i8 i_Stage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  StageSpec* pStageSpec = m_pStageSpecVec->pGet(i_Stage);
  
  return pStageSpec;
}

/*----------------------------------------------------------------------------*/
/* get stage spec vec                                                          */
/*----------------------------------------------------------------------------*/
StageSpecVec* PipelineSpec::pStageSpecVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  return m_pStageSpecVec;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print dynamic spec                                                         */
/*----------------------------------------------------------------------------*/
void PipelineSpec::vPrint(void)
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
__aFooterFunction(PipelineSpec)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
