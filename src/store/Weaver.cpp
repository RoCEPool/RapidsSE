/*============================================================================*/
/* Weaver.cpp -- Weaver of Cards                                              */
/*============================================================================*/

#include "Weaver.hpp"
#include "Optimiser.hpp"
#include "CardIter.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Weaver, CN_WEAVER)
__aNewFunction(Weaver, CN_WEAVER)
__vDeleteFunction(Weaver, CN_WEAVER)
__aFooterFunction(Weaver)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Weaver::Weaver(Bench* pBench, PipelineDef* pPipelineDef)
: OptimiserTool(CN_WEAVER)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  m_pPipelineDef = pPipelineDef;
  m_pDeck = nullptr;

  m_pBench     = pBench;
  m_pOptimiser = m_pBench->pOptimiser();
  m_pBoard     = nullptr;
  
  m_i_Stage    = 0;
  m_i_Op_stage = 0;
  m_bOpPrinted = false;
  m_pOptStmt   = nullptr;
  m_pCodeBlock = nullptr;
  m_bSawHash   = false;

  _checkInitialised_(CN_WEAVER);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Weaver::~Weaver(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  if (m_pOptStmt)
    delete m_pOptStmt;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* get optimised pipeline statement                                           */
/*----------------------------------------------------------------------------*/
ExtendedText* Weaver::pOptStmt(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  return m_pOptStmt;
}

/*----------------------------------------------------------------------------*/
/* set optimised pipeline statement                                           */
/*----------------------------------------------------------------------------*/
void Weaver::vOptStmt(ExtendedText* pOptSmt)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  m_pOptStmt = pOptSmt;
}

/*----------------------------------------------------------------------------*/
/* get code block                                                             */
/*----------------------------------------------------------------------------*/
ExtendedText* Weaver::pCodeBlock(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  return m_pCodeBlock;
}

/*----------------------------------------------------------------------------*/
/* set code block                                                             */
/*----------------------------------------------------------------------------*/
void Weaver::vCodeBlock(ExtendedText* pCodeBlock)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  m_pCodeBlock = pCodeBlock;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* weave : pipeline <name>                                                    */
/*----------------------------------------------------------------------------*/
void Weaver::vPipeline(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  vConcat_cmd("pipeline %s\n"_Stz, m_pOptimiser->szQueryNm());
}

/*----------------------------------------------------------------------------*/
/* weave : tag ... endTag                                                     */
/*----------------------------------------------------------------------------*/
void Weaver::vTags(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  vConcat_cmd("  tag <optimised> = 1 endTag\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* weave : begin                                                              */
/*----------------------------------------------------------------------------*/
void Weaver::vBegin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  vConcat_cmd("begin\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* weave : end ;                                                              */
/*----------------------------------------------------------------------------*/
void Weaver::vEnd(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  vConcat_cmd("\n"_Stz);
  vConcat_cmd("end"_Stz);
}

/*----------------------------------------------------------------------------*/
/* weave : code block                                                         */
/*----------------------------------------------------------------------------*/
void Weaver::vCppBlock(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  bool bHasCode = false;

  i8 nStages = m_pPipelineDef->nItems();
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    StageDef *pStageDef = m_pPipelineDef->pGet(i_Stage);
    if (pStageDef->bHasCode()) {
      bHasCode = true;
      break;
    }
  }

  if (! bHasCode)
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    StageDef *pStageDef = m_pPipelineDef->pGet(i_Stage);

    if (! m_pCodeBlock)
      m_pCodeBlock = new ExtendedText;
    m_pCodeBlock->vConcat(pStageDef->pCode());
  }
}

/*----------------------------------------------------------------------------*/
/* weave : ;                                                                  */
/*----------------------------------------------------------------------------*/
void Weaver::vSemicolon(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  //vConcat_cmd("\n"_Stz);
  vConcat_cmd(" ;\n"_Stz);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* weave a card                                                               */
/*----------------------------------------------------------------------------*/
void Weaver::vCard(Card* pCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  //pCard->vDump();

  eCK eCard = pCard->eCard();
  
  if (CK_c_STAGE == eCard)
    vStageNext(_pCard_Stage(pCard));
  else
    vOp(pCard);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* weave : stage S00 on origin :                                              */
/*----------------------------------------------------------------------------*/
void Weaver::vStageFirst(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  vPreStage();
  vTabStage();
  vConcat_cmd("stage S%02lld on origin : "_Stz, m_i_Stage++);
  vEolStage();
  
  vTabOp();
  vConcat_cmd("start"_Stz);           vEolOp();  m_i_Op_stage++;
  vTabOp();
  vConcat_cmd("pulse     once"_Stz);  vEolOp();  m_i_Op_stage++;
  vTabOp();
  vConcat_cmd("next"_Stz);            vEolOp();  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* weave : stage Snn on ??? :                                                 */
/*----------------------------------------------------------------------------*/
void Weaver::vStageNext(Card_Stage* pCard_Stage)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  eYG eStageTopo = (eYG) pCard_Stage->pSub_topology()->pSymbol()->Y_iGet(P_StageTopo);
  
  //vTabOp();  vConcat_cmd("next"_Stz);  vEolOp();

  vPreStage();
  vTabStage();
  vConcat_cmd("stage %s on %s : "_Stz, pCard_Stage->szFacet(SF_STG_NO), aYG_weave[eStageTopo]);
  vEolStage();

  m_i_Stage++;
  
  m_i_Op_stage = 0;
  m_bOpPrinted = false;
  m_bSawHash   = false;
}

/*----------------------------------------------------------------------------*/
/* weave : stage Snn on origin :                                              */
/*----------------------------------------------------------------------------*/
void Weaver::vStageLast(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  //vTabOp();  vConcat_cmd("next"_Stz);  vEolOp();
  
  vPreStage();
  vTabStage();
  vConcat_cmd("stage S%02lld on origin :"_Stz, m_i_Stage++);
  vEolStage();
  
  m_i_Op_stage = 0;
  
  vTabOp();
  vConcat_cmd("output"_Stz);       vEolOp();  m_i_Op_stage++;
  vTabOp();
  vConcat_cmd("sink"_Stz);         vEolOp();  m_i_Op_stage++;
  vTabOp();
  vConcat_cmd("first"_Stz);        vEolOp();  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* weave : dynamic :                                                          */
/*----------------------------------------------------------------------------*/
void Weaver::vDynamic(StageDef* pStageDef)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  UNUSED(pStageDef);

  vTabOp();
  vConcat_cmd("dynamic"_Stz);      vEolOp();  m_i_Op_stage++;
  //vTabOp();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* weave a card                                                               */
/*----------------------------------------------------------------------------*/
void Weaver::vOp(Card* pCard, bool bCommented)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  s4K sOpArgs;

  eCK eCard = pCard->eCard();

  if (CK_c_COMBINED == eCard)
    return;

  if (CK_c_SEND == eCard)
    if (! AM::JOB::bHasTag(C_OPTIMISE_PASSTHRU))
      return;

  switch (eCard) {

    case CK_c_ABSJOIN:      vArg_absJoin      (sOpArgs, _pCard_AbsJoin(pCard));      break;
    case CK_o_AGGREG:       vArg_aggreg       (sOpArgs, _pCard_Aggreg(pCard));       break;
    case CK_o_CAST:         vArg_cast         (sOpArgs, _pCard_Cast(pCard));         break;
    case CK_o_CROSSJOIN:    vArg_crossJoin    (sOpArgs, _pCard_CrossJoin(pCard));    break;
    case CK_o_FILTER:       vArg_filter       (sOpArgs, _pCard_Filter(pCard));       break;
    case CK_o_GBYAGG:       vArg_gbyAgg       (sOpArgs, _pCard_GbyAgg(pCard));       break;
    case CK_o_HASH:         vArg_hash         (sOpArgs, _pCard_Hash(pCard));         break;
    case CK_o_HAVING:       vArg_having       (sOpArgs, _pCard_Having(pCard));       break;
    case CK_o_INDEXJOIN:    vArg_indexJoin    (sOpArgs, _pCard_IndexJoin(pCard));    break;
    case CK_o_INDEXSCAN:    vArg_indexScan    (sOpArgs, _pCard_IndexScan(pCard));    break;
    case CK_o_INFLATE:      vArg_inflate      (sOpArgs, _pCard_Inflate(pCard));      break;
    case CK_o_KEYEXISTS:    vArg_keyExists    (sOpArgs, _pCard_KeyExists(pCard));    break;
    case CK_o_KEYNOTEXISTS: vArg_keyNotExists (sOpArgs, _pCard_KeyNotExists(pCard)); break;
    case CK_o_LIMIT:        vArg_limit        (sOpArgs, _pCard_Limit(pCard));        break;
    case CK_o_ORDAGG:       vArg_ordAgg       (sOpArgs, _pCard_OrdAgg(pCard));       break;
    case CK_o_PROJECT:      vArg_project      (sOpArgs, _pCard_Project(pCard));      break;
    case CK_o_SCAN:         vArg_scan         (sOpArgs, _pCard_Scan(pCard));         break;
    case CK_c_SEND:         vArg_send         (sOpArgs, _pCard_Send(pCard));         break;
    case CK_o_SORT:         vArg_sort         (sOpArgs, _pCard_Sort(pCard));         break;
    case CK_s_TABLE:        vArg_table        (sOpArgs, _pCard_Table(pCard));        break;
    case CK_o_TOP:          vArg_top          (sOpArgs, _pCard_Top(pCard));          break;
    case CK_o_WHERE:        vArg_where        (sOpArgs, _pCard_Where(pCard));        break;

    default:
      vTrace("Missing Case [%s:%lld] : %lld %s\n"_Stz,
             __FILE__, (i8)__LINE__,(i8) eCard, aCK[eCard]);
      NEVER_REACH;
  }

  s32 sOpNm;
       if (CK_o_CAST == eCard) sOpNm = "next"_Stz;
  else if (CK_o_CROSSJOIN == eCard) sOpNm = "indexJoin"_Stz;
  else if (CK_o_HAVING == eCard) sOpNm = "filter"_Stz;
  else if (CK_c_SEND == eCard) sOpNm = "next"_Stz;
  else if (CK_s_TABLE == eCard) sOpNm = "scan"_Stz;
  else if (CK_o_WHERE == eCard) sOpNm = "filter"_Stz;
  else                             sOpNm = aCK_short[eCard];

  if (bCommented)
    while (StrLib::bReplace(sOpArgs.azChar(), sOpArgs.nAreaBytes(), "\n    ","\n  //"))
      {;}

  vTabOp(bCommented);
  vConcat_cmd("%-9s %s"_Stz,
              sOpNm.azChar(),
              sOpArgs.szStz());
  vEolOp();

  m_i_Op_stage++;


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (CK_o_INFLATE == eCard && pCard->pSymbol()->Y_bHas(P_FusedProject)) {
    Card_Project *p_project = new Card_Project(CK_o_PROJECT);
    p_project->vAppendChild(_pCard_Inflate(pCard)->pSub_emit()->pDupCardRecursive());
    vOp(p_project, true);
    delete p_project;
  }
}

/*----------------------------------------------------------------------------*/
/* weave : abstractJoin " 1 "                                                 */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_absJoin(String* psOpArgs, Card_AbsJoin*   pCard_AbsJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  UNUSED(pCard_AbsJoin);

  psOpArgs->vCatSprint("abstractJoin \" 1 \""_Stz);
}


/*----------------------------------------------------------------------------*/
/* weave : aggreg <compiled> <expr>                                           */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_aggreg(String* psOpArgs, Card_Aggreg* pCard_Aggreg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  // 0) aggElist : 1) gbyList

  s4K sRpnExpr;

  pCard_Aggreg->vRpn_aggreg(sRpnExpr, pCard_Aggreg);

  psOpArgs->vCatSprint("\"%s \""_Stz, sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave :next ?                                                              */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_cast(String* psOpArgs, Card_Cast* pCard_Next)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  if (pCard_Next->pSymbol()->bHasFacet(SF_STG_CAST))
    psOpArgs->vCatSprint("%s"_Stz, pCard_Next->szFacet(SF_STG_CAST));
}

/*----------------------------------------------------------------------------*/
/* weave : indexJoin <compiled> <tableNm> <indexNm> <expr>                    */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_crossJoin(String* psOpArgs, Card_CrossJoin* pCard_CrossJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // indexJoin : 0) {table} 1) {index} 2) indexPred

  s4K sRpnExpr;

  Card_TableNm* p_tableNm = pCard_CrossJoin->pSub_tableNm();
  Card_IndexNm* p_indexNm = pCard_CrossJoin->pSub_indexNm();

  pCard_CrossJoin->vRpn_crossJoin(sRpnExpr, pCard_CrossJoin);

  psOpArgs->vCatSprint("%s %s \"%s \""_Stz
      , p_tableNm->szFacet(SF_TBL_NM)
      , p_indexNm->szFacet(SF_IDX_NM)
      , sRpnExpr.szStz());

}

/*----------------------------------------------------------------------------*/
/* weave : filter <expr>                                                      */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_filter(String* psOpArgs, Card_Filter* pCard_Filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  // filter : 0) filtElist

  s4K sRpnExpr;

  //pCard_Filter->vDump();

  pCard_Filter->vRpn_filter(sRpnExpr, pCard_Filter);
  
  psOpArgs->vCatSprint("\"%s \""_Stz, sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : aggGby <compiled> <expr>                                           */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_gbyAgg(String* psOpArgs, Card_GbyAgg* pCard_AggGby)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // 0) aggElist : 1) gbyList

  //pCard_Aggreg->vDump();
  s4K sRpnExpr;

  //pCard_AggGby->vDump();


  pCard_AggGby->vRpn_gbyAgg(sRpnExpr, pCard_AggGby);

  psOpArgs->vCatSprint("\"%s \""_Stz, sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : hash  <expr>                                                       */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_hash(String* psOpArgs, Card_Hash* pCard_Hash)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  m_bSawHash = true;

  Card_HshCol* p_hshCol = pCard_Hash->pSub_hshCol();
  UNUSED(p_hshCol);

  Card_Term* p_term = pCard_Hash->pSub_term();

  psOpArgs->vCatSprint("\"type %s %s\""_Stz,
                       aCT[p_term->eCTVal()],
                       p_term->szFacet(SF_EXP_COL));
}

/*----------------------------------------------------------------------------*/
/* weave : having  <expr>                                                     */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_having(String* psOpArgs, Card_Having* pCard_Having)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // filter : 0) filtElist

  s4K sRpnExpr;

  pCard_Having->vRpn_having(sRpnExpr, pCard_Having);

  psOpArgs->vCatSprint("\"%s \""_Stz, sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : indexJoin <compiled> <tableNm> <indexNm> <expr>                    */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_indexJoin(String* psOpArgs, Card_IndexJoin* pCard_IndexJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // indexJoin : 0) {table} 1) {index} 2) indexPred

  s4K sRpnExpr;

  Card_TableNm *p_tableNm = pCard_IndexJoin->pSub_tableNm();

  Card_Index *p_index = _pCard_Index(pCard_IndexJoin->pFindPpyRecursive(P_ChosenIndex, CK_INDEX));
  if (!p_index) {
    NEVER_REACH
  }
  Card_IndexNm *p_indexNm = p_index->pSub_indexNm();

  pCard_IndexJoin->vRpn_indexJoin(sRpnExpr, pCard_IndexJoin);

  psOpArgs->vCatSprint("%s %s \"%s \""_Stz,
                       p_tableNm->szFacet(SF_TBL_NM), p_indexNm->szFacet(SF_IDX_NM), sRpnExpr.szStz());

  if (pCard_IndexJoin->pSub_constraint()) {
    s4K sRpnConstraint;
    pCard_IndexJoin->vRpn_indexJoin_constraint(sRpnConstraint, pCard_IndexJoin);
    psOpArgs->vCatSprint("\n"_Stz);
    psOpArgs->vCatSprint("                \"%s \""_Stz, sRpnConstraint.szStz());
  }

}

/*----------------------------------------------------------------------------*/
/* weave : indexScan <compiled> <tableNm> <indexNm> <expr>                    */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_indexScan(String* psOpArgs, Card_IndexScan* pCard_IndexScan)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  // indexScan : 0) {table} 1) {index} 2) indexPred

  Card_TableNm* p_tableNm = pCard_IndexScan->pSub_tableNm();

  Card_Index* p_index = _pCard_Index(pCard_IndexScan->pFindPpyRecursive(P_ChosenIndex,CK_INDEX));
  if (! p_index) {
    NEVER_REACH
  }


  Card_IndexNm* p_indexNm = p_index->pSub_indexNm();
  s4K sRpnExpr;

  pCard_IndexScan->vRpn_indexScan(sRpnExpr, pCard_IndexScan);
  
  psOpArgs->vCatSprint("%s %s\n"_Stz, p_tableNm->szFacet(SF_TBL_NM), p_indexNm->szFacet(SF_IDX_NM));
  psOpArgs->vCatSprint("                \"%s \""_Stz, sRpnExpr.szStz());

}

/*----------------------------------------------------------------------------*/
/* weave : inflate <tableNm>                                                  */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_inflate(String* psOpArgs, Card_Inflate* pCard_Inflate)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // scan : 0) {table}

  s4K sRpnExpr;

  pCard_Inflate->vRpn_inflate(sRpnExpr, pCard_Inflate);

  Card_TableNm* p_tableNm = pCard_Inflate->pSub_tableNm();

  Stz* szTableNm = p_tableNm->szFacet(SF_TBL_NM);
  //m_i_Op_stage++;

  psOpArgs->vCatSprint("%s"_Stz, szTableNm);

  //pCard_Inflate->vDump(__func__);
  if (! pCard_Inflate->pSymbol()->Y_bHas(P_FusedProject))
    return;

  psOpArgs->vCatSprint(" // fused project"_Stz);

}

/*----------------------------------------------------------------------------*/
/* weave : keyExists <tableNm> <indexNm> <keyExpr> <expr>                     */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_keyExists(String* psOpArgs, Card_KeyExists* pCard_KeyExists)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  Card_TableNm* p_tableNm = pCard_KeyExists->pSub_tableNm();
  Card_IndexNm* p_indexNm = pCard_KeyExists->pSub_indexNm();

  s4K sRpnExpr;
  pCard_KeyExists->vRpn_keyExists(sRpnExpr, pCard_KeyExists);

  s4K sRpnConstraint;
  if (pCard_KeyExists->pSub_constraint())
    pCard_KeyExists->vRpn_keyExists_constraint(sRpnConstraint, pCard_KeyExists);
  else
    sRpnConstraint = " type boolean 1 1 == "_Stz;

  psOpArgs->vCatSprint("%s %s \"%s \" \"%s \""_Stz
      , p_tableNm->szFacet(SF_TBL_NM)
      , p_indexNm->szFacet(SF_IDX_NM)
      , sRpnExpr.szStz()
      , sRpnConstraint.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : keyNotExists <tableNm> <indexNm> <keyExpr> <expr>                  */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_keyNotExists(String* psOpArgs, Card_KeyNotExists* pCard_KeyNotExists)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  Card_TableNm* p_tableNm = pCard_KeyNotExists->pSub_tableNm();
  Card_IndexNm* p_indexNm = pCard_KeyNotExists->pSub_indexNm();

  s4K sRpnExpr;
  pCard_KeyNotExists->vRpn_keyNotExists(sRpnExpr, pCard_KeyNotExists);

  s4K sRpnConstraint;
  if (pCard_KeyNotExists->pSub_constraint())
    pCard_KeyNotExists->vRpn_keyNotExists_constraint(sRpnConstraint, pCard_KeyNotExists);
  else
    sRpnConstraint = " type boolean 1 1 == "_Stz;

  psOpArgs->vCatSprint("%s %s \"%s \" \"%s \""_Stz
      , p_tableNm->szFacet(SF_TBL_NM)
      , p_indexNm->szFacet(SF_IDX_NM)
      , sRpnExpr.szStz()
      , sRpnConstraint.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : limit <compiled> <rows>                                            */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_limit(String* psOpArgs, Card_Limit* pCard_Limit)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  // limit : 0) rows

  s4K sRpnExpr;

  pCard_Limit->vRpn_limit(sRpnExpr, pCard_Limit);

  psOpArgs->vCatSprint("%s"_Stz, sRpnExpr.szStz());
}


/*----------------------------------------------------------------------------*/
/* weave : ordagg <compiled> <expr>                                           */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_ordAgg(String* psOpArgs, Card_OrdAgg* pCard_OrdAgg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  // ordagg : 0) aggElist

  s4K sRpnExpr;

  pCard_OrdAgg->vRpn_ordAgg(sRpnExpr, pCard_OrdAgg);

  psOpArgs->vCatSprint("\"%s \""_Stz, sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : project <compiled> <expr>                                          */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_project(String* psOpArgs, Card_Project* pCard_Project)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  // project : 0) projElist
  
  s4K sRpnExpr;

  pCard_Project->vRpn_project(sRpnExpr, pCard_Project);
  
  psOpArgs->vCatSprint("\"%s \""_Stz, sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : scan <tableNm>                                                     */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_scan(String* psOpArgs, Card_Scan* pCard_Scan)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  // scan : 0) {table}

  s4K sRpnExpr;

  pCard_Scan->vRpn_scan(sRpnExpr, pCard_Scan);

  Card_TableNm* p_tableNm = pCard_Scan->pSub_tableNm();
  
  Stz* szTableNm = p_tableNm->szFacet(SF_TBL_NM);
  //m_i_Op_stage++;
  
  psOpArgs->vCatSprint("%s"_Stz, szTableNm);
}

/*----------------------------------------------------------------------------*/
/* weave :send ?                                                              */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_send(String* psOpArgs, Card_Send* pCard_send)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

//  pCard_Send->vDump(__func__ );

  if (XK_BROADCAST == (eXK)pCard_send->pSymbol()->Y_iGet(P_SendKind))
    psOpArgs->vCatSprint("broadcast"_Stz);
  else
  if (XK_HASHCAST == (eXK)pCard_send->pSymbol()->Y_iGet(P_SendKind))
    psOpArgs->vCatSprint("hashcast"_Stz);

}

/*----------------------------------------------------------------------------*/
/* weave : sort ???                                                           */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_sort(String* psOpArgs, Card_Sort* pCard_Sort)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  // sort : 0) ???

  s4K sRpnExpr;

  pCard_Sort->vRpn_sort(sRpnExpr, pCard_Sort);

  psOpArgs->vCatSprint("\"%s \""_Stz, sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : table <tableNm>                                                    */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_table(String* psOpArgs, Card_Table* pCard_Table)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // scan : 0) {table}


  Card_TableNm* p_tableNm = pCard_Table->pSub_tableNm();

  Stz* szTableNm = p_tableNm->szFacet(SF_TBL_NM);


  psOpArgs->vCatSprint("%s"_Stz, szTableNm);
}

/*----------------------------------------------------------------------------*/
/* weave : top <expr> <count>                                                 */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_top(String* psOpArgs, Card_Top* pCard_op)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  //

  s4K sRpnExpr;

  pCard_op->vRpn_top(sRpnExpr, pCard_op);

  psOpArgs->vCatSprint("\"%s\" %s"_Stz,
                       sRpnExpr.szStz(),
                       pCard_op->pSub_count()->szFacet(SF_INT_LIT));
}

/*----------------------------------------------------------------------------*/
/* weave : where <expr>                                                       */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_where(String* psOpArgs, Card_Where* pCard_Where)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // filter : 0) filtElist

  s4K sRpnExpr;

  pCard_Where->vRpn_where(sRpnExpr, pCard_Where);

  psOpArgs->vCatSprint("\"%s \""_Stz, sRpnExpr.szStz());
}



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* concat formatted string                                                    */
/*----------------------------------------------------------------------------*/
void Weaver::vConcat_cmd(Stz* aFormat, ...)
{
  va_list xVarArgs;

  va_start(xVarArgs,aFormat);

  vConcat_cmd_vArg_s(aFormat, xVarArgs);
  
  va_end(xVarArgs);
}

/*----------------------------------------------------------------------------*/
/* concat formatted string                                                    */
/*----------------------------------------------------------------------------*/
void Weaver::vConcat_cmd_vArg_s(Stz* aFormat, va_list xArgs)
{
  static char caFormatted[C_1MB] = {'\0'};

  // produce formatted line
  StrLib::vSprint(caFormatted, sizeof(caFormatted) - 3, xArgs, aFormat->azChar());

  if (! m_pOptStmt)
    m_pOptStmt = new ExtendedText;

  m_pOptStmt->vConcat((Stz*)caFormatted);
}

/*----------------------------------------------------------------------------*/
/* end-of-line after stage                                                    */
/*----------------------------------------------------------------------------*/
void Weaver::vEolStage(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  vConcat_cmd("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* end-of-line after op                                                       */
/*----------------------------------------------------------------------------*/
void Weaver::vEolOp(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  vConcat_cmd("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* spaceing before stage                                                      */
/*----------------------------------------------------------------------------*/
void Weaver::vPreStage(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  vConcat_cmd("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* tab before op                                                              */
/*----------------------------------------------------------------------------*/
void Weaver::vTabOp(bool bCommented)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  if (m_i_Op_stage) {
    if (bCommented)
      vConcat_cmd("  //| "_Stz);
    else
    if (m_bOpPrinted)
      vConcat_cmd("    | "_Stz);
    else
      vConcat_cmd("      "_Stz);
  }
  else {
    if (bCommented)
      vConcat_cmd("  //  "_Stz);
    else
      vConcat_cmd("      "_Stz);
  }

  if (! bCommented)
    m_bOpPrinted = true;
}

/*----------------------------------------------------------------------------*/
/* tab before stage                                                           */
/*----------------------------------------------------------------------------*/
void Weaver::vTabStage(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  vConcat_cmd("  "_Stz);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* weave a revised/optimised statement from given board                       */
/*----------------------------------------------------------------------------*/
void Weaver::vWeaveOptStmt(Board* pBoard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  RASSERT(BK_SUBQUERY == pBoard->eBoard());

  m_i_Stage = 0;
  m_i_Op_stage = 0;

  m_pBoard = pBoard;

  m_pDeck = m_pBoard->pDeck();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vPipeline();
  vTags();
  vBegin();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vStageFirst();

  Kid xKid(m_pDeck);
  i8 nStages = m_pPipelineDef->nItems();
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    StageDef* pStageDef = m_pPipelineDef->pGet(i_Stage);
    Card_Stage* p_stage = pStageDef->pCard_Stage();

    vStageNext(p_stage);

    xKid.vReposition(p_stage);
    while (xKid.bEach()) {

      if (CK_c_STAGE == xKid.eChild())
        break;
      if (CK_o_CAST == xKid.eChild())
        break;
      if (CK_c_SEND == xKid.eChild())
        break;

      bool bCommented = true;
      if (xKid.pChild()->bIsOne(CK_o_SCAN, CK_s_TABLE, CK_NONE))
        bCommented = false;

      vOp(xKid.pChild(),bCommented);
    }

    vDynamic(pStageDef);

    if (CK_o_CAST == xKid.eChild())
      vOp(xKid.pChild());
    else
    if (CK_c_SEND == xKid.eChild())
      vOp(xKid.pChild());

  }

  vStageLast();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vEnd();
  vCppBlock();
  vSemicolon();

  if (m_pOptStmt)
    m_pOptStmt->vVerify();
  if (m_pCodeBlock)
    m_pCodeBlock->vVerify();

  if (AM::JOB::bHasTag(C_WEAVED)) {
    m_pOptStmt->vDump();
    vDebugPoint();
  }
  if (AM::JOB::bHasTag(C_CODE_BLOCK)) {
    if (m_pCodeBlock)
      m_pCodeBlock->vDump();
    vDebugPoint();
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
