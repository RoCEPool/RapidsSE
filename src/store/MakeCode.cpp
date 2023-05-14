/*============================================================================*/
/* MakeCode.cpp -- Make Simpler                                            */
/*============================================================================*/

#include "MakeCode.hpp"

#include "Bench.hpp"
#include "Board.hpp"
#include "Optimiser.hpp"
#include "PipelineDef.hpp"
#include "Stencil_v2.hpp"

#include "Symbol.hpp"

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

  m_pPipelineDef = nullptr;
  m_pStageDef    = nullptr;
  m_pOpDef       = nullptr;
  m_pArgDef      = nullptr;

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

  vCraftPipelineDef(pBoard);


  eEC eError = Stencil_v2::eSynthesise(m_pPipelineDef);
  UNUSED(eError);
  delete m_pPipelineDef;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* craft pipeline def                                                         */
/*----------------------------------------------------------------------------*/
void MakeCode::vCraftPipelineDef(Board* pBoard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);

  s32 sQueryNm = pBoard->pBench()->pOptimiser()->szQueryNm();

  StageDef* pStageDef = nullptr;
  OpDef*    pOpDef    = nullptr;
  ArgDef*   pArgDef   = nullptr;

  m_pPipelineDef = new PipelineDef;

  m_pPipelineDef->vPipelineNm(sQueryNm);


  Kid xBoard(pBoard->pDeck());
  while (xBoard.bEach()) {

    if (xBoard.pChild()->szCardNm()->bSame("stage"_Stz)) {

      pStageDef = pCraftStageDef((Card_OP*)(xBoard.pChild()));

      pStageDef->vStage(m_pPipelineDef->nItems());
      m_pPipelineDef->vAppend(pStageDef);
    }
    else {
      RASSERT(pStageDef);
      //vTrace("Op %s\n"_Stz,xBoard.pChild()->szCardNm());

      pOpDef = pCraftOpDef((Card_OP*)(xBoard.pChild()));

      pOpDef->vOp(pStageDef->nItems());
      pStageDef->vAppend(pOpDef);
    }

  }

  m_pPipelineDef->vDump();
}

/*----------------------------------------------------------------------------*/
/* craft stage def                                                            */
/*----------------------------------------------------------------------------*/
StageDef* MakeCode::pCraftStageDef(Card_OP* p_OP)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);

  StageDef* pStageDef = new StageDef;

  pStageDef->vStageNm(p_OP->szSymbol());

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

  s2K sExpr;

  ArgDef* pArgDef;

  eCK eCard = p_OP->eCard();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  switch (eCard) {

    case CK_AGGREG:       p_OP->vRpnAggreg      (sExpr, _pCard_Aggreg      (p_OP)); break;
    case CK_CAST:         break;
    case CK_COMBINED:     p_OP->vRpnCombined    (sExpr, _pCard_Combined    (p_OP)); break;
    case CK_CROSSJOIN:    p_OP->vRpnCrossJoin   (sExpr, _pCard_CrossJoin   (p_OP)); break;
    case CK_FILTER:       p_OP->vRpnFilter      (sExpr, _pCard_Filter      (p_OP)); break;
    case CK_GBYAGG:       p_OP->vRpnGbyAgg      (sExpr, _pCard_GbyAgg      (p_OP)); break;
    case CK_HASH:         break;
    case CK_HAVING:       p_OP->vRpnHaving      (sExpr, _pCard_Having      (p_OP)); break;
    case CK_INDEXJOIN:    p_OP->vRpnIndexJoin   (sExpr, _pCard_IndexJoin   (p_OP)->pSub_index()); break;
    case CK_INDEXSCAN:    p_OP->vRpnIndexScan   (sExpr, _pCard_IndexScan   (p_OP)->pSub_index()); break;
    case CK_KEYEXISTS:    p_OP->vRpnKeyExists   (sExpr, _pCard_KeyExists   (p_OP)); break;
    case CK_KEYNOTEXISTS: p_OP->vRpnKeyNotExists(sExpr, _pCard_KeyNotExists(p_OP)); break;
    case CK_LIMIT:        p_OP->vRpnLimit       (sExpr, _pCard_Limit       (p_OP)); break;
    case CK_ORDAGG:       p_OP->vRpnOrdAgg      (sExpr, _pCard_OrdAgg      (p_OP)); break;
    case CK_PROJECT:      p_OP->vRpnProject     (sExpr, _pCard_Project     (p_OP)); break;
    case CK_SCAN:         p_OP->vRpnScan        (sExpr, _pCard_Scan        (p_OP)); break;
    case CK_SORT:         p_OP->vRpnSort        (sExpr, _pCard_Sort        (p_OP)); break;
    case CK_WHERE:        p_OP->vRpnWhere       (sExpr, _pCard_Where       (p_OP)); break;

    default:
      vTrace("Missing Case [%s:%lld] : %lld %s\n"_Stz,
             __FILE__, (i8)__LINE__,(i8) eCard, aCK[eCard]);
      NEVER_REACH;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  switch (eCard) {

    case CK_AGGREG:
      pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    case CK_CAST:
      pOpDef->vAppend(new ArgDef(0, "topology"_Stz, _pCard_Cast(p_OP)->szSymbol()));
      break;

    case CK_COMBINED:
      break;

    case CK_CROSSJOIN:
      pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_CrossJoin(p_OP)->pSub_tableNm()->szSymbol()));
      pOpDef->vAppend(new ArgDef(1, "indexNm"_Stz, _pCard_CrossJoin(p_OP)->pSub_tableNm()->szSymbol()));
      pOpDef->vAppend(new ArgDef(2, "expr"_Stz, sExpr));
      break;

    case CK_FILTER:
      pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    case CK_GBYAGG:
      pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    case CK_HASH: {
      Card_HshCol *p_hshCol = _pCard_Hash(p_OP)->pSub_hshCol();
      pOpDef->vAppend(new ArgDef(0, "type"_Stz,  aCT[Card::eTo_eCT_char(p_hshCol->pSymbol()->Y_szGet(P_Type))]));
      pOpDef->vAppend(new ArgDef(1, "column"_Stz,  p_hshCol->szSymbol()));
    }
      break;

    case CK_HAVING:
      pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    case CK_INDEXJOIN:
      pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_IndexJoin(p_OP)->pSub_tableNm()->szSymbol()));
      pOpDef->vAppend(new ArgDef(1, "indexNm"_Stz, _pCard_IndexJoin(p_OP)->pSub_tableNm()->szSymbol()));
      pOpDef->vAppend(new ArgDef(2, "expr"_Stz, sExpr));
      break;

    case CK_INDEXSCAN:
      pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_IndexScan(p_OP)->pSub_tableNm()->szSymbol()));
      pOpDef->vAppend(new ArgDef(1, "indexNm"_Stz, _pCard_IndexScan(p_OP)->pSub_tableNm()->szSymbol()));
      pOpDef->vAppend(new ArgDef(2, "expr"_Stz, sExpr));
      break;

    case CK_KEYEXISTS:
      pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_KeyExists(p_OP)->pSub_tableNm()->szSymbol()));
      pOpDef->vAppend(new ArgDef(1, "indexNm"_Stz, _pCard_KeyExists(p_OP)->pSub_tableNm()->szSymbol()));
      pOpDef->vAppend(new ArgDef(2, "expr"_Stz, sExpr));
      break;

    case CK_KEYNOTEXISTS:
      pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_KeyNotExists(p_OP)->pSub_tableNm()->szSymbol()));
      pOpDef->vAppend(new ArgDef(1, "indexNm"_Stz, _pCard_KeyNotExists(p_OP)->pSub_tableNm()->szSymbol()));
      pOpDef->vAppend(new ArgDef(2, "expr"_Stz, sExpr));
      break;

    case CK_LIMIT:
      pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    case CK_ORDAGG:
      pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    case CK_PROJECT:
      pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    case CK_SCAN:
      pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_Scan(p_OP)->pSub_tableNm()->szSymbol()));
      break;

    case CK_SORT:
      pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    case CK_WHERE:
      pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

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
