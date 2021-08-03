/*============================================================================*/
/* Steps_Tag.cpp - tag step group                                             */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Tag(void)
{
  vAddStep("tag::get"_Stz,  &Step::vTagGet);
}

/*----------------------------------------------------------------------------*/
/* decode tag into it value                                                   */
/*----------------------------------------------------------------------------*/
void Step::vTagGet(void)
{
  ENTRY();
  
  s32 sTagNm;
  m_pDataStack->vPopStr(sTagNm,__func__);

  TagSet* pTagSet = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pTagSet = m_pJob->pJobTags();

  i8 i_Tag = pTagSet->i_Find(sTagNm.szStz());
  
  if (C_UNKNOWN == i_Tag) {
    vTrace("vTagGet: unknown tag: %s\n"_Stz,sTagNm.szStz());
    pTagSet->vContentDump();
    
    THROW("Step::vTagGet : unknown tag\n"_Stz);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (TN_STRING == pTagSet->eToken(i_Tag)) {
    Stz* szValue = pTagSet->szValue(i_Tag);
    m_pDataStack->vPushStz(szValue);
  }
  else {
    i8 iValue = pTagSet->iValue(i_Tag);
    m_pDataStack->vPushInt(iValue);
  }

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
