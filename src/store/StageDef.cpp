/*============================================================================*/
/* StageDef.cpp -- Stage Metadata                                            */
/*============================================================================*/

#include "StageDef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,StageDef,CN_STAGEDEF)
__aNewFunction(StageDef,CN_STAGEDEF)
__vDeleteFunction(StageDef,CN_STAGEDEF)
__aFooterFunction(StageDef)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StageDef::StageDef(void)
    : Object(CN_STAGEDEF)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGEDEF, _FOOTER_);

  m_i_Stage = C_UNKNOWN;
  m_pCard_Stage = nullptr;

  _postcondition_(_ADDRESS_,_MAGIC_,CN_STAGEDEF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StageDef::StageDef(i8 i_Stage, Stz* szStageNm, Card_Stage* p_stage)
    : Object(CN_STAGEDEF)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGEDEF, _FOOTER_);

  vStage(i_Stage);
  vStageNm(szStageNm);
  m_sCode = ""_Stz;

  vCard_Stage(p_stage);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_STAGEDEF,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StageDef::~StageDef(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGEDEF,_FOOTER_);

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
Card_Stage* StageDef::pCard_Stage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGEDEF,_FOOTER_);

  return m_pCard_Stage;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void StageDef::vCard_Stage(Card_Stage* pCard_Stage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGEDEF,_FOOTER_);

  m_pCard_Stage = pCard_Stage;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Stz* StageDef::szCode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGEDEF,_FOOTER_);

  return m_sCode;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void StageDef::vCode(Stz* sCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGEDEF,_FOOTER_);

  m_sCode = sCode;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void StageDef::vCollapseOps(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGEDEF,_FOOTER_);

  i8 nOps = nItems();
  for (i8 i_Op = 0; i_Op < nOps - 1; i_Op++) {
    OpDef* pOpDef_curr = pGet(i_Op);
    OpDef* pOpDef_next = pGet(i_Op + 1);

    if (pOpDef_curr->m_sOpNm.bSame("scan"_Stz) &&
        pOpDef_next->m_sOpNm.bSame("project"_Stz)) {

//      pOpDef_next->m_bCollapsed = true;
//      pOpDef_curr->m_nCollapses++;
    }

  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void StageDef::vDump(i8 i_Tab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGEDEF,_FOOTER_);

  vTrace("%sStageDef: [%lld] %s (Code %p)\n"_Stz,
         Stz::szTab(i_Tab),m_i_Stage,m_sStageNm.szStz(),
         *szCode()->azChar() ? szCode() : nullptr);

  i8 nOps = nItems();
  for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
    OpDef* pOpDef = pGet(i_Op);
    pOpDef->vDump();
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 StageDef::i_Stage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGEDEF,_FOOTER_);

  return m_i_Stage;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void StageDef::vStage(i8 i_Stage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGEDEF,_FOOTER_);

  m_i_Stage = i_Stage;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Stz* StageDef::szStageNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGEDEF,_FOOTER_);

  return m_sStageNm;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void StageDef::vStageNm(Stz* szStageNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGEDEF,_FOOTER_);

  m_sStageNm = szStageNm;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void StageDef::vValidate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGEDEF,_FOOTER_);

  RASSERT(StrLib::bIsSqlName(m_sStageNm.azChar()));

  i8 nOps = nItems();
  for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
    OpDef* pOpDef = pGet(i_Op);
    pOpDef->vValidate();
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
