/*============================================================================*/
/* Reassemble.cpp -- Reassemble of Cards                                                */
/*============================================================================*/

#include "Reassemble.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Reassemble,CN_REASSEMBLE)
__aNewFunction(Reassemble,CN_REASSEMBLE)
__vDeleteFunction(Reassemble,CN_REASSEMBLE)
__aFooterFunction(Reassemble)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Reassemble::Reassemble(Board* pBoard)
: Object(CN_REASSEMBLE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REASSEMBLE,_FOOTER_);
        
  m_pBoard = pBoard;
  
  m_i_Stage = 0;
  m_i_Op_stage = 0;
  
  _checkInitialised_(CN_REASSEMBLE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_REASSEMBLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Reassemble::~Reassemble(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REASSEMBLE,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* build pipeline                                                             */
/*----------------------------------------------------------------------------*/
void Reassemble::vReassemble(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REASSEMBLE,_FOOTER_);
  
  //vPrintBoardSummary();

  vTrace("Build Pipeline\n"_Stz);

  ListExoIter2_t<Card>* pBoard_I = m_pBoard->pNewIter();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vReassemblePipeline(pBoard_I);
  vReassembleBegin(pBoard_I);

  vReassembleStageFirst(pBoard_I);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  while (pBoard_I->bHasNext()) {
    Card* pCard = pBoard_I->pNext();

    eCK eCard = pCard->eCard();
    
    switch (eCard) {
        
      case CK_SCAN:       vReassembleStageNext  (pBoard_I,pCard);
                               vReassembleOpScan     (pBoard_I,pCard);   break;
        
      case CK_HASHCAST:   vReassembleStageNext  (pBoard_I,pCard);   break;
        
      case CK_AGGREG: vReassembleOpAggreg   (pBoard_I,pCard);   break;
      case CK_FILTER:     vReassembleOpFilter   (pBoard_I,pCard);   break;
      case CK_INDEXJOIN:  vReassembleOpIndexJoin(pBoard_I,pCard);   break;
      case CK_INDEXSCAN:  vReassembleOpIndexScan(pBoard_I,pCard);   break;
      case CK_LIMIT:      vReassembleOpLimit    (pBoard_I,pCard);   break;
      case CK_PROJECTION:    vReassembleOpProject  (pBoard_I,pCard);   break;
      //case CK_SORTLIST:   vReassembleOpSort     (pBoard_I,pCard);   break;

      case CK_STREAMDEF:
        break;

      default:
        //{;}
        vTrace("  skipping %s\n"_Stz,aCK[eCard]);
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vReassembleStageLast(pBoard_I);


  vReassembleEnd(pBoard_I);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pBoard_I;
}

/*----------------------------------------------------------------------------*/
/* reassemble : pipeline <name>                                               */
/*----------------------------------------------------------------------------*/
void Reassemble::vReassemblePipeline(ListExoIter2_t<Card>* pBoard_I)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REASSEMBLE,_FOOTER_);
  
  vTrace("pipeline %s\n"_Stz,m_pBoard->szQueryNm());
}

/*----------------------------------------------------------------------------*/
/* reassemble : begin                                                         */
/*----------------------------------------------------------------------------*/
void Reassemble::vReassembleBegin(ListExoIter2_t<Card>* pBoard_I)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REASSEMBLE,_FOOTER_);

  vTrace("begin\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* reassemble : end ;                                                         */
/*----------------------------------------------------------------------------*/
void Reassemble::vReassembleEnd(ListExoIter2_t<Card>* pBoard_I)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REASSEMBLE,_FOOTER_);

  vTrace("\n"_Stz);
  vTrace("end;\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* reassemble : stage S00 on origin :                                         */
/*----------------------------------------------------------------------------*/
void Reassemble::vReassembleStageFirst(ListExoIter2_t<Card>* pBoard_I)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REASSEMBLE,_FOOTER_);
  
  vPreStage();
  vTabStage();
  vTrace("stage S%02lld on origin: "_Stz,m_i_Stage++);
  vEolStage();
  
  vTabOp();  vTrace("start"_Stz);       vEolOp();  m_i_Op_stage++;
  vTabOp();  vTrace("pulse     once"_Stz);  vEolOp();  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* reassemble : stage Snn on origin :                                         */
/*----------------------------------------------------------------------------*/
void Reassemble::vReassembleStageLast(ListExoIter2_t<Card>* pBoard_I)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REASSEMBLE,_FOOTER_);
  
  vTabOp();  vTrace("combine"_Stz);                vEolOp();
  vTabOp();  vTrace("next      /*cast???*/"_Stz);  vEolOp();
  
  vPreStage();
  vTabStage();
  vTrace("stage S%02lld on origin :"_Stz,m_i_Stage++);
  vEolStage();
  
  m_i_Op_stage = 0;
  
  vTabOp();  vTrace("output"_Stz);       vEolOp();  m_i_Op_stage++;
  vTabOp();  vTrace("sink"_Stz);         vEolOp();  m_i_Op_stage++;
  vTabOp();  vTrace("first"_Stz);        vEolOp();  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* reassemble : stage Snn on ??? :                                            */
/*----------------------------------------------------------------------------*/
void Reassemble::vReassembleStageNext(ListExoIter2_t<Card>* pBoard_I, Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REASSEMBLE,_FOOTER_);
  
  vTabOp();  vTrace("combine"_Stz);                vEolOp();
  vTabOp();  vTrace("next      /*cast???*/"_Stz);  vEolOp();

  vPreStage();
  vTabStage();
  vTrace("stage S%02lld on /*topo???*/ : "_Stz,m_i_Stage++);
  vEolStage();
  
  m_i_Op_stage = 0;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* reassemble : aggreg <compiled> <expr>                                      */
/*----------------------------------------------------------------------------*/
void Reassemble::vReassembleOpAggreg(ListExoIter2_t<Card>* pBoard_I, Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REASSEMBLE,_FOOTER_);
  
  Card* pByListCard = pBoard_I->pNext();
  if (! pByListCard || CK_BYLIST != pByListCard->eCard())
    return;
  
  Card_ByList* pCard_ByList = _pCard_ByList(pByListCard);
  
  vTabOp();
  vTrace("aggreg    2 <expr>"_Stz);
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* reassemble : filter <compiled> <expr>                                      */
/*----------------------------------------------------------------------------*/
void Reassemble::vReassembleOpFilter(ListExoIter2_t<Card>* pBoard_I, Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REASSEMBLE,_FOOTER_);
  
  vTabOp();
  vTrace("filter    2 <expr>"_Stz);
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* reassemble : indexJoin <compiled> <tableNm> <indexNm> <expr>               */
/*----------------------------------------------------------------------------*/
void Reassemble::vReassembleOpIndexJoin(ListExoIter2_t<Card>* pBoard_I, Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REASSEMBLE,_FOOTER_);
  
  Card_IndexJoin* pCard_IndexJoin = _pCard_IndexJoin(pCard);
  Index* pIndex = pCard_IndexJoin->pIndex();
  Stz* szIndexNm = pIndex->szIndexNm();

  Card_On* pCard_On = _pCard_On(pBoard_I->pNext());

  Card_Table* pCard_Table = _pCard_Table(pBoard_I->pNext());
  Table* pTable = _pTable(pCard_Table->pTable());
  Stz* szTableNm = pTable->szTableNm();


  vTabOp();
  vTrace("indexJoin 2 %s %s \"<expr>\""_Stz,szTableNm, szIndexNm);
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* reassemble : indexScan <compiled> <tableNm> <indexNm> <expr>               */
/*----------------------------------------------------------------------------*/
void Reassemble::vReassembleOpIndexScan(ListExoIter2_t<Card>* pBoard_I, Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REASSEMBLE,_FOOTER_);
  
  vTabOp();
  vTrace("indexScan 2 "_Stz);
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* reassemble : limit <compiled> <rows>                                       */
/*----------------------------------------------------------------------------*/
void Reassemble::vReassembleOpLimit(ListExoIter2_t<Card>* pBoard_I, Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REASSEMBLE,_FOOTER_);
  
  Card_Limit* pCard_Limit = _pCard_Limit(pCard);
  Stz* szRows = pCard_Limit->szGetProperty("rows"_Stz);

  vTabOp();
  vTrace("limit     2 %s"_Stz,szRows);
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* reassemble : project <compiled> <expr>                                     */
/*----------------------------------------------------------------------------*/
void Reassemble::vReassembleOpProject(ListExoIter2_t<Card>* pBoard_I, Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REASSEMBLE,_FOOTER_);
  
  vTabOp();
  vTrace("project   2 <expr>"_Stz);
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* reassemble : scan <tableNm>                                                */
/*----------------------------------------------------------------------------*/
void Reassemble::vReassembleOpScan(ListExoIter2_t<Card>* pBoard_I, Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REASSEMBLE,_FOOTER_);
  
  Card_Table* pCard_Table = _pCard_Table(pBoard_I->pNext());
  Table* pTable = _pTable(pCard_Table->pTable());
  Stz* szTableNm = pTable->szTableNm();

  vTabOp();
  vTrace("scan      %s"_Stz,szTableNm);
  vEolOp();
  m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/* reassemble : sort ???                                                      */
/*----------------------------------------------------------------------------*/
void Reassemble::vReassembleOpSort(ListExoIter2_t<Card>* pBoard_I, Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REASSEMBLE,_FOOTER_);
  
  vTabOp_commented();
  vTrace("sort      /*???*/"_Stz);
  vEolOp();
  //m_i_Op_stage++;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pre stage                                                                  */
/*----------------------------------------------------------------------------*/
void Reassemble::vPreStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REASSEMBLE,_FOOTER_);
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* tab stage                                                                  */
/*----------------------------------------------------------------------------*/
void Reassemble::vTabStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REASSEMBLE,_FOOTER_);
  
  vTrace("  "_Stz);
}

/*----------------------------------------------------------------------------*/
/* tab op                                                                     */
/*----------------------------------------------------------------------------*/
void Reassemble::vTabOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REASSEMBLE,_FOOTER_);
  
  if (m_i_Op_stage)
    vTrace("    | "_Stz);
  else
    vTrace("      "_Stz);
}

/*----------------------------------------------------------------------------*/
/* tab op                                                                     */
/*----------------------------------------------------------------------------*/
void Reassemble::vTabOp_commented(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REASSEMBLE,_FOOTER_);
  
  if (m_i_Op_stage)
    vTrace("//  | "_Stz);
  else
    vTrace("//    "_Stz);
}

/*----------------------------------------------------------------------------*/
/* end stage                                                                  */
/*----------------------------------------------------------------------------*/
void Reassemble::vEolStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REASSEMBLE,_FOOTER_);
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* end op                                                                     */
/*----------------------------------------------------------------------------*/
void Reassemble::vEolOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REASSEMBLE,_FOOTER_);
  
  vTrace("\n"_Stz);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
