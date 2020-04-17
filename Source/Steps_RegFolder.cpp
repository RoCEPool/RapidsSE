/*============================================================================*/
/* Steps_RegFolder.cpp - folder[] step group                                  */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegFolder(void)
{
  vAddStep("folder[].findSymbol"_Stz,     &Step::vRegFolderFindSymbol);
}

/*----------------------------------------------------------------------------*/
/* findInCurrent -- find and push word named on stack withinin curr dic       */
/*----------------------------------------------------------------------------*/
void Step::vRegFolderFindSymbol(void)
{
  ENTRY();
  
  s256 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  if (m_pTask->m_iTraceMask) {
    i8 i = 0; i++;
  }
  
  Word* pWord = m_oDicStack.pFindInCurrent(sName);
  
  m_pDataStack->vPushPtr(pWord);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
