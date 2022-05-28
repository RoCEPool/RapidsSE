/*============================================================================*/
/* Steps_RegSends.cpp -- Register Sends step group                            */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Sends.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegSends(void)
{
  vAddStep("Sends[]"_Stz,                &Step::vRegSendsGet);
  vAddStep("Sends[].delete"_Stz,         &Step::vRegSendsDelete);
  vAddStep("Sends[].handleCancelling"_Stz, &Step::vRegSendsHandleCancelling);
  vAddStep("Sends[].handleCleaning"_Stz,  &Step::vRegSendsHandleCleaning);
  vAddStep("Sends[].handleDataPulse"_Stz,&Step::vRegSendsHandleDataPulse);
  vAddStep("Sends[].handleDestroy"_Stz,  &Step::vRegSendsHandleDestroy);
  vAddStep("Sends[].handleEnd"_Stz,      &Step::vRegSendsHandleEnd);
  vAddStep("Sends[].handleEnding"_Stz,   &Step::vRegSendsHandleEnding);
  vAddStep("Sends[].handleEos"_Stz,      &Step::vRegSendsHandleEos);
  vAddStep("Sends[].handleErroring"_Stz, &Step::vRegSendsHandleErroring);
  vAddStep("Sends[].handleExiting"_Stz,  &Step::vRegSendsHandleExiting);
  vAddStep("Sends[].handleLeaving"_Stz,  &Step::vRegSendsHandleLeaving);
  vAddStep("Sends[].handleStart"_Stz,    &Step::vRegSendsHandleStart);
  vAddStep("Sends[].handleStreamDef"_Stz,&Step::vRegSendsHandleStreamDef);
  vAddStep("Sends[].handleStreamDefRht"_Stz,&Step::vRegSendsHandleStreamDefRht);
  vAddStep("Sends[].handleTriggered"_Stz,&Step::vRegSendsHandleTriggered);
  vAddStep("Sends[].isNull"_Stz,         &Step::vRegSendsIsNull);
  vAddStep("Sends[].new"_Stz,            &Step::vRegSendsNew);
}

/*----------------------------------------------------------------------------*/
/* Sends[].delete -- operand delete                                           */
/*----------------------------------------------------------------------------*/
void Step::vRegSendsDelete(void)
{
  ENTRY();
  
  Sends* pSends = _pSends(*m_pLogicUnit->m_xpSends);
  delete pSends;
  
  m_pLogicUnit->m_xpSends = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Sends[].get -- fetch operand                                               */
/*----------------------------------------------------------------------------*/
void Step::vRegSendsGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpSends);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Sends[].handleCancelling -- handle consume of cancelling marker            */
/*----------------------------------------------------------------------------*/
void Step::vRegSendsHandleCancelling(void)
{
  ENTRY();
  
  Sends* pSends = _pSends(*m_pLogicUnit->m_xpSends);
 
  pSends->vIncCancelling();
  
  m_pDataStack->vPushInt(pSends->bCancellingDone());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Sends[].handleCleaning -- handle consume of cleaningp marker               */
/*----------------------------------------------------------------------------*/
void Step::vRegSendsHandleCleaning(void)
{
  ENTRY();

  Sends* pSends = _pSends(*m_pLogicUnit->m_xpSends);

  pSends->vIncCleaning();

  m_pDataStack->vPushInt(pSends->bCleaningDone());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Sends[].handleDataPulse -- handle consume of data pulse                    */
/*----------------------------------------------------------------------------*/
void Step::vRegSendsHandleDataPulse(void)
{
  ENTRY();

  Sends* pSends = _pSends(*m_pLogicUnit->m_xpSends);

  m_pDataStack->vPushInt(pSends->bDataPulseDone());
  pSends->vIncDataPulse();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Sends[].handleDestroy -- handle consume of destory marker                  */
/*----------------------------------------------------------------------------*/
void Step::vRegSendsHandleDestroy(void)
{
  ENTRY();
  
  Sends* pSends = _pSends(*m_pLogicUnit->m_xpSends);
  
  pSends->vIncDestroy();
  
  m_pDataStack->vPushInt(pSends->bDestroyDone());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Sends[].handleEnding -- handle consume of ending marker                    */
/*----------------------------------------------------------------------------*/
void Step::vRegSendsHandleEnding(void)
{
  ENTRY();

  Sends* pSends = _pSends(*m_pLogicUnit->m_xpSends);

  pSends->vIncEnding();

  m_pDataStack->vPushInt(pSends->bEndingDone());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Sends[].handleEos -- handle consume of eos marker                          */
/*----------------------------------------------------------------------------*/
void Step::vRegSendsHandleEos(void)
{
  ENTRY();
  
  Sends* pSends = _pSends(*m_pLogicUnit->m_xpSends);
  
  pSends->vIncEos();
  
  m_pDataStack->vPushInt(pSends->bEosDone());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Sends[].handleErroring -- handle consume of erroring marker                */
/*----------------------------------------------------------------------------*/
void Step::vRegSendsHandleErroring(void)
{
  ENTRY();
  
  Sends* pSends = _pSends(*m_pLogicUnit->m_xpSends);
  
  pSends->vIncErroring();
  
  m_pDataStack->vPushInt(pSends->bErroringDone());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Sends[].handleExiting -- handle consume of exiting marker                  */
/*----------------------------------------------------------------------------*/
void Step::vRegSendsHandleExiting(void)
{
  ENTRY();
  
  Sends* pSends = _pSends(*m_pLogicUnit->m_xpSends);
  
  pSends->vIncExiting();
  
  m_pDataStack->vPushInt(pSends->bExitingDone());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Sends[].handleEnd -- handle consume of end marker                          */
/*----------------------------------------------------------------------------*/
void Step::vRegSendsHandleEnd(void)
{
  ENTRY();
  
  Sends* pSends = _pSends(*m_pLogicUnit->m_xpSends);
  
  pSends->vIncEnd();
  
  m_pDataStack->vPushInt(pSends->bEndDone());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Sends[].handleLeaving -- handle consume of leaving marker                  */
/*----------------------------------------------------------------------------*/
void Step::vRegSendsHandleLeaving(void)
{
  ENTRY();

  Sends* pSends = _pSends(*m_pLogicUnit->m_xpSends);

  bool bLeavingDone = pSends->bLeavingDone();
  pSends->vIncLeaving();

  m_pDataStack->vPushInt(bLeavingDone);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Sends[].handleStart -- handle consume of start marker                      */
/*----------------------------------------------------------------------------*/
void Step::vRegSendsHandleStart(void)
{
  ENTRY();
  
  Sends* pSends = _pSends(*m_pLogicUnit->m_xpSends);
  
  bool bStartDone = pSends->bStartDone();
  pSends->vIncStart();
  
  m_pDataStack->vPushInt(bStartDone);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Sends[].handleStreamDef -- handle consume of streamDef                     */
/*----------------------------------------------------------------------------*/
void Step::vRegSendsHandleStreamDef(void)
{
  ENTRY();
  
  Sends* pSends = _pSends(*m_pLogicUnit->m_xpSends);
  
  bool bStreamDefDone = pSends->bStreamDefDone();
  
  pSends->vIncStreamDef();
  
  m_pDataStack->vPushInt(bStreamDefDone);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Sends[].handleStreamDefRht -- handle consume of streamDefRht               */
/*----------------------------------------------------------------------------*/
void Step::vRegSendsHandleStreamDefRht(void)
{
  ENTRY();

  Sends* pSends = _pSends(*m_pLogicUnit->m_xpSends);

  bool bStreamDefRhtDone = pSends->bStreamDefRhtDone();

  pSends->vIncStreamDefRht();

  m_pDataStack->vPushInt(bStreamDefRhtDone);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Sends[].handleTriggered -- handle consume of triggered                     */
/*----------------------------------------------------------------------------*/
void Step::vRegSendsHandleTriggered(void)
{
  ENTRY();

  Sends* pSends = _pSends(*m_pLogicUnit->m_xpSends);

  bool bTriggeredDone = pSends->bTriggeredDone();

  pSends->vIncTriggered();

  m_pDataStack->vPushInt(bTriggeredDone);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Sends[].isNull -- is operand null ?                                        */
/*----------------------------------------------------------------------------*/
void Step::vRegSendsIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpSends.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Sends[].new -- new a consume Sends                                         */
/*----------------------------------------------------------------------------*/
void Step::vRegSendsNew(void)
{
  ENTRY();
  
  ASSERT (m_pLogicUnit->m_xpSends.bIsNull());
  
  i8 iSends = m_pDataStack->iPopInt(__func__);
  
  m_pLogicUnit->m_xpSends = new Sends(iSends);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
