/*============================================================================*/
/* Weaver2.cpp -- Weaver2 of Cards (for CS2)                                   */
/*============================================================================*/

#include "Weaver2.hpp"
#include "Optimiser.hpp"
#include "CardIter.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Weaver2, CN_WEAVER2)
__aNewFunction(Weaver2, CN_WEAVER2)
__vDeleteFunction(Weaver2, CN_WEAVER2)
__aFooterFunction(Weaver2)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Weaver2::Weaver2(Bench* pBench, PipelineDef* pPipelineDef)
: OptimiserTool(CN_WEAVER2)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);

  m_pPipelineDef  = pPipelineDef;
  m_pDeck = nullptr;

  m_pBench        = pBench;
  m_pOptimiser    = m_pBench->pOptimiser();
  m_pBoard        = nullptr;
  
  m_i_Stage    = 0;
  m_i_Op_stage = 0;
  m_bOpPrinted = false;
  p_pOptStmt   = nullptr;
  m_bSawHash   = false;


  _checkInitialised_(CN_WEAVER2);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Weaver2::~Weaver2(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);

  if (p_pOptStmt)
    delete p_pOptStmt;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* get optimised pipeline statement                                           */
/*----------------------------------------------------------------------------*/
Stz* Weaver2::szOptStmt(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);
  
  return p_pOptStmt->szTextBlock(0);
}

/*----------------------------------------------------------------------------*/
/* get optimised pipeline statement                                           */
/*----------------------------------------------------------------------------*/
ExtendedText* Weaver2::pOptStmt(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);

  return p_pOptStmt;
}

/*----------------------------------------------------------------------------*/
/* set optimised pipeline statement                                           */
/*----------------------------------------------------------------------------*/
void Weaver2::vOptStmt(ExtendedText* pOptSmt)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);

  p_pOptStmt = pOptSmt;
}



/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* weave : pipeline <name>                                                    */
/*----------------------------------------------------------------------------*/
void Weaver2::vPipeline(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);
  
  vConcat("pipeline %s\n"_Stz,m_pOptimiser->szQueryNm());
}

/*----------------------------------------------------------------------------*/
/* weave : tag ... endTag                                                     */
/*----------------------------------------------------------------------------*/
void Weaver2::vTags(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);
  
  vConcat("  tag <optimised> = 1 endTag\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* weave : begin                                                              */
/*----------------------------------------------------------------------------*/
void Weaver2::vBegin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);

  vConcat("begin\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* weave : end ;                                                              */
/*----------------------------------------------------------------------------*/
void Weaver2::vEnd(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);

  vConcat("\n"_Stz);
  vConcat("end\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* weave : __cpp_code__ ... __codeEnd__                                       */
/*----------------------------------------------------------------------------*/
void Weaver2::vCppBlock(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);

  bool bHasCode = false;

  i8 nStages = m_pPipelineDef->nItems();
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    StageDef *pStageDef = m_pPipelineDef->pGet(i_Stage);

    //vTrace("Code %s:\n%s\n"_Stz,pStageDef->szStageNm(),pStageDef->szCode());

    if (*pStageDef->szCode()->azChar()) {
      bHasCode = true;
      break;
    }
  }

  if (bHasCode) {

    vConcat("\n"_Stz);
    vConcat("__cpp_code__\n"_Stz);

    for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
      StageDef *pStageDef = m_pPipelineDef->pGet(i_Stage);
      vConcat("\n"_Stz);
      vConcat(pStageDef->szCode());
    }

    vConcat("\n"_Stz);
    vConcat("__codeEnd__\n"_Stz);

    vTrace("Code %s\n"_Stz, p_pOptStmt->szTextBlock(0));
    vTrace("Code %lld\n"_Stz, StrLib::nSize(p_pOptStmt->szTextBlock(0)->azChar()));

    vDebugPoint();
  }
}


/*----------------------------------------------------------------------------*/
/* weave : ;                                                                  */
/*----------------------------------------------------------------------------*/
void Weaver2::vSemicolon(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);

  vConcat("\n"_Stz);
  vConcat(";\n"_Stz);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* weave a card                                                               */
/*----------------------------------------------------------------------------*/
void Weaver2::vCard(Card* pCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);

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
void Weaver2::vStageFirst(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);
  
  vPreStage();
  vTabStage();
  vConcat("stage S%02lld on origin : "_Stz,m_i_Stage++);
  vEolStage();
  
  vTabOp();  vConcat("start"_Stz);           vEolOp();  m_i_Op_stage++;
  vTabOp();  vConcat("pulse     once"_Stz);  vEolOp();  m_i_Op_stage++;
  vTabOp();  vConcat("next"_Stz);            vEolOp();  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* weave : stage Snn on ??? :                                                 */
/*----------------------------------------------------------------------------*/
void Weaver2::vStageNext(Card_Stage* pCard_Stage)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);
  
  eYG eStageTopo = (eYG) pCard_Stage->pSub_topology()->pSymbol()->Y_iGet(P_StageTopo);
  
  //vTabOp();  vConcat("next"_Stz);  vEolOp();

  vPreStage();
  vTabStage();
  vConcat("stage %s on %s : "_Stz
          , pCard_Stage->szSymbol()
          ,aYG_weave[eStageTopo]);
  vEolStage();

  m_i_Stage++;
  
  m_i_Op_stage = 0;
  m_bOpPrinted = false;
  m_bSawHash   = false;
}

/*----------------------------------------------------------------------------*/
/* weave : stage Snn on origin :                                              */
/*----------------------------------------------------------------------------*/
void Weaver2::vStageLast(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);
  
  //vTabOp();  vConcat("next"_Stz);  vEolOp();
  
  vPreStage();
  vTabStage();
  vConcat("stage S%02lld on origin :"_Stz,m_i_Stage++);
  vEolStage();
  
  m_i_Op_stage = 0;
  
  vTabOp();  vConcat("output"_Stz);       vEolOp();  m_i_Op_stage++;
  vTabOp();  vConcat("sink"_Stz);         vEolOp();  m_i_Op_stage++;
  vTabOp();  vConcat("first"_Stz);        vEolOp();  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* weave : dynamic :                                                          */
/*----------------------------------------------------------------------------*/
void Weaver2::vDynamic(StageDef* pStageDef)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);

  vTabOp();  vConcat("dynamic"_Stz);      vEolOp();  m_i_Op_stage++;

  //if (AM::JOB::bHasTag(C_OPTIMISE_PASSTHRU)) {
    vTabOp();
    vConcat("next"_Stz);
    if (m_bSawHash)
      vConcat(" hashCast"_Stz);
    vEolOp();
    m_i_Op_stage++;
  //}

}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* weave a card                                                               */
/*----------------------------------------------------------------------------*/
void Weaver2::vOp(Card* pCard, bool bCommented)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);
  
  s4K sOpArgs;

  eCK eCard = pCard->eCard();

  if (CK_SEND == eCard)
    return;

  Card_OP* p_OP = (Card_OP*)pCard;

  switch (eCard) {

    case CK_ABSJOIN:      vArgAbsJoin      (sOpArgs,_pCard_AbsJoin(pCard));      break;
    case CK_AGGREG:       vArgAggreg       (sOpArgs,_pCard_Aggreg(pCard));       break;
    case CK_CAST:         vArgCast         (sOpArgs,_pCard_Cast(pCard));         break;
    case CK_COMBINED:     vArgCombined     (sOpArgs,_pCard_Combined(pCard));     break;
    case CK_CROSSJOIN:    vArgCrossJoin    (sOpArgs,_pCard_CrossJoin(pCard));    break;
    case CK_FILTER:       vArgFilter       (sOpArgs,_pCard_Filter(pCard));       break;
    case CK_GBYAGG:       vArgGbyAgg       (sOpArgs,_pCard_GbyAgg(pCard));       break;
    case CK_HASH:         vArgHash         (sOpArgs,_pCard_Hash(pCard));         break;
    case CK_HAVING:       vArgHaving       (sOpArgs,_pCard_Having(pCard));       break;
    case CK_INDEXJOIN:    vArgIndexJoin    (sOpArgs,_pCard_IndexJoin(pCard));    break;
    case CK_INDEXSCAN:    vArgIndexScan    (sOpArgs,_pCard_IndexScan(pCard));    break;
    case CK_KEYEXISTS:    vArgKeyExists    (sOpArgs,_pCard_KeyExists(pCard));    break;
    case CK_KEYNOTEXISTS: vArgKeyNotExists (sOpArgs,_pCard_KeyNotExists(pCard)); break;
    case CK_LIMIT:        vArgLimit        (sOpArgs,_pCard_Limit(pCard));        break;
    case CK_ORDAGG:       vArgOrdAgg       (sOpArgs,_pCard_OrdAgg(pCard));       break;
    case CK_PROJECT:      vArgProject      (sOpArgs,_pCard_Project(pCard));      break;
    case CK_SCAN:         vArgScan         (sOpArgs,_pCard_Scan(pCard));         break;
    case CK_SORT:         vArgSort         (sOpArgs,_pCard_Sort(pCard));         break;
    case CK_TABLE:        vArgTable        (sOpArgs,_pCard_Table(pCard));        break;
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
  else if (CK_TABLE == eCard)      sOpNm = "scan"_Stz;
  else if (CK_WHERE == eCard)      sOpNm = "filter"_Stz;
  else                             sOpNm = aCK_short[eCard];

  if (bCommented)
    while (StrLib::bReplace(sOpArgs.azChar(),"\n    ","\n  //"))
      {;}

  vTabOp(bCommented);
  vConcat("%-9s %s"_Stz,
          sOpNm.azChar(),
          sOpArgs.szStz());
  vEolOp();

  m_i_Op_stage++;
}
/*----------------------------------------------------------------------------*/
//void Weaver2::vOp(Card* pCard)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);
//
//  s4K sOpArgs;
//
//  eCK eCard = pCard->eCard();
//
//  switch (eCard) {
//
//    case CK_ABSJOIN:      vArgAbsJoin      (sOpArgs,_pCard_AbsJoin(pCard));      break;
//    case CK_AGGREG:       vArgAggreg       (sOpArgs,_pCard_Aggreg(pCard));       break;
//    case CK_CAST:         vArgCast         (sOpArgs,_pCard_Cast(pCard));         break;
//    case CK_COMBINED:     vArgCombined     (sOpArgs,_pCard_Combined(pCard));     break;
//    case CK_CROSSJOIN:    vArgCrossJoin    (sOpArgs,_pCard_CrossJoin(pCard));    break;
//    case CK_FILTER:       vArgFilter       (sOpArgs,_pCard_Filter(pCard));       break;
//    case CK_GBYAGG:       vArgGbyAgg       (sOpArgs,_pCard_GbyAgg(pCard));       break;
//    case CK_HASH:         vArgHash         (sOpArgs,_pCard_Hash(pCard));         break;
//    case CK_HAVING:       vArgHaving       (sOpArgs,_pCard_Having(pCard));       break;
//    case CK_INDEXJOIN:    vArgIndexJoin    (sOpArgs,_pCard_IndexJoin(pCard));    break;
//    case CK_INDEXSCAN:    vArgIndexScan    (sOpArgs,_pCard_IndexScan(pCard));    break;
//    case CK_KEYEXISTS:    vArgKeyExists    (sOpArgs,_pCard_KeyExists(pCard));    break;
//    case CK_KEYNOTEXISTS: vArgKeyNotExists (sOpArgs,_pCard_KeyNotExists(pCard)); break;
//    case CK_LIMIT:        vArgLimit        (sOpArgs,_pCard_Limit(pCard));        break;
//    case CK_ORDAGG:       vArgOrdAgg       (sOpArgs,_pCard_OrdAgg(pCard));       break;
//    case CK_PROJECT:      vArgProject      (sOpArgs,_pCard_Project(pCard));      break;
//    case CK_SCAN:         vArgScan         (sOpArgs,_pCard_Scan(pCard));         break;
//    case CK_SORT:         vArgSort         (sOpArgs,_pCard_Sort(pCard));         break;
//    case CK_WHERE:        vArgWhere        (sOpArgs,_pCard_Where(pCard));        break;
//
//    default:
//      vTrace("Missing Case [%s:%lld] : %lld %s\n"_Stz,
//             __FILE__, (i8)__LINE__,(i8) eCard, aCK[eCard]);
//      NEVER_REACH;
//  }
//
//  s32 sOpNm;
//  if (CK_CAST == eCard)       sOpNm = "next"_Stz;
//  else if (CK_CROSSJOIN == eCard)  sOpNm = "indexJoin"_Stz;
//  else if (CK_HAVING == eCard)     sOpNm = "filter"_Stz;
//  else if (CK_WHERE == eCard)      sOpNm = "filter"_Stz;
//  else                             sOpNm = aCK_short[eCard];
//
//  vTabOp();
//  vConcat("%-9s %s"_Stz,
//          sOpNm.azChar(),
//          sOpArgs.szStz());
//  vEolOp();
//
//  m_i_Op_stage++;
//}

/*----------------------------------------------------------------------------*/
/* weave : abstractJoin " 1 "                                                 */
/*----------------------------------------------------------------------------*/
void Weaver2::vArgAbsJoin(Stz* szOpArgs, Card_AbsJoin*   pCard_AbsJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);

  szOpArgs->vCatSprint("abstractJoin \" 1 \""_Stz);
}


/*----------------------------------------------------------------------------*/
/* weave : aggreg <compiled> <expr>                                           */
/*----------------------------------------------------------------------------*/
void Weaver2::vArgAggreg(Stz* szOpArgs, Card_Aggreg* pCard_Aggreg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);
  
  // 0) aggElist : 1) gbyList

  s4K sRpnExpr;

  pCard_Aggreg->vRpnAggreg(sRpnExpr,pCard_Aggreg);

  szOpArgs->vCatSprint("\"%s \""_Stz,sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave :next ?                                                              */
/*----------------------------------------------------------------------------*/
void Weaver2::vArgCast(Stz* szOpArgs, Card_Cast* pCard_Next)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);

  szOpArgs->vCatSprint("%s"_Stz, pCard_Next->szSymbol());
}

/*----------------------------------------------------------------------------*/
/* weave : combine 0                                                          */
/*----------------------------------------------------------------------------*/
void Weaver2::vArgCombined(Stz* szOpArgs, Card_Combined* pCard_Combine)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);
    
  // 0) combined :

  s4K sRpnExpr;

  pCard_Combine->vRpnCombined(sRpnExpr, pCard_Combine);

  szOpArgs->vCatSprint("%s"_Stz,sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : indexJoin <compiled> <tableNm> <indexNm> <expr>                    */
/*----------------------------------------------------------------------------*/
void Weaver2::vArgCrossJoin(Stz* szOpArgs, Card_CrossJoin* pCard_CrossJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);

  // indexJoin : 0) {table} 1) {index} 2) indexPred

  s4K sRpnExpr;

  Card_TableNm* p_tableNm = pCard_CrossJoin->pSub_tableNm();
  Card_IndexNm* p_indexNm = pCard_CrossJoin->pSub_indexNm();

  pCard_CrossJoin->vRpnCrossJoin(sRpnExpr,pCard_CrossJoin);

  szOpArgs->vCatSprint("%s %s \"%s \""_Stz
      , p_tableNm->szSymbol()
      , p_indexNm->szSymbol()
      ,sRpnExpr.szStz());

}

/*----------------------------------------------------------------------------*/
/* weave : filter <expr>                                                      */
/*----------------------------------------------------------------------------*/
void Weaver2::vArgFilter(Stz* szOpArgs, Card_Filter* pCard_Filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);
  
  // filter : 0) filtElist

  s4K sRpnExpr;

  //pCard_Filter->vDump();

  pCard_Filter->vRpnFilter(sRpnExpr,pCard_Filter);
  
  szOpArgs->vCatSprint("\"%s \""_Stz,sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : aggGby <compiled> <expr>                                           */
/*----------------------------------------------------------------------------*/
void Weaver2::vArgGbyAgg(Stz* szOpArgs, Card_GbyAgg* pCard_AggGby)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);

  // 0) aggElist : 1) gbyList

  //pCard_Aggreg->vDump();
  s4K sRpnExpr;

  //pCard_AggGby->vDump();


  pCard_AggGby->vRpnGbyAgg(sRpnExpr, pCard_AggGby);

  szOpArgs->vCatSprint("\"%s \""_Stz,sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : hash  <expr>                                                       */
/*----------------------------------------------------------------------------*/
void Weaver2::vArgHash(Stz* szOpArgs, Card_Hash* pCard_Hash)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);

  m_bSawHash = true;

  Card_HshCol* p_hshCol = pCard_Hash->pSub_hshCol();

  szOpArgs->vCatSprint("\"type %s %s\""_Stz,
                       aCT[Symbol::eTo_eCT_char(p_hshCol->pSymbol()->Y_szGet(P_Type))],
                       p_hshCol->szSymbol());
}

/*----------------------------------------------------------------------------*/
/* weave : having  <expr>                                                     */
/*----------------------------------------------------------------------------*/
void Weaver2::vArgHaving(Stz* szOpArgs, Card_Having* pCard_Having)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);

  // filter : 0) filtElist

  s4K sRpnExpr;

  pCard_Having->vRpnHaving(sRpnExpr,pCard_Having);

  szOpArgs->vCatSprint("\"%s \""_Stz,sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : indexJoin <compiled> <tableNm> <indexNm> <expr>                    */
/*----------------------------------------------------------------------------*/
void Weaver2::vArgIndexJoin(Stz* szOpArgs, Card_IndexJoin* pCard_IndexJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);

  // indexJoin : 0) {table} 1) {index} 2) indexPred

  s4K sRpnExpr;

  Card_TableNm* p_tableNm = pCard_IndexJoin->pSub_tableNm();

  Card_Index* p_index = _pCard_Index(pCard_IndexJoin->pFindPpyRecursive(P_ChosenIndex,CK_INDEX));
  if (! p_index) {
    NEVER_REACH
  }
  Card_IndexNm* p_indexNm = p_index->pSub_indexNm();

  pCard_IndexJoin->vRpnIndexJoin(sRpnExpr, p_index);

  if (Card_OuterJoin* p_outerJoin = pCard_IndexJoin->pSub_outerJoin()) {
    if (p_outerJoin->bIsSymbol("left_outer"_Stz))
      szOpArgs->vCatSprint("%s %s \"LEFT_OUTER %s \""_Stz,
                           p_tableNm->szSymbol(), p_indexNm->szSymbol(), sRpnExpr.szStz());
  }
  else {
    szOpArgs->vCatSprint("%s %s \"%s \""_Stz,
                         p_tableNm->szSymbol(), p_indexNm->szSymbol(), sRpnExpr.szStz());
  }
}

/*----------------------------------------------------------------------------*/
/* weave : indexScan <compiled> <tableNm> <indexNm> <expr>                    */
/*----------------------------------------------------------------------------*/
void Weaver2::vArgIndexScan(Stz* szOpArgs, Card_IndexScan* pCard_IndexScan)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);
  
  // indexScan : 0) {table} 1) {index} 2) indexPred

  Card_TableNm* p_tableNm = pCard_IndexScan->pSub_tableNm();

  Card_Index* p_index = _pCard_Index(pCard_IndexScan->pFindPpyRecursive(P_ChosenIndex,CK_INDEX));
  if (! p_index) {
    NEVER_REACH
  }


  Card_IndexNm* p_indexNm = p_index->pSub_indexNm();
  s4K sRpnExpr;

  pCard_IndexScan->vRpnIndexScan(sRpnExpr, p_index);
  
  szOpArgs->vCatSprint("%s %s\n"_Stz, p_tableNm->szSymbol(), p_indexNm->szSymbol());
  szOpArgs->vCatSprint("                \"%s \""_Stz,sRpnExpr.szStz());

}

/*----------------------------------------------------------------------------*/
/* weave : keyExists <tableNm> <indexNm> <keyExpr> <expr>                     */
/*----------------------------------------------------------------------------*/
void Weaver2::vArgKeyExists(Stz* szOpArgs, Card_KeyExists* pCard_KeyExists)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);

  Card_TableNm* p_tableNm = pCard_KeyExists->pSub_tableNm();
  Card_IndexNm* p_indexNm = pCard_KeyExists->pSub_indexNm();

  s4K sRpnExpr;
  pCard_KeyExists->vRpnKeyExists(sRpnExpr,pCard_KeyExists);

  s4K sRpnConstraint;
  if (pCard_KeyExists->pSub_constraint())
    pCard_KeyExists->vRpnKeyExists_constraint(sRpnConstraint, pCard_KeyExists);
  else
    sRpnConstraint = " type boolean 1 1 == "_Stz;

  szOpArgs->vCatSprint("%s %s \"%s \" \"%s \""_Stz
      , p_tableNm->szSymbol()
      , p_indexNm->szSymbol()
      ,sRpnExpr.szStz()
      ,sRpnConstraint.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : keyNotExists <tableNm> <indexNm> <keyExpr> <expr>                  */
/*----------------------------------------------------------------------------*/
void Weaver2::vArgKeyNotExists(Stz* szOpArgs, Card_KeyNotExists* p_keyNotExists)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);

  Card_TableNm* p_tableNm = p_keyNotExists->pSub_tableNm();
  Card_IndexNm* p_indexNm = p_keyNotExists->pSub_indexNm();

  s4K sRpnExpr;
  p_keyNotExists->vRpnKeyNotExists(sRpnExpr,p_keyNotExists);

  s4K sRpnConstraint;
  if (p_keyNotExists->pSub_constraint())
    p_keyNotExists->vRpnKeyNotExists_constraint(sRpnConstraint, p_keyNotExists);
  else
    sRpnConstraint = " type boolean 1 1 == "_Stz;

  szOpArgs->vCatSprint("%s %s \"%s \" \"%s \""_Stz
      , p_tableNm->szSymbol()
      , p_indexNm->szSymbol()
      ,sRpnExpr.szStz()
      ,sRpnConstraint.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : limit <compiled> <rows>                                            */
/*----------------------------------------------------------------------------*/
void Weaver2::vArgLimit(Stz* szOpArgs, Card_Limit* pCard_Limit)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);
  
  // limit : 0) rows

  s4K sRpnExpr;

  pCard_Limit->vRpnLimit(sRpnExpr,pCard_Limit);

  szOpArgs->vCatSprint("%s"_Stz,sRpnExpr.szStz());
}


/*----------------------------------------------------------------------------*/
/* weave : ordagg <compiled> <expr>                                           */
/*----------------------------------------------------------------------------*/
void Weaver2::vArgOrdAgg(Stz* szOpArgs, Card_OrdAgg* pCard_OrdAgg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);
  
  // ordagg : 0) aggElist

  s4K sRpnExpr;

  pCard_OrdAgg->vRpnOrdAgg(sRpnExpr, pCard_OrdAgg);

  szOpArgs->vCatSprint("\"%s \""_Stz,sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : project <compiled> <expr>                                          */
/*----------------------------------------------------------------------------*/
void Weaver2::vArgProject(Stz* szOpArgs, Card_Project* pCard_Project)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);
  
  // project : 0) projElist
  
  s4K sRpnExpr;

  pCard_Project->vRpnProject(sRpnExpr,pCard_Project);
  
  szOpArgs->vCatSprint("\"%s \""_Stz,sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : scan <tableNm>                                                     */
/*----------------------------------------------------------------------------*/
void Weaver2::vArgScan(Stz* szOpArgs, Card_Scan* pCard_Scan)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);
  
  // scan : 0) {table}

  s4K sRpnExpr;

  pCard_Scan->vRpnScan(sRpnExpr,pCard_Scan);

  Card_TableNm* p_tableNm = pCard_Scan->pSub_tableNm();
  
  Stz* szTableNm = p_tableNm->szSymbol();
  //m_i_Op_stage++;
  
  szOpArgs->vCatSprint("%s"_Stz,szTableNm);
}

/*----------------------------------------------------------------------------*/
/* weave : sort ???                                                           */
/*----------------------------------------------------------------------------*/
void Weaver2::vArgSort(Stz* szOpArgs, Card_Sort* pCard_Sort)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);
  
  // sort : 0) ???

  s4K sRpnExpr;

  pCard_Sort->vRpnSort(sRpnExpr,pCard_Sort);

  szOpArgs->vCatSprint("\"%s \""_Stz,sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : table <tableNm>                                                    */
/*----------------------------------------------------------------------------*/
void Weaver2::vArgTable(Stz* szOpArgs, Card_Table* pCard_Table)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);

  // scan : 0) {table}

  s4K sRpnExpr;

  pCard_Table->vRpnTable(sRpnExpr,pCard_Table);

  Card_TableNm* p_tableNm = pCard_Table->pSub_tableNm();

  Stz* szTableNm = p_tableNm->szSymbol();
  //m_i_Op_stage++;

  szOpArgs->vCatSprint("%s"_Stz,szTableNm);
}

/*----------------------------------------------------------------------------*/
/* weave : where <expr>                                                       */
/*----------------------------------------------------------------------------*/
void Weaver2::vArgWhere(Stz* szOpArgs, Card_Where* pCard_Where)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);

  // filter : 0) filtElist

  s4K sRpnExpr;

  pCard_Where->vRpnWhere(sRpnExpr,pCard_Where);

  szOpArgs->vCatSprint("\"%s \""_Stz,sRpnExpr.szStz());
}



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* concat formatted string                                                    */
/*----------------------------------------------------------------------------*/
void Weaver2::vConcat(Stz* aFormat, ...)
{
  va_list xVarArgs;

  va_start(xVarArgs,aFormat);

  vConcat_vargs(aFormat,xVarArgs);
  
  va_end(xVarArgs);
}

/*----------------------------------------------------------------------------*/
/* concat formatted string                                                    */
/*----------------------------------------------------------------------------*/
void Weaver2::vConcat_vargs(Stz* aFormat, va_list xArgs)
{
  static char caFormatted[C_1MB] = {'\0'};

  // produce formatted line
  vsnprintf(caFormatted,sizeof(caFormatted) - 3,aFormat->azChar(),xArgs);

  if (! p_pOptStmt)
    p_pOptStmt = new ExtendedText;

  p_pOptStmt->vConcat((Stz*)caFormatted);

}

/*----------------------------------------------------------------------------*/
/* end-of-line after stage                                                    */
/*----------------------------------------------------------------------------*/
void Weaver2::vEolStage(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);
  
  vConcat("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* end-of-line after op                                                       */
/*----------------------------------------------------------------------------*/
void Weaver2::vEolOp(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);
  
  vConcat("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* spaceing before stage                                                      */
/*----------------------------------------------------------------------------*/
void Weaver2::vPreStage(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);
  
  vConcat("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* tab before op                                                              */
/*----------------------------------------------------------------------------*/
void Weaver2::vTabOp(bool bCommented)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);
  
  if (m_i_Op_stage) {
    if (bCommented)
      vConcat("  //| "_Stz);
    else
    if (m_bOpPrinted)
      vConcat("    | "_Stz);
    else
      vConcat("      "_Stz);
  }
  else {
    if (bCommented)
      vConcat("  //  "_Stz);
    else
      vConcat("      "_Stz);
  }

  if (! bCommented)
    m_bOpPrinted = true;
}

/*----------------------------------------------------------------------------*/
/* tab before stage                                                           */
/*----------------------------------------------------------------------------*/
void Weaver2::vTabStage(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);
  
  vConcat("  "_Stz);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* weave a revised/optimised statement from given board                       */
/*----------------------------------------------------------------------------*/
void Weaver2::vWeaveOptStmt(Board* pBoard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);

#if 1
  vWeaveOptStmt2(pBoard);
  return;
#else
  RASSERT(BK_SUBQUERY == pBoard->eBoardKind());

  m_i_Stage = 0;
  m_i_Op_stage = 0;
  m_sOptStmt = ""_Stz;
  
  m_pBoard = pBoard;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vPipeline();
  vTags();
  vBegin();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vStageFirst();

  CardIter xCI(m_pBoard->pDeck());
  while (xCI.G_bEachMain())
    vCard(xCI.G.p_MainCard);

  vStageLast();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vEnd();
  vCppBlock();
  vSemicolon();

  //vTrace("%s\n"_Stz, szOptStmt());
#endif
}
/*----------------------------------------------------------------------------*/
void Weaver2::vWeaveOptStmt2(Board* pBoard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER2, _FOOTER_);

  RASSERT(BK_SUBQUERY == pBoard->eBoardKind());

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

  CardIter xCI(m_pDeck);
  i8 nStages = m_pPipelineDef->nItems();
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    StageDef* pStageDef = m_pPipelineDef->pGet(i_Stage);
    Card_Stage* p_stage = pStageDef->pCard_Stage();

    vStageNext(p_stage);

    if (* pStageDef->szCode()->azChar()) {

      xCI.G_vMainResynch(p_stage);
      while (xCI.G_bEachMain()) {

        if (CK_STAGE == xCI.G.e_MainCard)
          break;

        if (xCI.G.p_MainCard->bIsOne(CK_AGGREG, CK_INDEXJOIN, CK_INDEXSCAN,
                                     CK_LIMIT , CK_SCAN     , CK_TABLE,
                                     CK_NONE)) {
          vOp(xCI.G.p_MainCard);
        }
        else
        if (CK_CAST == xCI.G.e_MainCard ) {
          //xCI.G_vMainResynch(xCI.pPrevChild());
          break;
        }
        else {
          vOp(xCI.G.p_MainCard,/*commented*/true);
        }
      }

      vDynamic(pStageDef);

//      if (xCI.bIsActive() && xCI.G_bEachMain(CK_CAST)) {
//        vOp(xCI.G.p_MainCard);
//      }
    }
  }

  vStageLast();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vEnd();
  vCppBlock();
  vSemicolon();

  if (AM::JOB::bHasTag(C_WEAVED)) {
    vTrace("%s\n"_Stz, szOptStmt());
    vDebugPoint();
  }
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
