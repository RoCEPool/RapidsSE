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
  
  if (CK_STAGE == eCard)
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
  vConcat_cmd("stage %s on %s : "_Stz, pCard_Stage->szSymbol(SF_NONE), aYG_weave[eStageTopo]);
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

  if (CK_COMBINED == eCard)
    return;

  if (CK_SEND == eCard)
    if (! AM::JOB::bHasTag(C_OPTIMISE_PASSTHRU))
      return;

  switch (eCard) {

    case CK_ABSJOIN:      vArgAbsJoin      (sOpArgs,_pCard_AbsJoin(pCard));      break;
    case CK_AGGREG:       vArgAggreg       (sOpArgs,_pCard_Aggreg(pCard));       break;
    case CK_CAST:         vArgCast         (sOpArgs,_pCard_Cast(pCard));         break;
    case CK_CROSSJOIN:    vArgCrossJoin    (sOpArgs,_pCard_CrossJoin(pCard));    break;
    case CK_FILTER:       vArgFilter       (sOpArgs,_pCard_Filter(pCard));       break;
    case CK_GBYAGG:       vArgGbyAgg       (sOpArgs,_pCard_GbyAgg(pCard));       break;
    case CK_HASH:         vArgHash         (sOpArgs,_pCard_Hash(pCard));         break;
    case CK_HAVING:       vArgHaving       (sOpArgs,_pCard_Having(pCard));       break;
    case CK_INDEXJOIN:    vArgIndexJoin    (sOpArgs,_pCard_IndexJoin(pCard));    break;
    case CK_INDEXSCAN:    vArgIndexScan    (sOpArgs,_pCard_IndexScan(pCard));    break;
    case CK_INFLATE:      vArgInflate      (sOpArgs,_pCard_Inflate(pCard));      break;
    case CK_KEYEXISTS:    vArgKeyExists    (sOpArgs,_pCard_KeyExists(pCard));    break;
    case CK_KEYNOTEXISTS: vArgKeyNotExists (sOpArgs,_pCard_KeyNotExists(pCard)); break;
    case CK_LIMIT:        vArgLimit        (sOpArgs,_pCard_Limit(pCard));        break;
    case CK_ORDAGG:       vArgOrdAgg       (sOpArgs,_pCard_OrdAgg(pCard));       break;
    case CK_PROJECT:      vArgProject      (sOpArgs,_pCard_Project(pCard));      break;
    case CK_SCAN:         vArgScan         (sOpArgs,_pCard_Scan(pCard));         break;
    case CK_SEND:         vArgSend         (sOpArgs,_pCard_Send(pCard));         break;
    case CK_SORT:         vArgSort         (sOpArgs,_pCard_Sort(pCard));         break;
    case CK_TABLE:        vArgTable        (sOpArgs,_pCard_Table(pCard));        break;
    case CK_TOP:          vArgTop          (sOpArgs,_pCard_Top(pCard));          break;
    case CK_WHERE:        vArgWhere        (sOpArgs,_pCard_Where(pCard));        break;

    default:
      vTrace("Missing Case [%s:%lld] : %lld %s\n"_Stz,
             __FILE__, (i8)__LINE__,(i8) eCard, aCK[eCard]);
      NEVER_REACH;
  }

  s32 sOpNm;
       if (CK_CAST == eCard)       sOpNm = "next"_Stz;
  else if (CK_CROSSJOIN == eCard)  sOpNm = "indexJoin"_Stz;
  else if (CK_HAVING == eCard)     sOpNm = "filter"_Stz;
  else if (CK_SEND == eCard)       sOpNm = "next"_Stz;
  else if (CK_TABLE == eCard)      sOpNm = "scan"_Stz;
  else if (CK_WHERE == eCard)      sOpNm = "filter"_Stz;
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

  if (CK_INFLATE ==eCard && pCard->pSymbol()->Y_bHas(P_FusedProject)) {
    Card_Project *p_project = new Card_Project(CK_PROJECT);
    p_project->vAppendChild(_pCard_Inflate(pCard)->pSub_projection()->pDupCardRecursive());
    vOp(p_project, true);
    delete p_project;
  }
}

/*----------------------------------------------------------------------------*/
/* weave : abstractJoin " 1 "                                                 */
/*----------------------------------------------------------------------------*/
void Weaver::vArgAbsJoin(String* psOpArgs, Card_AbsJoin*   pCard_AbsJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  UNUSED(pCard_AbsJoin);

  psOpArgs->vCatSprint("abstractJoin \" 1 \""_Stz);
}


/*----------------------------------------------------------------------------*/
/* weave : aggreg <compiled> <expr>                                           */
/*----------------------------------------------------------------------------*/
void Weaver::vArgAggreg(String* psOpArgs, Card_Aggreg* pCard_Aggreg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  // 0) aggElist : 1) gbyList

  s4K sRpnExpr;

  pCard_Aggreg->vRpnAggreg(sRpnExpr,pCard_Aggreg);

  psOpArgs->vCatSprint("\"%s \""_Stz, sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave :next ?                                                              */
/*----------------------------------------------------------------------------*/
void Weaver::vArgCast(String* psOpArgs, Card_Cast* pCard_Next)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  psOpArgs->vCatSprint("%s"_Stz, pCard_Next->szSymbol(SF_NONE));
}

/*----------------------------------------------------------------------------*/
/* weave : indexJoin <compiled> <tableNm> <indexNm> <expr>                    */
/*----------------------------------------------------------------------------*/
void Weaver::vArgCrossJoin(String* psOpArgs, Card_CrossJoin* pCard_CrossJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // indexJoin : 0) {table} 1) {index} 2) indexPred

  s4K sRpnExpr;

  Card_TableNm* p_tableNm = pCard_CrossJoin->pSub_tableNm();
  Card_IndexNm* p_indexNm = pCard_CrossJoin->pSub_indexNm();

  pCard_CrossJoin->vRpnCrossJoin(sRpnExpr,pCard_CrossJoin);

  psOpArgs->vCatSprint("%s %s \"%s \""_Stz
      , p_tableNm->szSymbol(SF_NONE)
      , p_indexNm->szSymbol(SF_NONE)
      , sRpnExpr.szStz());

}

/*----------------------------------------------------------------------------*/
/* weave : filter <expr>                                                      */
/*----------------------------------------------------------------------------*/
void Weaver::vArgFilter(String* psOpArgs, Card_Filter* pCard_Filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  // filter : 0) filtElist

  s4K sRpnExpr;

  //pCard_Filter->vDump();

  pCard_Filter->vRpnFilter(sRpnExpr,pCard_Filter);
  
  psOpArgs->vCatSprint("\"%s \""_Stz, sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : aggGby <compiled> <expr>                                           */
/*----------------------------------------------------------------------------*/
void Weaver::vArgGbyAgg(String* psOpArgs, Card_GbyAgg* pCard_AggGby)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // 0) aggElist : 1) gbyList

  //pCard_Aggreg->vDump();
  s4K sRpnExpr;

  //pCard_AggGby->vDump();


  pCard_AggGby->vRpnGbyAgg(sRpnExpr, pCard_AggGby);

  psOpArgs->vCatSprint("\"%s \""_Stz, sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : hash  <expr>                                                       */
/*----------------------------------------------------------------------------*/
void Weaver::vArgHash(String* psOpArgs, Card_Hash* pCard_Hash)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  m_bSawHash = true;

  Card_HshCol* p_hshCol = pCard_Hash->pSub_hshCol();
  UNUSED(p_hshCol);

  Card_Term* p_term = pCard_Hash->pSub_term();

  psOpArgs->vCatSprint("\"type %s %s\""_Stz,
                       aCT[Symbol::eTo_eCT_char(p_term->pSymbol()->Y_szColType())],
                       p_term->szSymbol(SF_NONE));
}

/*----------------------------------------------------------------------------*/
/* weave : having  <expr>                                                     */
/*----------------------------------------------------------------------------*/
void Weaver::vArgHaving(String* psOpArgs, Card_Having* pCard_Having)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // filter : 0) filtElist

  s4K sRpnExpr;

  pCard_Having->vRpnHaving(sRpnExpr,pCard_Having);

  psOpArgs->vCatSprint("\"%s \""_Stz, sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : indexJoin <compiled> <tableNm> <indexNm> <expr>                    */
/*----------------------------------------------------------------------------*/
void Weaver::vArgIndexJoin(String* psOpArgs, Card_IndexJoin* pCard_IndexJoin)
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

  pCard_IndexJoin->vRpnIndexJoin(sRpnExpr, pCard_IndexJoin);

  psOpArgs->vCatSprint("%s %s \"%s \""_Stz,
                       p_tableNm->szSymbol(SF_NONE), p_indexNm->szSymbol(SF_NONE), sRpnExpr.szStz());

  if (pCard_IndexJoin->pSub_constraint()) {
    s4K sRpnConstraint;
    pCard_IndexJoin->vRpnIndexJoin_constraint(sRpnConstraint, pCard_IndexJoin);
    psOpArgs->vCatSprint("\n"_Stz);
    psOpArgs->vCatSprint("                \"%s \""_Stz, sRpnConstraint.szStz());
  }

}

/*----------------------------------------------------------------------------*/
/* weave : indexScan <compiled> <tableNm> <indexNm> <expr>                    */
/*----------------------------------------------------------------------------*/
void Weaver::vArgIndexScan(String* psOpArgs, Card_IndexScan* pCard_IndexScan)
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

  pCard_IndexScan->vRpnIndexScan(sRpnExpr, pCard_IndexScan);
  
  psOpArgs->vCatSprint("%s %s\n"_Stz, p_tableNm->szSymbol(SF_NONE), p_indexNm->szSymbol(SF_NONE));
  psOpArgs->vCatSprint("                \"%s \""_Stz, sRpnExpr.szStz());

}

/*----------------------------------------------------------------------------*/
/* weave : inflate <tableNm>                                                  */
/*----------------------------------------------------------------------------*/
void Weaver::vArgInflate(String* psOpArgs, Card_Inflate* pCard_Inflate)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // scan : 0) {table}

  s4K sRpnExpr;

  pCard_Inflate->vRpnInflate(sRpnExpr,pCard_Inflate);

  Card_TableNm* p_tableNm = pCard_Inflate->pSub_tableNm();

  Stz* szTableNm = p_tableNm->szSymbol(SF_NONE);
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
void Weaver::vArgKeyExists(String* psOpArgs, Card_KeyExists* pCard_KeyExists)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  Card_TableNm* p_tableNm = pCard_KeyExists->pSub_tableNm();
  Card_IndexNm* p_indexNm = pCard_KeyExists->pSub_indexNm();

  s4K sRpnExpr;
  pCard_KeyExists->vRpnKeyExists(sRpnExpr,pCard_KeyExists);

  s4K sRpnConstraint;
  if (pCard_KeyExists->pSub_constraint())
    pCard_KeyExists->vRpnKeyExists_constraint(sRpnConstraint, pCard_KeyExists);
  else
    sRpnConstraint = " type boolean 1 1 == "_Stz;

  psOpArgs->vCatSprint("%s %s \"%s \" \"%s \""_Stz
      , p_tableNm->szSymbol(SF_NONE)
      , p_indexNm->szSymbol(SF_NONE)
      , sRpnExpr.szStz()
      , sRpnConstraint.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : keyNotExists <tableNm> <indexNm> <keyExpr> <expr>                  */
/*----------------------------------------------------------------------------*/
void Weaver::vArgKeyNotExists(String* psOpArgs, Card_KeyNotExists* pCard_KeyNotExists)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  Card_TableNm* p_tableNm = pCard_KeyNotExists->pSub_tableNm();
  Card_IndexNm* p_indexNm = pCard_KeyNotExists->pSub_indexNm();

  s4K sRpnExpr;
  pCard_KeyNotExists->vRpnKeyNotExists(sRpnExpr, pCard_KeyNotExists);

  s4K sRpnConstraint;
  if (pCard_KeyNotExists->pSub_constraint())
    pCard_KeyNotExists->vRpnKeyNotExists_constraint(sRpnConstraint, pCard_KeyNotExists);
  else
    sRpnConstraint = " type boolean 1 1 == "_Stz;

  psOpArgs->vCatSprint("%s %s \"%s \" \"%s \""_Stz
      , p_tableNm->szSymbol(SF_NONE)
      , p_indexNm->szSymbol(SF_NONE)
      , sRpnExpr.szStz()
      , sRpnConstraint.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : limit <compiled> <rows>                                            */
/*----------------------------------------------------------------------------*/
void Weaver::vArgLimit(String* psOpArgs, Card_Limit* pCard_Limit)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  // limit : 0) rows

  s4K sRpnExpr;

  pCard_Limit->vRpnLimit(sRpnExpr,pCard_Limit);

  psOpArgs->vCatSprint("%s"_Stz, sRpnExpr.szStz());
}


/*----------------------------------------------------------------------------*/
/* weave : ordagg <compiled> <expr>                                           */
/*----------------------------------------------------------------------------*/
void Weaver::vArgOrdAgg(String* psOpArgs, Card_OrdAgg* pCard_OrdAgg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  // ordagg : 0) aggElist

  s4K sRpnExpr;

  pCard_OrdAgg->vRpnOrdAgg(sRpnExpr, pCard_OrdAgg);

  psOpArgs->vCatSprint("\"%s \""_Stz, sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : project <compiled> <expr>                                          */
/*----------------------------------------------------------------------------*/
void Weaver::vArgProject(String* psOpArgs, Card_Project* pCard_Project)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  // project : 0) projElist
  
  s4K sRpnExpr;

  pCard_Project->vRpnProject(sRpnExpr,pCard_Project);
  
  psOpArgs->vCatSprint("\"%s \""_Stz, sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : scan <tableNm>                                                     */
/*----------------------------------------------------------------------------*/
void Weaver::vArgScan(String* psOpArgs, Card_Scan* pCard_Scan)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  // scan : 0) {table}

  s4K sRpnExpr;

  pCard_Scan->vRpnScan(sRpnExpr,pCard_Scan);

  Card_TableNm* p_tableNm = pCard_Scan->pSub_tableNm();
  
  Stz* szTableNm = p_tableNm->szSymbol(SF_NONE);
  //m_i_Op_stage++;
  
  psOpArgs->vCatSprint("%s"_Stz, szTableNm);
}

/*----------------------------------------------------------------------------*/
/* weave :send ?                                                              */
/*----------------------------------------------------------------------------*/
void Weaver::vArgSend(String* psOpArgs, Card_Send* pCard_send)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

//  pCard_Send->vDump(__func__ );

  if (XK_BROADCAST == (eXK)pCard_send->pSymbol()->Y_iGet(P_SendKind))
      psOpArgs->vCatSprint("%s broadcast"_Stz, pCard_send->szSymbol(SF_NONE));
  else
  if (XK_HASHCAST == (eXK)pCard_send->pSymbol()->Y_iGet(P_SendKind))
    psOpArgs->vCatSprint("%s hashcast"_Stz, pCard_send->szSymbol(SF_NONE));
  else
    psOpArgs->vCatSprint("%s"_Stz, pCard_send->szSymbol(SF_NONE));
}

/*----------------------------------------------------------------------------*/
/* weave : sort ???                                                           */
/*----------------------------------------------------------------------------*/
void Weaver::vArgSort(String* psOpArgs, Card_Sort* pCard_Sort)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  // sort : 0) ???

  s4K sRpnExpr;

  pCard_Sort->vRpnSort(sRpnExpr,pCard_Sort);

  psOpArgs->vCatSprint("\"%s \""_Stz, sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : table <tableNm>                                                    */
/*----------------------------------------------------------------------------*/
void Weaver::vArgTable(String* psOpArgs, Card_Table* pCard_Table)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // scan : 0) {table}


  Card_TableNm* p_tableNm = pCard_Table->pSub_tableNm();

  Stz* szTableNm = p_tableNm->szSymbol(SF_NONE);


  psOpArgs->vCatSprint("%s"_Stz, szTableNm);
}

/*----------------------------------------------------------------------------*/
/* weave : top <expr> <count>                                                 */
/*----------------------------------------------------------------------------*/
void Weaver::vArgTop(String* psOpArgs, Card_Top* pCard_op)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  //

  s4K sRpnExpr;

  pCard_op->vRpnTop(sRpnExpr, pCard_op);

  psOpArgs->vCatSprint("\"%s\" %s"_Stz,
                       sRpnExpr.szStz(),
                       pCard_op->pSub_count()->szSymbol(SF_NONE));
}

/*----------------------------------------------------------------------------*/
/* weave : where <expr>                                                       */
/*----------------------------------------------------------------------------*/
void Weaver::vArgWhere(String* psOpArgs, Card_Where* pCard_Where)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // filter : 0) filtElist

  s4K sRpnExpr;

  pCard_Where->vRpnWhere(sRpnExpr,pCard_Where);

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

  vConcat_cmd_vargs(aFormat, xVarArgs);
  
  va_end(xVarArgs);
}

/*----------------------------------------------------------------------------*/
/* concat formatted string                                                    */
/*----------------------------------------------------------------------------*/
void Weaver::vConcat_cmd_vargs(Stz* aFormat, va_list xArgs)
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

      if (CK_STAGE == xKid.eChild())
        break;
      if (CK_CAST == xKid.eChild())
        break;
      if (CK_SEND == xKid.eChild())
        break;

      bool bCommented = true;
      if (xKid.pChild()->bIsOne(CK_SCAN, CK_TABLE, CK_NONE))
        bCommented = false;

      vOp(xKid.pChild(),bCommented);
    }

    vDynamic(pStageDef);

    if (CK_CAST == xKid.eChild())
      vOp(xKid.pChild());
    else
    if (CK_SEND == xKid.eChild())
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
