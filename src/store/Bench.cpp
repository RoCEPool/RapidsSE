/*============================================================================*/
/* Bench.cpp -- Bench                                                         */
/*============================================================================*/

#include "Bench.hpp"
#include "Dissect.hpp"
#include "Reassemble.hpp"

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
/* vDissect the pipeline spec                                                 */
/*----------------------------------------------------------------------------*/
void Bench::vDissect(PipelineSpec* pPipelineSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
      
  Dissect* pDissect = new Dissect(this);
  pDissect->vDissectSpec(pPipelineSpec);
  delete pDissect;
}

/*----------------------------------------------------------------------------*/
/* expand                                                                     */
/*----------------------------------------------------------------------------*/
void Bench::vExpand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
    
  ListExoIter2_t<Board>* pBench_I = pNewIter();

  while (pBench_I->bHasNext()) {
    Board* pBoard = pBench_I->pNext();
    
    m_pTools->vExpandStreamDefs(pBoard);
  }
  delete pBench_I;
}

/*----------------------------------------------------------------------------*/
/* rework                                                                     */
/*----------------------------------------------------------------------------*/
void Bench::vRework(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* reassemble                                                                 */
/*----------------------------------------------------------------------------*/
void Bench::vSynthesize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
        
  Board* pBoard = pGet(0);
  Reassemble* pReassemble = new Reassemble(pBoard);
  pReassemble->vReassemble();
  delete pReassemble;
  
}

/*----------------------------------------------------------------------------*/
/* print bench                                                                */
/*----------------------------------------------------------------------------*/
void Bench::vPrintBench(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  i8 nBoards = nItems();
  for (i8 i_Board = 0; i_Board < nBoards; i_Board++) {
    Board* pBoard = pGet(i_Board);
    
    Ribbon* pRibbon = new Ribbon(0);
  
    pBoard->vPinBoard(pRibbon,i_Board);
    pRibbon->vPrintRibbon();
  
    delete pRibbon;
  }
  
  s256 sIn;
  fgets(sIn.azChar(),128,stdin);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
