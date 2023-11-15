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
    else {
      vCraftOpDef();
      m_pCI->vSnapFluxArriving();
    }
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
                             m_pCI->G.p_stage->szFacet(SF_NONE),
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

  if (m_pCI->G.p_OP->bIsCard(CK_ORDAGG))
    vDebugPoint();

  if (m_pCI->G.p_OP->bIsOne(CK_CAST,CK_COMBINED,CK_SEND,CK_NONE))
    return;

  if (m_pCI->G.p_OP->bIsCard(CK_SCAN)) {
    return;
  }

  m_pOpDef = new OpDef(m_pStageDef->nItems(), m_pCI->G.p_OP);
  m_pStageDef->vAppend(m_pOpDef);

  vCraftStreamDefs();

  vCraftColPpg();
  vCraftColInj();
  vCraftKeyMap();

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

    case CK_AGGREG:       p_OP->vRpn_aggreg     (sExpr, _pCard_Aggreg(p_OP));       break;
    case CK_CROSSJOIN:    p_OP->vRpn_crossJoin  (sExpr, _pCard_CrossJoin(p_OP)); break;
    case CK_FILTER:       p_OP->vRpn_filter(sExpr, _pCard_Filter(p_OP)); break;
    case CK_GBYAGG:       p_OP->vRpn_gbyAgg(sExpr, _pCard_GbyAgg(p_OP)); break;
    case CK_HASH:         p_OP->vRpn_hash(sExpr, _pCard_Hash(p_OP)); break;
    case CK_HAVING:       p_OP->vRpn_having(sExpr, _pCard_Having(p_OP)); break;
    case CK_INDEXJOIN:    p_OP->vRpn_indexJoin(sExpr, _pCard_IndexJoin(p_OP)); break;
    case CK_INDEXSCAN:    p_OP->vRpn_indexScan(sExpr, _pCard_IndexScan(p_OP)); break;
    case CK_INFLATE:      p_OP->vRpn_inflate(sExpr, _pCard_Inflate(p_OP)); break;
    case CK_KEYEXISTS:    p_OP->vRpn_keyExists(sExpr, _pCard_KeyExists(p_OP)); break;
    case CK_KEYNOTEXISTS: p_OP->vRpn_keyNotExists(sExpr, _pCard_KeyNotExists(p_OP)); break;
    case CK_LIMIT:        p_OP->vRpn_limit(sExpr, _pCard_Limit(p_OP)); break;
    case CK_ORDAGG:       p_OP->vRpn_ordAgg(sExpr, _pCard_OrdAgg(p_OP)); break;
    case CK_PROJECT:      p_OP->vRpn_project(sExpr, _pCard_Project(p_OP)); break;
    case CK_SCAN:         p_OP->vRpn_scan(sExpr, _pCard_Scan(p_OP)); break;
    case CK_SORT:         p_OP->vRpn_sort(sExpr, _pCard_Sort(p_OP)); break;
    case CK_TOP:          p_OP->vRpn_top(sExpr, _pCard_Top(p_OP)); break;
    case CK_WHERE:        p_OP->vRpn_where(sExpr, _pCard_Where(p_OP)); break;

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
      m_pOpDef->vAppend(new ArgDef(0, "topology"_Stz, _pCard_Cast(p_OP)->szFacet(SF_NONE)));
      break;

    case CK_CROSSJOIN:
      m_pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_CrossJoin(p_OP)->pSub_tableNm()->szFacet(SF_NONE)));
      m_pOpDef->vAppend(new ArgDef(1, "indexNm"_Stz, _pCard_CrossJoin(p_OP)->pSub_tableNm()->szFacet(SF_NONE)));
      m_pOpDef->vAppend(new ArgDef(2, "expr"_Stz, sExpr));
      break;

    case CK_FILTER:
      m_pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    case CK_GBYAGG:
      m_pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    case CK_HASH:
      if (m_pStageDef->i_Stage() == 5 && m_pOpDef->i_Op() == 1)
        vDebugPoint();
      m_pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      m_pOpDef->m_pColHsh = new ColHsh;
      m_pOpDef->m_pColHsh->vUpdate(_pCard_Hash(p_OP)->pSub_hashing());
      break;

    case CK_HAVING:
      m_pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    case CK_INDEXJOIN:
      m_pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_IndexJoin(p_OP)->pSub_tableNm()->szFacet(SF_NONE)));
      m_pOpDef->vAppend(new ArgDef(1, "indexNm"_Stz, _pCard_IndexJoin(p_OP)->pSub_tableNm()->szFacet(SF_NONE)));
      m_pOpDef->vAppend(new ArgDef(2, "expr"_Stz, sExpr));

      if (p_OP->pSub_constraint()) {
        p_OP->vRpn_indexJoin_constraint(sConstraint, _pCard_IndexJoin(p_OP));
        m_pOpDef->vAppend(new ArgDef(4, "constraint"_Stz, sConstraint));
      }
      break;

    case CK_INDEXSCAN:
      m_pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_IndexScan(p_OP)->pSub_tableNm()->szFacet(SF_NONE)));
      m_pOpDef->vAppend(new ArgDef(1, "indexNm"_Stz, _pCard_IndexScan(p_OP)->pSub_tableNm()->szFacet(SF_NONE)));
      m_pOpDef->vAppend(new ArgDef(2, "expr"_Stz, sExpr));
      break;

    case CK_INFLATE:
      m_pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_Inflate(p_OP)->pSub_tableNm()->szFacet(SF_NONE)));
      break;

    case CK_KEYEXISTS:
      p_OP->vRpn_keyExists_constraint(sConstraint, _pCard_KeyExists(p_OP));

      m_pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_KeyExists(p_OP)->pSub_tableNm()->szFacet(SF_NONE)));
      m_pOpDef->vAppend(new ArgDef(1, "indexNm"_Stz, _pCard_KeyExists(p_OP)->pSub_indexNm()->szFacet(SF_NONE)));
      m_pOpDef->vAppend(new ArgDef(2, "expr"_Stz, sExpr));
      m_pOpDef->vAppend(new ArgDef(3, "constraint"_Stz, sConstraint));
      break;

    case CK_KEYNOTEXISTS:
      p_OP->vRpn_keyNotExists_constraint(sConstraint, _pCard_KeyNotExists(p_OP));

      m_pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_KeyNotExists(p_OP)->pSub_tableNm()->szFacet(SF_NONE)));
      m_pOpDef->vAppend(new ArgDef(1, "indexNm"_Stz, _pCard_KeyNotExists(p_OP)->pSub_indexNm()->szFacet(SF_NONE)));
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
      m_pOpDef->vAppend(new ArgDef(0, "tableNm"_Stz, _pCard_Scan(p_OP)->pSub_tableNm()->szFacet(SF_NONE)));
      break;

    case CK_SEND:
      break;

    case CK_SORT:
      m_pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      break;

    case CK_TOP:
      m_pOpDef->vAppend(new ArgDef(0, "expr"_Stz, sExpr));
      m_pOpDef->vAppend(new ArgDef(0, "count"_Stz, _pCard_Top(p_OP)->pSub_count()->szFacet(SF_NONE)));
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

  if (m_pStageDef->i_Stage() == 6) {
    vDebugPoint();
    if (m_pOpDef->i_Op() == 0)
      vDebugPoint();
  }

  Card_OP* p_OP = m_pCI->G.p_OP;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // arriving

  if (p_OP->bIsCard(CK_INDEXJOIN)) {
    m_pOpDef->m_pStreamDef_arriving =
        m_pCI->Flux.m_pArriving->pToStreamDef(nullptr, nullptr, p_OP);
  }
  else

  if (m_pStreamDef_prior)
    m_pOpDef->m_pStreamDef_arriving = m_pStreamDef_prior->pDup();

  else
  if (p_OP->bIsCard(CK_INDEXJOIN)) {
    m_pOpDef->m_pStreamDef_arriving =
        m_pCI->Flux.m_pArriving->pToStreamDef(nullptr, nullptr, p_OP);
  }
  else
  if (p_OP->bIsCard(CK_INFLATE)) {
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // emitted

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
void XF_Synthesise_pipelineDef::vCraftColInj(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_PIPELINEDEF, _FOOTER_);

  if (m_pStageDef->i_Stage() == 2) {
    vDebugPoint();
    if (m_pOpDef->i_Op() == 0)
      vDebugPoint();
  }


  Card* p_OP_curr = m_pCI->G.p_OP;

  if (! p_OP_curr->bIsOne(CK_CROSSJOIN, CK_INDEXJOIN, CK_INDEXSCAN, CK_INFLATE,
                          CK_KEYEXISTS, CK_KEYNOTEXISTS, CK_SCAN, CK_NONE))
    return;

  eCK eCard_OP = p_OP_curr->eCard();
  Card_Table* p_table = p_OP_curr->pSub_table();
  RASSERT(p_table->pSymbol()->bIs_Tbl_Syn() || p_table->pSymbol()->bIs_Tbl_Syn_rep());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Table* pTable = (Table*)p_table->pSymbol()->Y_pGet(P_Pointer);
  RASSERT(pTable);
  i8 nCrwCols = pTable->pStreamDef()->nColumns();
  ColInj* pColInj = new ColInj(nCrwCols);
  m_pOpDef->vColInj(pColInj);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (CK_SCAN      == eCard_OP ||
      CK_INFLATE   == eCard_OP ||
      CK_INDEXJOIN == eCard_OP ||
      CK_CROSSJOIN == eCard_OP ||
      CK_INDEXSCAN == eCard_OP) {
    vCraftColInj(pColInj, p_OP_curr);
  }

  pColInj->vValidate();
}

/*----------------------------------------------------------------------------*/
/* craft the columns injected to emitted map for given card                   */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_pipelineDef::vCraftColInj(ColInj* pColInj, Card *pSubCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_PIPELINEDEF, _FOOTER_);

  Card_Injectable* p_injected = pSubCard->pSub_injectable();
  Card_Emit*   p_emit = pSubCard->pSub_emit();

  //p_emit->vDump(__func__);

  if (m_pStageDef->i_Stage() == 2) {
    vDebugPoint();
    if (m_pOpDef->i_Op() == 0)
      vDebugPoint();
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add colInj descriptor(s) for given injected expr_i

  auto bfAddDescriptors = [pColInj, p_emit] (Card* pCard_expr_i) {

    // get details of column injected (e.g. $b.1)
    Card_Expr*   p_expr_i = _pCard_Expr(pCard_expr_i);
    i8    i_CrwCol_expr_i = StrLib::iGleanInt(p_expr_i->szFacet(SF_ITM_COL)->azChar());

    Symbol* pSym_tblCol_i = p_expr_i->pFirstTblCol();
    Stz*       szTblCol_i = pSym_tblCol_i->szSF_chrSeq(SF_NONE);
    //pSym_tblCol_i->vDump();

    // test if the injected column is actually emitted?
    // n.b. it may be emitted more than once
    Card_Expr *p_prior_expr_e = nullptr;

    Kid xKid(p_emit);
    for (;;) {
      Card_Expr *p_expr_e =
          p_emit->pFind_expr_Via_term0(pSym_tblCol_i, &xKid);
      if (! p_expr_e)
        return C_CONTINUE_RECURSION;

      // -- injected column is actually emitted --
      // e.g. expr $1 as C0_C_CUSTKEY

      //p_expr_e->vDump(__func__);

      // get details of emitted column
      i8 i_StructCell_e = p_expr_e->pSymbol()->iSF_ordinal() - 1;

      // add descriptor
      pColInj->vAdd(i_CrwCol_expr_i,
                    i_StructCell_e,
                    p_expr_e->szFacet(SF_ITM_COL),
                    szTblCol_i,
                    p_expr_e->szFacet(SF_NMD_COL));

      if (p_prior_expr_e) {
        pColInj->vRepeatsPrior();
        vDebugPoint();
      }
      p_prior_expr_e = p_expr_e;
    }
    NEVER_REACH
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  p_injected->bApplyRecursive(CK_EXPR, bfAddDescriptors);
}

/*----------------------------------------------------------------------------*/
/* craft the columns propagated for this pipeline                             */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_pipelineDef::vCraftColPpg(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_PIPELINEDEF, _FOOTER_);

  if (m_pStageDef->i_Stage() == 2) {
    vDebugPoint();
    if (m_pOpDef->i_Op() == 0)
      vDebugPoint();
  }

  if (! m_pCI->G.p_OP->bIsOne(CK_CROSSJOIN, CK_INDEXJOIN, CK_NONE))
    return;

  if (! m_pCI->Flux.m_pArriving)
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  //i8 nArriving = m_pCI->G.p_arriving->nChildCards();
  i8 nArriving = m_pCI->Flux.m_pArriving->nChildCards();
  ColPpg* pColPpg = new ColPpg(nArriving);
  m_pOpDef->vColPpg(pColPpg);

  vCraftColPpg(pColPpg, m_pCI->G.p_OP);
}

/*----------------------------------------------------------------------------*/
/* craft the columns propagated for given card                                */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_pipelineDef::vCraftColPpg(ColPpg* pColPpg, Card *pSubCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_PIPELINEDEF, _FOOTER_);

  Card_Emit* p_arriving = m_pCI->Flux.m_pArriving;
  Card_Emit* p_emit  = pSubCard->pSub_emit();

  if (AM::JOB::bHasTag(C_OPTIMISER_TRACE)) {
    p_arriving->vDump(__func__, "--ARRIVING at SubCard--"_Stz);
    p_emit->vDump(__func__, "--EMITTED by SubCard--"_Stz);
  }

  if (m_pStageDef->i_Stage() == 1) {
    vDebugPoint();
    if (m_pOpDef->i_Op() == 0)
      vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add colPpg descriptor(s) for given arriving expr_a

  auto bfAddPropagatedArriving = [this, pColPpg, p_emit](Card *pCard_expr_a) {

    eSF eViaFacet;
    s256 sHow;

    // arriving related
    Card_Expr* p_expr_a = _pCard_Expr(pCard_expr_a);
    Symbol*  pExprSym_a = p_expr_a->pSymbol();

    i8 i_CellIn;
    i8 i_CellOut;
    Card_Expr* p_expr_e;

    Symbol* pInSym_a;
    Symbol* pOutSym_e;

    if (AM::JOB::bHasTag(C_OPTIMISER_TRACE))
      p_expr_a->vDump(__func__,"--EXPR ARRIVING at SubCard--"_Stz);

    if (m_pStageDef->i_Stage() == 4) {
      vDebugPoint();
      if (m_pOpDef->i_Op() == 0)
        vDebugPoint();
    }


    if (p_expr_a->bIsScalarCol()) {
      // + scalar

      bool bTermSym_a_Is_tblCol = false;
      Symbol*  pTermSym_a = p_expr_a->pGet_term0()->pSymbol();
      if (pTermSym_a && pTermSym_a->bIs_Tbl_Col())
        bTermSym_a_Is_tblCol = true;

      if (bTermSym_a_Is_tblCol && (p_expr_e = p_emit->pFind_expr_Via_term0(pTermSym_a, &eViaFacet))) {
        // 1) ARRIVING TERM: $t.c
        sHow.vSprint("--FOUND emitted expr for arriving $t.c: %s~%s--"_Stz,
                     pTermSym_a->szSF_chrSeq(eViaFacet),aSF[eViaFacet]);
        if (AM::JOB::bHasTag(C_OPTIMISER_TRACE))
          p_expr_e->vDump(__func__, sHow);
        pInSym_a = pTermSym_a;
      }
      else
      if ((p_expr_e = p_emit->pFind_expr_Via_term0(pExprSym_a, &eViaFacet))) {
        // 2) ARRIVING EXPR: $n
        sHow.vSprint("--FOUND emitted expr for arriving $n; %s~%s--"_Stz,
                     pExprSym_a->szSF_chrSeq(eViaFacet),aSF[eViaFacet]);
        if (AM::JOB::bHasTag(C_OPTIMISER_TRACE))
          p_expr_e->vDump(__func__, sHow);
        pInSym_a = pExprSym_a;
      }
      else {
        // not propagated
        //vTrace("--NOT-FOUND expr\n"_Stz);
        return C_CONTINUE_RECURSION;
      }

      i_CellIn  = p_expr_a->pSymbol()->iSF_ordinal() - 1;
      i_CellOut = p_expr_e->pSymbol()->iSF_ordinal() - 1;

      pOutSym_e = p_expr_e->pGet_term0()->pSymbol();

      pColPpg->vAdd(i_CellIn,
                    i_CellOut,
                    p_expr_e->szFacet(SF_NONE),
                    pInSym_a->szSF_chrSeq(SF_NONE),
                    pOutSym_e->szSF_chrSeq(SF_NONE));

      return C_CONTINUE_RECURSION;
    }

    else {
      // - computed

      //if ((p_expr_e = p_emit->pGet_expr(pExprSym_a))) {
      if ((p_expr_e = p_emit->pContains(pExprSym_a))) {

//      p_emit->vDump(__func__);
//      p_expr_a->vDump(__func__);
//      p_expr_e->vDump(__func__);

        if (AM::JOB::bHasTag(C_OPTIMISER_TRACE))
          p_expr_e->vDump(__func__,"--EXPR EMITTED at SubCard--"_Stz);

        pColPpg->vAdd(pExprSym_a->iSF_ordinal() - 1,
                      p_expr_e->pSymbol()->iSF_ordinal() - 1,
                      p_expr_e->szFacet(SF_NONE),
                      pExprSym_a->szSF_chrSeq(SF_NONE),
                      p_expr_e->pSymbol()->szSF_chrSeq(SF_NONE));
        return C_CONTINUE_RECURSION;
      }

      return C_CONTINUE_RECURSION;
    }
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  p_arriving->bApplyRecursive(CK_EXPR, bfAddPropagatedArriving);
}

/*----------------------------------------------------------------------------*/
/* craft the key mapping                                                      */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_pipelineDef::vCraftKeyMap(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_PIPELINEDEF, _FOOTER_);

  if (! m_pCI->G.p_OP->bIsOne(CK_ORDAGG, CK_NONE))
    return;

  m_pOpDef->m_pKeyMap = new KeyMap;

  m_pCI->G.p_emit = m_pCI->G.p_OP->pSub_emit();
  for (Kid xKey(m_pCI->G.p_OP->pSub_key()); xKey.bEach(); /**/) {
    //xKey.p_expr()->vDump(__func__,"KEY EXPR"_Stz);
    if ((m_pCI->G.p_expr = m_pCI->G.p_emit->pFind_expr_Via_term0(xKey.p_expr()->pSymbol()))) {
      //m_pCI->G.p_expr->vDump(__func__, "EMIT EXPR"_Stz);

      i8 i_EmittedCol = m_pCI->G.p_expr->pSymbol()->iSF_ordinal() - 1;
      i8 i_Cell = xKey.p_expr()->pSymbol()->iSF_ordinal() - 1;
      m_pOpDef->m_pKeyMap->vAdd(i_EmittedCol,i_Cell);
    }
    else
      NEVER_REACH
  }
  vDebugPoint();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
