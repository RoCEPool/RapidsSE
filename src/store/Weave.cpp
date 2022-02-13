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
  
  vPipeline(pBoard_I);
  vTags(pBoard_I);
  vBegin(pBoard_I);

  vStageFirst(pBoard_I);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  while (pBoard_I->bHasNext()) {
    Card* pCard = pBoard_I->pNext();

    eCK eCard = pCard->eCard();
    
    switch (eCard) {
        
      case CK_STAGE:      vStageNext  (pBoard_I,pCard);   break;
                
      case CK_AGGREG:     vOpAggreg   (pBoard_I,pCard);   break;
      case CK_COMBINE:    vOpCombine  (pBoard_I,pCard);   break;
      case CK_FILTER:     vOpFilter   (pBoard_I,pCard);   break;
      case CK_INDEXJOIN:  vOpIndexJoin(pBoard_I,pCard);   break;
      case CK_INDEXSCAN:  vOpIndexScan(pBoard_I,pCard);   break;
      case CK_LIMIT:      vOpLimit    (pBoard_I,pCard);   break;
      case CK_PROJECT:    vOpProject  (pBoard_I,pCard);   break;
      case CK_SCAN:       vOpScan     (pBoard_I,pCard);   break;
    //case CK_SORT:       vOpSort     (pBoard_I,pCard);   break;

      case CK_STREAMDEF:
        break;

      default:
        //{;}
        vTrace("  skipping %s\n"_Stz,aCK[eCard]);
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vStageLast(pBoard_I);

  vEnd(pBoard_I);

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
void Weave::vPipeline(ListExoIter2_t<Card>* pBoard_I)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  vConcat("pipeline %s\n"_Stz,m_pBoard->szQueryNm());
}

/*----------------------------------------------------------------------------*/
/* reassemble : tag ... endTag                                                */
/*----------------------------------------------------------------------------*/
void Weave::vTags(ListExoIter2_t<Card>* pBoard_I)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  vConcat("  tag <optimised> = 1 endTag\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* reassemble : begin                                                         */
/*----------------------------------------------------------------------------*/
void Weave::vBegin(ListExoIter2_t<Card>* pBoard_I)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);

  vConcat("begin\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* reassemble : end ;                                                         */
/*----------------------------------------------------------------------------*/
void Weave::vEnd(ListExoIter2_t<Card>* pBoard_I)
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
void Weave::vStageFirst(ListExoIter2_t<Card>* pBoard_I)
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
/* reassemble : stage Snn on origin :                                         */
/*----------------------------------------------------------------------------*/
void Weave::vStageLast(ListExoIter2_t<Card>* pBoard_I)
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
/* reassemble : stage Snn on ??? :                                            */
/*----------------------------------------------------------------------------*/
void Weave::vStageNext(ListExoIter2_t<Card>* pBoard_I, Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  Card_Stage* pCard_Stage = _pCard_Stage(pCard);

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
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* reassemble : aggreg <compiled> <expr>                                      */
/*----------------------------------------------------------------------------*/
void Weave::vOpAggreg(ListExoIter2_t<Card>* pBoard_I, Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  Card* pByListCard = pBoard_I->pNext();
  if (! pByListCard || CK_BYLIST != pByListCard->eCard())
    return;
  
  //Card_ByList* pCard_ByList = _pCard_ByList(pByListCard);
  
  vTabOp();
  vConcat("aggreg    2 <expr>"_Stz);
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* reassemble : combine                                                       */
/*----------------------------------------------------------------------------*/
void Weave::vOpCombine(ListExoIter2_t<Card>* pBoard_I, Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
    
  vTabOp();
  vConcat("combined  0"_Stz);
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* reassemble : filter <compiled> <expr>                                      */
/*----------------------------------------------------------------------------*/
void Weave::vOpFilter(ListExoIter2_t<Card>* pBoard_I, Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  s4K sExpr;
  
  pCard->vDump();
  
  vExpr_filter(sExpr,pCard);
  
  vTabOp();
  vConcat("filter    2 \"%s\""_Stz,sExpr.szStz());
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* reassemble : indexJoin <compiled> <tableNm> <indexNm> <expr>               */
/*----------------------------------------------------------------------------*/
void Weave::vOpIndexJoin(ListExoIter2_t<Card>* pBoard_I, Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  Card_IndexJoin* pCard_IndexJoin = _pCard_IndexJoin(pCard);
  Index* pIndex = pCard_IndexJoin->pIndex();
  Stz* szIndexNm = pIndex->szIndexNm();

  Card_On* pCard_On = _pCard_On(pBoard_I->pNext());

  Card_Table* pCard_Table = _pCard_Table(pBoard_I->pNext());
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
void Weave::vOpIndexScan(ListExoIter2_t<Card>* pBoard_I, Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  vTabOp();
  vConcat("indexScan 2 "_Stz);
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* reassemble : limit <compiled> <rows>                                       */
/*----------------------------------------------------------------------------*/
void Weave::vOpLimit(ListExoIter2_t<Card>* pBoard_I, Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  Card_Limit* pCard_Limit = _pCard_Limit(pCard);
  Stz* szRows = pCard_Limit->szGetProperty("rows"_Stz);

  vTabOp();
  vConcat("limit     2 %s"_Stz,szRows);
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* reassemble : project <compiled> <expr>                                     */
/*----------------------------------------------------------------------------*/
void Weave::vOpProject(ListExoIter2_t<Card>* pBoard_I, Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  s4K sExpr;
  
  vExpr_project(sExpr,pCard);
  
  vTabOp();
  vConcat("project   2 \"%s\""_Stz,sExpr.szStz());
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* reassemble : scan <tableNm>                                                */
/*----------------------------------------------------------------------------*/
void Weave::vOpScan(ListExoIter2_t<Card>* pBoard_I, Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  //Card_Table* pCard_Table = _pCard_Table(pBoard_I->pNext());
  //Table* pTable = _pTable(pCard_Table->pTable());
  
  Card_Scan* pCard_Scan = _pCard_Scan(pCard);
  
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
void Weave::vOpSort(ListExoIter2_t<Card>* pBoard_I, Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  vTabOp_commented();
  vConcat("sort      /*???*/"_Stz);
  vEolOp();
  //m_i_Op_stage++;
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
/* expr for project                                                           */
/*----------------------------------------------------------------------------*/
void Weave::vExpr_filter(Stz* szExpr, Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  //pCard->vDump();
  
  Card_Filter* pCard_Filter = _pCard_Filter(pCard);

  pCard_Filter->vToExpr(szExpr);
}

/*----------------------------------------------------------------------------*/
/* expr for project                                                           */
/*----------------------------------------------------------------------------*/
void Weave::vExpr_project(Stz* szExpr, Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  //pCard->vDump();
  
  szExpr->vCopy(""_Stz);

  Card_Project* pCard_Project = _pCard_Project(pCard);

  i8 nSubCards = pCard_Project->nItems();
  for (i8 i_Projection = 0; i_Projection < nSubCards; i_Projection++) {
    Card_Projection* pCard_Projection =
      _pCard_Projection(pCard_Project->pGet(i_Projection));
  
    if (i_Projection)
      szExpr->vConcat(" , "_Stz);
    vExpr_projection(szExpr,pCard_Projection);
  }
  
  /*
  Card_Projection* pCard_Projection
    = _pCard_Projection(pCard->pFindSubCard(CK_PROJECTION));

  Card_Predicate* pCard_Predicate
    = _pCard_Predicate(pCard_Projection->pFindSubCard(CK_PREDICATE));

  pCard_Predicate->vToExpr(szExpr);
   
  */
}

/*----------------------------------------------------------------------------*/
/* expr for project                                                           */
/*----------------------------------------------------------------------------*/
void Weave::vExpr_projection(Stz* szExpr, Card_Projection* pCard_Projection)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  i8 nSubCards = pCard_Projection->nItems();
  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
    Card* pCard = pCard_Projection->pGet(i_SubCard);
    
    eCK eCard = pCard->eCard();
    
    switch (eCard) {

      case CK_TYPE:      vExpr_type     (szExpr, _pCard_Type(pCard)); break;
      case CK_AS:        vExpr_as       (szExpr, _pCard_As(pCard));   break;
      case CK_PREDICATE: vExpr_predicate(szExpr, _pCard_Predicate(pCard)); break;

      default:
        //{;}
        vTrace("  skipping %s\n"_Stz,aCK[eCard]);
    }
    
  }

}

/*----------------------------------------------------------------------------*/
/* expr for as                                                                */
/*----------------------------------------------------------------------------*/
void Weave::vExpr_as(Stz* szExpr, Card_As* pCard_As)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  Stz* szAsNm = pCard_As->szAsNm();

  szExpr->vConcat(" as "_Stz);
  szExpr->vConcat(szAsNm);
}

/*----------------------------------------------------------------------------*/
/* expr for predicate                                                         */
/*----------------------------------------------------------------------------*/
void Weave::vExpr_predicate(Stz* szExpr, Card_Predicate* pCard_Predicate)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  i8 nSubCards = pCard_Predicate->nItems();
  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
    Card* pCard = pCard_Predicate->pGet(i_SubCard);
    
    eCK eCard = pCard->eCard();
    
    switch (eCard) {

      case CK_TERM:      vExpr_term     (szExpr, _pCard_Term(pCard)); break;

      default:
        //{;}
        vTrace("  skipping %s\n"_Stz,aCK[eCard]);
    }

  }
  
}

/*----------------------------------------------------------------------------*/
/* expr for term                                                              */
/*----------------------------------------------------------------------------*/
void Weave::vExpr_term(Stz* szExpr, Card_Term* pCard_Term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  Stz* szTerm = pCard_Term->szTerm();

  szExpr->vConcat(" "_Stz);
  szExpr->vConcat(szTerm);
}

/*----------------------------------------------------------------------------*/
/* expr for type                                                              */
/*----------------------------------------------------------------------------*/
void Weave::vExpr_type(Stz* szExpr, Card_Type* pCard_Type)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  Stz* szType = pCard_Type->szType();

  szExpr->vConcat("type "_Stz);
  szExpr->vConcat(szType);
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
/* tab stage                                                                  */
/*----------------------------------------------------------------------------*/
void Weave::vTabStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  vConcat("  "_Stz);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
