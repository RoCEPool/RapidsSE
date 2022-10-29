/*============================================================================*/
/* Weaver.cpp -- Weaver of Cards                                                */
/*============================================================================*/

#include "Weaver.hpp"
#include "Optimiser.hpp"


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
Weaver::Weaver(Bench* pBench)
: OptimiserTool(CN_WEAVER)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  m_pBench        = pBench;
  m_pOptimiser    = m_pBench->pOptimiser();
  m_pBoard        = nullptr;
  
  m_i_Stage = 0;
  m_i_Op_stage = 0;
  
  _checkInitialised_(CN_WEAVER);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Weaver::~Weaver(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* get optimised pipeline statement                                           */
/*----------------------------------------------------------------------------*/
Stz* Weaver::szOptStmt(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  return m_sOptStmt;
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
  
  vConcat("pipeline %s\n"_Stz,m_pOptimiser->szQueryNm());
}

/*----------------------------------------------------------------------------*/
/* weave : tag ... endTag                                                     */
/*----------------------------------------------------------------------------*/
void Weaver::vTags(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  vConcat("  tag <optimised> = 1 endTag\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* weave : begin                                                              */
/*----------------------------------------------------------------------------*/
void Weaver::vBegin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  vConcat("begin\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* weave : end ;                                                              */
/*----------------------------------------------------------------------------*/
void Weaver::vEnd(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  vConcat("\n"_Stz);
  vConcat("end ;\n"_Stz);
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
  vConcat("stage S%02lld on origin : "_Stz,m_i_Stage++);
  vEolStage();
  
  vTabOp();  vConcat("start"_Stz);           vEolOp();  m_i_Op_stage++;
  vTabOp();  vConcat("pulse     once"_Stz);  vEolOp();  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* weave : stage Snn on ??? :                                                 */
/*----------------------------------------------------------------------------*/
void Weaver::vStageNext(Card_Stage* pCard_Stage)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  eYG eStageTopo = (eYG) pCard_Stage->pSub_topology()->Y_iVal(P_StageTopo);
  
  vTabOp();  vConcat("next"_Stz);  vEolOp();

  vPreStage();
  vTabStage();
  vConcat("stage %s on %s : "_Stz
          ,pCard_Stage->sz()
          ,aYG_weave[eStageTopo]);
  vEolStage();

  m_i_Stage++;
  
  m_i_Op_stage = 0;
}

/*----------------------------------------------------------------------------*/
/* weave : stage Snn on origin :                                              */
/*----------------------------------------------------------------------------*/
void Weaver::vStageLast(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  vTabOp();  vConcat("next"_Stz);  vEolOp();
  
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
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* weave a card                                                               */
/*----------------------------------------------------------------------------*/
void Weaver::vOp(Card* pCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  s4K sOpArgs;

  eCK eCard = pCard->eCard();
  
  switch (eCard) {

    case CK_ABSJOIN:    vArgAbsJoin  (sOpArgs,_pCard_AbsJoin(pCard));   break;
    case CK_AGGREG:     vArgAggreg   (sOpArgs,_pCard_Aggreg(pCard));    break;
    case CK_COMBINED:   vArgCombined (sOpArgs,_pCard_Combined(pCard));  break;
    case CK_FILTER:     vArgFilter   (sOpArgs,_pCard_Filter(pCard));    break;
    case CK_HAVING:     vArgHaving   (sOpArgs, _pCard_Having(pCard));   break;
    case CK_INDEXJOIN:  vArgIndexJoin(sOpArgs,_pCard_IndexJoin(pCard)); break;
    case CK_INDEXSCAN:  vArgIndexScan(sOpArgs,_pCard_IndexScan(pCard)); break;
    case CK_KEYEXISTS:  vArgKeyExists(sOpArgs,_pCard_KeyExists(pCard)); break;
    case CK_LIMIT:      vArgLimit    (sOpArgs,_pCard_Limit(pCard));     break;
    case CK_PROJECT:    vArgProject  (sOpArgs,_pCard_Project(pCard));   break;
    case CK_SCAN:       vArgScan     (sOpArgs,_pCard_Scan(pCard));      break;
    case CK_SORT:       vArgSort     (sOpArgs,_pCard_Sort(pCard));      break;
    case CK_WHERE:      vArgWhere    (sOpArgs,_pCard_Where(pCard));     break;

    default:
      vTrace("  skipping %s\n"_Stz,aCK[eCard]);
      return;
  }
  
  vTabOp();
  vConcat("%-9s %s"_Stz,
          CK_WHERE == eCard ? "filter"_Stz : aCK_short[eCard],
          sOpArgs.szStz());
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* weave : abstractJoin " 1 "                                                 */
/*----------------------------------------------------------------------------*/
void Weaver::vArgAbsJoin(Stz* szOpArgs, Card_AbsJoin*   pCard_AbsJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  szOpArgs->vCatSprint("abstractJoin \" 1 \""_Stz);
}

/*----------------------------------------------------------------------------*/
/* weave : aggreg <compiled> <expr>                                           */
/*----------------------------------------------------------------------------*/
void Weaver::vArgAggreg(Stz* szOpArgs, Card_Aggreg* pCard_Aggreg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  // 0) aggElist : 1) gbyList

  //pCard_Aggreg->vDump();
  s4K sRpnExpr;
  
  vRpnAggreg(sRpnExpr,pCard_Aggreg);

  szOpArgs->vCatSprint("\"%s \""_Stz,sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : combine 0                                                          */
/*----------------------------------------------------------------------------*/
void Weaver::vArgCombined(Stz* szOpArgs, Card_Combined* pCard_Combine)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
    
  // 0) combined :

  s4K sRpnExpr;

  vRpnCombined(sRpnExpr, pCard_Combine);

  szOpArgs->vCatSprint("%s"_Stz,sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : filter <expr>                                                      */
/*----------------------------------------------------------------------------*/
void Weaver::vArgFilter(Stz* szOpArgs, Card_Filter* pCard_Filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  // filter : 0) filtElist

  s4K sRpnExpr;
  
  vRpnFilter(sRpnExpr,pCard_Filter);
  
  szOpArgs->vCatSprint("\"%s \""_Stz,sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : having  <expr>                                                     */
/*----------------------------------------------------------------------------*/
void Weaver::vArgHaving(Stz* szOpArgs, Card_Having* pCard_Having)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // filter : 0) filtElist

  s4K sRpnExpr;

  vRpnHaving(sRpnExpr,pCard_Having);

  szOpArgs->vCatSprint("\"%s \""_Stz,sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : indexJoin <compiled> <tableNm> <indexNm> <expr>                    */
/*----------------------------------------------------------------------------*/
void Weaver::vArgIndexJoin(Stz* szOpArgs, Card_IndexJoin* pCard_IndexJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  // indexJoin : 0) {table} 1) {index} 2) indexPred

  //pCard_IndexJoin->vDump();

  s4K sRpnExpr;

//  Card_IndexPtr* pCard_IndexPtr = _pCard_IndexPtr(pCard_IndexJoin->pFindSubCard(CK_INDEXPTR));
//
//  Index* pIndex = pCard_IndexPtr->pIndex();
//  Stz* szIndexNm = pIndex->szIndexNm();
//
//  Card_TablePtr* pCard_TablePtr = _pCard_TablePtr(pCard_IndexJoin->pFindSubCard(CK_TABLEPTR));
//  Table* pTable = pCard_TablePtr->pTable();
//  Stz* szTableNm = pTable->szTableNm();
//
//  //Card_IndexPred* pCard_IndexPred = _pCard_IndexPred(pCard_IndexJoin->pFindSubCard(CK_INDEXPRED));

  Card_TableNm* p_tableNm = pCard_IndexJoin->pSub_tableNm();
  Card_IndexNm* p_indexNm = pCard_IndexJoin->pSub_indexNm();

  vRpnIndexJoin(sRpnExpr,pCard_IndexJoin);

  szOpArgs->vCatSprint("%s %s \"%s \""_Stz
                       ,p_tableNm->szValue()
                       ,p_indexNm->szValue()
                       ,sRpnExpr.szStz());

}

/*----------------------------------------------------------------------------*/
/* weave : indexScan <compiled> <tableNm> <indexNm> <expr>                    */
/*----------------------------------------------------------------------------*/
void Weaver::vArgIndexScan(Stz* szOpArgs, Card_IndexScan* pCard_IndexScan)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  // indexScan : 0) {table} 1) {index} 2) indexPred

  Card_TableNm* p_tableNm = pCard_IndexScan->pSub_tableNm();

  //Index* pIndex = pCard_IndexScan->pSub_indexPtr()->pIndex();

  Card_IndexNm* p_indexNm = pCard_IndexScan->pSub_indexNm();
  s4K sRpnExpr;

  vRpnIndexScan(sRpnExpr, pCard_IndexScan);
  
  szOpArgs->vCatSprint("%s %s\n"_Stz,p_tableNm->szValue(),p_indexNm->szValue());
  szOpArgs->vCatSprint("                \"%s \""_Stz,sRpnExpr.szStz());

}

/*----------------------------------------------------------------------------*/
/* weave : keyExists <tableNm> <indexNm> <keyExpr> <expr>                     */
/*----------------------------------------------------------------------------*/
void Weaver::vArgKeyExists(Stz* szOpArgs, Card_KeyExists* pCard_KeyExists)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  Card_TableNm* p_tableNm = pCard_KeyExists->pSub_tableNm();
  Card_IndexNm* p_indexNm = pCard_KeyExists->pSub_indexNm();

  s4K sRpnExpr;
  vRpnKeyExists(sRpnExpr,pCard_KeyExists);

  s4K sRpnTest = " type boolean 1 1 == "_Stz;

  szOpArgs->vCatSprint("%s %s \"%s \" \"%s \""_Stz
      ,p_tableNm->szValue()
      ,p_indexNm->szValue()
      ,sRpnExpr.szStz()
      ,sRpnTest.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : limit <compiled> <rows>                                            */
/*----------------------------------------------------------------------------*/
void Weaver::vArgLimit(Stz* szOpArgs, Card_Limit* pCard_Limit)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  // limit : 0) rows

  //pCard_Limit->vDump();

  s4K sRpnExpr;
  
  vRpnLimit(sRpnExpr,pCard_Limit);

  szOpArgs->vCatSprint("%s"_Stz,sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : ordagg <compiled> <expr>                                           */
/*----------------------------------------------------------------------------*/
void Weaver::vArgOrdAgg(Stz* szOpArgs, Card_Aggreg* pCard_Aggreg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  // ordagg : 0) aggElist

  s4K sRpnExpr;
  
  vRpnAggreg(sRpnExpr,pCard_Aggreg);

  szOpArgs->vCatSprint("\"%s \""_Stz,sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : project <compiled> <expr>                                          */
/*----------------------------------------------------------------------------*/
void Weaver::vArgProject(Stz* szOpArgs, Card_Project* pCard_Project)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  // project : 0) projElist
  
  s4K sRpnExpr;
  
  vRpnProject(sRpnExpr,pCard_Project);
  
  szOpArgs->vCatSprint("\"%s \""_Stz,sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : scan <tableNm>                                                     */
/*----------------------------------------------------------------------------*/
void Weaver::vArgScan(Stz* szOpArgs, Card_Scan* pCard_Scan)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  // scan : 0) {table}

  s4K sRpnExpr;
  
  vRpnScan(sRpnExpr,pCard_Scan);

  Card_TableNm* p_tableNm = pCard_Scan->pSub_tableNm();
  
  Stz* szTableNm = p_tableNm->szValue();
  //m_i_Op_stage++;
  
  szOpArgs->vCatSprint("%s"_Stz,szTableNm);
}

/*----------------------------------------------------------------------------*/
/* weave : sort ???                                                           */
/*----------------------------------------------------------------------------*/
void Weaver::vArgSort(Stz* szOpArgs, Card_Sort* pCard_Sort)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  // sort : 0) ???

  //pCard_Sort->vDump();
  
  s4K sRpnExpr;
  
  vRpnSort(sRpnExpr,pCard_Sort);

  szOpArgs->vCatSprint("\"%s \""_Stz,sRpnExpr.szStz());
}

/*----------------------------------------------------------------------------*/
/* weave : where <expr>                                                       */
/*----------------------------------------------------------------------------*/
void Weaver::vArgWhere(Stz* szOpArgs, Card_Where* pCard_Where)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  // filter : 0) filtElist

  s4K sRpnExpr;

  vRpnWhere(sRpnExpr,pCard_Where);

  szOpArgs->vCatSprint("\"%s \""_Stz,sRpnExpr.szStz());
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* produce <expr> for aggreg op                                               */
/*----------------------------------------------------------------------------*/
void Weaver::vRpnAggreg(Stz* szRpnExpr, Card_Aggreg* pCard_Aggreg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  i8 nSubCards;

  Card_GbyElist* pCard_GbyElist =
          _pCard_GbyElist(pCard_Aggreg->pFindSubCard(CK_GBYELIST));

  if (! pCard_GbyElist)
    return;

  szRpnExpr->vConcat(" groupBy"_Stz);
  szRpnExpr->vConcat("\n                   "_Stz);

  nSubCards = pCard_GbyElist->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pCard_SUB = pCard_GbyElist->pGet(i_Child);
    if (! pCard_SUB->bIsCard(CK_EXPR))
      return;

    if (i_Child) {
      szRpnExpr->vConcat("\n                 "_Stz);
      szRpnExpr->vConcat(" ,"_Stz);
    }

    Card_Expr* pCard_Expr = _pCard_Expr(pCard_SUB);
    vExpr(szRpnExpr,pCard_Expr);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -



  Card_AggElist* pCard_AggElist =
    _pCard_AggElist(pCard_Aggreg->pFindSubCard(CK_AGGELIST));

  if (! pCard_AggElist)
    return;

  szRpnExpr->vConcat("\n                 "_Stz);
  szRpnExpr->vConcat(" aggregates"_Stz);
  szRpnExpr->vConcat("\n                   "_Stz);

  nSubCards = pCard_AggElist->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pCard_SUB = pCard_AggElist->pGet(i_Child);
    if (! pCard_SUB->bIsCard(CK_EXPR))
      return;
    
    if (i_Child) {
      szRpnExpr->vConcat("\n                 "_Stz);
      szRpnExpr->vConcat(" ,"_Stz);
    }

    Card_Expr* pCard_Expr = _pCard_Expr(pCard_SUB);
    vExpr(szRpnExpr,pCard_Expr);
  }
  

}

/*----------------------------------------------------------------------------*/
/* expr for combine                                                           */
/*----------------------------------------------------------------------------*/
void Weaver::vRpnCombined(Stz* szRpnExpr, Card_Combined* pCard_Combined)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  Card_Term* pCard_Term = _pCard_Term(pCard_Combined->pGet(0));
  szRpnExpr->vCatSprint("%s"_Stz,pCard_Term->sz());
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for filter op                                               */
/*----------------------------------------------------------------------------*/
void Weaver::vRpnFilter(Stz* szRpnExpr, Card_Filter* pCard_Filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  //pCard_Filter->vDump();

  szRpnExpr->vCatSprint(" type BOOLEAN"_Stz);

  Card* pCard_test = pCard_Filter->pSub_boolCalc();
  if (CK_BOOLCALC == pCard_test->eCard()) {
    Card_BoolCalc *pCard_CalcBool = _pCard_BoolCalc(pCard_test);
    vBoolCalc(szRpnExpr, pCard_CalcBool);
  }
  else {
    Card_RpnCalc *pCard_Calc = pCard_Filter->pSub_rpnCalc();
    vRpnCalc(szRpnExpr, pCard_Calc);
  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for having op                                               */
/*----------------------------------------------------------------------------*/
void Weaver::vRpnHaving(Stz* szRpnExpr, Card_Having* pCard_Having)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  //pCard_Having->vDump();

  Card_Expr* pCard_Expr = _pCard_Expr(pCard_Having->pGet(0));

  Card_Type* pCard_Type = _pCard_Type(pCard_Expr->pGet(0));
  vType(szRpnExpr,pCard_Type);

  Card_RpnCalc* pCard_Calc = _pCard_RpnCalc(pCard_Expr->pGet(1));
  vRpnCalc(szRpnExpr, pCard_Calc);

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexJoin op                                            */
/*----------------------------------------------------------------------------*/
void Weaver::vRpnIndexJoin(Stz* szRpnExpr, Card_IndexJoin* pCard_IndexJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  Card_Bounds* pCard_joinPred = pCard_IndexJoin->pSub_bounds();
  if (pCard_joinPred) {
    i8 nSubs = pCard_joinPred->nChildCards();
    for (i8 i_Sub = 0; i_Sub < nSubs; i_Sub++) {
      Card* pSubCard = pCard_joinPred->pGet(i_Sub);

      if (! pSubCard->bIsCard(CK_TERM))
        continue;

      Card_Term* p_term = _pCard_Term(pSubCard);
      szRpnExpr->vCatSprint(" %s"_Stz,p_term->sz());
    }
    return;
  }

  Card_IndexPred* pCard_IndexPred = _pCard_IndexPred(pCard_IndexJoin->pFindSubCard(CK_INDEXPRED));

  if (! pCard_IndexPred) {
    szRpnExpr->vConcat(" <<no: indexPred>>"_Stz);
    return;
  }

  i8 nSubs = pCard_IndexPred->nChildCards();
  for (i8 i_Sub = 0; i_Sub < nSubs; i_Sub++) {
    Card* pSubCard = pCard_IndexPred->pGet(i_Sub);

    if (! pSubCard->bIsCard(CK_TERM))
      continue;

    Card_Term* p_term = _pCard_Term(pSubCard);
    szRpnExpr->vCatSprint(" %s"_Stz,p_term->sz());
  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexJoin op                                            */
/*----------------------------------------------------------------------------*/
void Weaver::vRpnKeyExists(Stz* szRpnExpr, Card_KeyExists* pCard_KeyExists)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  Card_Bounds *pCard_joinPred = pCard_KeyExists->pSub_bounds();
  if (pCard_joinPred) {
    i8 nSubs = pCard_joinPred->nChildCards();
    for (i8 i_Sub = 0; i_Sub < nSubs; i_Sub++) {
      Card *pSubCard = pCard_joinPred->pGet(i_Sub);

      if (!pSubCard->bIsCard(CK_TERM))
        continue;

      Card_Term *p_term = _pCard_Term(pSubCard);
      szRpnExpr->vCatSprint(" %s"_Stz, p_term->sz());
    }
    return;
  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexScan op                                            */
/*----------------------------------------------------------------------------*/
void Weaver::vRpnIndexScan(Stz* szRpnExpr, Card_IndexScan* pCard_IndexScan)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  Card_CmpPred* p_predCmp;
  Card_Term*    p_term;

  Card_LBound* p_lBound = pCard_IndexScan->pSub_lBound();
  if (p_lBound) {
    szRpnExpr->vConcat(" LBOUND"_Stz);

    p_predCmp = p_lBound->pSub_cmpPred();

    //p_term = _pCard_Term(p_predCmp->pGetChild(0));

    //szRpnExpr->vCatSprint(" $I%lld"_Stz, p_term->iMark2()+1);

    p_predCmp->vConcatChildren(szRpnExpr,CK_TERM);

    szRpnExpr->vCatSprint(" %s"_Stz, p_predCmp->sz());

  }

  Card_UBound* p_uBound = pCard_IndexScan->pSub_uBound();
  if (p_uBound) {
    szRpnExpr->vConcat(" UBOUND"_Stz);

    p_predCmp = p_uBound->pSub_cmpPred();

    //p_term = _pCard_Term(p_predCmp->pGetChild(0));

    //szRpnExpr->vCatSprint(" $I%lld"_Stz, p_term->iMark2()+1);

    p_predCmp->vConcatChildren(szRpnExpr,CK_TERM);

    szRpnExpr->vCatSprint(" %s"_Stz, p_predCmp->sz());

  }

}
/*----------------------------------------------------------------------------*/
/* produce <expr> for limit op                                                */
/*----------------------------------------------------------------------------*/
void Weaver::vRpnLimit(Stz* szRpnExpr, Card_Limit*  pCard_Limit)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  szRpnExpr->vCatSprint(" %lls"_Stz,pCard_Limit->sz());
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for project op                                              */
/*----------------------------------------------------------------------------*/
void Weaver::vRpnProject(Stz* szRpnExpr, Card_Project* pCard_Project)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  //pCard_Project->vDump();
    
  Card_ProjElist* pCard_ProjElist =
    _pCard_ProjElist(pCard_Project->pFindSubCard(CK_PROJELIST));

  i8 nSubCards = pCard_ProjElist->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    
    Card* pSubCard = pCard_ProjElist->pGet(i_Child);
    eCK eSubCard = pSubCard->eCard();
    
    if (CK_EXPR != eSubCard)
      continue;

    Card_Expr* pCard_Expr = _pCard_Expr(pSubCard);
  
    if (i_Child) {
      szRpnExpr->vConcat("\n                 "_Stz);
      szRpnExpr->vConcat(" ,"_Stz);
    }
    else if (nSubCards > 1)
      szRpnExpr->vConcat(" "_Stz);

    vExpr(szRpnExpr,pCard_Expr);
  }
  
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for scan op                                                 */
/*----------------------------------------------------------------------------*/
void Weaver::vRpnScan(Stz* szRpnExpr, Card_Scan* pCard_Scan)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for sort op                                                 */
/*----------------------------------------------------------------------------*/
void Weaver::vRpnSort(Stz* szRpnExpr, Card_Sort* pCard_Sort)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  Card_SbyElist* pCard_SbyElist =
          _pCard_SbyElist(pCard_Sort->pFindSubCard(CK_SBYELIST));

  i8 nSubCards = pCard_SbyElist->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {

    Card* pSubCard = pCard_SbyElist->pGet(i_Child);
    eCK eSubCard = pSubCard->eCard();

    if (CK_EXPR != eSubCard)
      continue;

    Card_Expr* pCard_Expr = _pCard_Expr(pSubCard);

    if (i_Child)
      szRpnExpr->vConcat(" ,"_Stz);

    vExpr(szRpnExpr,pCard_Expr);

    szRpnExpr->vConcat(" ASC"_Stz);

  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for filter op                                               */
/*----------------------------------------------------------------------------*/
void Weaver::vRpnWhere(Stz* szRpnExpr, Card_Where* pCard_Where)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  //pCard_Filter->vDump();

  szRpnExpr->vCatSprint(" type BOOLEAN"_Stz);

  Card* pCard_test = pCard_Where->pSub_boolCalc();
  if (CK_BOOLCALC == pCard_test->eCard()) {
    Card_BoolCalc *pCard_CalcBool = _pCard_BoolCalc(pCard_test);
    vBoolCalc(szRpnExpr, pCard_CalcBool);
  }
  else {
    Card_RpnCalc *pCard_Calc = pCard_Where->pSub_rpnCalc();
    vRpnCalc(szRpnExpr, pCard_Calc);
  }
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_Expr                                              */
/*----------------------------------------------------------------------------*/
void Weaver::vExpr(Stz* szRpnExpr, Card_Expr* pCard_Expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  i8 nSubCards = pCard_Expr->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pCard = pCard_Expr->pGet(i_Child);
    
    eCK eCard = pCard->eCard();
    
    switch (eCard) {

      case CK_AGGREGATES:
        vAggregates(szRpnExpr, _pCard_Aggregates(pCard));
        break;

      case CK_AS:
        vAs(szRpnExpr, _pCard_As(pCard));
        break;
        
      case CK_RPNCALC:
        vRpnCalc(szRpnExpr, _pCard_RpnCalc(pCard));
        break;

      case CK_TYPE:
        vType(szRpnExpr, _pCard_Type(pCard));
        break;
        
      default:
        //{;}
        vTrace("  skipping %s\n"_Stz,aCK[eCard]);
    }
    
  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_RpnCalc                                          */
/*----------------------------------------------------------------------------*/
void Weaver::vRpnCalc(Stz* szRpnExpr, Card_RpnCalc* pCard_RpnCalc)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  //if (pCard_RpnCalc->bAggOp())
  //  szRpnExpr->vConcat(" )"_Stz);
  
  i8 nSubCards = pCard_RpnCalc->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pCard = pCard_RpnCalc->pGet(i_Child);
    
    eCK eCard = pCard->eCard();
    
    switch (eCard) {

      case CK_TERM:
        vTerm(szRpnExpr,_pCard_Term(pCard));
        break;

      default:
        //{;}
        vTrace("  skipping %s\n"_Stz,aCK[eCard]);
    }

  }
  
}

/*----------------------------------------------------------------------------*/
/* append <rpnExpr> for <calcBool>                                            */
/*----------------------------------------------------------------------------*/
void Weaver::vBoolCalc(Stz* szRpnExpr, Card_BoolCalc* pCard_BoolCalc)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  i8 nSubCards = pCard_BoolCalc->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pCard_BoolCalc->pGet(i_Child);

    eCK eSubCard = pSubCard->eCard();

    switch (eSubCard) {
      case CK_ORPRED:
        vOrPred(szRpnExpr, _pCard_OrPred(pSubCard), 0);
        break;
      case CK_ANDPRED:
        vAndPred(szRpnExpr, _pCard_AndPred(pSubCard), 0);
        break;
      case CK_CMPPRED:
        vCmpPred(szRpnExpr, _pCard_CmpPred(pSubCard), 0);
        break;
      case CK_TSTPRED:
        vTstPred(szRpnExpr, _pCard_TstPred(pSubCard), 0);
        break;
      default:
        vOtherPred(szRpnExpr, pSubCard, 0);
        break;
    }
  }

}

/*----------------------------------------------------------------------------*/
/* append <rpnExpr> for <predOr>                                              */
/*----------------------------------------------------------------------------*/
void Weaver::vOrPred(Stz* szRpnExpr, Card_OrPred* pCard_OrPred, i8 i)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  i8 nSubCards = pCard_OrPred->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pCard_OrPred->pGet(i_Child);

    eCK eSubCard = pSubCard->eCard();

    switch (eSubCard) {
      case CK_ORPRED:
        vOrPred(szRpnExpr, _pCard_OrPred(pSubCard), 0);
        break;
      case CK_ANDPRED:
        vAndPred(szRpnExpr, _pCard_AndPred(pSubCard), 0);
        break;
      case CK_CMPPRED:
        vCmpPred(szRpnExpr, _pCard_CmpPred(pSubCard), 0);
        break;
      case CK_TSTPRED:
        vTstPred(szRpnExpr, _pCard_TstPred(pSubCard), 0);
        break;
      default:
        vOtherPred(szRpnExpr, pSubCard, 0);
        break;
    }
  }

  szRpnExpr->vCatSprint(" or"_Stz);

}

/*----------------------------------------------------------------------------*/
/* append <rpnExpr> for <predAnd>                                             */
/*----------------------------------------------------------------------------*/
void Weaver::vAndPred(Stz* szRpnExpr, Card_AndPred* pCard_AndPred, i8 i)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  i8 nSubCards = pCard_AndPred->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pCard_AndPred->pGet(i_Child);

    eCK eSubCard = pSubCard->eCard();

    switch (eSubCard) {
      case CK_ORPRED:
        vOrPred(szRpnExpr, _pCard_OrPred(pSubCard), 0);
        break;
      case CK_ANDPRED:
        vAndPred(szRpnExpr, _pCard_AndPred(pSubCard), 0);
        break;
      case CK_CMPPRED:
        vCmpPred(szRpnExpr, _pCard_CmpPred(pSubCard), 0);
        break;
      case CK_TSTPRED:
        vTstPred(szRpnExpr, _pCard_TstPred(pSubCard), 0);
        break;
      default:
        vOtherPred(szRpnExpr, pSubCard, 0);
        break;
    }
  }

  szRpnExpr->vCatSprint(" and"_Stz);

}

/*----------------------------------------------------------------------------*/
/* append <rpnExpr> for <predCmp>                                             */
/*----------------------------------------------------------------------------*/
void Weaver::vCmpPred(Stz* szRpnExpr, Card_CmpPred* pCard_CmpPred, i8 i)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  i8 nSubCards = pCard_CmpPred->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pCard_CmpPred->pGet(i_Child);

    eCK eSubCard = pSubCard->eCard();

    switch (eSubCard) {
      case CK_ORPRED:
        vOrPred(szRpnExpr, _pCard_OrPred(pSubCard), 0);
        break;
      case CK_ANDPRED:
        vAndPred(szRpnExpr, _pCard_AndPred(pSubCard), 0);
        break;
      case CK_CMPPRED:
        vCmpPred(szRpnExpr, _pCard_CmpPred(pSubCard), 0);
        break;
      case CK_TSTPRED:
        vTstPred(szRpnExpr, _pCard_TstPred(pSubCard), 0);
        break;
      default:
        vOtherPred(szRpnExpr, pSubCard, 0);
        break;
    }
  }

  szRpnExpr->vCatSprint(" %s"_Stz, pCard_CmpPred->sz());

}

/*----------------------------------------------------------------------------*/
/* append <rpnExpr> for <predCmp>                                             */
/*----------------------------------------------------------------------------*/
void Weaver::vTstPred(Stz* szRpnExpr, Card_TstPred* pCard_TstPred, i8 i)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  i8 nSubCards = pCard_TstPred->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pCard_TstPred->pGet(i_Child);

    eCK eSubCard = pSubCard->eCard();

    switch (eSubCard) {
      case CK_ORPRED:
        vOrPred(szRpnExpr, _pCard_OrPred(pSubCard), 0);
        break;
      case CK_ANDPRED:
        vAndPred(szRpnExpr, _pCard_AndPred(pSubCard), 0);
        break;
      case CK_CMPPRED:
        vCmpPred(szRpnExpr, _pCard_CmpPred(pSubCard), 0);
        break;
      case CK_TSTPRED:
        vTstPred(szRpnExpr, _pCard_TstPred(pSubCard), 0);
        break;
      default:
        vOtherPred(szRpnExpr, pSubCard, 0);
        break;
    }
  }

  szRpnExpr->vCatSprint(" %s"_Stz, pCard_TstPred->sz());
}


/*----------------------------------------------------------------------------*/
/* append <rpnExpr> for <other>                                               */
/*----------------------------------------------------------------------------*/
void Weaver::vOtherPred(Stz* szRpnExpr, Card* pCard_Other, i8 i)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  eCK eOtherCard = pCard_Other->eCard();

  switch (eOtherCard) {
    case CK_TERM:
      szRpnExpr->vCatSprint(" %s"_Stz,pCard_Other->sz());
      break;
    default:
      break;
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_PRED                                              */
/*----------------------------------------------------------------------------*/
void Weaver::vPred(Stz* szRpnExpr, Card_PRED* pCard_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  if (! pCard_PRED)
    return;
  
  eCK eCard = pCard_PRED->eCard();
  
  switch (eCard) {
    
    case CK_ORPRED:
      vPredOr(szRpnExpr,_pCard_OrPred(pCard_PRED));
      break;
      
    case CK_ANDPRED:
      vPredAnd(szRpnExpr,_pCard_AndPred(pCard_PRED));
      break;
      
    case CK_CMPPRED:
      vPredCmp(szRpnExpr,_pCard_CmpPred(pCard_PRED));
      break;

    default:
      vTrace("vCardPred: unexpected eCard %lld %s\n"_Stz
             ,(i8)eCard
             ,aTE[eCard]);
      RASSERT(0);
      break;
  }
  
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_OrExpr                                            */
/*----------------------------------------------------------------------------*/
void Weaver::vPredOr(Stz* szRpnExpr, Card_OrPred* pCard_PredOr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
    
  Card_PRED* pCard0 = (Card_PRED*)pCard_PredOr->pGet(0);
  vPred(szRpnExpr,pCard0);

  Card_PRED* pCard1 = (Card_PRED*)pCard_PredOr->pGet(1);
  vPred(szRpnExpr,pCard1);

  szRpnExpr->vConcat(" or"_Stz);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_AndExpr                                           */
/*----------------------------------------------------------------------------*/
void Weaver::vPredAnd(Stz* szRpnExpr, Card_AndPred* pCard_PredAnd)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  Card_PRED* pCard0 = (Card_PRED*)pCard_PredAnd->pGet(0);
  vPred(szRpnExpr,pCard0);

  Card_PRED* pCard1 = (Card_PRED*)pCard_PredAnd->pGet(1);
  vPred(szRpnExpr,pCard1);

  szRpnExpr->vConcat(" and"_Stz);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_CmpExpr                                           */
/*----------------------------------------------------------------------------*/
void Weaver::vPredCmp(Stz* szRpnExpr, Card_CmpPred* pCard_PredCmp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  Card_Term* pCard_Term0 = _pCard_Term(pCard_PredCmp->pGet(0));
  vTerm(szRpnExpr,pCard_Term0);

  Card_Term* pCard_Term1 = _pCard_Term(pCard_PredCmp->pGet(1));
  vTerm(szRpnExpr,pCard_Term1);
  
  Card_Term* pCard_Term2 = _pCard_Term(pCard_PredCmp->pGet(2));
  vTerm(szRpnExpr,pCard_Term2);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_Aaggregates                                       */
/*----------------------------------------------------------------------------*/
void Weaver::vAggregates(Stz* szRpnExpr, Card_Aggregates* pCard_Aggregates)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);

  szRpnExpr->vCatSprint(" aggregates"_Stz);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_As                                                */
/*----------------------------------------------------------------------------*/
void Weaver::vAs(Stz* szRpnExpr, Card_As* pCard_As)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
    
  szRpnExpr->vCatSprint(" as %s"_Stz,pCard_As->sz());
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_Term                                              */
/*----------------------------------------------------------------------------*/
void Weaver::vTerm(Stz* szRpnExpr, Card_Term* pCard_Term)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  szRpnExpr->vCatSprint(" %s"_Stz,pCard_Term->sz());
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_Type                                              */
/*----------------------------------------------------------------------------*/
void Weaver::vType(Stz* szRpnExpr, Card_Type* pCard_Type)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  szRpnExpr->vCatSprint(" type %s"_Stz,pCard_Type->sz());
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* concat formatted string                                                    */
/*----------------------------------------------------------------------------*/
void Weaver::vConcat(Stz* aFormat, ...)
{
  va_list xVarArgs;

  va_start(xVarArgs,aFormat);

  vConcat_vargs(aFormat,xVarArgs);
  
  va_end(xVarArgs);
}

/*----------------------------------------------------------------------------*/
/* concat formatted string                                                    */
/*----------------------------------------------------------------------------*/
void Weaver::vConcat_vargs(Stz* aFormat, va_list xArgs)
{
  static char caFormatted[C_1MB] = {'\0'};

  // produce formatted line
  vsnprintf(caFormatted,sizeof(caFormatted) - 3,aFormat->azChar(),xArgs);
 
  m_sOptStmt.vConcat((Stz*)caFormatted);
  
  //vTrace("%s"_Stz,caFormatted);
}

/*----------------------------------------------------------------------------*/
/* end-of-line after stage                                                    */
/*----------------------------------------------------------------------------*/
void Weaver::vEolStage(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  vConcat("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* end-of-line after op                                                       */
/*----------------------------------------------------------------------------*/
void Weaver::vEolOp(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  vConcat("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* spaceing before stage                                                      */
/*----------------------------------------------------------------------------*/
void Weaver::vPreStage(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  vConcat("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* tab before op                                                              */
/*----------------------------------------------------------------------------*/
void Weaver::vTabOp(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  if (m_i_Op_stage)
    vConcat("    | "_Stz);
  else
    vConcat("      "_Stz);
}

/*----------------------------------------------------------------------------*/
/* tab before commented op                                                    */
/*----------------------------------------------------------------------------*/
void Weaver::vTabOp_commented(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  if (m_i_Op_stage)
    vConcat("//  | "_Stz);
  else
    vConcat("//    "_Stz);
}

/*----------------------------------------------------------------------------*/
/* tab before stage                                                           */
/*----------------------------------------------------------------------------*/
void Weaver::vTabStage(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_WEAVER, _FOOTER_);
  
  vConcat("  "_Stz);
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

  RASSERT(BK_SUBPIPE == pBoard->eBoard());

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

  BoardIter xBI(m_pBoard);
  while (Card* pMainCard = xBI.pEachMain())
    vCard(pMainCard);

  vStageLast();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vEnd();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
