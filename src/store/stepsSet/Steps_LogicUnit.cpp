/*============================================================================*/
/* Steps_LogicUnit.cpp -- LogicUnit step group                                */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

#include "Op.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

#include "MY.hpp"
#include "TagConstants.h"


extern EnvFlags gxEnvFlags;;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_LogicUnit(void)
{
  vAddStep("LogicUnit[].callAgain@"_Stz,    &Step::vLogicUnitCallAgainFetch);
  vAddStep("LogicUnit[].callAgain!"_Stz,    &Step::vLogicUnitCallAgainStore);
  vAddStep("LogicUnit[].calledAgain@"_Stz,  &Step::vLogicUnitCalledAgainFetch);
  vAddStep("LogicUnit[].calledAgain!"_Stz,  &Step::vLogicUnitCalledAgainStore);
  vAddStep("LogicUnit[].cleanup"_Stz,       &Step::vLogicUnitCleanup);
  vAddStep("LogicUnit[].compilerToExit"_Stz,&Step::vLogicUnitCompilerToExit);

  vAddStep("LogicUnit[].diaryAction"_Stz,   &Step::vLogicUnitDiaryAction);
  vAddStep("LogicUnit[].diaryDir"_Stz,      &Step::vLogicUnitDiaryDir);
  vAddStep("LogicUnit[].diary@"_Stz,        &Step::vLogicUnitDiaryFetch);
  vAddStep("LogicUnit[].diaryJobNote"_Stz,  &Step::vLogicUnitDiaryJobNote);
  vAddStep("LogicUnit[].diaryName"_Stz,     &Step::vLogicUnitDiaryName);
  vAddStep("LogicUnit[].diaryNote"_Stz,     &Step::vLogicUnitDiaryNote);
  vAddStep("LogicUnit[].diaryTitle"_Stz,    &Step::vLogicUnitDiaryTitle);
    
  vAddStep("LogicUnit[].handler@"_Stz,      &Step::vLogicUnitHandlerFetch);
  vAddStep("LogicUnit[].handler!"_Stz,      &Step::vLogicUnitHandlerStore);
  
  vAddStep("LogicUnit[].init"_Stz,          &Step::vLogicUnitInit);
  vAddStep("LogicUnit[].IsLftCrwBuffer@"_Stz,  &Step::vLogicUnitIsLftCrwBufferFetch);
  vAddStep("LogicUnit[].IsLftCrwBuffer!"_Stz,  &Step::vLogicUnitIsLftCrwBufferStore);
  vAddStep("LogicUnit[].intStack@"_Stz,     &Step::vLogicUnitIntStackFetch);
  vAddStep("LogicUnit[].intStack!"_Stz,     &Step::vLogicUnitIntStackStore);

  vAddStep("LogicUnit[].jobMetrics@"_Stz,   &Step::vLogicUnitJobMetricsFetch);
  vAddStep("LogicUnit[].jobMetrics!"_Stz,   &Step::vLogicUnitJobMetricsStore);
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].callAgain@ --                                                  */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCallAgainFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xRluCommon.m_bCallAgain);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].callAgain! --                                                  */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCallAgainStore(void)
{
  ENTRY();

  i8 iCallAgain = m_pDataStack->iPopIntBol(__func__);
  
  m_pLogicUnit->m_xRluCommon.m_bCallAgain = iCallAgain;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].calledAgain@ --                                                */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCalledAgainFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xRluCommon.m_bCalledAgain);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].calledAgain! --                                                */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCalledAgainStore(void)
{
  ENTRY();

  i8 iCallAgain = m_pDataStack->iPopIntBol(__func__);
  
  m_pLogicUnit->m_xRluCommon.m_bCalledAgain = iCallAgain;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].cleanup --                                                     */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCleanup(void)
{
  ENTRY();

  m_pLogicUnit->vCleanup();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].compilerToExit --                                              */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCompilerToExit(void)
{
  ENTRY();

  bool bCompilerToExit = m_pLogicUnit->m_bCompilerToExit;

  m_pDataStack->vPushInt(bCompilerToExit);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].diaryAction -- add a longer diary action note                  */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDiaryAction(void)
{
  ENTRY();
 
  i8 i_EnumGroup = m_pDataStack->iPopInt(__func__);
  i8 i_Enum      = m_pDataStack->iPopInt(__func__);
  i8 iItem       = m_pDataStack->iPopInt(__func__);

  i8 i_Action       = m_pDataStack->iPopInt(__func__);
  i8 i_ActEnumGroup = m_pDataStack->iPopInt(__func__);
  i8 i_ActEnum      = m_pDataStack->iPopInt(__func__);

  if ( ! AM::bRLUDiary() ) {
    EXIT();
    return;
  }
  
  m_pLogicUnit->m_oSince.vUpdate();
  i8 nNanoSeconds = m_pLogicUnit->m_oSince.iNanoseconds();
  
  i8 nQueueLength = m_pOp->nQueuedOperands();

  m_pLogicUnit->vDiaryNote(i_EnumGroup,i_Enum,iItem,nNanoSeconds,
                           i_Action,i_ActEnumGroup,i_ActEnum,nQueueLength);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].diary@ -- diary fetch                                          */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDiaryFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(m_pLogicUnit->m_pRluDiary);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].diaryJobNote -- set diary job note                             */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDiaryJobNote(void)
{
  ENTRY();
  
  s256 sJobNote;
  m_pDataStack->vPopStr(sJobNote, __func__);

  if ( ! AM::bRLUDiary() ) {
    EXIT();
    return;
  }
  
  m_pLogicUnit->vDiaryJobNote(sJobNote);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].diaryTitle -- set diary title                                  */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDiaryName(void)
{
  ENTRY();
  
  s32 sName;
  m_pDataStack->vPopStr(sName, __func__);

  if ( ! AM::bRLUDiary() ) {
    EXIT();
    return;
  }
  
  m_pLogicUnit->vDiaryName(sName);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].diaryNote -- add a diary note                                  */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDiaryNote(void)
{
  ENTRY();
  
  i8 i_EnumGroup = m_pDataStack->iPopInt(__func__);
  i8 i_Enum      = m_pDataStack->iPopInt(__func__);
  i8 iItem       = m_pDataStack->iPopInt(__func__);
  
  if ( ! AM::bRLUDiary() ) {
    EXIT();
    return;
  }
  
  m_pLogicUnit->m_oSince.vUpdate();
  i8 nNanoSeconds = m_pLogicUnit->m_oSince.iNanoseconds();
  
  
  i8 nQueueLength = m_pOp->nQueuedOperands();
  
  m_pLogicUnit->vDiaryNote(i_EnumGroup,i_Enum,iItem,nNanoSeconds,
                           0,0,0,nQueueLength);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].diaryDir -- set diary dir                                      */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDiaryDir(void)
{
  ENTRY();
  
  s256 sDir;
  m_pDataStack->vPopStr(sDir, __func__);

  if ( ! AM::bRLUDiary() ) {
    EXIT();
    return;
  }
  
  m_pLogicUnit->vDiaryDir(sDir);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].diaryTitle -- set diary title                                  */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDiaryTitle(void)
{
  ENTRY();
  
  s256 sTitle;
  m_pDataStack->vPopStr(sTitle, __func__);

  if ( ! AM::bRLUDiary() ) {
    EXIT();
    return;
  }
  
  m_pLogicUnit->vDiaryTitle(sTitle);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].handler@ --                                                    */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitHandlerFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpHandler);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].handler! --                                                    */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitHandlerStore(void)
{
  ENTRY();

  Word* pHandler = _pWord(m_pDataStack->pPopPtr(__func__));
  
  m_pLogicUnit->m_xpHandler = pHandler;
  
  PsxThd::vTlsSet(giTLS_HandlerKey,(void*)pHandler->szWordNm());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].init -- init the logic unit                                    */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitInit(void)
{
  ENTRY();
  
  m_pLogicUnit->vInit();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].IsLftCrwBuffer@ -- is the left buffer a crw buffer ?            */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitIsLftCrwBufferFetch(void)
{
  ENTRY();

  bool bIsLftCrwBuffer = m_pLogicUnit->m_bIsLftCrwBuffer;

  m_pDataStack->vPushInt(bIsLftCrwBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].IsLftCrwBuffer! -- store is left buffer a crw buffer ?         */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitIsLftCrwBufferStore(void)
{
  ENTRY();

  i8 iIsLeftCrwBuffer = m_pDataStack->iPopIntBol(__func__);

  m_pLogicUnit->m_bIsLftCrwBuffer = iIsLeftCrwBuffer;

  EXIT();
}



/*----------------------------------------------------------------------------*/
/* LogicUnit[].IntStack@ -- int stack fetch                                   */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitIntStackFetch(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpIntStack);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].IntStack! -- int stack store                                   */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitIntStackStore(void)
{
  ENTRY();

  IntStack* pIntStack = _pIntStack(m_pDataStack->pPopPtr(__func__));

  m_pLogicUnit->m_xpIntStack = pIntStack;
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].jobMetrics@ -- job metrics fetch                               */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitJobMetricsFetch(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpJobMetrics);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].jobMetrics! -- job metrics store                               */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitJobMetricsStore(void)
{
  ENTRY();

  JobMetrics* pJobMetrics = _pJobMetrics(m_pDataStack->pPopPtr(__func__));

  m_pLogicUnit->m_xpJobMetrics = pJobMetrics;
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
