/*============================================================================*/
/* Bench.cpp -- Bench                                                         */
/*============================================================================*/

#include "Bench.hpp"
#include "Board.hpp"
#include "Optimiser.hpp"

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

  m_pCI = new CardIter(this);
  
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
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
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

  i8 nBoards = this->nItems();
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

  i8 nBoards = this->nItems();
  for (i8 i_each = 0; i_each < nBoards; i_each++) {
    Board *pBoard = this->pGet(i_each);

    if (eBoard == pBoard->eBoardKind() && (C_UNKNOWN == i_Board ||
                                       i_Board == pBoard->i_Board()))
      return pBoard;
  }

  return nullptr;
}

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

  i8 nBoards = this->nItems();
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
/*                                               vSeries_A0_Make_Weavable */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vSA0_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->m_pCardCtl->vUseShorter();

  vST00_Begin();
}

/*----------------------------------------------------------------------------*/
/* inspect filters and remove cmp predicates which are in bounds              */
/*----------------------------------------------------------------------------*/
void Bench::vSA0_T05_Cull_Nonused_bounds_of_Chosen_index(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->vTransform(__func__, "((desc))"_Stz);

  vAssert_Board_mqA();

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain()) {

    if (!(m_pCI->G.p_MainCard->bIsCard(CK_INDEXJOIN) ||
          m_pCI->G.p_MainCard->bIsCard(CK_INDEXSCAN)))
      continue;
    if (!(m_pCI->G.p_indexList = m_pCI->G.p_MainCard->pSub_indexList()))
      continue;

    // have <bounds> & <filter> under <main>

    // find & ignore the first ebound
    bool bFoundFirst = false;

    CardIter xCI_indexes(m_pCI->G.p_indexList);
    while ((m_pCI->G.p_index = _pCard_Index(xCI_indexes.pEachChild(CK_INDEX)))) {

      bool bChosenIndex = m_pCI->G.p_index->Y_bHas(P_ChosenIndex);

      m_pCI->G.p_bounds = m_pCI->G.p_index->pSub_bounds();
      if (! m_pCI->G.p_bounds)
        continue;

      CardIter xCI_bounds(m_pCI->G.p_bounds);
      while (xCI_bounds.bEachChild()) {
        if (bChosenIndex && ! bFoundFirst && xCI_bounds.G.m_pChild->bIsCard(CK_EBOUND)) {
          bFoundFirst = true;
          continue;
        }

        xCI_bounds.G.m_pChild->vMark1(MARK_AT);
      }

      m_pCI->vPrintBoard(__LINE__);
      m_pCI->G.p_bounds->vCullBound(MARK_AT);

    }
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  m_pCI->vPrintBoard();

  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/* inspect filters and remove cmp predicates which are in bounds              */
/*----------------------------------------------------------------------------*/
void Bench::vSA0_T10_Eliminate_bounds_From_filter(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->vTransform(__func__, "((desc))"_Stz);

  vAssert_Board_mqA();

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark any cmpPred that are a bound in bounds

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain()) {

    if (! (m_pCI->G.p_MainCard->bIsCard(CK_INDEXJOIN) ||
           m_pCI->G.p_MainCard->bIsCard(CK_INDEXSCAN)))
      continue;
    if (! (m_pCI->G.p_bounds = m_pCI->G.p_MainCard->pSub_bounds()))
      continue;
    if (! (m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter()))
      continue;

    // have <bounds> & <filter> under <main>

    CardIter xCR(m_pCI->G.p_filter);
    while ((m_pCI->G.p_cmpPred = _pCard_CmpPred(xCR.pRecurseEach(CK_CMPPRED)))) {
      if (m_pCI->G.p_cmpPred->bSubsetOf_bounds(m_pCI->G.p_bounds))
        m_pCI->G.p_cmpPred->vMark1(MARK_AT);
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cull marked cmpPred from filter

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain()) {
    if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter()))
      m_pCI->G.p_filter->vCullPred(MARK_AT);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete any filter if empty

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain()) {
    if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter()))
      m_pCI->G.p_MainCard->vDeleteFilterIfEmpty(m_pCI->G.p_filter);
  }

  m_pCI->vPrintBoard();

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* individuate <scan|indexJoin>/<filter> as main <filter>                     */
/*----------------------------------------------------------------------------*/
void Bench::vSA0_T20_Individuate_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "individuate <scan|indexJoin>/<filter> as main <filter>"_Stz);

  vAssert_Board_mqA();

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_SCAN,CK_FILTER)) {
    m_pCI->G.p_scan->vRemoveChild(m_pCI->G.p_filter);
    m_pCI->G.pDeck->vInsertAfter(m_pCI->G.p_filter, m_pCI->G.p_scan);

    if (! m_pCI->G.p_filter->pSub_level()) {
      m_pCI->G.p_filter->vPush(m_pCI->G.p_scan->pSub_level()->pDupCardAndSubCards());
    }
  }

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_INDEXSCAN,CK_FILTER)) {
    Card_Filter::vIndividuate_filter(m_pCI);
  }

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_INDEXJOIN,CK_FILTER)) {
    Board::vSA0_T10_Individuate_indexJoin_filter_b(m_pCI);
  }

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_CROSSJOIN,CK_FILTER)) {
    Board::vSA0_T10c_Individuate_crossJoin_filter(m_pCI);
  }


  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vSA0_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vSTZZ_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                       vSeries_E0_To_StmOdn */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* individuate <scan|indexJoin>/<filter> as main <filter>                     */
/*----------------------------------------------------------------------------*/
void Bench::vSE0_T10_Convert_TblColOdn_To_StmOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "individuate <scan|indexJoin>/<filter> as main <filter>"_Stz);

  vAssert_Board_mqA();

  m_pCI->G.pBoard->vMayHaveStmColOdn(true);

  while (m_pCI->G_bEachMain()) {

    Board::vConvert_tblColOdn_To_stmOdn(m_pCI);
  }

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vSE0_T20_Convert_tblColOdn_Of_constraint_To_stmColNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "((20))))"_Stz);

  vAssert_Board_mqA();

  m_pCI->G.pBoard->vMayHaveStmColOdn(true);

  while (m_pCI->G_bEachMain()) {

    if (! (   CK_KEYEXISTS    == m_pCI->G.e_MainCard
              || CK_KEYNOTEXISTS == m_pCI->G.e_MainCard
    ))
      continue;

    m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint();

    Board::vConvert_tblColOdn_Of_constraint_To_stmColNm(m_pCI);
  }

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                vSeries_Z0_Restate_Pipeline */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* reinstate stages                                                           */
/*----------------------------------------------------------------------------*/
void Bench::vSZ0_T20_Reinstate_stagesV3(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->vTransform(__func__, "reinstate stages"_Stz);

  vAssert_Board_mqA();

  //m_pCI->m_pCardCtl->vPrintSubCards(true,CK_PROJELIST,0);
  //m_pCI->m_pCardCtl->vPrintSubCards(true,CK_RPNCALC,0);

  m_pCI->vPrintBoard(__LINE__);

  Board::vSZ0_T20_Reinstate_stagesV3(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* add partitioning                                                           */
/*----------------------------------------------------------------------------*/
void Bench::vSZ0_T30_Add_Partitioning(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->vTransform(__func__, "add partitioning"_Stz);

  vAssert_Board_mqA();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_Next* p_next_prior = nullptr;
  Card_Combined* p_combined_prior = nullptr;

  Card* p_main_prior = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  while (m_pCI->G_bEachMain()) {
    if (CK_INDEXJOIN == m_pCI->G.e_MainCard ||
        CK_INDEXSCAN == m_pCI->G.e_MainCard ||
        CK_KEYEXISTS == m_pCI->G.e_MainCard ||
        CK_KEYNOTEXISTS == m_pCI->G.e_MainCard ||
        CK_SCAN == m_pCI->G.e_MainCard) {
      if (p_main_prior) {
        i8 nSkip = Board::iStage_Partitioning(m_pCI, p_main_prior, p_combined_prior,
                                              p_next_prior, m_pCI->G.p_MainCard);
        while (nSkip--)
          m_pCI->G_vSkipNextMain();
        m_pCI->vPrintBoard(__LINE__);
        vDebugPoint();
      }
      p_main_prior = m_pCI->G.p_MainCard;
    }
    else
    if (CK_COMBINED == m_pCI->G.e_MainCard) {
      p_combined_prior = m_pCI->G.p_combined;
    }
    else
    if (CK_NEXT == m_pCI->G.e_MainCard) {
      p_next_prior = m_pCI->G.p_next;
    }

  }

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vAssert_Board_any(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  // nothing to do?
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vAssert_Board_mqA(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  RASSERT(0 == m_pCI->G.pBoard->i_Board());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vAssert_Board_sqB(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  RASSERT(1 == m_pCI->G.pBoard->i_Board());
}

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

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vSTZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
