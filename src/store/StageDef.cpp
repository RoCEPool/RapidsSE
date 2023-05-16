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
void StageDef::vDump(i8 i_Tab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGEDEF,_FOOTER_);

  vTrace("%sStageDef: [%lld] %s\n"_Stz,
         Stz::szTab(i_Tab),m_i_Stage,m_sStageNm.szStz());

  i8 nOps = nItems();
  for (i8 i_Stage = 0; i_Stage < nOps; i_Stage++) {
    OpDef* pOpDef = pGet(i_Stage);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
