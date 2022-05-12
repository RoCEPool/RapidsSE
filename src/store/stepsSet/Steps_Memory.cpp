/*============================================================================*/
/* Steps_Memory.cpp - memory step group                                       */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Memory(void)
{
  vAddStep("memory.byteDump"_Stz,   &Step::vMemoryByteDump);
  vAddStep("memory::statistics"_Stz,&Step::vMemoryStatistics);
  vAddStep("memory.wordDump"_Stz,   &Step::vMemoryWordDump);
}

/*----------------------------------------------------------------------------*/
/* memory.bytedump -- byte-oriented dump                                      */
/*----------------------------------------------------------------------------*/
void Step::vMemoryByteDump(void)
{
  ENTRY();
  
  i8 iSize = m_pDataStack->iPopInt(__func__);
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  //pWord->vByteDump(iSize);
  MemLib::vHexDump(pWord,iSize);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* memory.statistics-- trace memory statistics                                */
/*----------------------------------------------------------------------------*/
void Step::vMemoryStatistics(void)
{
  ENTRY();

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* memory.worddump -- word-oriented dump                                      */
/*----------------------------------------------------------------------------*/
void Step::vMemoryWordDump(void)
{
  ENTRY();
  
  i8 iSize = m_pDataStack->iPopInt(__func__);
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  //pWord->vWordDump(iSize);
  MemLib::vWordDump(pWord,iSize);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
