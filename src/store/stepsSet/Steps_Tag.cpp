/*============================================================================*/
/* Steps_Tag.cpp - tag step group                                             */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Job.hpp"
#include "Op.hpp"

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

  TagSet* pTags = nullptr;

  i8 i_Tag = C_UNKNOWN;
  
  //RASSERT( (m_pOp && ! m_pJob) || (! m_pOp && m_pJob) );
  
  if (C_UNKNOWN == i_Tag && m_pOp) {
    pTags = m_pOp->pOpTags();
    i_Tag = pTags->i_Find(sTagNm.szStz());
  }

  if (C_UNKNOWN == i_Tag && m_pJob_task) {
    pTags = m_pJob_task->pJobTags();
    i_Tag = pTags->i_Find(sTagNm.szStz());
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  if (C_UNKNOWN == i_Tag) {
    vTrace("vTagGet: unknown tag: %s\n"_Stz,sTagNm.szStz());
    pTags->vContentDump();
    
    THROW("Step::vTagGet : unknown tag\n"_Stz);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (TN_STRING == pTags->eToken(i_Tag)) {
    Stz* szValue = pTags->szValue(i_Tag);
    //vTrace("TagGet %s stz %s\n"_Stz,sTagNm.azChar(),szValue);
    m_pDataStack->vPushStz(szValue);
  }
  else {
    i8 iValue = pTags->iValue(i_Tag);
    //vTrace("TagGet %s int %lld\n"_Stz,sTagNm.azChar(),iValue);
    m_pDataStack->vPushInt(iValue);
  }

  EXIT();
}


/*----------------------------------------------------------------------------*/
//void Step::vTagGet(void)
//{
//  ENTRY();
//  
//  i8 i_Tag = C_UNKNOWN;
//  
//  s32 sTagNm;
//  m_pDataStack->vPopStr(sTagNm,__func__);
//
//  TagSet* pTagSet = nullptr;
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  
//  if (m_pOp) {
//    pTagSet = m_pOp->pOpTagSet();
//    i_Tag = pTagSet->i_Find(sTagNm.szStz());
//  }
//
//  if (C_UNKNOWN == i_Tag && m_pJob) {
//    pTagSet = m_pJob->pJobTags();
//    i_Tag = pTagSet->i_Find(sTagNm.szStz());
//  }
//  
//  if (C_UNKNOWN == i_Tag) {
//    Job* pPipelineJob = AM::pPipelineJob();
//    if (pPipelineJob) {
//      pTagSet = pPipelineJob->pJobTags();
//      i_Tag = pTagSet->i_Find(sTagNm.szStz());
//    }
//  }
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    
//  if (C_UNKNOWN == i_Tag) {
//    vTrace("vTagGet: unknown tag: %s\n"_Stz,sTagNm.szStz());
//    pTagSet->vContentDump();
//    
//    THROW("Step::vTagGet : unknown tag\n"_Stz);
//  }
//  
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  
//  if (TN_STRING == pTagSet->eToken(i_Tag)) {
//    Stz* szValue = pTagSet->szValue(i_Tag);
//    m_pDataStack->vPushStz(szValue);
//  }
//  else {
//    i8 iValue = pTagSet->iValue(i_Tag);
//    m_pDataStack->vPushInt(iValue);
//  }
//
//  EXIT();
//}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
