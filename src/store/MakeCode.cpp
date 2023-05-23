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

  m_pPipelineDef   = nullptr;
  m_pStageDef      = nullptr;
  m_pOpDef         = nullptr;
  m_pArgDef        = nullptr;

  m_pOpDef_leading = nullptr;
  m_pColsReq       = nullptr;

  _checkInitialised_(CN_MAKECODE);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MakeCode::~MakeCode(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);

  delete m_pPipelineDef;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* retrieve m_pPipelineDef                                                    */
/*----------------------------------------------------------------------------*/
PipelineDef* MakeCode::pPipelineDef(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);

  return m_pPipelineDef;
}

/*----------------------------------------------------------------------------*/
/* store m_pPipelineDef                                                       */
/*----------------------------------------------------------------------------*/
void MakeCode::vPipelineDef(PipelineDef* pPipelineDef)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);

  m_pPipelineDef = pPipelineDef;
}


/*----------------------------------------------------------------------------*/
/* call synthesiser                                                           */
/*----------------------------------------------------------------------------*/
void MakeCode::vSynthesise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);

  Board* pBoard = m_pCI->m_pBench->pGetBoard((i8)0);

  vCraftPipelineDef(pBoard);

  if (AM::JOB::bHasTag(C_WEAVER2)) {
    eEC eError = Stencil_v2::eSynthesise(m_pPipelineDef);
    UNUSED(eError);
  }

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

  m_pPipelineDef = new PipelineDef(sQueryNm);

  Kid xBoard(pBoard->pDeck());
  while (xBoard.bEach()) {

    if (xBoard.pChild()->szCardNm()->bSame("stage"_Stz)) {
      m_pOpDef_leading = nullptr;
      m_pColsReq = nullptr;
      vCraftStageDef(xBoard.p_stage());
    }
    else {
      RASSERT(m_pStageDef);
      //vTrace("Op %s\n"_Stz,xBoard.pChild()->szCardNm());
      if (! m_pOpDef_leading) {
        m_pOpDef_leading = m_pOpDef;
        m_pColsReq = new ColsReq;
        m_pOpDef_leading->vColsReq(m_pColsReq);
      }
      vCraftOpDef(xBoard.p_OP());
    }

  }
}

/*----------------------------------------------------------------------------*/
/* craft stage def                                                            */
/*----------------------------------------------------------------------------*/
void MakeCode::vCraftStageDef(Card_Stage* p_stage)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);

  m_pStageDef = new StageDef(m_pPipelineDef->nItems(),
                             p_stage->szSymbol(),
                             p_stage);
  m_pPipelineDef->vAppend(m_pStageDef);
}

/*----------------------------------------------------------------------------*/
/* craft op def                                                               */
/*----------------------------------------------------------------------------*/
void MakeCode::vCraftOpDef(Card_OP* p_OP)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);

  eCK eCard = p_OP->eCard();
  if (CK_COMBINED == eCard)
    return;

  if (CK_CAST == eCard) {
    //vTrace("CAST: %s\n"_Stz,p_OP->szSymbol());
    if (!p_OP->szSymbol()->bSame("hashCast"_Stz))
      return;
  }

  m_pOpDef = new OpDef(m_pStageDef->nItems(),p_OP->szCardNm());
  m_pStageDef->vAppend(m_pOpDef);

  vCraftStreamDefs(p_OP);
  vCraftIndexDefs(p_OP);

  vCraftArgDefs(p_OP);

  vUpdateColsReq(p_OP);

}

/*----------------------------------------------------------------------------*/
/* craft arg def                                                              */
/*----------------------------------------------------------------------------*/
void MakeCode::vCraftArgDefs(Card_OP* p_OP)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);

  s2K sExpr;

  eCK eCard = p_OP->eCard();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  switch (eCard) {

    case CK_AGGREG:       p_OP->vRpnAggreg      (sExpr, _pCard_Aggreg      (p_OP)); break;
    case CK_CAST:         break;
    //case CK_COMBINED:     p_OP->vRpnCombined    (sExpr, _pCard_Combined    (p_OP)); break;
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
      m_pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    case CK_CAST:
      m_pOpDef->vAppend(new ArgDef(0, "topology"_Stz, _pCard_Cast(p_OP)->szSymbol()));
      break;

//    case CK_COMBINED:
//      break;

    case CK_CROSSJOIN:
      m_pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_CrossJoin(p_OP)->pSub_tableNm()->szSymbol()));
      m_pOpDef->vAppend(new ArgDef(1, "indexNm"_Stz, _pCard_CrossJoin(p_OP)->pSub_tableNm()->szSymbol()));
      m_pOpDef->vAppend(new ArgDef(2, "expr"_Stz, sExpr));
      break;

    case CK_FILTER:
      m_pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    case CK_GBYAGG:
      m_pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    case CK_HASH: {
      Card_HshCol *p_hshCol = _pCard_Hash(p_OP)->pSub_hshCol();
      m_pOpDef->vAppend(new ArgDef(0, "type"_Stz,  aCT[Symbol::eTo_eCT_char(p_hshCol->pSymbol()->Y_szGet(P_Type))]));
      m_pOpDef->vAppend(new ArgDef(1, "column"_Stz,  p_hshCol->szSymbol()));
    }
      break;

    case CK_HAVING:
      m_pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    case CK_INDEXJOIN:
      m_pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_IndexJoin(p_OP)->pSub_tableNm()->szSymbol()));
      m_pOpDef->vAppend(new ArgDef(1, "indexNm"_Stz, _pCard_IndexJoin(p_OP)->pSub_tableNm()->szSymbol()));
      m_pOpDef->vAppend(new ArgDef(2, "expr"_Stz, sExpr));
      break;

    case CK_INDEXSCAN:
      m_pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_IndexScan(p_OP)->pSub_tableNm()->szSymbol()));
      m_pOpDef->vAppend(new ArgDef(1, "indexNm"_Stz, _pCard_IndexScan(p_OP)->pSub_tableNm()->szSymbol()));
      m_pOpDef->vAppend(new ArgDef(2, "expr"_Stz, sExpr));
      break;

    case CK_KEYEXISTS:
      m_pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_KeyExists(p_OP)->pSub_tableNm()->szSymbol()));
      m_pOpDef->vAppend(new ArgDef(1, "indexNm"_Stz, _pCard_KeyExists(p_OP)->pSub_tableNm()->szSymbol()));
      m_pOpDef->vAppend(new ArgDef(2, "expr"_Stz, sExpr));
      break;

    case CK_KEYNOTEXISTS:
      m_pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_KeyNotExists(p_OP)->pSub_tableNm()->szSymbol()));
      m_pOpDef->vAppend(new ArgDef(1, "indexNm"_Stz, _pCard_KeyNotExists(p_OP)->pSub_tableNm()->szSymbol()));
      m_pOpDef->vAppend(new ArgDef(2, "expr"_Stz, sExpr));
      break;

    case CK_LIMIT:
      m_pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    case CK_ORDAGG:
      m_pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    case CK_PROJECT:
      m_pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    case CK_SCAN:
      m_pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_Scan(p_OP)->pSub_tableNm()->szSymbol()));
      break;

    case CK_SORT:
      m_pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    case CK_WHERE:
      m_pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    default:
      vTrace("Missing Case [%s:%lld] : %lld %s\n"_Stz,
             __FILE__, (i8)__LINE__,(i8) eCard, aCK[eCard]);
      NEVER_REACH;
  }

  return;
}

/*----------------------------------------------------------------------------*/
/* craft index def's                                                          */
/*----------------------------------------------------------------------------*/
void MakeCode::vCraftIndexDefs(Card_OP* p_OP)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);

  eCK eCard = p_OP->eCard();

  if (CK_INDEXJOIN == eCard)
    m_pOpDef->m_pIndexDef_join   = p_OP->pCraftIndexDef_join();

  if (CK_GBYAGG == eCard)
    m_pOpDef->m_pIndexDef_gbyAgg = p_OP->pCraftIndexDef_gbyAgg();
}

/*----------------------------------------------------------------------------*/
/* craft stream def's                                                         */
/*----------------------------------------------------------------------------*/
void MakeCode::vCraftStreamDefs(Card_OP* p_OP)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);

  m_pOpDef->m_pStreamDef_arriving = p_OP->pCraftStreamDef_arriving();
  m_pOpDef->m_pStreamDef_emitted  = p_OP->pCraftStreamDef_emitted();
}

/*----------------------------------------------------------------------------*/
/* update the current cols required                                           */
/*----------------------------------------------------------------------------*/
void MakeCode::vUpdateColsReq(Card_OP* p_OP)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);

  eCK eCard = p_OP->eCard();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  switch (eCard) {

    case CK_AGGREG:
    case CK_CROSSJOIN:
    case CK_FILTER:
    case CK_GBYAGG:
    case CK_HAVING:
    case CK_INDEXJOIN:
    case CK_INDEXSCAN:
    case CK_KEYEXISTS:
    case CK_KEYNOTEXISTS:
    case CK_ORDAGG:
    case CK_PROJECT:
    case CK_SCAN:
      return;

    default:
      return;
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
