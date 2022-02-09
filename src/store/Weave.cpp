/*============================================================================*/
/* Weave.cpp -- Weave of Cards                                                */
/*============================================================================*/

#include "Weave.hpp"

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
/* build pipeline                                                             */
/*----------------------------------------------------------------------------*/
void Weave::vWeavePipelineStmt(Board* pBoard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  //vPrintBoardSummary();

  m_pBoard = pBoard;
  
  //vTrace("Build Pipeline\n"_Stz);

  ListExoIter2_t<Card>* pBoard_I = m_pBoard->pNewIter();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vPipeline(pBoard_I);
  vBegin(pBoard_I);

  vStageFirst(pBoard_I);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  while (pBoard_I->bHasNext()) {
    Card* pCard = pBoard_I->pNext();

    eCK eCard = pCard->eCard();
    
    switch (eCard) {
        
      case CK_SCAN:       vStageNext  (pBoard_I,pCard);
                          vOpScan     (pBoard_I,pCard);   break;
        
      case CK_HASHCAST:   vStageNext  (pBoard_I,pCard);   break;
        
      case CK_AGGREG:     vOpAggreg   (pBoard_I,pCard);   break;
      case CK_FILTER:     vOpFilter   (pBoard_I,pCard);   break;
      case CK_INDEXJOIN:  vOpIndexJoin(pBoard_I,pCard);   break;
      case CK_INDEXSCAN:  vOpIndexScan(pBoard_I,pCard);   break;
      case CK_LIMIT:      vOpLimit    (pBoard_I,pCard);   break;
      case CK_PROJECTION: vOpProject  (pBoard_I,pCard);   break;
      //case CK_SORTLIST:   vOpSort     (pBoard_I,pCard);   break;

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
Stz* Weave::szPipelineStmt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  return m_sPipelineStmt;
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
  vConcat("end;\n"_Stz);
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
  vConcat("stage S%02lld on origin: "_Stz,m_i_Stage++);
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
  
  vTabOp();  vConcat("next      /*cast???*/"_Stz);  vEolOp();
  
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
  
  vTabOp();  vConcat("next      /*cast???*/"_Stz);  vEolOp();

  vPreStage();
  vTabStage();
  vConcat("stage S%02lld on /*topo???*/ : "_Stz,m_i_Stage++);
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
  
  Card_ByList* pCard_ByList = _pCard_ByList(pByListCard);
  
  vTabOp();
  vConcat("aggreg    2 <expr>"_Stz);
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* reassemble : filter <compiled> <expr>                                      */
/*----------------------------------------------------------------------------*/
void Weave::vOpFilter(ListExoIter2_t<Card>* pBoard_I, Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WEAVE,_FOOTER_);
  
  vTabOp();
  vConcat("filter    2 <expr>"_Stz);
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
  
  vTabOp();
  vConcat("project   2 <expr>"_Stz);
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
 
  m_sPipelineStmt.vConcat((Stz*)caFormatted);
  
  //vTrace("%s"_Stz,caFormatted);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
