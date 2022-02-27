/*============================================================================*/
/* Weave.cpp -- Weave of Cards                                                */
/*============================================================================*/

#include "Weave.hpp"

#include "eST.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Weave,CN_WEAVE)
__aNewFunction(Weave,CN_WEAVE)
__vDeleteFunction(Weave,CN_WEAVE)
__aFooterFunction(Weave)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Weave::Weave(void)
: Object(CN_WEAVE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
        
  m_pBoard = nullptr;
  
  m_i_Stage = 0;
  m_i_Op_stage = 0;
  
  _checkInitialised_(CN_WEAVE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Weave::~Weave(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* weave a revised/optimised statement from given board                       */
/*----------------------------------------------------------------------------*/
void Weave::vWeaveOptimisedStmt(Board* pBoard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  //vPrintBoardSummary();

  m_pBoard = pBoard;
  
  //vTrace("Build Pipeline\n"_Stz);

  ListExoIter2_t<Card>* pBoard_I = m_pBoard->pNewIter();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vPipeline();
  vTags();
  vBegin();

  vStageFirst();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  while (pBoard_I->bHasNext()) {
    Card* pCard = pBoard_I->pNext();

    eCK eCard = pCard->eCard();
    
    switch (eCard) {
        
      case CK_STAGE:      vStageNext  (_pCard_Stage(pCard));       break;
                
      case CK_AGGREG:     vOpAggreg   (_pCard_Aggreg(pCard));      break;
      case CK_COMBINE:    vOpCombine  (_pCard_Combine(pCard));     break;
      case CK_FILTER:     vOpFilter   (_pCard_Filter(pCard));      break;
      case CK_INDEXJOIN:  vOpIndexJoin(_pCard_IndexJoin(pCard));   break;
      case CK_INDEXSCAN:  vOpIndexScan(_pCard_IndexScan(pCard));   break;
      case CK_LIMIT:      vOpLimit    (_pCard_Limit(pCard));       break;
      case CK_PROJECT:    vOpProject  (_pCard_Project(pCard));     break;
      case CK_SCAN:       vOpScan     (_pCard_Scan(pCard));        break;
    //case CK_SORT:       vOpSort     (_pCard_Sort(pCard));        break;

      case CK_STREAMDEF:
        break;

      default:
        //{;}
        vTrace("  skipping %s\n"_Stz,aCK[eCard]);
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vStageLast();

  vEnd();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pBoard_I;
}

/*----------------------------------------------------------------------------*/
/* get optimised pipeline statement                                           */
/*----------------------------------------------------------------------------*/
Stz* Weave::szWeavedStmt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  return m_sWeavedStmt;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* weave : pipeline <name>                                                    */
/*----------------------------------------------------------------------------*/
void Weave::vPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  vConcat("pipeline %s\n"_Stz,m_pBoard->szQueryNm());
}

/*----------------------------------------------------------------------------*/
/* weave : tag ... endTag                                                     */
/*----------------------------------------------------------------------------*/
void Weave::vTags(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  vConcat("  tag <optimised> = 1 endTag\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* weave : begin                                                              */
/*----------------------------------------------------------------------------*/
void Weave::vBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);

  vConcat("begin\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* weave : end ;                                                              */
/*----------------------------------------------------------------------------*/
void Weave::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);

  vConcat("\n"_Stz);
  vConcat("end ;\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* weave : stage S00 on origin :                                              */
/*----------------------------------------------------------------------------*/
void Weave::vStageFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
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
void Weave::vStageNext(Card_Stage* pCard_Stage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  eYG eStageTopology = pCard_Stage->eStageTopology();
  
  vTabOp();  vConcat("next"_Stz);  vEolOp();

  vPreStage();
  vTabStage();
  vConcat("stage S%02lld on %s : "_Stz
          ,m_i_Stage++
          ,aYG_weave[eStageTopology]);
  vEolStage();
  
  m_i_Op_stage = 0;
}

/*----------------------------------------------------------------------------*/
/* weave : stage Snn on origin :                                              */
/*----------------------------------------------------------------------------*/
void Weave::vStageLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
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
/* weave : aggreg <compiled> <expr>                                           */
/*----------------------------------------------------------------------------*/
void Weave::vOpAggreg(Card_Aggreg* pCard_Aggreg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  // 0) aggElist : 1) gbyList

  s4K sRpnExpr;
  
  vAggreg(sRpnExpr,pCard_Aggreg);

  vTabOp();
  vConcat("aggreg    2 \"%s\""_Stz,sRpnExpr.szStz());
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* weave : combine 0                                                          */
/*----------------------------------------------------------------------------*/
void Weave::vOpCombine(Card_Combine* pCard_Combine)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
    
  // 0) combined :

  s4K sRpnExpr;
  
  vCombine(sRpnExpr,pCard_Combine);

  vTabOp();
  vConcat("combined  0"_Stz);
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* weave : filter <compiled> <expr>                                           */
/*----------------------------------------------------------------------------*/
void Weave::vOpFilter(Card_Filter* pCard_Filter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  // filter : 0) filtElist

  s4K sRpnExpr;
  
  vFilter(sRpnExpr,pCard_Filter);
  
  vTabOp();
  vConcat("filter    2 \"%s\""_Stz,sRpnExpr.szStz());
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* weave : indexJoin <compiled> <tableNm> <indexNm> <expr>                    */
/*----------------------------------------------------------------------------*/
void Weave::vOpIndexJoin(Card_IndexJoin* pCard_IndexJoin)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  // indexJoin : 0) {table} 1) {index} 2) indexPred

  s4K sRpnExpr;
  
  vIndexJoin(sRpnExpr,pCard_IndexJoin);

  Index* pIndex = pCard_IndexJoin->pIndex();
  Stz* szIndexNm = pIndex->szIndexNm();

  Card_Table* pCard_Table = nullptr; //_pCard_Table(pBoard_I->pNext());
  Table* pTable = _pTable(pCard_Table->pTable());
  Stz* szTableNm = pTable->szTableNm();
  
  vTabOp();
  vConcat("indexJoin 2 %s %s \"<expr>\""_Stz,szTableNm, szIndexNm);
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* weave : indexScan <compiled> <tableNm> <indexNm> <expr>                    */
/*----------------------------------------------------------------------------*/
void Weave::vOpIndexScan(Card_IndexScan* pCard_IndexScan)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  // indexScan : 0) {table} 1) {index} 2) indexPred

  Index* pIndex = pCard_IndexScan->pFindSubCard(CK_INDEX)->pIndex();
  s4K sRpnExpr;
  
  vIndexScan(sRpnExpr,pCard_IndexScan);

  vTabOp();
  vConcat("indexScan 2 %s %s \"%s\""_Stz
          ,pIndex->pTable()->szTableNm()
          ,pIndex->szIndexNm()
          ,sRpnExpr.szStz()
          );
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* weave : limit <compiled> <rows>                                            */
/*----------------------------------------------------------------------------*/
void Weave::vOpLimit(Card_Limit* pCard_Limit)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  // limit : 0) rows
  
  s4K sRpnExpr;
  
  vLimit(sRpnExpr,pCard_Limit);

  vTabOp();
  vConcat("limit     2 %s"_Stz,sRpnExpr.szStz());
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* weave : ordagg <compiled> <expr>                                           */
/*----------------------------------------------------------------------------*/
void Weave::vOpOrdAgg(Card_Aggreg* pCard_Aggreg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  // ordagg : 0) aggElist

  s4K sRpnExpr;
  
  vAggreg(sRpnExpr,pCard_Aggreg);

  vTabOp();
  vConcat("aggreg    2 \"%s\""_Stz,sRpnExpr.szStz());
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* weave : project <compiled> <expr>                                          */
/*----------------------------------------------------------------------------*/
void Weave::vOpProject(Card_Project* pCard_Project)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  // project : 0) projElist
  
  s4K sRpnExpr;
  
  vProject(sRpnExpr,pCard_Project);
  
  vTabOp();
  vConcat("project   2 \"%s\""_Stz,sRpnExpr.szStz());
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* weave : scan <tableNm>                                                     */
/*----------------------------------------------------------------------------*/
void Weave::vOpScan(Card_Scan* pCard_Scan)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  // scan : 0) {table}

  s4K sRpnExpr;
  
  vScan(sRpnExpr,pCard_Scan);

  Table* pTable = pCard_Scan->pFindSubCard(CK_TABLE)->pTable();
  
  Stz* szTableNm = pTable->szTableNm();

  vTabOp();
  vConcat("scan      %s"_Stz,szTableNm);
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* weave : sort ???                                                           */
/*----------------------------------------------------------------------------*/
void Weave::vOpSort(Card_Sort* pCard_Sort)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  // sort : 0) ???
  
  s4K sRpnExpr;
  
  vSort(sRpnExpr,pCard_Sort);

  vTabOp_commented();
  vConcat("sort      /*???*/"_Stz);
  vEolOp();
  //m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* produce <expr> for aggreg op                                               */
/*----------------------------------------------------------------------------*/
void Weave::vAggreg(Stz* szRpnExpr, Card_Aggreg* pCard_Aggreg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
    
  Card_AggElist* pCard_AggElist =
    _pCard_AggElist(pCard_Aggreg->pFindSubCard(CK_AGGELIST));

  i8 nSubCards = pCard_AggElist->nItems();
  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
    Card* pCard_SUB = pCard_AggElist->pGet(i_SubCard);
    if (! pCard_SUB->bIsCard(CK_EXPR))
      return;
    
    if (i_SubCard)
      szRpnExpr->vConcat(" , "_Stz);

    Card_Expr* pCard_Expr = _pCard_Expr(pCard_SUB);
    vTypedExpr(szRpnExpr,pCard_Expr);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Card_GbyElist* pCard_GbyElist =
    _pCard_GbyElist(pCard_Aggreg->pFindSubCard(CK_GBYELIST));
  
  if (! pCard_GbyElist)
    return;

  szRpnExpr->vConcat(" groupBy "_Stz);

  nSubCards = pCard_GbyElist->nItems();
  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
    Card* pCard_SUB = pCard_GbyElist->pGet(i_SubCard);
    if (! pCard_SUB->bIsCard(CK_EXPR))
      return;
    
    if (i_SubCard)
      szRpnExpr->vConcat(" , "_Stz);

    Card_Expr* pCard_Expr = _pCard_Expr(pCard_SUB);
    vTypedExpr(szRpnExpr,pCard_Expr);
  }
}

/*----------------------------------------------------------------------------*/
/* expr for combine                                                           */
/*----------------------------------------------------------------------------*/
void Weave::vCombine(Stz* szRpnExpr, Card_Combine* pCard_Combine)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  // ???
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for filter op                                               */
/*----------------------------------------------------------------------------*/
void Weave::vFilter(Stz* szRpnExpr, Card_Filter* pCard_Filter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  //pCard_Filter->vDump();

  Card_Expr* pCard_Expr = _pCard_Expr(pCard_Filter->pGet(0));

  Card_Type* pCard_Type = _pCard_Type(pCard_Expr->pGet(0));
  vType(szRpnExpr,pCard_Type);
  
  Card_ExprBool* pCard_ExprBool = _pCard_ExprBool(pCard_Expr->pGet(1));
  vBoolExpr(szRpnExpr,pCard_ExprBool);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexJoin op                                            */
/*----------------------------------------------------------------------------*/
void Weave::vIndexJoin(Stz* szRpnExpr, Card_IndexJoin* pCard_IndexJoin)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  // ????
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexScan op                                            */
/*----------------------------------------------------------------------------*/
void Weave::vIndexScan(Stz* szRpnExpr, Card_IndexScan* pCard_IndexScan)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  Card_PredCmp* pCard_PredCmp;
  Card_Term* pCard_Term;
  Stz* szTerm;
  
  //pCard_IndexScan->vDump();
  
  // lower bound
  
  Card_LBound* pCard_LBound
    = _pCard_LBound(pCard_IndexScan->pFindSubCard(CK_LBOUND));
  
  if (pCard_LBound) {
    szRpnExpr->vConcat("LBOUND"_Stz);
    
    pCard_PredCmp = _pCard_PredCmp(pCard_LBound->pGet(0));

    pCard_Term = _pCard_Term(pCard_PredCmp->pGet(0));
    szTerm = pCard_Term->szTerm();
    szRpnExpr->vConcat(" $I1"_Stz);

    pCard_Term = _pCard_Term(pCard_PredCmp->pGet(1));
    szTerm = pCard_Term->szTerm();
    szRpnExpr->vConcat(" "_Stz);
    szRpnExpr->vConcat(szTerm);

    pCard_Term = _pCard_Term(pCard_PredCmp->pGet(2));
    szTerm = pCard_Term->szTerm();
    szRpnExpr->vConcat(" "_Stz);
    szRpnExpr->vConcat(szTerm);
  }
  
  // upper bound
  
  Card_UBound* pCard_UBound
    = _pCard_UBound(pCard_IndexScan->pFindSubCard(CK_UBOUND));
  
  if (pCard_UBound) {
    szRpnExpr->vConcat("UBOUND"_Stz);
    
    pCard_PredCmp = _pCard_PredCmp(pCard_UBound->pGet(0));

    pCard_Term = _pCard_Term(pCard_PredCmp->pGet(0));
    szTerm = pCard_Term->szTerm();
    szRpnExpr->vConcat(" $I1"_Stz);

    pCard_Term = _pCard_Term(pCard_PredCmp->pGet(1));
    szTerm = pCard_Term->szTerm();
    szRpnExpr->vConcat(" "_Stz);
    szRpnExpr->vConcat(szTerm);

    pCard_Term = _pCard_Term(pCard_PredCmp->pGet(2));
    szTerm = pCard_Term->szTerm();
    szRpnExpr->vConcat(" "_Stz);
    szRpnExpr->vConcat(szTerm);
  }
  
  // range bound

  Card_RBound* pCard_RBound
    = _pCard_RBound(pCard_IndexScan->pFindSubCard(CK_RBOUND));
  
  if (pCard_RBound) {
    szRpnExpr->vConcat("LBOUND"_Stz);
    
    pCard_PredCmp = _pCard_PredCmp(pCard_RBound->pGet(0));

    pCard_Term = _pCard_Term(pCard_PredCmp->pGet(0));
    szTerm = pCard_Term->szTerm();
    szRpnExpr->vConcat(" $I1"_Stz);

    pCard_Term = _pCard_Term(pCard_PredCmp->pGet(1));
    szTerm = pCard_Term->szTerm();
    szRpnExpr->vConcat(" "_Stz);
    szRpnExpr->vConcat(szTerm);

    pCard_Term = _pCard_Term(pCard_PredCmp->pGet(2));
    szTerm = pCard_Term->szTerm();
    szRpnExpr->vConcat(" "_Stz);
    szRpnExpr->vConcat(szTerm);
    
    szRpnExpr->vConcat( " UBOUND"_Stz);
    
    pCard_PredCmp = _pCard_PredCmp(pCard_RBound->pGet(1));

    pCard_Term = _pCard_Term(pCard_PredCmp->pGet(0));
    szTerm = pCard_Term->szTerm();
    szRpnExpr->vConcat(" $I1"_Stz);

    pCard_Term = _pCard_Term(pCard_PredCmp->pGet(1));
    szTerm = pCard_Term->szTerm();
    szRpnExpr->vConcat(" "_Stz);
    szRpnExpr->vConcat(szTerm);

    pCard_Term = _pCard_Term(pCard_PredCmp->pGet(2));
    szTerm = pCard_Term->szTerm();
    szRpnExpr->vConcat(" "_Stz);
    szRpnExpr->vConcat(szTerm);
  }

  Card_EBound* pCard_EBound
    = _pCard_EBound(pCard_IndexScan->pFindSubCard(CK_EBOUND));
  
  if (pCard_EBound) {
    pCard_PredCmp = _pCard_PredCmp(pCard_EBound->pGet(0));
    
    szRpnExpr->vConcat("LBOUND"_Stz);
    
    pCard_Term = _pCard_Term(pCard_PredCmp->pGet(0));
    szTerm = pCard_Term->szTerm();
    szRpnExpr->vConcat(" $I1"_Stz);

    pCard_Term = _pCard_Term(pCard_PredCmp->pGet(1));
    szTerm = pCard_Term->szTerm();
    szRpnExpr->vConcat(" "_Stz);
    szRpnExpr->vConcat(szTerm);

//    pCard_Term = _pCard_Term(pCard_PredCmp->pGet(2));
//    szTerm = pCard_Term->szTerm();
//    szRpnExpr->vConcat(" "_Stz);
//    szRpnExpr->vConcat(szTerm);
    szRpnExpr->vConcat(" >="_Stz);

    szRpnExpr->vConcat( " UBOUND"_Stz);

    pCard_Term = _pCard_Term(pCard_PredCmp->pGet(0));
    szTerm = pCard_Term->szTerm();
    szRpnExpr->vConcat(" $I1"_Stz);

    pCard_Term = _pCard_Term(pCard_PredCmp->pGet(1));
    szTerm = pCard_Term->szTerm();
    szRpnExpr->vConcat(" "_Stz);
    szRpnExpr->vConcat(szTerm);

//    pCard_Term = _pCard_Term(pCard_PredCmp->pGet(2));
//    szTerm = pCard_Term->szTerm();
//    szRpnExpr->vConcat(" "_Stz);
//    szRpnExpr->vConcat(szTerm);
    szRpnExpr->vConcat(" <="_Stz);
  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for limit op                                                */
/*----------------------------------------------------------------------------*/
void Weave::vLimit(Stz* szRpnExpr, Card_Limit*  pCard_Limit)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  i8 nRows = pCard_Limit->nRows();

  szRpnExpr->vCatSprint("%lld"_Stz,nRows);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for project op                                              */
/*----------------------------------------------------------------------------*/
void Weave::vProject(Stz* szRpnExpr, Card_Project* pCard_Project)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  //pCard->vDump();
  
  szRpnExpr->vCopy(""_Stz);
  
  Card_ProjElist* pCard_ProjElist =
    _pCard_ProjElist(pCard_Project->pFindSubCard(CK_PROJELIST));

  i8 nSubCards = pCard_ProjElist->nItems();
  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
    
    Card* pSubCard = pCard_ProjElist->pGet(i_SubCard);
    eCK eSubCard = pSubCard->eCard();
    
    if (CK_EXPR != eSubCard)
      continue;

    Card_Expr* pCard_Expr = _pCard_Expr(pSubCard);
  
    if (i_SubCard)
      szRpnExpr->vConcat(" , "_Stz);
    vTypedExpr(szRpnExpr,pCard_Expr);
  }
  
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for scan op                                                 */
/*----------------------------------------------------------------------------*/
void Weave::vScan(Stz* szRpnExpr, Card_Scan* pCard_Scan)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for sort op                                                 */
/*----------------------------------------------------------------------------*/
void Weave::vSort(Stz* szRpnExpr, Card_Sort* pCard_Sort)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_ExprBool                                          */
/*----------------------------------------------------------------------------*/
void Weave::vBoolExpr(Stz* szRpnExpr, Card_ExprBool* pCard_ExprBool)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
 
  Card_PRED* pCard0 = (Card_PRED*)pCard_ExprBool->pGet(0);
  vPred(szRpnExpr,pCard0);
}


/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_ExprCalc                                          */
/*----------------------------------------------------------------------------*/
void Weave::vCalcExpr(Stz* szRpnExpr, Card_ExprCalc* pCard_ExprCalc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  if (pCard_ExprCalc->bAggOp())
    szRpnExpr->vConcat(" )"_Stz);
  
  i8 nSubCards = pCard_ExprCalc->nItems();
  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
    Card* pCard = pCard_ExprCalc->pGet(i_SubCard);
    
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
/* produce <expr> for pCard_Expr                                         */
/*----------------------------------------------------------------------------*/
void Weave::vTypedExpr(Stz* szRpnExpr, Card_Expr* pCard_Expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  i8 nSubCards = pCard_Expr->nItems();
  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
    Card* pCard = pCard_Expr->pGet(i_SubCard);
    
    eCK eCard = pCard->eCard();
    
    switch (eCard) {

      case CK_AS:
        vAs(szRpnExpr, _pCard_As(pCard));
        break;
        
      case CK_EXPRCALC:
        vCalcExpr(szRpnExpr, _pCard_ExprCalc(pCard));
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
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_As                                                */
/*----------------------------------------------------------------------------*/
void Weave::vAs(Stz* szRpnExpr, Card_As* pCard_As)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
    
  szRpnExpr->vCatSprint(" as %s"_Stz,pCard_As->szAs());
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_Term                                              */
/*----------------------------------------------------------------------------*/
void Weave::vTerm(Stz* szRpnExpr, Card_Term* pCard_Term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  szRpnExpr->vCatSprint(" %s"_Stz,pCard_Term->szTerm());
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_Type                                              */
/*----------------------------------------------------------------------------*/
void Weave::vType(Stz* szRpnExpr, Card_Type* pCard_Type)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  szRpnExpr->vCatSprint("type %s"_Stz,pCard_Type->szType());
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_PRED                                              */
/*----------------------------------------------------------------------------*/
void Weave::vPred(Stz* szRpnExpr, Card_PRED* pCard_PRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  if (! pCard_PRED)
    return;
  
  eCK eCard = pCard_PRED->eCard();
  
  switch (eCard) {
    
    case CK_PREDOR:
      vPredOr(szRpnExpr,_pCard_PredOr(pCard_PRED));
      break;
      
    case CK_PREDAND:
      vPredAnd(szRpnExpr,_pCard_PredAnd(pCard_PRED));
      break;
      
    case CK_PREDCMP:
      vPredCmp(szRpnExpr,_pCard_PredCmp(pCard_PRED));
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
void Weave::vPredOr(Stz* szRpnExpr, Card_PredOr* pCard_PredOr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
    
  Card_PRED* pCard0 = (Card_PRED*)pCard_PredOr->pGet(0);
  vPred(szRpnExpr,pCard0);

  Card_PRED* pCard1 = (Card_PRED*)pCard_PredOr->pGet(1);
  vPred(szRpnExpr,pCard1);

  szRpnExpr->vConcat(" or"_Stz);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_AndExpr                                           */
/*----------------------------------------------------------------------------*/
void Weave::vPredAnd(Stz* szRpnExpr, Card_PredAnd* pCard_PredAnd)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  Card_PRED* pCard0 = (Card_PRED*)pCard_PredAnd->pGet(0);
  vPred(szRpnExpr,pCard0);

  Card_PRED* pCard1 = (Card_PRED*)pCard_PredAnd->pGet(1);
  vPred(szRpnExpr,pCard1);

  szRpnExpr->vConcat(" and"_Stz);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_CmpExpr                                           */
/*----------------------------------------------------------------------------*/
void Weave::vPredCmp(Stz* szRpnExpr, Card_PredCmp* pCard_PredCmp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
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
/* concat formatted string                                                    */
/*----------------------------------------------------------------------------*/
void Weave::vConcat(Stz* aFormat, ...)
{
  va_list xVarArgs;

  va_start(xVarArgs,aFormat);

  vConcat_vargs(aFormat,xVarArgs);
  
  va_end(xVarArgs);
}

/*----------------------------------------------------------------------------*/
/* concat formatted string                                                    */
/*----------------------------------------------------------------------------*/
void Weave::vConcat_vargs(Stz* aFormat, va_list xArgs)
{
  static char caFormatted[C_1MB] = {'\0'};

  // produce formatted line
  vsnprintf(caFormatted,sizeof(caFormatted) - 3,aFormat->azChar(),xArgs);
 
  m_sWeavedStmt.vConcat((Stz*)caFormatted);
  
  //vTrace("%s"_Stz,caFormatted);
}

/*----------------------------------------------------------------------------*/
/* end-of-line after stage                                                    */
/*----------------------------------------------------------------------------*/
void Weave::vEolStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  vConcat("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* end-of-line after op                                                       */
/*----------------------------------------------------------------------------*/
void Weave::vEolOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  vConcat("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* spaceing before stage                                                      */
/*----------------------------------------------------------------------------*/
void Weave::vPreStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  vConcat("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* tab before op                                                              */
/*----------------------------------------------------------------------------*/
void Weave::vTabOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  if (m_i_Op_stage)
    vConcat("    | "_Stz);
  else
    vConcat("      "_Stz);
}

/*----------------------------------------------------------------------------*/
/* tab before commented op                                                    */
/*----------------------------------------------------------------------------*/
void Weave::vTabOp_commented(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  if (m_i_Op_stage)
    vConcat("//  | "_Stz);
  else
    vConcat("//    "_Stz);
}

/*----------------------------------------------------------------------------*/
/* tab before stage                                                           */
/*----------------------------------------------------------------------------*/
void Weave::vTabStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  vConcat("  "_Stz);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
