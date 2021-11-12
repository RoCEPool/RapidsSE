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
    
  m_pBoard = new Board("Board"_Stz);

  _checkInitialised_(CN_BENCH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Bench::~Bench(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  delete m_pBoard;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* vDissect the pipeline spec                                                   */
/*----------------------------------------------------------------------------*/
void Bench::vDissect(PipelineSpec* pPipelineSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
      
  Dissect* pDissect = new Dissect(m_pBoard);
  pDissect->vDissectSpec(pPipelineSpec);
  delete pDissect;
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
        
  Reassemble* pReassemble = new Reassemble(m_pBoard);
  pReassemble->vReassemble();
  delete pReassemble;
}

/*----------------------------------------------------------------------------*/
/* print bench                                                                */
/*----------------------------------------------------------------------------*/
void Bench::vPrintBench(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  Ribbon* pRibbon = new Ribbon(0);
  
  m_pBoard->vPinBoard(pRibbon);    
  pRibbon->vPrintRibbon();
  
  delete pRibbon;
  
  s256 sIn;
  fgets(sIn.azChar(),128,stdin);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
