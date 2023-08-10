/*============================================================================*/
/* XF_Synthesise_Code.cpp -- Make Simpler                                            */
/*============================================================================*/

#include "XF_Synthesise_Code.hpp"

#include "Bench.hpp"
#include "Board.hpp"
#include "Optimiser.hpp"
#include "PipelineDef.hpp"
#include "Stencil_v2.hpp"

#include "Symbol.hpp"
#include "Table.hpp"
#include "StreamDef.hpp"

#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Synthesise_Code, CN_XF_SYNTHESISE_CODE)
__aNewFunction(XF_Synthesise_Code, CN_XF_SYNTHESISE_CODE)
__vDeleteFunction(XF_Synthesise_Code, CN_XF_SYNTHESISE_CODE)
__aFooterFunction(XF_Synthesise_Code)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Synthesise_Code::XF_Synthesise_Code(CardIter* pCI)
: Object(CN_XF_SYNTHESISE_CODE)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_CODE, _FOOTER_);

  m_pCI = pCI;

  m_pPipelineDef = nullptr;
  m_pStageDef    = nullptr;
  m_pOpDef       = nullptr;

  m_pStreamDef_prior = nullptr;

  _checkInitialised_(CN_XF_SYNTHESISE_CODE);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_CODE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Synthesise_Code::~XF_Synthesise_Code(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_CODE, _FOOTER_);

  delete m_pPipelineDef;
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_Code::vXF_Make_Code(CardIter* pCI)
{
  _static_

  XF_Synthesise_Code* pXF_Make_Code = new XF_Synthesise_Code(pCI);
  pXF_Make_Code->vSynthesise();

  PipelineDef* pPipelineDef = pXF_Make_Code->pPipelineDef();
  pXF_Make_Code->vPipelineDef(nullptr);
  pCI->m_pBench->pOptimiser()->vPipelineDef(pPipelineDef);

  delete pXF_Make_Code;

  pCI->m_pBench->vMadeCode(true);
}





/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* retrieve m_pPipelineDef                                                    */
/*----------------------------------------------------------------------------*/
PipelineDef* XF_Synthesise_Code::pPipelineDef(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_CODE, _FOOTER_);

  return m_pPipelineDef;
}

/*----------------------------------------------------------------------------*/
/* store m_pPipelineDef                                                       */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_Code::vPipelineDef(PipelineDef* pPipelineDef)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_CODE, _FOOTER_);

  m_pPipelineDef = pPipelineDef;
}


/*----------------------------------------------------------------------------*/
/* call synthesiser                                                           */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_Code::vSynthesise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_CODE, _FOOTER_);

  Board* pBoard = m_pCI->m_pBench->pGetBoard((i8)0);

  vCraftPipelineDef(pBoard);

  if (AM::JOB::bHasTag(C_NOCODESYNTHESIS))
    return;

  eEC eError = Stencil_v2::eSynthesise(m_pPipelineDef);
  UNUSED(eError);

}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* craft pipeline def                                                         */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_Code::vCraftPipelineDef(Board* pBoard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_CODE, _FOOTER_);

  s32 sQueryNm = pBoard->pBench()->pOptimiser()->szQueryNm();

  m_pPipelineDef = new PipelineDef(sQueryNm);

  Kid xDeck(pBoard->pDeck());
  while (xDeck.bEach()) {

    if (xDeck.pChild()->szCardNm()->bSame("stage"_Stz)) {
      vCraftStageDef(xDeck.p_stage());
    }
    else {
      RASSERT(m_pStageDef);
      vCraftOpDef(xDeck.p_OP());
    }

  }

  m_pPipelineDef->vCollapseOps();
  m_pPipelineDef->vValidate();

  if (AM::JOB::bHasTag(C_PIPELINE_DEF)) {
    m_pPipelineDef->vDump();
    vDebugPoint();
  }
}

/*----------------------------------------------------------------------------*/
/* craft stage def                                                            */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_Code::vCraftStageDef(Card_Stage* p_stage)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_CODE, _FOOTER_);

  m_pStageDef = new StageDef(m_pPipelineDef->nItems(),
                             p_stage->szSymbol(SF_NONE),
                             p_stage);
  m_pPipelineDef->vAppend(m_pStageDef);
}

/*----------------------------------------------------------------------------*/
/* craft op def                                                               */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_Code::vCraftOpDef(Card_OP* p_OP)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_CODE, _FOOTER_);

  if (p_OP->bIsCard(CK_KEYEXISTS))
    vDebugPoint();

  if (p_OP->bIsOne(CK_CAST,CK_COMBINED,CK_SEND,CK_NONE))
    return;

       if (p_OP->bIsCard(CK_HAVING))
    m_pOpDef = new OpDef(m_pStageDef->nItems(),"filter"_Stz,p_OP->szBearing());
  else if (p_OP->bIsCard(CK_HASH))
    m_pOpDef = new OpDef(m_pStageDef->nItems(),"hash"_Stz,p_OP->szBearing());
  else if (p_OP->bIsCard(CK_TABLE))
    m_pOpDef = new OpDef(m_pStageDef->nItems(),"scan"_Stz,p_OP->szBearing());
  else if (p_OP->bIsCard(CK_WHERE))
    m_pOpDef = new OpDef(m_pStageDef->nItems(),"filter"_Stz,p_OP->szBearing());
  else
    m_pOpDef = new OpDef(m_pStageDef->nItems(),p_OP->szCardNm(),p_OP->szBearing());

  m_pStageDef->vAppend(m_pOpDef);

  vCraftStreamDefs(p_OP);
  vCraftIndexes(p_OP);
  vCraftIndexDefs(p_OP);
  vCraftArgDefs(p_OP);
  vUpdateColsReq(p_OP);
}

/*----------------------------------------------------------------------------*/
/* craft arg def                                                              */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_Code::vCraftArgDefs(Card_OP* p_OP)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_CODE, _FOOTER_);

  s2K sExpr;
  s2K sConstraint;

  eCK eCard = p_OP->eCard();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  switch (eCard) {

    case CK_AGGREG:       p_OP->vRpnAggreg      (sExpr, _pCard_Aggreg      (p_OP)); break;
    case CK_CAST:         break;
    case CK_COMBINED:     break;
    case CK_CROSSJOIN:    p_OP->vRpnCrossJoin   (sExpr, _pCard_CrossJoin   (p_OP)); break;
    case CK_FILTER:       p_OP->vRpnFilter      (sExpr, _pCard_Filter      (p_OP)); break;
    case CK_GBYAGG:       p_OP->vRpnGbyAgg      (sExpr, _pCard_GbyAgg      (p_OP)); break;
    case CK_HASH:         p_OP->vRpnHash        (sExpr, _pCard_Hash        (p_OP)); break;
    case CK_HAVING:       p_OP->vRpnHaving      (sExpr, _pCard_Having      (p_OP)); break;
    case CK_INDEXJOIN:    p_OP->vRpnIndexJoin   (sExpr, _pCard_IndexJoin   (p_OP)); break;
    case CK_INDEXSCAN:    p_OP->vRpnIndexScan   (sExpr, _pCard_IndexScan   (p_OP)); break;
    case CK_KEYEXISTS:    p_OP->vRpnKeyExists   (sExpr, _pCard_KeyExists   (p_OP)); break;
    case CK_KEYNOTEXISTS: p_OP->vRpnKeyNotExists(sExpr, _pCard_KeyNotExists(p_OP)); break;
    case CK_LIMIT:        p_OP->vRpnLimit       (sExpr, _pCard_Limit       (p_OP)); break;
    case CK_ORDAGG:       p_OP->vRpnOrdAgg      (sExpr, _pCard_OrdAgg      (p_OP)); break;
    case CK_PROJECT:      p_OP->vRpnProject     (sExpr, _pCard_Project     (p_OP)); break;
    case CK_SCAN:         p_OP->vRpnScan        (sExpr, _pCard_Scan        (p_OP)); break;
    case CK_SEND:         break;
    case CK_SORT:         p_OP->vRpnSort        (sExpr, _pCard_Sort        (p_OP)); break;
    case CK_TABLE:        break;
    case CK_TOP:          p_OP->vRpnTop         (sExpr, _pCard_Top         (p_OP)); break;
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
      m_pOpDef->vAppend(new ArgDef(0, "topology"_Stz, _pCard_Cast(p_OP)->szSymbol(SF_NONE)));
      break;

//    case CK_COMBINED:
//      break;

    case CK_CROSSJOIN:
      m_pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_CrossJoin(p_OP)->pSub_tableNm()->szSymbol(SF_NONE)));
      m_pOpDef->vAppend(new ArgDef(1, "indexNm"_Stz, _pCard_CrossJoin(p_OP)->pSub_tableNm()->szSymbol(SF_NONE)));
      m_pOpDef->vAppend(new ArgDef(2, "expr"_Stz, sExpr));
      break;

    case CK_FILTER:
      m_pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    case CK_GBYAGG:
      m_pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    case CK_HASH: {
      m_pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
    }
      break;

    case CK_HAVING:
      m_pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    case CK_INDEXJOIN:
      m_pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_IndexJoin(p_OP)->pSub_tableNm()->szSymbol(SF_NONE)));
      m_pOpDef->vAppend(new ArgDef(1, "indexNm"_Stz, _pCard_IndexJoin(p_OP)->pSub_tableNm()->szSymbol(SF_NONE)));
      m_pOpDef->vAppend(new ArgDef(2, "expr"_Stz, sExpr));

      if (p_OP->pSub_constraint()) {
        p_OP->vRpnIndexJoin_constraint(sConstraint, _pCard_IndexJoin(p_OP));
        m_pOpDef->vAppend(new ArgDef(4, "constraint"_Stz, sConstraint));
      }
      break;

    case CK_INDEXSCAN:
      m_pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_IndexScan(p_OP)->pSub_tableNm()->szSymbol(SF_NONE)));
      m_pOpDef->vAppend(new ArgDef(1, "indexNm"_Stz, _pCard_IndexScan(p_OP)->pSub_tableNm()->szSymbol(SF_NONE)));
      m_pOpDef->vAppend(new ArgDef(2, "expr"_Stz, sExpr));
      break;

    case CK_KEYEXISTS:
      p_OP->vRpnKeyExists_constraint(sConstraint, _pCard_KeyExists(p_OP));

      m_pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_KeyExists(p_OP)->pSub_tableNm()->szSymbol(SF_NONE)));
      m_pOpDef->vAppend(new ArgDef(1, "indexNm"_Stz, _pCard_KeyExists(p_OP)->pSub_indexNm()->szSymbol(SF_NONE)));
      m_pOpDef->vAppend(new ArgDef(2, "expr"_Stz, sExpr));
      m_pOpDef->vAppend(new ArgDef(3, "constraint"_Stz, sConstraint));
      break;

    case CK_KEYNOTEXISTS:
      p_OP->vRpnKeyNotExists_constraint(sConstraint, _pCard_KeyNotExists(p_OP));

      m_pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_KeyNotExists(p_OP)->pSub_tableNm()->szSymbol(SF_NONE)));
      m_pOpDef->vAppend(new ArgDef(1, "indexNm"_Stz, _pCard_KeyNotExists(p_OP)->pSub_indexNm()->szSymbol(SF_NONE)));
      m_pOpDef->vAppend(new ArgDef(2, "expr"_Stz, sExpr));
      m_pOpDef->vAppend(new ArgDef(3, "constraint"_Stz, sConstraint));
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
      m_pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_Scan(p_OP)->pSub_tableNm()->szSymbol(SF_NONE)));
      break;

    case CK_SEND:
      break;

    case CK_SORT:
      m_pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    case CK_TABLE:
      m_pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_Table(p_OP)->pSub_tableNm()->szSymbol(SF_NONE)));
      break;

    case CK_TOP:
      m_pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      m_pOpDef->vAppend(new ArgDef(0, "count"_Stz, _pCard_Top(p_OP)->pSub_count()->szSymbol(SF_NONE)));
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
/* craft indexes                                                              */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_Code::vCraftIndexes(Card_OP* p_OP)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_CODE, _FOOTER_);

  eCK eCard = p_OP->eCard();

  if (CK_CROSSJOIN == eCard)
    m_pOpDef->m_pIndex_indexJoin   = p_OP->pCraftIndex_indexJoin();
  else
  if (CK_INDEXJOIN == eCard)
    m_pOpDef->m_pIndex_indexJoin   = p_OP->pCraftIndex_indexJoin();
  else
  if (CK_INDEXSCAN == eCard)
    m_pOpDef->m_pIndex_indexScan   = p_OP->pCraftIndex_indexScan();
  else
  if (CK_KEYEXISTS == eCard)
    m_pOpDef->m_pIndex_indexSemi   = p_OP->pCraftIndex_indexScan();
  else
  if (CK_KEYNOTEXISTS == eCard)
    m_pOpDef->m_pIndex_indexSemi   = p_OP->pCraftIndex_indexScan();
}

/*----------------------------------------------------------------------------*/
/* craft index def's                                                          */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_Code::vCraftIndexDefs(Card_OP* p_OP)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_CODE, _FOOTER_);

  eCK eCard = p_OP->eCard();

  if (CK_GBYAGG == eCard)
    m_pOpDef->m_pIndexDef_gbyAgg = p_OP->pCraftIndexDef_gbyAgg();
  else
  if (CK_SORT == eCard)
    m_pOpDef->m_pIndexDef_sort = p_OP->pCraftIndexDef_sort();
  else
  if (CK_TOP == eCard)
    m_pOpDef->m_pIndexDef_top = p_OP->pCraftIndexDef_top();
}

/*----------------------------------------------------------------------------*/
/* craft stream def's                                                         */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_Code::vCraftStreamDefs(Card_OP* p_OP)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_CODE, _FOOTER_);

  if (m_pStreamDef_prior)
    m_pOpDef->m_pStreamDef_arriving = m_pStreamDef_prior->pDup();

  ColMap** ppColMap = nullptr;
  if (p_OP->bIsCard(CK_GBYAGG))
    ppColMap = &m_pOpDef->m_pColMap_gbyAgg;
  else
  if (p_OP->bIsCard(CK_SORT))
    ppColMap = &m_pOpDef->m_pColMap_sort;
  else
  if (p_OP->bIsCard(CK_TOP))
    ppColMap = &m_pOpDef->m_pColMap_top;

  m_pOpDef->m_pStreamDef_emitted = p_OP->pCraftStreamDef_emitted(ppColMap);

  if (! m_pOpDef->m_pStreamDef_emitted && p_OP->bIsCard(CK_HASH))
    m_pOpDef->m_pStreamDef_emitted = m_pOpDef->m_pStreamDef_arriving->pDup();

  if (m_pOpDef->m_pStreamDef_emitted)
    m_pStreamDef_prior = m_pOpDef->m_pStreamDef_emitted;
}

/*----------------------------------------------------------------------------*/
/* update the current cols required                                           */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_Code::vUpdateColsReq(Card_OP* p_OP)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_CODE, _FOOTER_);

  eCK eCard_OP = p_OP->eCard();

  Card_Table* p_table = nullptr;

  switch (eCard_OP) {

    case CK_CROSSJOIN:
    case CK_INDEXJOIN:
    case CK_INDEXSCAN:
    case CK_KEYEXISTS:
    case CK_KEYNOTEXISTS:
    case CK_SCAN:
      p_table = p_OP->pSub_table();
       break;

    case CK_TABLE:
      p_table = _pCard_Table(p_OP);
      break;

    default:
      return;
  }

  RASSERT(p_table->pSymbol()->bIs_Tbl_Syn() || p_table->pSymbol()->bIs_Tbl_Syn_rep());


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  ColsReq* pColsReq = new ColsReq;
  m_pOpDef->vColsReq(pColsReq);

  i8 nUpdates = 0;
  Kid xKid(m_pCI->G.pDeck);
  for (xKid.vSynch(p_OP); xKid.bEach(); /**/) {
    Card* p_later = xKid.pChild();
    eCK eCard_later = p_later->eCard();

    switch (eCard_later) {

      case CK_FILTER:
        vUpdateColsReq_filter(p_table, pColsReq, _pCard_Filter(p_later));
        nUpdates++;
        continue;

      case CK_PROJECT:
        vUpdateColsReq_project(p_table, pColsReq, _pCard_Project(p_later));
        nUpdates++;
        continue;

      default:
        continue;
    }

  }

  if (! nUpdates) {
    if (CK_SCAN == eCard_OP) {
      vUpdateColsReq_Scan(p_table, pColsReq, _pCard_Scan(p_OP));
    }
    else
    if (CK_INDEXJOIN == eCard_OP) {
      vUpdateColsReq_IndexJoin(p_table, pColsReq, _pCard_IndexJoin(p_OP));
    }
    else {
      //NEVER_REACH
    }
  }

  pColsReq->vValidate();
}

/*----------------------------------------------------------------------------*/
/* update the current cols required                                           */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_Code::vUpdateColsReq_filter(Card_Table* p_table,
                                               ColsReq* pColsReq,
                                               Card_Filter* p_filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_CODE, _FOOTER_);

  Card_BoolCalc* p_boolCalc = p_filter->pSub_boolCalc();
  vUpdateColsReq_terms(p_table, pColsReq,p_boolCalc);
}

/*----------------------------------------------------------------------------*/
/* update the current cols required                                           */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_Code::vUpdateColsReq_project(Card_Table* p_table,
                                                ColsReq* pColsReq,
                                                Card_Project* p_project)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_CODE, _FOOTER_);

  Card_Projection* p_projection = p_project->pSub_projection();
  vUpdateColsReq_terms(p_table, pColsReq,p_projection);
}

/*----------------------------------------------------------------------------*/
/* update the current cols required                                           */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_Code::vUpdateColsReq_Scan(Card_Table* p_table,
                                             ColsReq* pColsReq,
                                             Card_Scan* p_scan)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_CODE, _FOOTER_);

//  Card_Projection* p_projection = p_scan->pSub_projection();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambda = [pColsReq] (Card* pCard) {
    Card_Expr* p_expr = _pCard_Expr(pCard);

    Symbol* pSymbol_tblCol = p_expr->pFirstTblCol();

    i8 i_Col = pSymbol_tblCol->iSF_ordinal();

    pColsReq->vAddCol(i_Col, false, i_Col);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  p_scan->bApplyRecursive(CK_EXPR,bLambda);

}

/*----------------------------------------------------------------------------*/
/* update the current cols required                                           */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_Code::vUpdateColsReq_IndexJoin(Card_Table* p_table,
                                                  ColsReq* pColsReq,
                                                  Card_IndexJoin* p_indexJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_CODE, _FOOTER_);

//  Card_Projection* p_projection = p_indexJoin->pSub_projection();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambda = [p_table, pColsReq] (Card* pCard) {
    Card_Expr* p_expr = _pCard_Expr(pCard);

    Symbol* pSymbol_tblCol = p_expr->pFirstTblCol();
    if (! pSymbol_tblCol)
      return C_CONTINUE_RECURSION;

    Stz* szTblCol = pSymbol_tblCol->szSF_symbol(SF_COL_TC);
    if (! p_table->bIsTblColOf(szTblCol))
      return C_CONTINUE_RECURSION;

    i8 i_Col = StrLib::iGleanInt(szTblCol->azChar());

    pColsReq->vAddCol(i_Col, false, i_Col);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  p_indexJoin->bApplyRecursive(CK_EXPR,bLambda);

}

/*----------------------------------------------------------------------------*/
/* update the current cols required                                           */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_Code::vUpdateColsReq_terms(Card_Table* p_table,
                                              ColsReq* pColsReq,
                                              Card* pSubCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_CODE, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambda = [p_table, pColsReq] (Card_Term* p_term) {

    if (!p_term->pSymbol()->bIs_Col_No())
      return C_CONTINUE_RECURSION;

    if (! p_term->pSymbol()->Y_bHas(P_TblCol))
      return C_CONTINUE_RECURSION;

    Symbol* pSymbol = p_term->pSymbol();
    if (! p_table->bIsTblColOf(pSymbol->Y_szGet(P_TblCol)))
      return C_CONTINUE_RECURSION;

    Stz* szTblCol = pSymbol->Y_szGet(P_TblCol);
    i8 i_Col = StrLib::iGleanInt(szTblCol->azChar());

    s32 sColNm;
    if (Table* pTable = _pTable(p_table->pSymbol()->Y_pGet(P_Pointer))) {
      StreamDef* pStreamDef = pTable->pStreamDef();
      Column* pColumn = pStreamDef->pGetColumn(i_Col-1);

      sColNm.vSprint("C%lld_%s"_Stz,
                     i_Col-1,
                     pColumn->szColNm());

    }
    else
      sColNm = "??"_Stz;

    pColsReq->vAddCol(i_Col, false, i_Col);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pSubCard->bApplyRecursive_term(bLambda);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
