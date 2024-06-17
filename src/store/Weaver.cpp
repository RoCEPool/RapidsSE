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
Weaver::Weaver(CardIter* pCI, Bench* pBench, PipelineDef* pPipelineDef)
: OptimiserTool(CN_WEAVER)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  m_pCI          = pCI;
  m_pPipelineDef = pPipelineDef;
  m_pDeck        = nullptr;

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
/* weave : dynamic :                                                          */
/*----------------------------------------------------------------------------*/
void Weaver::vPtnSend(void)
{
	_precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

	vTabOp();
	vConcat_cmd("ptnSendSE"_Stz);      vEolOp();
	//vTabOp();
}

/*----------------------------------------------------------------------------*/
/* weave : dynamic :                                                          */
/*----------------------------------------------------------------------------*/
void Weaver::vPtnConsume(i8 iThreads)
{
	_precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

	vTabOp();
	vConcat_cmd("ptnConsume %lld"_Stz, iThreads);      vEolOp();
	//vTabOp();
}

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
  
  if (CK_o_STAGE == eCard)
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
void Weaver::vStageNext(Card_Stage* p_stage)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  eYG eStageTopo = (eYG) p_stage->pSub_topology()->pSymbol()->Y_iGet(P_StageTopo);

  //vTabOp();  vConcat_cmd("next"_Stz);  vEolOp();

	i8 iThreads = p_stage->pSymbol()->Y_iGet(P_Threads);

  vPreStage();
  vTabStage();

	if (iThreads > 1)
		vConcat_cmd("stage %s on %s multiple threads %lld : "_Stz, p_stage->szFacet(SF_STG_NM), aYG_weave[eStageTopo], iThreads);
	else
    vConcat_cmd("stage %s on %s : "_Stz, p_stage->szFacet(SF_STG_NM), aYG_weave[eStageTopo]);

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

  if (CK_o_SEND == eCard)
    if (! AM::JOB::bHasTag(C_OPTIMISE_PASSTHRU))
      return;

  switch (eCard) {

    case CK_o_ABSJOIN:      vArg_absJoin      (sOpArgs, _pCard_AbsJoin(pCard));      break;
    case CK_o_AGGREG:       vArg_aggreg       (sOpArgs, _pCard_Aggreg(pCard));       break;
    case CK_o_CAST:         vArg_cast         (sOpArgs, _pCard_Cast(pCard));         break;
    case CK_o_CROSSJOIN:    vArg_crossJoin    (sOpArgs, _pCard_CrossJoin(pCard));    break;
    case CK_o_FILTER:       vArg_filter       (sOpArgs, _pCard_Filter(pCard));       break;
    case CK_o_GBYAGG:       vArg_gbyAgg       (sOpArgs, _pCard_GbyAgg(pCard));       break;
    case CK_o_GLOBALHASH:   vArg_globalHash   (sOpArgs, _pCard_GlobalHash(pCard));   break;
    case CK_o_HASH:         vArg_hash         (sOpArgs, _pCard_Hash(pCard));         break;
    case CK_o_HAVING:       vArg_having       (sOpArgs, _pCard_Having(pCard));       break;
    case CK_o_INDEXJOIN:    vArg_indexJoin    (sOpArgs, _pCard_IndexJoin(pCard));    break;
    case CK_o_INDEXSCAN:    vArg_indexScan    (sOpArgs, _pCard_IndexScan(pCard));    break;
    case CK_o_INFLATE:      vArg_inflate      (sOpArgs, _pCard_Inflate(pCard));      break;
    case CK_o_KEYEXISTS:    vArg_keyExists    (sOpArgs, _pCard_KeyExists(pCard));    break;
    case CK_o_KEYNOTEXISTS: vArg_keyNotExists (sOpArgs, _pCard_KeyNotExists(pCard)); break;
    case CK_o_LIMIT:        vArg_limit        (sOpArgs, _pCard_Limit(pCard));        break;
    case CK_o_ORDAGG:       vArg_ordAgg       (sOpArgs, _pCard_OrdAgg(pCard));       break;
    case CK_o_PDSROW:       vArg_pdsRow       (sOpArgs, _pCard_PdsRow(pCard));       break;
    case CK_o_PROJECT:      vArg_project      (sOpArgs, _pCard_Project(pCard));      break;
    case CK_o_RRFJOIN:      vArg_rrfJoin      (sOpArgs, _pCard_RrfJoin(pCard));      break;
    case CK_o_SCAN:         vArg_scan         (sOpArgs, _pCard_Scan(pCard));         break;
    case CK_o_SEND:         vArg_send         (sOpArgs, _pCard_Send(pCard));         break;
    case CK_o_SORT:         vArg_sort         (sOpArgs, _pCard_Sort(pCard));         break;
    case CK_p_TABLE:        vArg_table        (sOpArgs, _pCard_Table(pCard));        break;
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
  else if (CK_o_SEND == eCard) sOpNm = "next"_Stz;
  else if (CK_p_TABLE == eCard) sOpNm = "scan"_Stz;
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
void Weaver::vArg_absJoin(String* psOpArgs, Card_AbsJoin*   p_absJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  UNUSED(p_absJoin);

  psOpArgs->vCatSprint("abstractJoin \" 1 \""_Stz);
}


/*----------------------------------------------------------------------------*/
/* weave : aggreg <compiled> <expr>                                           */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_aggreg(String* psOpArgs, Card_Aggreg* p_aggreg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // 0) aggElist : 1) gbyList

  s4K sRpnExpr;

  p_aggreg->vRpn_aggreg(m_pCI, sRpnExpr, p_aggreg);

  psOpArgs->vCatSprint("\"%s \""_Stz, sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave :next ?                                                              */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_cast(String* psOpArgs, Card_Cast* p_next)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  if (p_next->pSymbol()->bHasFacet(SF_STG_CAST))
    psOpArgs->vCatSprint("%s"_Stz, p_next->szFacet(SF_STG_CAST));
}

/*----------------------------------------------------------------------------*/
/* weave : indexJoin <compiled> <tableNm> <indexNm> <expr>                    */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_crossJoin(String* psOpArgs, Card_CrossJoin* p_crossJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // indexJoin : 0) {table} 1) {index} 2) indexPred

  s4K sRpnExpr;

  Card_TableNm* p_tableNm = p_crossJoin->pSub_tableNm();
  Card_IndexNm* p_indexNm = p_crossJoin->pSub_indexNm();

  p_crossJoin->vRpn_crossJoin(m_pCI, sRpnExpr, p_crossJoin);

  psOpArgs->vCatSprint("%s %s \"%s \""_Stz
      , p_tableNm->szFacet(SF_TBL_NM)
      , p_indexNm->szFacet(SF_IDX_NM)
      , sRpnExpr.szStz());

}

/*----------------------------------------------------------------------------*/
/* weave : filter <expr>                                                      */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_filter(String* psOpArgs, Card_Filter* p_filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // filter : 0) filtElist

  s4K sRpnExpr;

  //p_filter->vDump();

  p_filter->vRpn_filter(m_pCI, sRpnExpr, p_filter);

  psOpArgs->vCatSprint("\"%s \""_Stz, sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : aggGby <compiled> <expr>                                           */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_gbyAgg(String* psOpArgs, Card_GbyAgg* p_aggGby)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // 0) aggElist : 1) gbyList

  //p_aggreg->vDump();
  s4K sRpnExpr;

  //p_aggGby->vDump();


  p_aggGby->vRpn_gbyAgg(m_pCI, sRpnExpr, p_aggGby);

  psOpArgs->vCatSprint("\"%s \""_Stz, sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : globalHash ...                                                     */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_globalHash(String* psOpArgs, Card_GlobalHash* p_globalHash)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  UNUSED(psOpArgs);
  UNUSED(p_globalHash);
}

/*----------------------------------------------------------------------------*/
/* weave : hash  <expr>                                                       */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_hash(String* psOpArgs, Card_Hash* p_hash)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  m_bSawHash = true;

  Card_HshCol* p_hshCol = p_hash->pSub_hshCol();
  UNUSED(p_hshCol);

  Card_Term* p_term = p_hash->pSub_term();

  psOpArgs->vCatSprint("\"type %s %s\""_Stz,
                       aCT[p_term->eCTVal()],
                       p_term->szFacet(SF_EXP_COL));
}

/*----------------------------------------------------------------------------*/
/* weave : having  <expr>                                                     */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_having(String* psOpArgs, Card_Having* p_having)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // filter : 0) filtElist

  s4K sRpnExpr;

  p_having->vRpn_having(m_pCI, sRpnExpr, p_having);

  psOpArgs->vCatSprint("\"%s \""_Stz, sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : indexJoin <compiled> <tableNm> <indexNm> <expr>                    */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_indexJoin(String* psOpArgs, Card_IndexJoin* p_indexJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // indexJoin : 0) {table} 1) {index} 2) indexPred

  s4K sRpnExpr;

  Card_TableNm *p_tableNm = p_indexJoin->pSub_tableNm();

  Card_Index *p_index = _pCard_Index(p_indexJoin->pFindPpyRecursive(P_ChosenIndex, CK_INDEX));
  if (!p_index) {
    NEVER_REACH
  }
  Card_IndexNm *p_indexNm = p_index->pSub_indexNm();

  p_indexJoin->vRpn_indexJoin(m_pCI, sRpnExpr, p_indexJoin);

  psOpArgs->vCatSprint("%s %s \"%s \""_Stz,
                       p_tableNm->szFacet(SF_TBL_NM), p_indexNm->szFacet(SF_IDX_NM), sRpnExpr.szStz());

  if (p_indexJoin->pSub_constraint()) {
    s4K sRpnConstraint;
    p_indexJoin->vRpn_indexJoin_constraint(m_pCI, sRpnConstraint, p_indexJoin);
    psOpArgs->vCatSprint("\n"_Stz);
    psOpArgs->vCatSprint("                \"%s \""_Stz, sRpnConstraint.szStz());
  }

}

/*----------------------------------------------------------------------------*/
/* weave : indexScan <compiled> <tableNm> <indexNm> <expr>                    */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_indexScan(String* psOpArgs, Card_IndexScan* p_indexScan)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // indexScan : 0) {table} 1) {index} 2) indexPred

  Card_TableNm* p_tableNm = p_indexScan->pSub_tableNm();

  Card_Index* p_index = _pCard_Index(p_indexScan->pFindPpyRecursive(P_ChosenIndex,CK_INDEX));
  if (! p_index) {
    NEVER_REACH
  }


  Card_IndexNm* p_indexNm = p_index->pSub_indexNm();
  s4K sRpnExpr;

  p_indexScan->vRpn_indexScan(m_pCI, sRpnExpr, p_indexScan);

  psOpArgs->vCatSprint("%s %s\n"_Stz, p_tableNm->szFacet(SF_TBL_NM), p_indexNm->szFacet(SF_IDX_NM));
  psOpArgs->vCatSprint("                \"%s \""_Stz, sRpnExpr.szStz());

}

/*----------------------------------------------------------------------------*/
/* weave : inflate <tableNm>                                                  */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_inflate(String* psOpArgs, Card_Inflate* p_inflate)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // scan : 0) {table}

  s4K sRpnExpr;

  p_inflate->vRpn_inflate(m_pCI, sRpnExpr, p_inflate);

  Card_TableNm* p_tableNm = p_inflate->pSub_tableNm();

  Stz* szTableNm = p_tableNm->szFacet(SF_TBL_NM);
  //m_i_Op_stage++;

  psOpArgs->vCatSprint("%s"_Stz, szTableNm);

  //p_inflate->vDump(__func__);
  if (! p_inflate->pSymbol()->Y_bHas(P_FusedProject))
    return;

  psOpArgs->vCatSprint(" // fused project"_Stz);

}

/*----------------------------------------------------------------------------*/
/* weave : keyExists <tableNm> <indexNm> <keyExpr> <expr>                     */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_keyExists(String* psOpArgs, Card_KeyExists* p_keyExists)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  Card_TableNm* p_tableNm = p_keyExists->pSub_tableNm();
  Card_IndexNm* p_indexNm = p_keyExists->pSub_indexNm();

  s4K sRpnExpr;
  p_keyExists->vRpn_keyExists(m_pCI, sRpnExpr, p_keyExists);

  s4K sRpnConstraint;
  if (p_keyExists->pSub_constraint())
    p_keyExists->vRpn_keyExists_constraint(m_pCI, sRpnConstraint, p_keyExists);
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
void Weaver::vArg_keyNotExists(String* psOpArgs, Card_KeyNotExists* p_keyNotExists)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  Card_TableNm* p_tableNm = p_keyNotExists->pSub_tableNm();
  Card_IndexNm* p_indexNm = p_keyNotExists->pSub_indexNm();

  s4K sRpnExpr;
  p_keyNotExists->vRpn_keyNotExists(m_pCI, sRpnExpr, p_keyNotExists);

  s4K sRpnConstraint;
  if (p_keyNotExists->pSub_constraint())
    p_keyNotExists->vRpn_keyNotExists_constraint(m_pCI, sRpnConstraint, p_keyNotExists);
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
void Weaver::vArg_limit(String* psOpArgs, Card_Limit* p_limit)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // limit : 0) rows

  s4K sRpnExpr;

  p_limit->vRpn_limit(m_pCI, sRpnExpr, p_limit);

  psOpArgs->vCatSprint("%s"_Stz, sRpnExpr.szStz());
}


/*----------------------------------------------------------------------------*/
/* weave : ordagg <compiled> <expr>                                           */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_ordAgg(String* psOpArgs, Card_OrdAgg* p_ordAgg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // ordagg : 0) aggElist

  s4K sRpnExpr;

  p_ordAgg->vRpn_ordAgg(m_pCI, sRpnExpr, p_ordAgg);

  psOpArgs->vCatSprint("\"%s \""_Stz, sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : pdsRow                                                             */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_pdsRow(String* psOpArgs, Card_PdsRow* p_pdsRow)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  UNUSED(psOpArgs);
  UNUSED(p_pdsRow);
}

/*----------------------------------------------------------------------------*/
/* weave : project <compiled> <expr>                                          */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_project(String* psOpArgs, Card_Project* p_project)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // project : 0) projElist

  s4K sRpnExpr;

  p_project->vRpn_project(m_pCI, sRpnExpr, p_project);

  psOpArgs->vCatSprint("\"%s \""_Stz, sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : rrfJoin ...                                                        */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_rrfJoin(String* psOpArgs, Card_RrfJoin* p_rrfJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  UNUSED(psOpArgs);
  UNUSED(p_rrfJoin);
}

/*----------------------------------------------------------------------------*/
/* weave : scan <tableNm>                                                     */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_scan(String* psOpArgs, Card_Scan* p_scan)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // scan : 0) {table}

  s4K sRpnExpr;

  p_scan->vRpn_scan(m_pCI, sRpnExpr, p_scan);

  Card_TableNm* p_tableNm = p_scan->pSub_tableNm();

  Stz* szTableNm = p_tableNm->szFacet(SF_TBL_NM);
  //m_i_Op_stage++;

  psOpArgs->vCatSprint("%s"_Stz, szTableNm);
}

/*----------------------------------------------------------------------------*/
/* weave :send ?                                                              */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_send(String* psOpArgs, Card_Send* p_send)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

//  p_send->vDump(__func__ );

  if (XK_BROADCAST == (eXK)p_send->pSymbol()->Y_iGet(P_SendKind))
    psOpArgs->vCatSprint("broadcast"_Stz);
  else
  if (XK_HASHCAST == (eXK)p_send->pSymbol()->Y_iGet(P_SendKind))
    psOpArgs->vCatSprint("hashcast"_Stz);

}

/*----------------------------------------------------------------------------*/
/* weave : sort ???                                                           */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_sort(String* psOpArgs, Card_Sort* p_sort)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // sort : 0) ???

  s4K sRpnExpr;

  p_sort->vRpn_sort(m_pCI, sRpnExpr, p_sort);

  psOpArgs->vCatSprint("\"%s \""_Stz, sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : table <tableNm>                                                    */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_table(String* psOpArgs, Card_Table* p_table)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // scan : 0) {table}


  Card_TableNm* p_tableNm = p_table->pSub_tableNm();

  Stz* szTableNm = p_tableNm->szFacet(SF_TBL_NM);


  psOpArgs->vCatSprint("%s"_Stz, szTableNm);
}

/*----------------------------------------------------------------------------*/
/* weave : top <expr> <count>                                                 */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_top(String* psOpArgs, Card_Top* p_op)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  //

  s4K sRpnExpr;

  p_op->vRpn_top(m_pCI, sRpnExpr, p_op);

  psOpArgs->vCatSprint("\"%s\" %s"_Stz,
                       sRpnExpr.szStz(),
                       p_op->pSub_count()->szFacet(SF_INT_LIT));
}

/*----------------------------------------------------------------------------*/
/* weave : where <expr>                                                       */
/*----------------------------------------------------------------------------*/
void Weaver::vArg_where(String* psOpArgs, Card_Where* p_where)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // filter : 0) filtElist

  s4K sRpnExpr;

  p_where->vRpn_where(m_pCI, sRpnExpr, p_where);

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
  char caFormatted[C_1MB] = {'\0'};

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

  Kid xDeck(m_pDeck);
  i8 nStages = m_pPipelineDef->nItems();
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    StageDef* pStageDef = m_pPipelineDef->pGet(i_Stage);
    Card_Stage* p_stage = pStageDef->pCard_Stage();

    vStageNext(p_stage);

    xDeck.vReposition(p_stage);
    while (xDeck.bEach()) {

      if (CK_o_STAGE == xDeck.eChild())
        break;
      if (CK_o_CAST == xDeck.eChild())
        break;
      if (CK_o_SEND == xDeck.eChild())
        break;

      bool bCommented = true;
      if (xDeck.pChild()->bEither(CK_o_SCAN, CK_p_TABLE, CK_NONE))
        bCommented = false;

      vOp(xDeck.pChild(), bCommented);
    }

		i8 iThreads = p_stage->pSymbol()->Y_iGet(P_Threads);
		if (iThreads > 1)
			vPtnSend();

    vDynamic(pStageDef);

		if (iThreads > 1)
			vPtnConsume(iThreads);

    if (CK_o_CAST == xDeck.eChild())
      vOp(xDeck.pChild());
    else
    if (CK_o_SEND == xDeck.eChild())
      vOp(xDeck.pChild());

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
