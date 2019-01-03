/*============================================================================*/
/* Steps_Memory.cpp - memory step group                                       */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Memory(void)
{
  vAddStep("memory.byteDump"_Stz,   &Step::vMemoryByteDump);
  vAddStep("memory::list"_Stz,       &Step::vMemoryList);
  vAddStep("memory::reset"_Stz,      &Step::vMemoryReset);
  vAddStep("memory.wordDump"_Stz,   &Step::vMemoryWordDump);
}

/*----------------------------------------------------------------------------*/
/* memory.bytedump -- byte-oriented dump                                      */
/*----------------------------------------------------------------------------*/
void Step::vMemoryByteDump(MP_Task mpTask)
{
ENTRY("vMemoryByteDump"_Stz);
  
  i8 iSize;
  if ( ! mpTask->m_pDataStack->bPop(iSize) )
    THROW_INVALID_DSTACK("memory.bytedump"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("memory.bytedump"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  //pWord->vByteDump(iSize);
  MemLib::vHexDump(pWord,iSize);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* memory.reset -- reset memory statistics                                    */
/*----------------------------------------------------------------------------*/
void Step::vMemoryReset(MP_Task mpTask)
{
  ENTRY("vMemoryReset"_Stz);
  
  gpAttachMap->m_iMemCopyBytes = 0;
  gpAttachMap->m_iMemMoveBytes = 0;
  gpAttachMap->m_iMemScribbleBytes = 0;
  gpAttachMap->m_iMemSetBytes  = 0;
  gpAttachMap->m_iMemZeroBytes = 0;
  
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    gpAttachMap->m_xMemCopy[i_Class] = 0;
    gpAttachMap->m_xMemMove[i_Class] = 0;
    gpAttachMap->m_xMemScribble[i_Class] = 0;
    gpAttachMap->m_xMemSet[i_Class] = 0;
    gpAttachMap->m_xMemZero[i_Class] = 0;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* memory.list-- trace memory statistics                                      */
/*----------------------------------------------------------------------------*/
void Step::vMemoryList(MP_Task mpTask)
{
  ENTRY("vMemoryList"_Stz);

  MemLib::vList();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* memory.worddump -- word-oriented dump                                      */
/*----------------------------------------------------------------------------*/
void Step::vMemoryWordDump(MP_Task mpTask)
{
ENTRY("vMemoryWordDump"_Stz);
  
  i8 iSize;
  if ( ! mpTask->m_pDataStack->bPop(iSize) )
    THROW_INVALID_DSTACK("memory.worddump"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("memory.worddump"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  //pWord->vWordDump(iSize);
  MemLib::vWordDump(pWord,iSize);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
