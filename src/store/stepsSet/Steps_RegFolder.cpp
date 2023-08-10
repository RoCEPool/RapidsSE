/*============================================================================*/
/* Steps_RegFolder.cpp - Folder[] step group                                  */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegFolder(void)
{
  vAddStep("Folder[].findSymbol"_Stz,     &Step::vRegFolderFindSymbol);
}

/*----------------------------------------------------------------------------*/
/* findInCurrent -- find and push word named on stack withinin curr dic       */
/*----------------------------------------------------------------------------*/
void Step::vRegFolderFindSymbol(void)
{
  ENTRY();
  
  s256 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  if (m_iTraceMask) {
    i8 i = 0; i++;
  }
  
  Word* pWord = m_oDicStack.pFindInCurrent(sName);
  
  m_pDataStack->vPushPtr(pWord);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
