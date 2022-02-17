/*============================================================================*/
/* Bench.cpp -- Bench                                                         */
/*============================================================================*/

#include "Bench.hpp"
//#include "Cue.hpp"

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
Bench::Bench(void)
: Object(CN_BENCH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
    
  m_pTools = new Tools;

  _checkInitialised_(CN_BENCH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Bench::~Bench(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  delete m_pTools;
  
  vDeleteAll();
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print bench                                                                */
/*----------------------------------------------------------------------------*/
void Bench::vPrintBench(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  i8 nBoards = this->nItems();
  for (i8 i_Board = 0; i_Board < nBoards; i_Board++) {
    Board* pBoard = this->pGet(i_Board);
    
    Ribbon* pRibbon = new Ribbon(0);
  
    pBoard->vPinCards(pRibbon,i_Board);
    pRibbon->vPrintRibbon();
  
    delete pRibbon;
  }
  
  //s256 sIn;
  //fgets(sIn.azChar(),128,stdin);
  
  //Cue::vGetReturn("bench> "_Stz);
}


/*----------------------------------------------------------------------------*/
/* renumber all cards                                                         */
/*----------------------------------------------------------------------------*/
void Bench::vRenumberCards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
  
  ListExoIter2_t<Board>* pBench_I = this->pNewIter();

  while (pBench_I->bHasNext()) {
    Board* pBoard = pBench_I->pNext();
    
    ListExoIter2_t<Card>* pBoard_I = pBoard->pNewIter();

    while (pBoard_I->bHasNext()) {
      Card* pWorkUnit = pBoard_I->pNext();
      
      pWorkUnit->vCardNo(-1);
      pWorkUnit->vRenumberSubCards();
    }
    delete pBoard_I;

  }
  delete pBench_I;
}




/*----------------------------------------------------------------------------*/
/* call closure for each card                                                 */
/*----------------------------------------------------------------------------*/
void Bench::vWalkUnits(std::function<void(Card* pCard)> f_vClosure)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  ListExoIter2_t<Board>* pBench_I = pNewIter();

  while (pBench_I->bHasNext()) {
    Board* pBoard = pBench_I->pNext();
    
    pBoard->vWalk(f_vClosure);
  }
  delete pBench_I;
}

/*----------------------------------------------------------------------------*/
/* call closure for each board                                                */
/*----------------------------------------------------------------------------*/
void Bench::vWalkAllCards(std::function<void(Card* pCard)> f_vClosure)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  ListExoIter2_t<Board>* pBench_I = pNewIter();

  while (pBench_I->bHasNext()) {
    Board* pBoard = pBench_I->pNext();
    
    pBoard->vWalkAllCards(f_vClosure);
  }
  delete pBench_I;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
