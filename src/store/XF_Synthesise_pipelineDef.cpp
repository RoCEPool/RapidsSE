/*============================================================================*/
/* XF_Synthesise_pipelineDef.cpp -- Synthesise PipelineDef                    */
/*============================================================================*/

#include "XF_Synthesise_pipelineDef.hpp"

#include "Bench.hpp"
#include "Board.hpp"
#include "Optimiser.hpp"
#include "PipelineDef.hpp"
#include "Stencil.hpp"

#include "Symbol.hpp"
#include "Table.hpp"
#include "StreamDef.hpp"

#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Synthesise_pipelineDef, CN_XF_SYNTHESISE_PIPELINEDEF)
__aNewFunction(XF_Synthesise_pipelineDef, CN_XF_SYNTHESISE_PIPELINEDEF)
__vDeleteFunction(XF_Synthesise_pipelineDef, CN_XF_SYNTHESISE_PIPELINEDEF)
__aFooterFunction(XF_Synthesise_pipelineDef)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Synthesise_pipelineDef::XF_Synthesise_pipelineDef(CardIter* pCI)
    : XF_SYNTHESISE(CN_XF_SYNTHESISE_PIPELINEDEF,pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_PIPELINEDEF, _FOOTER_);

  m_pCI  = pCI;

  m_pPipelineDef = nullptr;
  m_pStageDef    = nullptr;
  m_pOpDef       = nullptr;

  m_pStreamDef_prior = nullptr;

  _checkInitialised_(CN_XF_SYNTHESISE_PIPELINEDEF);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_PIPELINEDEF, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Synthesise_pipelineDef::~XF_Synthesise_pipelineDef(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_PIPELINEDEF, _FOOTER_);

  delete m_pPipelineDef;
  //RASSERT(! m_pStageDef);
  //RASSERT(! m_pOpDef);

}


/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_pipelineDef::vSynthesise(CardIter* pCI)
{
  _static_

  UNUSED(pCI);

  XF_Synthesise_pipelineDef *pXF_Synthesise_pipelineDef = nullptr;

  try {
    pXF_Synthesise_pipelineDef = new XF_Synthesise_pipelineDef(pCI);
    pXF_Synthesise_pipelineDef->vSynthesise();

    PipelineDef *pPipelineDef = pXF_Synthesise_pipelineDef->pPipelineDef();
    pXF_Synthesise_pipelineDef->vPipelineDef(nullptr);
    pCI->m_pBench->pOptimiser()->vPipelineDef(pPipelineDef);

    delete pXF_Synthesise_pipelineDef;
  }

  catch (Exception xException) {
    delete pXF_Synthesise_pipelineDef;
    throw xException;
  }
  catch (Error xError) {
    delete pXF_Synthesise_pipelineDef;
    throw xError;
  }

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* retrieve m_pPipelineDef                                                    */
/*----------------------------------------------------------------------------*/
PipelineDef* XF_Synthesise_pipelineDef::pPipelineDef(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_PIPELINEDEF, _FOOTER_);

  return m_pPipelineDef;
}

/*----------------------------------------------------------------------------*/
/* store m_pPipelineDef                                                       */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_pipelineDef::vPipelineDef(PipelineDef* pPipelineDef)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_PIPELINEDEF, _FOOTER_);

  m_pPipelineDef = pPipelineDef;
}

/*----------------------------------------------------------------------------*/
/* call synthesiser                                                           */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_pipelineDef::vSynthesise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_PIPELINEDEF, _FOOTER_);

  vCraftPipelineDef();
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* craft pipeline def                                                         */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_pipelineDef::vCraftPipelineDef(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_PIPELINEDEF, _FOOTER_);

  Board* pBoard = m_pCI->m_pBench->pGetBoard((i8)0);

  s32 sQueryNm = pBoard->pBench()->pOptimiser()->szQueryNm();

  m_pPipelineDef = new PipelineDef(sQueryNm);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain_NONE()) {
    if (m_pCI->G.p_OP->bIsCard(CK_STAGE))
      vCraftStageDef();
    else
      vCraftOpDef();
  }

  //m_pPipelineDef->vCollapseOps();
  m_pPipelineDef->vValidate();

  if (AM::JOB::bHasTag(C_PIPELINE_DEF)) {
    m_pPipelineDef->vDump();
    vDebugPoint();
  }
}

/*----------------------------------------------------------------------------*/
/* craft stage def                                                            */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_pipelineDef::vCraftStageDef(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_PIPELINEDEF, _FOOTER_);

  m_pStageDef = new StageDef(m_pPipelineDef->nItems(),
                             m_pCI->G.p_stage->szSymbol(SF_NONE),
                             m_pCI->G.p_stage);
  m_pPipelineDef->vAppend(m_pStageDef);
}

/*----------------------------------------------------------------------------*/
/* craft op def                                                               */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_pipelineDef::vCraftOpDef(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_PIPELINEDEF, _FOOTER_);

  RASSERT(m_pStageDef);

//  if (m_pCI->G.p_OP->bIsCard(CK_KEYEXISTS))
//    vDebugPoint();

  if (m_pCI->G.p_OP->bIsOne(CK_CAST,CK_COMBINED,CK_SEND,CK_NONE))
    return;
  if (m_pCI->G.p_OP->bIsCard(CK_SCAN))
    return;

  m_pOpDef = new OpDef(m_pStageDef->nItems(), m_pCI->G.p_OP);
  m_pStageDef->vAppend(m_pOpDef);

  vCraftStreamDefs();
  vCraftColsReq();

  vCraftIndexes();
  vCraftIndexDefs();

  vCraftArgDefs();
}

/*----------------------------------------------------------------------------*/
/* craft arg def                                                              */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_pipelineDef::vCraftArgDefs(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_PIPELINEDEF, _FOOTER_);

  Card_OP* p_OP = m_pCI->G.p_OP;

  s2K sExpr;
  s2K sConstraint;

  eCK eCard = p_OP->eCard();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  switch (eCard) {

    case CK_AGGREG:       p_OP->vRpnAggreg      (sExpr, _pCard_Aggreg      (p_OP)); break;
    case CK_CROSSJOIN:    p_OP->vRpnCrossJoin   (sExpr, _pCard_CrossJoin   (p_OP)); break;
    case CK_FILTER:       p_OP->vRpnFilter      (sExpr, _pCard_Filter      (p_OP)); break;
    case CK_GBYAGG:       p_OP->vRpnGbyAgg      (sExpr, _pCard_GbyAgg      (p_OP)); break;
    case CK_HASH:         p_OP->vRpnHash        (sExpr, _pCard_Hash        (p_OP)); break;
    case CK_HAVING:       p_OP->vRpnHaving      (sExpr, _pCard_Having      (p_OP)); break;
    case CK_INDEXJOIN:    p_OP->vRpnIndexJoin   (sExpr, _pCard_IndexJoin   (p_OP)); break;
    case CK_INDEXSCAN:    p_OP->vRpnIndexScan   (sExpr, _pCard_IndexScan   (p_OP)); break;
    case CK_INFLATE:      p_OP->vRpnInflate     (sExpr, _pCard_Inflate     (p_OP)); break;
    case CK_KEYEXISTS:    p_OP->vRpnKeyExists   (sExpr, _pCard_KeyExists   (p_OP)); break;
    case CK_KEYNOTEXISTS: p_OP->vRpnKeyNotExists(sExpr, _pCard_KeyNotExists(p_OP)); break;
    case CK_LIMIT:        p_OP->vRpnLimit       (sExpr, _pCard_Limit       (p_OP)); break;
    case CK_ORDAGG:       p_OP->vRpnOrdAgg      (sExpr, _pCard_OrdAgg      (p_OP)); break;
    case CK_PROJECT:      p_OP->vRpnProject     (sExpr, _pCard_Project     (p_OP)); break;
    case CK_SCAN:         p_OP->vRpnScan        (sExpr, _pCard_Scan        (p_OP)); break;
    case CK_SORT:         p_OP->vRpnSort        (sExpr, _pCard_Sort        (p_OP)); break;
    case CK_TOP:          p_OP->vRpnTop         (sExpr, _pCard_Top         (p_OP)); break;
    case CK_WHERE:        p_OP->vRpnWhere       (sExpr, _pCard_Where       (p_OP)); break;

    case CK_CAST:
    case CK_COMBINED:
    case CK_SEND:
      break;

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

    case CK_HASH:
      m_pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      m_pOpDef->m_pColHsh = new ColHsh;
      m_pOpDef->m_pColHsh->vUpdate(_pCard_Hash(p_OP)->pSub_hashing());
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

    case CK_INFLATE:
      m_pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_Inflate(p_OP)->pSub_tableNm()->szSymbol(SF_NONE)));
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
void XF_Synthesise_pipelineDef::vCraftIndexes(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_PIPELINEDEF, _FOOTER_);

  Card_OP* p_OP = m_pCI->G.p_OP;

  eCK eCard = p_OP->eCard();

  if (CK_CROSSJOIN == eCard)
    m_pOpDef->m_pIndex_indexJoin   = p_OP->pGetIndex_chosenIndex();
  else
  if (CK_INDEXJOIN == eCard)
    m_pOpDef->m_pIndex_indexJoin   = p_OP->pGetIndex_chosenIndex();
  else
  if (CK_INDEXSCAN == eCard)
    m_pOpDef->m_pIndex_indexScan   = p_OP->pGetIndex_chosenIndex();
  else
  if (CK_KEYEXISTS == eCard)
    m_pOpDef->m_pIndex_indexSemi   = p_OP->pGetIndex_chosenIndex();
  else
  if (CK_KEYNOTEXISTS == eCard)
    m_pOpDef->m_pIndex_indexSemi   = p_OP->pGetIndex_chosenIndex();
}

/*----------------------------------------------------------------------------*/
/* craft index def's                                                          */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_pipelineDef::vCraftIndexDefs(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_PIPELINEDEF, _FOOTER_);

  Card_OP* p_OP = m_pCI->G.p_OP;

  eCK eCard = p_OP->eCard();

  if (CK_GBYAGG == eCard)
    m_pOpDef->m_pIndexDef_gbyAgg = p_OP->pCraftIndexDef_key();
  else
  if (CK_SORT == eCard)
    m_pOpDef->m_pIndexDef_sort = p_OP->pCraftIndexDef_key();
  else
  if (CK_TOP == eCard)
    m_pOpDef->m_pIndexDef_top = p_OP->pCraftIndexDef_key();
}

/*----------------------------------------------------------------------------*/
/* craft stream def's for operator                                            */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_pipelineDef::vCraftStreamDefs(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_PIPELINEDEF, _FOOTER_);

  Card_OP* p_OP = m_pCI->G.p_OP;

#ifdef Z_FUSE
  if (p_OP->bIsCard(CK_INDEXJOIN)) {
    Card_Arriving* p_arriving = p_OP->pSub_arriving();
    m_pOpDef->m_pStreamDef_arriving =
        p_arriving->pToStreamDef(nullptr, nullptr, p_OP);
  }
  else
#endif

  if (m_pStreamDef_prior)
    m_pOpDef->m_pStreamDef_arriving = m_pStreamDef_prior->pDup();

#ifdef Z_FUSE
  else
  if (p_OP->bIsCard(CK_INDEXJOIN) || p_OP->bIsCard(CK_INDEXSCAN) || p_OP->bIsCard(CK_SCAN)) {
    Card_Arriving* p_arriving = p_OP->pSub_arriving();
    m_pOpDef->m_pStreamDef_arriving =
        p_arriving->pToStreamDef(nullptr, nullptr, p_OP);
  }
#endif

  if (p_OP->bIsCard(CK_INDEXJOIN))
    vDebugPoint();

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
/* craft the columns required maps for this pipeline                          */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_pipelineDef::vCraftColsReq(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_PIPELINEDEF, _FOOTER_);

  Card* p_OP_curr = m_pCI->G.p_OP;

  if (! p_OP_curr->bIsOne(CK_CROSSJOIN, CK_INDEXJOIN, CK_INDEXSCAN,
                          CK_KEYEXISTS, CK_KEYNOTEXISTS, CK_SCAN, CK_INFLATE, CK_NONE))
    return;

  eCK eCard_OP = p_OP_curr->eCard();
  Card_Table* p_table = p_OP_curr->pSub_table();
  RASSERT(p_table->pSymbol()->bIs_Tbl_Syn() || p_table->pSymbol()->bIs_Tbl_Syn_rep());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RASSERT(! m_pOpDef->pColsReq());
  ColsReq* pColsReq = new ColsReq;
  m_pOpDef->vColsReq(pColsReq);
  ColsReq_v2* pColsReq_v2 = new ColsReq_v2;
  m_pOpDef->vColsReq_v2(pColsReq_v2);

  for (Kid xDeck(m_pCI->G.pDeck,p_OP_curr); xDeck.bEach(); /**/) {
    Card* p_OP_later = xDeck.pChild();
    eCK eCard_later = p_OP_later->eCard();

    switch (eCard_later) {

      case CK_FILTER:
        vCraftColsReq_terms(p_table, pColsReq, pColsReq_v2,
                            p_OP_curr->pSub_stream(), p_OP_later->pSub_boolCalc());
        continue;

      case CK_PROJECT:
        vCraftColsReq_terms(p_table, pColsReq, pColsReq_v2,
                            p_OP_curr->pSub_stream(), p_OP_later->pSub_projection());
        continue;

      default:
        continue;
    }

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // if no mapping so far then attempt to map scan/indexScan

  if (! pColsReq_v2->m_nCols) {
    if (CK_SCAN == eCard_OP || CK_INFLATE == eCard_OP || CK_INDEXJOIN == eCard_OP || CK_INDEXSCAN == eCard_OP) {
      vCraftColsReq_stmCol(pColsReq, pColsReq_v2,
                           p_OP_curr->pSub_stream(), p_OP_curr);
    }
//    else
//    if (CK_INDEXJOIN == eCard_OP) {
//      vCraftColsReq_tblCol(p_table, pColsReq, pColsReq_v2,
//                           p_OP_curr->pSub_stream(), p_OP_curr);
//    }
  }

  // todo: indexScan?  Chat with ALka

  //RASSERT(pColsReq_v2->m_nMapped);

  pColsReq->vValidate();
  pColsReq_v2->vValidate();
}

/*----------------------------------------------------------------------------*/
/* craft the columns required map for given card                              */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_pipelineDef::vCraftColsReq_stmCol(ColsReq* pColsReq,
                                                     ColsReq_v2* pColsReq_v2,
                                                     Card_Stream* p_stream,
                                                     Card *pSubCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_PIPELINEDEF, _FOOTER_);

#ifndef Z_FUSE
  if (pSubCard->bIsCard(CK_INDEXJOIN))
    return;
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // map first stmCol of given expr

  auto bfMap_stmCol = [pColsReq] (Card* pCard_expr) {

    Card_Expr* p_expr = _pCard_Expr(pCard_expr);
    Symbol* pSymbol_stmCol = p_expr->pFirstStmCol();

    if (! pSymbol_stmCol)
      return C_CONTINUE_RECURSION;

    i8 i_Col = pSymbol_stmCol->iSF_ordinal();

    pColsReq->vAddCol(i_Col, false, i_Col);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pSubCard->bApplyRecursive(CK_EXPR, bfMap_stmCol);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  UNUSED(pColsReq_v2);
  UNUSED(p_stream);

#ifdef Z_FUSE
  Card_Arriving* p_arriving = p_stream->pSub_arriving();
  Card_Projection* p_projection = p_stream->pSub_projection();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // map first stmCol of given expr

  auto bfMap_stmCol_v2 = [p_arriving, pColsReq_v2] (Card* pCard_expr) {
    UNUSED(pColsReq_v2);
    Card_Expr* p_expr_a = _pCard_Expr(pCard_expr);
    Symbol* pSymbol_stmCol_a = p_expr_a->pFirstStmCol();
    Card_As* p_as_a = p_expr_a->pSub_as();

    Card_Expr* p_expr_p =
        p_arriving->pFind_expr_By_as_stripItem(p_as_a->pSymbol());
    if (p_expr_p) {
      Card_As *p_as_p = p_expr_p->pSub_as();

//    p_expr_a->vDump(__func__);
//    p_as_a->vDump(__func__);
//    p_expr_p->vDump(__func__);
//    p_as_p->vDump(__func__);

      Symbol *pSymbol_a = p_as_a->pSymbol();
      i8 i_CrwCol_a = pSymbol_a->iSF_ordinal();

      Symbol *pSymbol_p = p_as_p->pSymbol();
      i8 i_StructCell_p = pSymbol_p->iSF_ordinal();

      pColsReq_v2->vAddMapping(i_CrwCol_a, i_StructCell_p, p_as_p->szSymbol(SF_NONE));
    }
    else
    if (pSymbol_stmCol_a &&
        (p_expr_p = p_arriving->pFind_expr_Of_symbol(pSymbol_stmCol_a))) {
      //Card_As *p_as_p = p_expr_p->pSub_as();

      Symbol *pSymbol_a = p_expr_p->pSymbol();
      i8 i_CrwCol_a = pSymbol_a->iSF_ordinal();

      i8 i_StructCell_p = pCard_expr->pSymbol()->iSF_ordinal();

      pColsReq_v2->vAddMapping(i_CrwCol_a, i_StructCell_p, p_as_a->szSymbol(SF_NONE));

    }
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  p_projection->bApplyRecursive(CK_EXPR, bfMap_stmCol_v2);
#endif

}
/*----------------------------------------------------------------------------*/
//void XF_Synthesise_pipelineDef::vCraftColsReq_stmCol(ColsReq* pColsReq,
//                                                     ColsReq_v2* pColsReq_v2,
//                                                     Card_Stream* p_stream,
//                                                     Card *pSubCard)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_PIPELINEDEF, _FOOTER_);
//
//#ifndef Z_FUSE
//  if (pSubCard->bIsCard(CK_INDEXJOIN))
//    return;
//#endif
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // map first stmCol of given expr
//
//  auto bfMap_stmCol = [pColsReq] (Card* pCard_expr) {
//
//    Card_Expr* p_expr = _pCard_Expr(pCard_expr);
//    Symbol* pSymbol_stmCol = p_expr->pFirstStmCol();
//    i8 i_Col = pSymbol_stmCol->iSF_ordinal();
//
//    pColsReq->vAddCol(i_Col, false, i_Col);
//
//    return C_CONTINUE_RECURSION;
//  };
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  pSubCard->bApplyRecursive(CK_EXPR, bfMap_stmCol);
//
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  UNUSED(pColsReq_v2);
//  UNUSED(p_stream);
//
//#ifdef Z_FUSE
//  Card_Arriving* p_arriving = p_stream->pSub_arriving();
//  Card_Projection* p_projection = p_stream->pSub_projection();
//
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // map first stmCol of given expr
//
//  auto bfMap_stmCol_v2 = [p_projection, pColsReq_v2] (Card* pCard_expr) {
//    UNUSED(pColsReq_v2);
//    Card_Expr* p_expr_a = _pCard_Expr(pCard_expr);
//    //Symbol* pSymbol_stmCol_a = p_expr_a->pFirstStmCol();
//    Card_As* p_as_a = p_expr_a->pSub_as();
//
//    Card_Expr* p_expr_p =
//        p_projection->pFind_expr_By_as_stripItem(p_as_a->pSymbol());
//    if (p_expr_p) {
//      Card_As *p_as_p = p_expr_p->pSub_as();
//
////    p_expr_a->vDump(__func__);
////    p_as_a->vDump(__func__);
////    p_expr_p->vDump(__func__);
////    p_as_p->vDump(__func__);
//
//      Symbol *pSymbol_a = p_as_a->pSymbol();
//      i8 i_CrwCol_a = pSymbol_a->iSF_ordinal();
//
//      Symbol *pSymbol_p = p_as_p->pSymbol();
//      i8 i_StructCell_p = pSymbol_p->iSF_ordinal();
//
//      pColsReq_v2->vAddMapping(i_CrwCol_a, i_StructCell_p, p_as_p->szSymbol(SF_NONE));
//    }
////    else
////    if (pSymbol_stmCol_a &&
////        (p_expr_p = p_projection->pFindExprOfSymbol(pSymbol_stmCol_a))) {
////      Card_As *p_as_p = p_expr_p->pSub_as();
////
////      Symbol *pSymbol_a = p_as_a->pSymbol();
////      i8 i_CrwCol_a = pSymbol_a->iSF_ordinal();
////
////      Symbol *pSymbol_p = p_as_p->pSymbol();
////      i8 i_StructCell_p = pSymbol_p->iSF_ordinal();
////
////      pColsReq_v2->vAddMapping(i_CrwCol_a, i_StructCell_p, p_as_p->szSymbol(SF_NONE));
////
////    }
//    return C_CONTINUE_RECURSION;
//  };
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  p_arriving->bApplyRecursive(CK_EXPR, bfMap_stmCol_v2);
//#endif
//
//}





/*----------------------------------------------------------------------------*/
//void XF_Synthesise_pipelineDef::vCraftColsReq_stmCol(ColsReq* pColsReq,
//                                                     ColsReq_v2* pColsReq_v2,
//                                                     Card_Stream* p_stream,
//                                                     Card *pSubCard)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_PIPELINEDEF, _FOOTER_);
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // map first stmCol of given expr
//
//  auto bfMap_stmCol = [pColsReq] (Card* pCard_expr) {
//
//    Card_Expr* p_expr = _pCard_Expr(pCard_expr);
//    Symbol* pSymbol_stmCol = p_expr->pFirstStmCol();
//    i8 i_Col = pSymbol_stmCol->iSF_ordinal();
//
//    pColsReq->vAddCol(i_Col, false, i_Col);
//
//    return C_CONTINUE_RECURSION;
//  };
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  pSubCard->bApplyRecursive(CK_EXPR, bfMap_stmCol);
//
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  UNUSED(pColsReq_v2);
//  UNUSED(p_stream);
//
//#ifdef Z_FUSE
//  Card_Arriving* p_arriving = p_stream->pSub_arriving();
//  Card_Projection* p_projection = p_stream->pSub_projection();
//
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // map first stmCol of given expr
//
//  auto bfMap_stmCol_v2 = [p_arriving, pColsReq_v2] (Card* pCard_expr) {
//    UNUSED(pColsReq_v2);
//    Card_Expr* p_expr_projection = _pCard_Expr(pCard_expr);
//    Card_As* p_as_projection = p_expr_projection->pSub_as();
//
////    p_expr_projection->vDump(__func__);
////    p_arriving->vDump(__func__);
//
//    Card_Expr* p_expr_arriving =
//        p_arriving->pFind_expr_By_as_stripItem(p_as_projection->pSymbol());
//
//    p_expr_arriving->vDump(__func__);
//    Symbol* pSymbol_stmCol_a = p_expr_arriving->pFirstStmCol();
//    i8 i_CrwCol = pSymbol_stmCol_a->iSF_ordinal() -1;
//
////    Symbol* pSymbol_stmCol_p = p_expr_projection->pFirstStmCol();
////    i8 i_StructCell = pSymbol_stmCol_p->iSF_ordinal();
//
//    pColsReq_v2->vAddMapping(i_CrwCol, pColsReq_v2->m_nMapped);
//
//    return C_CONTINUE_RECURSION;
//  };
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  p_projection->bApplyRecursive(CK_EXPR, bfMap_stmCol_v2);
//#endif
//
//}

/*----------------------------------------------------------------------------*/
/* craft the columns required map for given card                              */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_pipelineDef::vCraftColsReq_tblCol(Card_Table* p_table,
                                                     ColsReq* pColsReq,
                                                     ColsReq_v2* pColsReq_v2,
                                                     Card_Stream* p_stream,
                                                     Card *pSubCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_PIPELINEDEF, _FOOTER_);
  UNUSED(p_stream);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfMap_tblCol = [p_table, pColsReq, pColsReq_v2] (Card* pCard_expr) {
    Card_Expr* p_expr = _pCard_Expr(pCard_expr);

    Symbol* pSymbol_tblCol = p_expr->pFirstTblCol();
    if (! pSymbol_tblCol)
      return C_CONTINUE_RECURSION;

    Stz* szTblCol = pSymbol_tblCol->szSF_symbol(SF_COL_TC);
    if (! p_table->bIsTblColOf(szTblCol))
      return C_CONTINUE_RECURSION;

    i8 i_Col = StrLib::iGleanInt(szTblCol->azChar());

    pColsReq->vAddCol(i_Col, false, i_Col);
    pColsReq_v2->vAddMapping(i_Col, i_Col, nullptr);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pSubCard->bApplyRecursive(CK_EXPR, bfMap_tblCol);
}

/*----------------------------------------------------------------------------*/
/* craft the columns required map for given card                              */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_pipelineDef::vCraftColsReq_terms(Card_Table* p_table,
                                                    ColsReq* pColsReq,
                                                    ColsReq_v2* pColsReq_v2,
                                                    Card_Stream* p_stream,
                                                    Card* pSubCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_PIPELINEDEF, _FOOTER_);
  UNUSED(p_stream);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfMap_terms = [p_table, pColsReq, pColsReq_v2] (Card_Term* p_term) {

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
    pColsReq_v2->vAddMapping(i_Col, i_Col, sColNm);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pSubCard->bApplyRecursive_term(bfMap_terms);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
