/*============================================================================*/
/* Bench.cpp -- Bench                                                         */
/*============================================================================*/

#include "Bench.hpp"
#include "Board.hpp"
#include "Optimiser.hpp"
#include "CardIter.hpp"
#include "Smoosh.hpp"
#include "Smooth.hpp"
#include "Fold.hpp"
#include "Flattening.hpp"
#include "Optimisations.hpp"
#include "MakeWeavable.hpp"

#include "CarryForward.hpp"
#include "Error.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Bench,CN_BENCH)
__aNewFunction(Bench,CN_BENCH)
__vDeleteFunction(Bench,CN_BENCH)
__aFooterFunction(Bench)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Bench::Bench(Optimiser* pOptimiser)
: Object(CN_BENCH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
    
  m_pOptimiser = pOptimiser;

  m_cPrefix = ' ';

  m_bIsFlat = false;

  m_iOdnLo      = 1;
  m_iOdnHi      = C_UNKNOWN;
  m_cTableSyn   = 'a';

  m_pCI = new CardIter(this);

  m_pFlattening = nullptr;
  
  _checkInitialised_(CN_BENCH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Bench::~Bench(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
  delete m_pCI;
  vDeleteAll();

  delete m_pFlattening;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- series ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* transform matrix                                                           */
/*----------------------------------------------------------------------------*/
bool Bench::bApplyTransformationMatrix(eEC* peErrorCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  bool bClean = true;

  //m_pCardCtl->vReset();
  //m_pCardCtl->vPrintSubCards(false,CK_AGGELIST,CK_KEY,CK_PROJECTION,0);
  //m_pCardCtl->vPrintSubCards(CK_AGGELIST, true);
  //m_pCardCtl->vPrintSubCards(CK_KEY, true);

  try {
    //m_pOptimiser->pCardCtl()->vPrintSubCards(false, //CK_AGGELIST,
        //CK_KEY, //CK_PROJECTION,
        //CK_RPNCALC,
    //                                          0);
    m_pOptimiser->pCardCtl()->vSaveShorter();
    m_pOptimiser->pCardCtl()->vSaveXhand();

    vApplyTransformationMatrix();

    bClean = true;
  }

  catch (Exception xException) {
    // set peErrorCode
    bClean = false;
  }

  catch (Error xError) {
    *peErrorCode = xError.eErrorCode();
    // todo: this should read xError.szErrorMsg()
    //Stz* szError = xError.aErrorMsg();

    bClean = false;
  }

  return bClean;
}

/*----------------------------------------------------------------------------*/
/* apply the recursive transformation matrix                                  */
/*----------------------------------------------------------------------------*/
void Bench::vApplyTransformationMatrix(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vSeries_PrepareSubQueries();
  vSeries_MakeRunnable();

  vSeries_Optimisations();

  MakeWeavable* pMakeWeavable = new MakeWeavable(m_pCI);
  pMakeWeavable->vSeries_MakeWeavable();
  delete pMakeWeavable;
}

/*----------------------------------------------------------------------------*/
/* apply the reducePipeline set of transformations to all subPipe boards      */
/*----------------------------------------------------------------------------*/
void Bench::vSeries_PrepareSubQueries(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vSeries(BK_SUBQUERY, 0, __func__);

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {
    m_pCI->vUsingBoard(BK_SUBQUERY, m_pCI->G.i_Board);

    vGroup_InitialAdjustments();
    vGroup_CondenseInfo();
    vGroup_MetaAnalysis();
    vGroup_PreliminaryConversions();
  }

  vTransform_OuterQueryAnalysis();

  vEndSeries(BK_SUBQUERY);
}

/*----------------------------------------------------------------------------*/
/* apply the reducePipeline set of transformations to all subPipe boards      */
/*----------------------------------------------------------------------------*/
void Bench::vSeries_MakeRunnable(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vSeries(BK_SUBQUERY, 0, __func__);

  m_pCI->G.pSubQuery = m_pCI->G.pBoard->pSubQuery();

  m_pFlattening = new Flattening(m_pCI);

  m_pFlattening->vSeries_Flatten_recurse();

  delete m_pFlattening;
  m_pFlattening = nullptr;

  vEndSeries(BK_SUBQUERY);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vSeries_Optimisations(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vSeries(BK_SUBQUERY, 0, __func__);

  i8 i_Board = 0;

  m_pCI->vUsingBoard(BK_SUBQUERY, i_Board);

  Optimisations* pOptimisations = new Optimisations(m_pCI);

  pOptimisations->vGroup_ChooseEEops();

  delete pOptimisations;

  vEndSeries(BK_SUBQUERY);
}



/*----------------------------------------------------------------------------*/
/*-- group -------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_InitialAdjustments(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vTransform_MoveOn();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_CondenseInfo(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vTransform_DeleteUnneeded();
  vTransform_ScanBecomesTable();
  vTransform_AddLevel();
  vTransform_NotOps();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_MetaAnalysis(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vTransform_TableAnalysis();
  vTransform_ProjectionAnalysis();
  vTransform_IndexAnalysis();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_PreliminaryConversions(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  //vTransform_StmColToTblCol_originalModel();
  vTransform_RpnCalcToBoolCalc();
}


/*----------------------------------------------------------------------------*/
/*-- transform ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* move all <absJoin>/<on> to <where>                                         */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_MoveOn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "move <on> in <absJoin>'s to <where>"_Stz);

  Card_Where::vMove_All_on_To_where(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Condense Info                                                              */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_DeleteUnneeded(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "delete superfluous <absJoin>'s"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete All AbsJoin
  Card_AbsJoin::vDeleteAllAbsJoin(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete All Combined
  m_pCI->G_vRestart();
  Card_Combined::vDeleteAllCombined(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete All Send
  m_pCI->G_vRestart();
  Card_Send::vDeleteAllSend(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete All Stage
  m_pCI->G_vRestart();
  Card_Stage::vDeleteAllStage(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Condense Info                                                              */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_ScanBecomesTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "delete superfluous <absJoin>'s"_Stz);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain(CK_SCAN))
    Card_Scan::vScanBecomesTable(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Add Levels                                                                 */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_AddLevel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "delete superfluous <absJoin>'s"_Stz);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain())
    ((Card_OP*)m_pCI->G.p_MainCard)->vAddLevel();

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Not Ops                                                                    */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_NotOps(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachSubOfMain(CK_RPNCALC))
    m_pCI->G.p_rpnCalc->vConvert_sqyOp_not_To_notSqyOp();
}

/*----------------------------------------------------------------------------*/
/* Table Analysis                                                             */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_TableAnalysis(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "add a <tableSyn> to all <table>'s"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Affix Table Syntax To All Table
  Card_Table::vAffixTableSyn(m_pCI, &m_cTableSyn);

//  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_TABLENM))  {
//    m_pCI->G.p_table->vSet_Syn(&m_cTableSyn);
//  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Table Partition Under All Table
  m_pCI->G_vRestart();
  Card_Table::vProduceTablePtns(m_pCI);

//  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_TABLENM)) {
//
//    m_pCI->G.p_tablePtn = Card_TablePtn::pDerive(m_pCI->G.p_table);
//    m_pCI->G.p_table->vAppend(m_pCI->G.p_tablePtn);
//
//    Card_TableNm* p_tableNm = m_pCI->G.p_table->pSub_tableNm();
//    Table* pTable = AM::pTableFind(p_tableNm->sz());
//
//    m_pCI->G.p_size = Card_Size::pDerive(pTable);
//    m_pCI->G.p_table->vAppend(m_pCI->G.p_size);
//  }

  m_pCI->vEndTransform();
}
/*----------------------------------------------------------------------------*/
//void Bench::vS20b_T_Table_Analysis(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
//
//  m_pCI->vTransform(__func__, "add a <tableSyn> to all <table>'s"_Stz);
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // Affix Table Syntax To All Table
//
//  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_TABLENM))  {
//    m_pCI->G.p_table->vSet_Syn(&m_cTableSyn);
//  }
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // Produce Table Partition Under All Table
//
//  m_pCI->G_vRestart();
//
//  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_TABLENM))
//    m_pCI->G.p_table->vProjectionAnalysis(this);
//
//
//  m_pCI->vEndTransform();
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_IndexAnalysis(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "move @c predicates to a filter under table"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Append Index List Under All Table
  Card_Table::vAppendIndexList(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Affix TblColOdn To All IdxCol

  m_pCI->G_vRestart();
  Card_Table::vAffixTableColOrd(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Projection Analysis                                                        */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_ProjectionAnalysis(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "((desc))"_Stz);

  Card_Projection::vProjection_Analysis_v2(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_RpnCalcToBoolCalc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "all $n become $c.n"_Stz);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_RPNCALC))
    ((Card_OP*)m_pCI->G.p_MainCard)->vConvert_rpnCalc_To_boolCalc(m_pCI);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_HAVING, CK_RPNCALC))
    ((Card_OP*)m_pCI->G.p_MainCard)->vConvert_rpnCalc_To_boolCalc(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Analyse all sub-queries                                                    */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_OuterQueryAnalysis(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  //m_pCI->vTransform(__func__, "Analyse all sub-queries"_Stz);

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {

    SubQuery* pOuterQuery = m_pCI->G.pBoard->pSubQuery();
    pOuterQuery->vDisposition(QD_LINEAR);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    m_pCI->vRestart();
    while (m_pCI->G_bEachMain()) {
      if (m_pCI->G.p_MainCard->bIsCard(CK_AGGREG) ||
          m_pCI->G.p_MainCard->bIsCard(CK_GBYAGG)) {
        pOuterQuery->vDisposition(QD_AGGREGATION);
        break;
      }
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // search for from calls to sub-queries

    m_pCI->vRestart();
    while (m_pCI->G_bEachMain(CK_CALL)) {

//      Stz* szBoardNm = m_pCI->G.p_call->szValue();
//      Board* pBoard = pGetBoard(szBoardNm);
//      SubQuery *pSubQuery = pBoard->pSubQuery();
//      pSubQuery->vCaller(m_pCI->G.i_Board);
//      pSubQuery->vClause(QC_FROM_CLAUSE);

      Board::vOuter_Query_Analysis_Of_from_call(m_pCI);
      pOuterQuery->vSubQueries(pOuterQuery->nSubQueries() + 1);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // search for where calls to sub-queries

    m_pCI->vRestart();
    while (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
      Board::vOuter_Query_Analysis_Of_where_boolCalc(m_pCI);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // search for having calls to sub-queries

    m_pCI->vRestart();
    while (m_pCI->G_bEachMainWithSub(CK_HAVING, CK_BOOLCALC)) {
      Board::vOuter_Query_Analysis_Of_having_boolCalc(m_pCI);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // search for parameters

    m_pCI->G_vRestart();
    i8 nParams = 0;
    while (m_pCI->G_bEachMain(CK_CORRELATION))
       nParams++;
    pOuterQuery->vCorrCols(nParams);

  }

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {
    SubQuery* pOuterQuery = m_pCI->G.pBoard->pSubQuery();
    pOuterQuery->vDump();
  }

    //m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*-- other -------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/






/*----------------------------------------------------------------------------*/
/* add a new board                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vAddBoard(Board* pBoard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  //RASSERT(! pGetBoard(pBoard->eBoardKind()));

  vPush(pBoard);
}

/*----------------------------------------------------------------------------*/
/* does given board exist?                                                    */
/*----------------------------------------------------------------------------*/
bool Bench::bBoardExists(eBK eBoard, i8 i_SubBoard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  if (pGetBoard(eBoard,i_SubBoard))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get number of board                                                        */
/*----------------------------------------------------------------------------*/
i8 Bench::nBoards(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  i8 nBoards = nItems();

  return nBoards;
}

/*----------------------------------------------------------------------------*/
/* get card registers                                                         */
/*----------------------------------------------------------------------------*/
CardCtl* Bench::pCardCtl(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  return m_pOptimiser->pCardCtl();
}

/*----------------------------------------------------------------------------*/
/* clear marks                                                                */
/*----------------------------------------------------------------------------*/
void Bench::vClearMarks(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  i8 nBoards = this->nBoards();
  for (i8 i_Board = 0; i_Board < nBoards; i_Board++) {
    Board *pBoard = this->pGet(i_Board);

    pBoard->vClearMarks();
  }
}



/*----------------------------------------------------------------------------*/
/* get board of given kind                                                    */
/*----------------------------------------------------------------------------*/
Board* Bench::pGetBoard(eBK eBoard, i8 i_Board)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  if (i_Board >= nBoards())
    return nullptr;

  Board *pBoard = this->pGet(i_Board);
  ASSERT(pBoard->i_Board() == i_Board);
  return pBoard;

//  i8 nBoards = this->nBoards();
//  for (i8 i_each = 0; i_each < nBoards; i_each++) {
//    Board *pBoard = this->pGet(i_each);
//
//    if (eBoard == pBoard->eBoardKind() &&
//        i_Board == pBoard->i_Board())
//      return pBoard;
//  }
//
//  return nullptr;
}
/*----------------------------------------------------------------------------*/
//Board* Bench::pGetBoard(eBK eBoard, i8 i_Board)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);
//
//  i8 nBoards = this->nBoards();
//  for (i8 i_each = 0; i_each < nBoards; i_each++) {
//    Board *pBoard = this->pGet(i_each);
//
//    if (eBoard == pBoard->eBoardKind() && (C_UNKNOWN == i_Board ||
//                                           i_Board == pBoard->i_Board()))
//      return pBoard;
//  }
//
//  return nullptr;
//}


/*----------------------------------------------------------------------------*/
/* get board                                                                  */
/*----------------------------------------------------------------------------*/
Board* Bench::pGetBoard(i8 i_Board)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  Board *pBoard = this->pGet(i_Board);

  return pBoard;
}

/*----------------------------------------------------------------------------*/
/* get board of given name                                                    */
/*----------------------------------------------------------------------------*/
Board* Bench::pGetBoard(Stz* szBoardNm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  i8 nBoards = this->nBoards();
  for (i8 i_Board = 0; i_Board < nBoards; i_Board++) {
    Board *pBoard = this->pGet(i_Board);

    if (pBoard->szBoardNm()->bSame(szBoardNm))
      return pBoard;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* is the bench flat?                                                         */
/*----------------------------------------------------------------------------*/
bool Bench::bIsFlat(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  return m_bIsFlat;
}

/*----------------------------------------------------------------------------*/
/* set is the bench flat?                                                     */
/*----------------------------------------------------------------------------*/
void Bench::vIsFlat(bool bIsFlat)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_bIsFlat = bIsFlat;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Optimiser* Bench::pOptimiser(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  return m_pOptimiser;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
char Bench::cPrefix(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  return m_cPrefix;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vPrefix(char cPrefix)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_cPrefix = cPrefix;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vEndSeries(eBK eBoard, i8 i_Board)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  RASSERT(0 == m_pCI->iTransformDepth());

  //m_pTransforms->vXform(C_NA);
  //m_pCardCtl->vUseXhand();

  m_pCI->vPrintBench(eBoard, i_Board);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vSeries(eBK eBoard, i8 i_Board, const char* azHand)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->vUsingBoard(eBoard, i_Board);
  m_pCI->vXhand(azHand);

  vClearMarks();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                      vSeries_80_Index_Scan */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS80_T30_Morph_scan_To_indexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "((replace with an explanation))"_Stz);

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMainWith2Sub(CK_SCAN,CK_INDEXLIST,CK_BOUNDS)) {
    if (m_pCI->G.p_bounds->pSub_lBound() || m_pCI->G.p_bounds->pSub_uBound()) {
      m_pCI->G.p_scan->vCard(CK_INDEXSCAN);

      m_pCI->G.p_indexList->vChoose_index_For_indexScan();
    }
  }

  m_pCI->vPrintBoard();

  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                 vSeries_90_Stream_Ordering */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS90_T30_Derive_order(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->vTransform(__func__, "remove <sort> if <sbyElist> is the same as the <gbyElist>"_Stz);

  
  Board::vDerive_order(m_pCI);

  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vST00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  if (! m_pCI->m_pCardCtl->bBoardPrint())
    return;

  char* azSeparator = (char*)
      "<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>"
      "<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>"
      "<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>";

  vTrace("\n%s\n%186s\n%s\n"_Stz,
         azSeparator,
         m_pCI->szXhand(),
         azSeparator);

  m_pCI->vXdesc(""_Stz);
  m_pCI->vXform(""_Stz);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
