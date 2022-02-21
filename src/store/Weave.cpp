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
/* reassemble : pipeline <name>                                               */
/*----------------------------------------------------------------------------*/
void Weave::vPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  vConcat("pipeline %s\n"_Stz,m_pBoard->szQueryNm());
}

/*----------------------------------------------------------------------------*/
/* reassemble : tag ... endTag                                                */
/*----------------------------------------------------------------------------*/
void Weave::vTags(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  vConcat("  tag <optimised> = 1 endTag\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* reassemble : begin                                                         */
/*----------------------------------------------------------------------------*/
void Weave::vBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);

  vConcat("begin\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* reassemble : end ;                                                         */
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
/* reassemble : stage S00 on origin :                                         */
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
/* reassemble : stage Snn on ??? :                                            */
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
          ,aST_weave[eStageTopology]);
  vEolStage();
  
  m_i_Op_stage = 0;
}

/*----------------------------------------------------------------------------*/
/* reassemble : stage Snn on origin :                                         */
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
/* reassemble : aggreg <compiled> <expr>                                      */
/*----------------------------------------------------------------------------*/
void Weave::vOpAggreg(Card_Aggreg* pCard_Aggreg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  s4K sExpr;
  
  vAggreg(sExpr,pCard_Aggreg);

  vTabOp();
  vConcat("aggreg    2 <expr>"_Stz);
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* reassemble : combine                                                       */
/*----------------------------------------------------------------------------*/
void Weave::vOpCombine(Card_Combine* pCard_Combine)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
    
  s4K sExpr;
  
  vCombine(sExpr,pCard_Combine);

  vTabOp();
  vConcat("combined  0"_Stz);
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* reassemble : filter <compiled> <expr>                                      */
/*----------------------------------------------------------------------------*/
void Weave::vOpFilter(Card_Filter* pCard_Filter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  s4K sExpr;
  
  vFilter(sExpr,pCard_Filter);
  
  vTabOp();
  vConcat("filter    2 \"%s\""_Stz,sExpr.szStz());
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* reassemble : indexJoin <compiled> <tableNm> <indexNm> <expr>               */
/*----------------------------------------------------------------------------*/
void Weave::vOpIndexJoin(Card_IndexJoin* pCard_IndexJoin)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
//
//  Card_On* pCard_On = _pCard_On(pBoard_I->pNext());
//
//
//
  
  s4K sExpr;
  
  vIndexJoin(sExpr,pCard_IndexJoin);

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
/* reassemble : indexScan <compiled> <tableNm> <indexNm> <expr>               */
/*----------------------------------------------------------------------------*/
void Weave::vOpIndexScan(Card_IndexScan* pCard_IndexScan)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  Index* pIndex = pCard_IndexScan->pIndex();
  s4K sExpr;
  
  vIndexScan(sExpr,pCard_IndexScan);

  vTabOp();
  vConcat("indexScan 2 %s %s \"%s\""_Stz
          ,pIndex->pTable()->szTableNm()
          ,pIndex->szIndexNm()
          ,sExpr.szStz()
          );
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* reassemble : limit <compiled> <rows>                                       */
/*----------------------------------------------------------------------------*/
void Weave::vOpLimit(Card_Limit* pCard_Limit)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  s4K sExpr;
  
  vLimit(sExpr,pCard_Limit);

  vTabOp();
  vConcat("limit     2 %s"_Stz,sExpr.szStz());
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* reassemble : project <compiled> <expr>                                     */
/*----------------------------------------------------------------------------*/
void Weave::vOpProject(Card_Project* pCard_Project)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  s4K sExpr;
  
  vProject(sExpr,pCard_Project);
  
  vTabOp();
  vConcat("project   2 \"%s\""_Stz,sExpr.szStz());
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* reassemble : scan <tableNm>                                                */
/*----------------------------------------------------------------------------*/
void Weave::vOpScan(Card_Scan* pCard_Scan)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  //Card_Table* pCard_Table = _pCard_Table(pBoard_I->pNext());
  //Table* pTable = _pTable(pCard_Table->pTable());
    
  
  s4K sExpr;
  
  vScan(sExpr,pCard_Scan);

  Table* pTable = pCard_Scan->pTable();
  
  Stz* szTableNm = pTable->szTableNm();

  vTabOp();
  vConcat("scan      %s"_Stz,szTableNm);
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* reassemble : sort ???                                                      */
/*----------------------------------------------------------------------------*/
void Weave::vOpSort(Card_Sort* pCard_Sort)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  s4K sExpr;
  
  vSort(sExpr,pCard_Sort);

  vTabOp_commented();
  vConcat("sort      /*???*/"_Stz);
  vEolOp();
  //m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* expr for aggreg                                                            */
/*----------------------------------------------------------------------------*/
void Weave::vAggreg(Stz* szExpr, Card_Aggreg* pCard_Aggreg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  //  Card* pByListCard = pBoard_I->pNext();
  //  if (! pByListCard || CK_BYLIST != pByListCard->eCard())
  //    return;
    
  //Card_ByList* pCard_ByList = _pCard_ByList(pByListCard);
}

/*----------------------------------------------------------------------------*/
/* expr for combine                                                           */
/*----------------------------------------------------------------------------*/
void Weave::vCombine(Stz* szExpr, Card_Combine* pCard_Combine)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* expr for filter                                                            */
/*----------------------------------------------------------------------------*/
void Weave::vFilter(Stz* szExpr, Card_Filter* pCard_Filter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  pCard_Filter->vDump();

  Card_TypedExpr* pCard_TypedExpr = _pCard_TypedExpr(pCard_Filter->pGet(0));

  Card_Type* pCard_Type = _pCard_Type(pCard_TypedExpr->pGet(0));
  vType(szExpr,pCard_Type);
  
  Card_BoolPred* pCard_BoolPred = _pCard_BoolPred(pCard_TypedExpr->pGet(1));
  vFilterPred(szExpr,pCard_BoolPred);
}

/*----------------------------------------------------------------------------*/
/* expr for filter                                                            */
/*----------------------------------------------------------------------------*/
void Weave::vIndexJoin(Stz* szExpr, Card_IndexJoin* pCard_IndexJoin)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* expr for indexScan                                                         */
/*----------------------------------------------------------------------------*/
void Weave::vIndexScan(Stz* szExpr, Card_IndexScan* pCard_IndexScan)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  pCard_IndexScan->vDump();
  
  szExpr->vCopy("lbound "_Stz);
  
  // assume lower bound for the moment
  
  Card_LBound* pCard_LBound
    = _pCard_LBound(pCard_IndexScan->pFindSubCard(CK_LBOUND));
  
  Card_ComparePred* pCard_ComparePred = _pCard_ComparePred(pCard_LBound->pGet(0));

  Card_Term* pCard_Term = _pCard_Term(pCard_ComparePred->pGet(0));
  Stz* szTerm = pCard_Term->szTerm();
  szExpr->vConcat(" $I1"_Stz);

  pCard_Term = _pCard_Term(pCard_ComparePred->pGet(1));
  szTerm = pCard_Term->szTerm();
  szExpr->vConcat(" "_Stz);
  szExpr->vConcat(szTerm);

  pCard_Term = _pCard_Term(pCard_ComparePred->pGet(2));
  szTerm = pCard_Term->szTerm();
  szExpr->vConcat(" "_Stz);
  szExpr->vConcat(szTerm);

}

/*----------------------------------------------------------------------------*/
/* expr for limit                                                             */
/*----------------------------------------------------------------------------*/
void Weave::vLimit(Stz* szExpr, Card_Limit*  pCard_Limit)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  Stz* szRows = pCard_Limit->szGetProperty("rows"_Stz);

  szExpr->vConcat(szRows);
}

/*----------------------------------------------------------------------------*/
/* expr for project                                                           */
/*----------------------------------------------------------------------------*/
void Weave::vProject(Stz* szExpr, Card_Project* pCard_Project)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  //pCard->vDump();
  
  szExpr->vCopy(""_Stz);

  i8 nSubCards = pCard_Project->nItems();
  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
    
    Card* pSubCard = pCard_Project->pGet(i_SubCard);
    eCK eSubCard = pSubCard->eCard();
    
    if (CK_TYPEDEXPR != eSubCard)
      continue;

    Card_TypedExpr* pCard_TypedExpr = _pCard_TypedExpr(pSubCard);
  
    if (i_SubCard)
      szExpr->vConcat(" , "_Stz);
    vProjection(szExpr,pCard_TypedExpr);
  }
  
}

/*----------------------------------------------------------------------------*/
/* expr for scan                                                              */
/*----------------------------------------------------------------------------*/
void Weave::vScan(Stz* szExpr, Card_Scan* pCard_Scan)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* expr for sort                                                              */
/*----------------------------------------------------------------------------*/
void Weave::vSort(Stz* szExpr, Card_Sort* pCard_Sort)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* reassemble : filter <compiled> <expr>                                      */
/*----------------------------------------------------------------------------*/
void Weave::vFilterPred(Stz* szExpr, Card_BoolPred* pCard_BoolPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
 
  Card_PRED* pCard0 = (Card_PRED*)pCard_BoolPred->pGet(0);
  vPred(szExpr,pCard0);
}

/*----------------------------------------------------------------------------*/
/* expr for predicate                                                         */
/*----------------------------------------------------------------------------*/
void Weave::vPredicate(Stz* szExpr, Card_Predicate* pCard_Predicate)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  i8 nSubCards = pCard_Predicate->nItems();
  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
    Card* pCard = pCard_Predicate->pGet(i_SubCard);
    
    eCK eCard = pCard->eCard();
    
    switch (eCard) {

      case CK_TERM:      vTerm     (szExpr, _pCard_Term(pCard)); break;

      default:
        //{;}
        vTrace("  skipping %s\n"_Stz,aCK[eCard]);
    }

  }
  
}

/*----------------------------------------------------------------------------*/
/* expr for projection                                                        */
/*----------------------------------------------------------------------------*/
void Weave::vProjection(Stz* szExpr, Card_TypedExpr* pCard_TypedExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  i8 nSubCards = pCard_TypedExpr->nItems();
  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
    Card* pCard = pCard_TypedExpr->pGet(i_SubCard);
    
    eCK eCard = pCard->eCard();
    
    switch (eCard) {

      case CK_AS:
        vAs(szExpr, _pCard_As(pCard));
        break;
        
      case CK_PREDICATE:
        vPredicate(szExpr, _pCard_Predicate(pCard));
        break;
        
      case CK_CALCPRED:
        vProjectPred(szExpr, _pCard_CalcPred(pCard));
        break;

      case CK_TYPE:
        vType(szExpr, _pCard_Type(pCard));
        break;
        
      default:
        //{;}
        vTrace("  skipping %s\n"_Stz,aCK[eCard]);
    }
    
  }

}

/*----------------------------------------------------------------------------*/
/* expr for predicate                                                         */
/*----------------------------------------------------------------------------*/
void Weave::vProjectPred(Stz* szExpr, Card_CalcPred* pCard_CalcPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  i8 nSubCards = pCard_CalcPred->nItems();
  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
    Card* pCard = pCard_CalcPred->pGet(i_SubCard);
    
    eCK eCard = pCard->eCard();
    
    switch (eCard) {

      case CK_TERM:      vTerm     (szExpr, _pCard_Term(pCard)); break;

      default:
        //{;}
        vTrace("  skipping %s\n"_Stz,aCK[eCard]);
    }

  }
  
}



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* reassemble : filter <compiled> <expr>                                      */
/*----------------------------------------------------------------------------*/
void Weave::vPred(Stz* szExpr, Card_PRED* pCard_PRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  if (! pCard_PRED)
    return;
  
  eCK eCard = pCard_PRED->eCard();
  
  switch (eCard) {
    
    case CK_ORPRED:
      vPredOr(szExpr,_pCard_OrPred(pCard_PRED));
      break;
      
    case CK_ANDPRED:
      vPredAnd(szExpr,_pCard_AndPred(pCard_PRED));
      break;
      
    case CK_COMPAREPRED:
      vPredCompare(szExpr,_pCard_ComparePred(pCard_PRED));
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
/* reassemble : filter <compiled> <expr>                                      */
/*----------------------------------------------------------------------------*/
void Weave::vPredOr(Stz* szExpr, Card_OrPred* pCard_OrPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
    
  Card_PRED* pCard0 = (Card_PRED*)pCard_OrPred->pGet(0);
  vPred(szExpr,pCard0);

  Card_PRED* pCard1 = (Card_PRED*)pCard_OrPred->pGet(1);
  vPred(szExpr,pCard1);

  szExpr->vConcat(" or"_Stz);
}

/*----------------------------------------------------------------------------*/
/* reassemble : filter <compiled> <expr>                                      */
/*----------------------------------------------------------------------------*/
void Weave::vPredAnd(Stz* szExpr, Card_AndPred* pCard_AndPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  Card_PRED* pCard0 = (Card_PRED*)pCard_AndPred->pGet(0);
  vPred(szExpr,pCard0);

  Card_PRED* pCard1 = (Card_PRED*)pCard_AndPred->pGet(1);
  vPred(szExpr,pCard1);

  szExpr->vConcat(" and"_Stz);
}

/*----------------------------------------------------------------------------*/
/* reassemble : filter <compiled> <expr>                                      */
/*----------------------------------------------------------------------------*/
void Weave::vPredCompare(Stz* szExpr, Card_ComparePred* pCard_ComparePred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  Card_Term* pCard_Term0 = _pCard_Term(pCard_ComparePred->pGet(0));
  vTerm(szExpr,pCard_Term0);

  Card_Term* pCard_Term1 = _pCard_Term(pCard_ComparePred->pGet(1));
  vTerm(szExpr,pCard_Term1);
  
  Card_Term* pCard_Term2 = _pCard_Term(pCard_ComparePred->pGet(2));
  vTerm(szExpr,pCard_Term2);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* expr for as                                                                */
/*----------------------------------------------------------------------------*/
void Weave::vAs(Stz* szExpr, Card_As* pCard_As)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
    
  szExpr->vCatSprint(" as %s"_Stz,pCard_As->szAs());
}

/*----------------------------------------------------------------------------*/
/* reassemble : filter <compiled> <expr>                                      */
/*----------------------------------------------------------------------------*/
void Weave::vType(Stz* szExpr, Card_Type* pCard_Type)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  szExpr->vCatSprint("type %s"_Stz,pCard_Type->szType());
}

/*----------------------------------------------------------------------------*/
/* reassemble : filter <compiled> <expr>                                      */
/*----------------------------------------------------------------------------*/
void Weave::vTerm(Stz* szExpr, Card_Term* pCard_Term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  szExpr->vCatSprint(" %s"_Stz,pCard_Term->szTerm());
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
/* end stage                                                                  */
/*----------------------------------------------------------------------------*/
void Weave::vEolStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  vConcat("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* end op                                                                     */
/*----------------------------------------------------------------------------*/
void Weave::vEolOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  vConcat("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* pre stage                                                                  */
/*----------------------------------------------------------------------------*/
void Weave::vPreStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  vConcat("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* tab op                                                                     */
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
/* tab op                                                                     */
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
/* tab stage                                                                  */
/*----------------------------------------------------------------------------*/
void Weave::vTabStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  vConcat("  "_Stz);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
