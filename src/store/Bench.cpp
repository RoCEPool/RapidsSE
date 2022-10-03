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
  
  _checkInitialised_(CN_BENCH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Bench::~Bench(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
  
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

  RASSERT(! pGetBoard(pBoard->eBoard()));

  vPush(pBoard);
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
/* get board of given kind                                                    */
/*----------------------------------------------------------------------------*/
Board* Bench::pGetBoard(eBK eBoard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  i8 nBoards = this->nItems();
  for (i8 i_Board = 0; i_Board < nBoards; i_Board++) {
    Board *pBoard = this->pGet(i_Board);

    if (eBoard == pBoard->eBoard())
      return pBoard;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* print bench                                                                */
/*----------------------------------------------------------------------------*/
void Bench::vPrintBench(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  i8 nBoards = this->nItems();
  for (i8 i_Board = 0; i_Board < nBoards; i_Board++) {
    Board* pBoard = this->pGet(i_Board);
    
    Ribbon* pRibbon = new Ribbon(nullptr,pBoard,0);
  
    pBoard->vPinCards(pRibbon,i_Board);
    pRibbon->vPrintRibbon();
  
    delete pRibbon;
  }
  
  //s256 sIn;
  //fgets(sIn.azChar(),128,stdin);
  
  //Cue::vGetReturn("bench> "_Stz);
}

/*----------------------------------------------------------------------------*/
/* print bench                                                                */
/*----------------------------------------------------------------------------*/
Optimiser* Bench::pOptimiser(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  return m_pOptimiser;
}

/*----------------------------------------------------------------------------*/
/* renumber all cards                                                         */
/*----------------------------------------------------------------------------*/
//void Bench::vRenumberMainCards(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
//
//  ListExoIter2_t<Board>* pBench_I = this->pNewIter();
//
//  while (pBench_I->bHasNext()) {
//    Board* pBoard = pBench_I->pNext();
//
//    ListExoIter2_t<Card>* pBoard_I = pBoard->m_pCard_SubSet->pNewIter();
//
//    while (pBoard_I->bHasNext()) {
//      Card* pWorkUnit = pBoard_I->pNext();
//
//      pWorkUnit->vCardNo(-1);
//      pWorkUnit->vRenumberMainCards();
//    }
//    delete pBoard_I;
//
//  }
//  delete pBench_I;
//}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
