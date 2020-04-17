/*============================================================================*/
/* Steps_Memory.cpp - memory step group                                       */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Memory(void)
{
  vAddStep("memory.byteDump"_Stz,   &Step::vMemoryByteDump);
  vAddStep("memory::reset"_Stz,     &Step::vMemoryReset);
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
/* memory.reset -- reset memory statistics                                    */
/*----------------------------------------------------------------------------*/
void Step::vMemoryReset(void)
{
  ENTRY();
  
  MemLib::vReset();
    
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
