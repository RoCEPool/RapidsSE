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
  pPipelineDef->vDump();


  Kid xBoard(pBoard->pDeck());
  while (xBoard.bEach()) {

    if (xBoard.pChild()->szCardNm()->bSame("stage"_Stz)) {

      pStageDef = pCraftStageDef((Card_OP*)(xBoard.pChild()));

      pStageDef->vStage(pPipelineDef->nItems());
      pPipelineDef->vAppend(pStageDef);

      pStageDef->vDump();
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


  pOpDef->vDump();

  return pOpDef;
}

/*----------------------------------------------------------------------------*/
/* craft arg def                                                              */
/*----------------------------------------------------------------------------*/
void MakeCode::vCraftArgDefs(Card_OP* p_OP, OpDef* pOpDef)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);

  return;
  s2K sArg;

  ArgDef* pArgDef = new ArgDef;

  eCK eCard = p_OP->eCard();

  switch (eCard) {

    case CK_AGGREG:       p_OP->vRpnAggreg   (sArg, _pCard_Aggreg(p_OP));    break;
    case CK_CAST:         break;
    case CK_COMBINED:     p_OP->vRpnCombined (sArg, _pCard_Combined(p_OP));  break;
    case CK_CROSSJOIN:    p_OP->vRpnCrossJoin(sArg, _pCard_CrossJoin(p_OP)); break;
    case CK_FILTER:       p_OP->vRpnFilter   (sArg, _pCard_Filter(p_OP));    break;
    case CK_GBYAGG:       p_OP->vRpnGbyAgg   (sArg, _pCard_GbyAgg(p_OP));    break;
    case CK_HASH:         break;
    case CK_HAVING:       p_OP->vRpnHaving   (sArg, _pCard_Having(p_OP));    break;
    case CK_INDEXJOIN:    p_OP->vRpnIndexJoin(sArg, _pCard_Index(p_OP));     break;
    case CK_INDEXSCAN:    p_OP->vRpnIndexScan(sArg, _pCard_Index(p_OP));     break;
    case CK_KEYEXISTS:    p_OP->vRpnKeyExists(sArg, _pCard_KeyExists(p_OP)); break;
    case CK_KEYNOTEXISTS: p_OP->vRpnKeyNotExists(sArg, _pCard_KeyNotExists(p_OP)); break;
    case CK_LIMIT:        p_OP->vRpnLimit    (sArg, _pCard_Limit(p_OP));     break;
    case CK_ORDAGG:       p_OP->vRpnOrdAgg   (sArg, _pCard_OrdAgg(p_OP));    break;
    case CK_PROJECT:      p_OP->vRpnProject  (sArg, _pCard_Project(p_OP));   break;
    case CK_SCAN:         p_OP->vRpnScan     (sArg, _pCard_Scan(p_OP));      break;
    case CK_SORT:         p_OP->vRpnSort     (sArg, _pCard_Sort(p_OP));      break;
    case CK_WHERE:        p_OP->vRpnWhere    (sArg, _pCard_Where(p_OP));     break;

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
