/*============================================================================*/
/* StageDef.cpp -- Stage Metadata                                            */
/*============================================================================*/

#include "StageDef.hpp"
#include "Card_Topology.hpp"

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
StageDef::StageDef(i8 i_Stage, Stz* szStageNm, Card_Stage* p_stage)
    : AbstractDef(CN_STAGEDEF)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGEDEF, _FOOTER_);

  vStage(i_Stage);
  vStageNm(szStageNm);

  m_pCode = nullptr;

  vCard_Stage(p_stage);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_STAGEDEF,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StageDef::~StageDef(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGEDEF,_FOOTER_);

  delete m_pCode;

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

  if (! m_pCode)
    return nullptr;

  return m_pCode->szTextBlock(0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void StageDef::vCode(Stz* szCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGEDEF,_FOOTER_);

  if (! m_pCode)
    m_pCode = new ExtendedText;

  m_pCode->vCopy(szCode);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
ExtendedText* StageDef::pCode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGEDEF,_FOOTER_);

  return m_pCode;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void StageDef::vCode(ExtendedText* pCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGEDEF,_FOOTER_);

  m_pCode = pCode;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void StageDef::vDump(i8 i_Tab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGEDEF,_FOOTER_);

  Card_Topology* p_topology = m_pCard_Stage->pSub_topology();

  vTrace("%sStageDef: [%lld] %s Code %p (%s)\n"_Stz,
         Stz::szTab(i_Tab),
         m_i_Stage,
         m_sStageNm.szStz(),
         szCode(),

         aYG[p_topology->pSymbol()->Y_iGet(P_StageTopo)]
  );

  // - - - - - - - - - - - - - - - - - - - - - -

  i8 nOps = nItems();
  for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
    OpDef* pOpDef = pGet(i_Op);

    pOpDef->vDump(i_Tab+1);
  }

}

/*----------------------------------------------------------------------------*/
/* does this stage have a code block?                                         */
/*----------------------------------------------------------------------------*/
bool StageDef::bHasCode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGEDEF,_FOOTER_);

  if (m_pCode && * m_pCode->szTextBlock(0)->azChar())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get stage subscript                                                        */
/*----------------------------------------------------------------------------*/
i8 StageDef::i_Stage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGEDEF,_FOOTER_);

  return m_i_Stage;
}

/*----------------------------------------------------------------------------*/
/* set stage subscript                                                        */
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
