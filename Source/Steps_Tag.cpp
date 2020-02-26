/*============================================================================*/
/* Steps_Tag.cpp - tag step group                                             */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Tag(void)
{
  vAddStep("tagGet"_Stz,  &Step::vTagGet);
  vAddStep("tagSet"_Stz,  &Step::vTagSet);
}

/*----------------------------------------------------------------------------*/
/* decode tag into it value                                                   */
/*----------------------------------------------------------------------------*/
void Step::vTagGet(void)
{
  ENTRY(); // ENTRY("vTagGet"_Stz);
  
  s32 sTagNm;
  m_pDataStack->vPopStr(sTagNm,__func__);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Job* pJob = m_pTask->pJob();
      
  EnameSet* pEnameSet = pJob->pEnameSet();
  
  Stz* szValue = pEnameSet->szValue(sTagNm.szStz());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pDataStack->vPushStz(szValue);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* set tag value                                                              */
/*----------------------------------------------------------------------------*/
void Step::vTagSet(void)
{
  ENTRY(); // ENTRY("vTagSet"_Stz);
  
  s32 sTagNm;
  m_pDataStack->vPopStr(sTagNm,__func__);

  s4K sTagValue;
  m_pDataStack->vPopStr(sTagValue,__func__);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Job* pJob = m_pTask->pJob();
      
  EnameSet* pEnameSet = pJob->pEnameSet();

  pEnameSet->vAdd(sTagNm,sTagValue);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
