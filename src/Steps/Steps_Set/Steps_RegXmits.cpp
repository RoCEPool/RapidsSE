/*============================================================================*/
/* Steps_RegXmits.cpp -- Register Xmits step group                            */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegXmits(void)
{
  vAddStep("Xmits[]"_Stz,                &Step::vRegXmitsGet);
  vAddStep("Xmits[].delete"_Stz,         &Step::vRegXmitsDelete);
  vAddStep("Xmits[].handleCancel"_Stz,   &Step::vRegXmitsHandleCancel);
  vAddStep("Xmits[].handleDestroy"_Stz,  &Step::vRegXmitsHandleDestroy);
  vAddStep("Xmits[].handleEos"_Stz,      &Step::vRegXmitsHandleEos);
  vAddStep("Xmits[].handleError"_Stz,    &Step::vRegXmitsHandleError);
  vAddStep("Xmits[].handleExit"_Stz,     &Step::vRegXmitsHandleExit);
  vAddStep("Xmits[].handleDataPulse"_Stz,&Step::vRegXmitsHandleDataPulse);
  vAddStep("Xmits[].handlePulsed"_Stz,   &Step::vRegXmitsHandlePulsed);
  vAddStep("Xmits[].handleStart"_Stz,    &Step::vRegXmitsHandleStart);
  vAddStep("Xmits[].handleStreamDef"_Stz,&Step::vRegXmitsHandleStreamDef);
  vAddStep("Xmits[].isNull"_Stz,         &Step::vRegXmitsIsNull);
  vAddStep("Xmits[].new"_Stz,            &Step::vRegXmitsNew);
}

/*----------------------------------------------------------------------------*/
/* Xmits[].delete -- operand delete                                           */
/*----------------------------------------------------------------------------*/
void Step::vRegXmitsDelete(void)
{
  ENTRY();
  
  Xmits* pXmits = _pXmits(*m_pLogicUnit->m_xpXmits);
  delete pXmits;
  
  m_pLogicUnit->m_xpXmits = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Xmits[].get -- fetch operand                                               */
/*----------------------------------------------------------------------------*/
void Step::vRegXmitsGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpXmits);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Xmits[].handleCancel -- handle consume of cancel marker                    */
/*----------------------------------------------------------------------------*/
void Step::vRegXmitsHandleCancel(void)
{
  ENTRY();
  
  Xmits* pXmits = _pXmits(*m_pLogicUnit->m_xpXmits);
 
  pXmits->vIncExit();
  
  m_pDataStack->vPushInt(pXmits->bExitDone());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Xmits[].handleDestroy -- handle consume of destory marker                  */
/*----------------------------------------------------------------------------*/
void Step::vRegXmitsHandleDestroy(void)
{
  ENTRY();
  
  Xmits* pXmits = _pXmits(*m_pLogicUnit->m_xpXmits);
  
  pXmits->vIncDestroy();
  
  m_pDataStack->vPushInt(pXmits->bDestroyDone());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Xmits[].handleEos -- handle consume of eos marker                          */
/*----------------------------------------------------------------------------*/
void Step::vRegXmitsHandleEos(void)
{
  ENTRY();
  
  Xmits* pXmits = _pXmits(*m_pLogicUnit->m_xpXmits);
  
  pXmits->vIncEos();
  
  m_pDataStack->vPushInt(pXmits->bEosDone());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Xmits[].handleError -- handle consume of error marker                      */
/*----------------------------------------------------------------------------*/
void Step::vRegXmitsHandleError(void)
{
  ENTRY();
  
  Xmits* pXmits = _pXmits(*m_pLogicUnit->m_xpXmits);
  
  pXmits->vIncError();
  
  m_pDataStack->vPushInt(pXmits->bErrorDone());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Xmits[].handleExit -- handle consume of exit marker                        */
/*----------------------------------------------------------------------------*/
void Step::vRegXmitsHandleExit(void)
{
  ENTRY();
  
  Xmits* pXmits = _pXmits(*m_pLogicUnit->m_xpXmits);
  
  pXmits->vIncExit();
  
  m_pDataStack->vPushInt(pXmits->bExitDone());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Xmits[].handleDataPulse -- handle consume of data pulse                    */
/*----------------------------------------------------------------------------*/
void Step::vRegXmitsHandleDataPulse(void)
{
  ENTRY();
  
  Xmits* pXmits = _pXmits(*m_pLogicUnit->m_xpXmits);
  
  m_pDataStack->vPushInt(pXmits->bDataPulseDone());
  pXmits->vIncDataPulse();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Xmits[].handlePulsed -- handle consume of pulsed marker                    */
/*----------------------------------------------------------------------------*/
void Step::vRegXmitsHandlePulsed(void)
{
  ENTRY();
  
  Xmits* pXmits = _pXmits(*m_pLogicUnit->m_xpXmits);
  
  pXmits->vIncPulsed();
  
  m_pDataStack->vPushInt(pXmits->bPulsedDone());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Xmits[].handleStart -- handle consume of start marker                      */
/*----------------------------------------------------------------------------*/
void Step::vRegXmitsHandleStart(void)
{
  ENTRY();
  
  Xmits* pXmits = _pXmits(*m_pLogicUnit->m_xpXmits);
  
  bool bStartDone = pXmits->bStartDone();
  pXmits->vIncStart();
  
  m_pDataStack->vPushInt(bStartDone);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Xmits[].handleStreamDef -- handle consume of streamDef                     */
/*----------------------------------------------------------------------------*/
void Step::vRegXmitsHandleStreamDef(void)
{
  ENTRY();
  
  Xmits* pXmits = _pXmits(*m_pLogicUnit->m_xpXmits);
  
  bool bStreamDefDone = pXmits->bStreamDefDone();
  
  pXmits->vIncStreamDef();
  
  m_pDataStack->vPushInt(bStreamDefDone);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Xmits[].isNull -- is operand null ?                                        */
/*----------------------------------------------------------------------------*/
void Step::vRegXmitsIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpXmits.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Xmits[].new -- new a consume xmits                                         */
/*----------------------------------------------------------------------------*/
void Step::vRegXmitsNew(void)
{
  ENTRY();
  
  ASSERT (m_pLogicUnit->m_xpXmits.bIsNull());
  
  i8 iXmits = m_pDataStack->iPopInt(__func__);
  
  m_pLogicUnit->m_xpXmits = new Xmits(iXmits);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
