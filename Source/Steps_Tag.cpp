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
  ENTRY();
  
  s32 sTagNm;
  m_pDataStack->vPopStr(sTagNm,__func__);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Job* pJob = m_pTask->pJob();
      
  TagSet* pJobTags = pJob->pJobTags();

  i8 i_Tag = pJobTags->i_Find(sTagNm.szStz());
  
  if (C_UNKNOWN == i_Tag) {
    vTrace("vTagGet: unknown tag: %s\n"_Stz,sTagNm.szStz());
    pJobTags->vContentDump();
    
    THROW("Step::vTagGet : unknown tag\n"_Stz);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (TN_STRING == pJobTags->eToken(i_Tag)) {
    Stz* szValue = pJobTags->szValue(i_Tag);
    m_pDataStack->vPushStz(szValue);
  }
  else {
    i8 iValue = pJobTags->iValue(i_Tag);
    m_pDataStack->vPushInt(iValue);
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* set tag value                                                              */
/*----------------------------------------------------------------------------*/
void Step::vTagSet(void)
{
  ENTRY();
  
  s32 sTagNm;
  m_pDataStack->vPopStr(sTagNm,__func__);

  sSTMT sTagValue;
  m_pDataStack->vPopStr(sTagValue,__func__);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Job* pJob = m_pTask->pJob();
      
  TagSet* pJobTags = pJob->pJobTags();

  pJobTags->vInsert(sTagNm,sTagValue);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
