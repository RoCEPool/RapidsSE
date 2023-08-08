/*============================================================================*/
/* PipelineDef.cpp -- Pipeline Metadata                                      */
/*============================================================================*/

#include "PipelineDef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,PipelineDef,CN_PIPELINEDEF)
__aNewFunction(PipelineDef,CN_PIPELINEDEF)
__vDeleteFunction(PipelineDef,CN_PIPELINEDEF)
__aFooterFunction(PipelineDef)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PipelineDef::PipelineDef(Stz* szPipelineNm)
    : Object(CN_PIPELINEDEF)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINEDEF, _FOOTER_);

  vPipelineNm(szPipelineNm);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PIPELINEDEF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PipelineDef::~PipelineDef(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINEDEF,_FOOTER_);

  vDeleteAll();
}


/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PipelineDef::vCollapseOps(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINEDEF,_FOOTER_);

  RASSERT(StrLib::bIsSqlName(m_sPipelineNm.azChar()));

  i8 nStages = nItems();
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    StageDef* pStageDef = pGet(i_Stage);
    pStageDef->vCollapseOps();
  }

  vDebugPoint();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PipelineDef::vDump(i8 i_Tab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINEDEF,_FOOTER_);

  vTrace("%sPipelineDef: %s\n"_Stz,
         Stz::szTab(i_Tab),
         m_sPipelineNm.szStz());
  vTrace("%sFileNm: %s\n"_Stz,
         Stz::szTab(i_Tab+1),
         m_sFileNm.szStz());

  // - - - - - - - - - - - - - - - - - - - - - -

  i8 nStages = nItems();
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    StageDef* pStageDef = pGet(i_Stage);

    pStageDef->vDump(i_Tab+1);
  }

  vTrace("%sEnd PipelineDef\n"_Stz,
         Stz::szTab(i_Tab));

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Stz* PipelineDef::szPipelineNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINEDEF,_FOOTER_);

  return m_sPipelineNm;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PipelineDef::vPipelineNm(Stz* szPipelineNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINEDEF,_FOOTER_);

  m_sPipelineNm = szPipelineNm;

  File::vMakeTempNm(m_sFileNm,szPipelineNm);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PipelineDef::vValidate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINEDEF,_FOOTER_);

  RASSERT(StrLib::bIsSqlName(m_sPipelineNm.azChar()));

  i8 nStages = nItems();
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    StageDef* pStageDef = pGet(i_Stage);
    pStageDef->vValidate();
  }

  vDebugPoint();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
