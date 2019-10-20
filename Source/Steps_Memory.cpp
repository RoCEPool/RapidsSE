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
ENTRY("vMemoryByteDump"_Stz);
  
  i8 iSize;
  if ( ! m_pDataStack->bPopI8(iSize) )
    THROW_DSTACK();
  
  Object* pWordObject;
  if ( ! m_pDataStack->bPopObject(pWordObject) )
    THROW_DSTACK();
  Word* pWord = _pWord(pWordObject);
  
  //pWord->vByteDump(iSize);
  MemLib::vHexDump(pWord,iSize);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* memory.reset -- reset memory statistics                                    */
/*----------------------------------------------------------------------------*/
void Step::vMemoryReset(void)
{
  ENTRY("vMemoryReset"_Stz);
  
  MemLib::vReset();
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* memory.statistics-- trace memory statistics                                */
/*----------------------------------------------------------------------------*/
void Step::vMemoryStatistics(void)
{
  ENTRY("vMemoryStatistics"_Stz);

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* memory.worddump -- word-oriented dump                                      */
/*----------------------------------------------------------------------------*/
void Step::vMemoryWordDump(void)
{
ENTRY("vMemoryWordDump"_Stz);
  
  i8 iSize;
  if ( ! m_pDataStack->bPopI8(iSize) )
    THROW_DSTACK();
  
  Object* pWordObject;
  if ( ! m_pDataStack->bPopObject(pWordObject) )
    THROW_DSTACK();
  Word* pWord = _pWord(pWordObject);
  
  //pWord->vWordDump(iSize);
  MemLib::vWordDump(pWord,iSize);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
