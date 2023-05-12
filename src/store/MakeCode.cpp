/*============================================================================*/
/* MakeCode.cpp -- Make Simpler                                            */
/*============================================================================*/

#include "MakeCode.hpp"

#include "Bench.hpp"
#include "Board.hpp"
#include "Optimiser.hpp"
#include "PipelineDef.hpp"
#include "Stencil_v2.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, MakeCode, CN_MAKECODE)
__aNewFunction(MakeCode, CN_MAKECODE)
__vDeleteFunction(MakeCode, CN_MAKECODE)
__aFooterFunction(MakeCode)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MakeCode::MakeCode(CardIter* pCI)
: Object(CN_MAKECODE)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);

  m_pCI = pCI;

  _checkInitialised_(CN_MAKECODE);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MakeCode::~MakeCode(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* call synthesiser                                                           */
/*----------------------------------------------------------------------------*/
void MakeCode::vSynthesise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);

  Board* pBoard = m_pCI->m_pBench->pGetBoard((i8)0);

  PipelineDef*  pPipelineDef = pCraftPipelineDef(pBoard);


  eEC eError = Stencil_v2::eSynthesise(pPipelineDef);
  UNUSED(eError);
  delete pPipelineDef;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* craft pipeline def                                                         */
/*----------------------------------------------------------------------------*/
PipelineDef* MakeCode::pCraftPipelineDef(Board* pBoard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);

  s32 sQueryNm = pBoard->pBench()->pOptimiser()->szQueryNm();

  StageDef* pStageDef = nullptr;
  OpDef*    pOpDef    = nullptr;
  ArgDef*   pArgDef   = nullptr;

  PipelineDef* pPipelineDef = new PipelineDef;

  pPipelineDef->vPipelineNm(sQueryNm);
  //vTrace("Pipeline %s\n"_Stz,sQueryNm.szStz());


  Kid xBoard(pBoard->pDeck());
  while (xBoard.bEach()) {

    if (xBoard.pChild()->szCardNm()->bSame("stage"_Stz)) {
      //vTrace("Stage %s\n"_Stz,xBoard.pChild()->szCardNm());

      pStageDef = pCraftStageDef((Card_OP*)(xBoard.pChild()));

      pStageDef->vStage(pPipelineDef->nItems());
      pPipelineDef->vAppend(pStageDef);
    }
    else {
      RASSERT(pStageDef);
      //vTrace("Op %s\n"_Stz,xBoard.pChild()->szCardNm());

      pOpDef = pCraftOpDef((Card_OP*)(xBoard.pChild()));

      pOpDef->vOp(pStageDef->nItems());
      pStageDef->vAppend(pOpDef);
    }

  }

  return pPipelineDef;
}

/*----------------------------------------------------------------------------*/
/* craft stage def                                                            */
/*----------------------------------------------------------------------------*/
StageDef* MakeCode::pCraftStageDef(Card_OP* p_OP)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);

  StageDef* pStageDef = new StageDef;

  pStageDef->vStageNm(p_OP->szCardNm());

  return pStageDef;
}

/*----------------------------------------------------------------------------*/
/* craft op def                                                               */
/*----------------------------------------------------------------------------*/
OpDef* MakeCode::pCraftOpDef(Card_OP* p_OP)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);

  OpDef* pOpDef = new OpDef;

  pOpDef->vOpNm(p_OP->szCardNm());

  vCraftArgDefs(p_OP,pOpDef);

  return pOpDef;
}

/*----------------------------------------------------------------------------*/
/* craft arg def                                                              */
/*----------------------------------------------------------------------------*/
void MakeCode::vCraftArgDefs(Card_OP* p_OP, OpDef* pOpDef)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);

  s2K sArg;
  return;

  ArgDef* pArgDef = new ArgDef;

  eCK eCard = p_OP->eCard();

  switch (eCard) {

    case CK_ABSJOIN:      p_OP->vCraftArg(sArg); break;
    case CK_AGGREG:       p_OP->vCraftArg(sArg); break;
    case CK_CAST:         p_OP->vCraftArg(sArg); break;
    case CK_COMBINED:     p_OP->vCraftArg(sArg); break;
    case CK_CROSSJOIN:    p_OP->vCraftArg(sArg); break;
    case CK_FILTER:       p_OP->vCraftArg(sArg); break;
    case CK_GBYAGG:       p_OP->vCraftArg(sArg); break;
    case CK_HASH:         p_OP->vCraftArg(sArg); break;
    case CK_HAVING:       p_OP->vCraftArg(sArg); break;
    case CK_INDEXJOIN:    p_OP->vCraftArg(sArg); break;
    case CK_INDEXSCAN:    p_OP->vCraftArg(sArg); break;
    case CK_KEYEXISTS:    p_OP->vCraftArg(sArg); break;
    case CK_KEYNOTEXISTS: p_OP->vCraftArg(sArg); break;
    case CK_LIMIT:        p_OP->vCraftArg(sArg); break;
    case CK_ORDAGG:       p_OP->vCraftArg(sArg); break;
    case CK_PROJECT:      p_OP->vCraftArg(sArg); break;
    case CK_SCAN:         p_OP->vCraftArg(sArg); break;
    case CK_SORT:         p_OP->vCraftArg(sArg); break;
    case CK_WHERE:        p_OP->vCraftArg(sArg); break;

    default:
      vTrace("Missing Case [%s:%lld] : %lld %s\n"_Stz,
             __FILE__, (i8)__LINE__,(i8) eCard, aCK[eCard]);
      NEVER_REACH;
  }

  return;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
