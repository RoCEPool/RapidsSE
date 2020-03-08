/*============================================================================*/
/* Steps_Tag.cpp - tag step group                                             */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Tag(void)
{
  vAddStep("tag::get"_Stz,  &Step::vTagGet);
  vAddStep("tag::set"_Stz,  &Step::vTagSet);
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

  i8 i_Tag = pEnameSet->i_Find(sTagNm.szStz());
  
  if (C_UNKNOWN == i_Tag) {
    vTrace("vTagGet: unknown tag: %s\n"_Stz,sTagNm.szStz());
    pEnameSet->vContentDump();
    
    THROW("Step::vTagGet : unknown tag\n"_Stz);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (TN_STRING == pEnameSet->eToken(i_Tag)) {
    Stz* szValue = pEnameSet->szValue(i_Tag);
    m_pDataStack->vPushStz(szValue);
  }
  else {
    i8 iValue = pEnameSet->iValue(i_Tag);
    m_pDataStack->vPushInt(iValue);
  }

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
