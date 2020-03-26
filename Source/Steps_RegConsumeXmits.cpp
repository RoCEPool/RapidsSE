/*============================================================================*/
/* Steps_RegConsumeXmits.cpp -- Register ConsumeXmits step group              */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegConsumeXmits(void)
{
  vAddStep("ConsumeXmits[]"_Stz,          &Step::vRegConsumeXmitsGet);
  vAddStep("ConsumeXmits[].cancel"_Stz,   &Step::vRegConsumeXmitsCancel);
  vAddStep("ConsumeXmits[].delete"_Stz,   &Step::vRegConsumeXmitsDelete);
  vAddStep("ConsumeXmits[].eos"_Stz,      &Step::vRegConsumeXmitsEos);
  vAddStep("ConsumeXmits[].error"_Stz,    &Step::vRegConsumeXmitsError);
  vAddStep("ConsumeXmits[].exit"_Stz,     &Step::vRegConsumeXmitsExit);
  vAddStep("ConsumeXmits[].isNull"_Stz,   &Step::vRegConsumeXmitsIsNull);
  vAddStep("ConsumeXmits[].new"_Stz,      &Step::vRegConsumeXmitsNew);
  vAddStep("ConsumeXmits[].pulse"_Stz,    &Step::vRegConsumeXmitsPulse);
  vAddStep("ConsumeXmits[].pulsed"_Stz,   &Step::vRegConsumeXmitsPulsed);
  vAddStep("ConsumeXmits[].start"_Stz,    &Step::vRegConsumeXmitsStart);
  vAddStep("ConsumeXmits[].streamDef"_Stz,&Step::vRegConsumeXmitsStreamDef);
}

/*----------------------------------------------------------------------------*/
/* ConsumeXmits[].cancel -- handle consume of cancel marker                   */
/*----------------------------------------------------------------------------*/
void Step::vRegConsumeXmitsCancel(void)
{
  ENTRY(); // ENTRY("vRegConsumeXmitsCancel"_Stz);
  
  ConsumeXmits* pConsumeXmits = _pConsumeXmits(*m_pRegSet->m_xpConsumeXmits);
 
  pConsumeXmits->vIncExit();
  
  m_pDataStack->vPushInt(pConsumeXmits->bExitDone());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ConsumeXmits[].delete -- operand delete                                    */
/*----------------------------------------------------------------------------*/
void Step::vRegConsumeXmitsDelete(void)
{
  ENTRY(); // ENTRY("vRegConsumeXmitsDelete"_Stz);
  
  ConsumeXmits* pConsumeXmits = _pConsumeXmits(*m_pRegSet->m_xpConsumeXmits);
  delete pConsumeXmits;
  
  m_pRegSet->m_xpConsumeXmits = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ConsumeXmits[].eos -- handle consume of eos marker                         */
/*----------------------------------------------------------------------------*/
void Step::vRegConsumeXmitsEos(void)
{
  ENTRY(); // ENTRY("vRegConsumeXmitsEos"_Stz);
  
  ConsumeXmits* pConsumeXmits = _pConsumeXmits(*m_pRegSet->m_xpConsumeXmits);
  
  pConsumeXmits->vIncEos();
  
  m_pDataStack->vPushInt(pConsumeXmits->bEosDone());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ConsumeXmits[].error -- handle consume of error marker                     */
/*----------------------------------------------------------------------------*/
void Step::vRegConsumeXmitsError(void)
{
  ENTRY(); // ENTRY("vRegConsumeXmitsError"_Stz);
  
  ConsumeXmits* pConsumeXmits = _pConsumeXmits(*m_pRegSet->m_xpConsumeXmits);
  
  pConsumeXmits->vIncError();
  
  m_pDataStack->vPushInt(pConsumeXmits->bErrorDone());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ConsumeXmits[].exit -- handle consume of exit marker                       */
/*----------------------------------------------------------------------------*/
void Step::vRegConsumeXmitsExit(void)
{
  ENTRY(); // ENTRY("vRegConsumeXmitsExit"_Stz);
  
  ConsumeXmits* pConsumeXmits = _pConsumeXmits(*m_pRegSet->m_xpConsumeXmits);
  
  pConsumeXmits->vIncExit();
  
  m_pDataStack->vPushInt(pConsumeXmits->bExitDone());
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
/* ConsumeXmits[].get -- fetch operand                                        */
/*----------------------------------------------------------------------------*/
void Step::vRegConsumeXmitsGet(void)
{
  ENTRY(); // ENTRY("vRegConsumeXmitsGet"_Stz);
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpConsumeXmits);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ConsumeXmits[].isNull -- is operand null ?                                 */
/*----------------------------------------------------------------------------*/
void Step::vRegConsumeXmitsIsNull(void)
{
  ENTRY(); // ENTRY("vRegConsumeXmitsIsNull"_Stz);
  
  m_pDataStack->vPushInt(m_pRegSet->m_xpConsumeXmits.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ConsumeXmits[].new -- new a consume xmits                                  */
/*----------------------------------------------------------------------------*/
void Step::vRegConsumeXmitsNew(void)
{
  ENTRY(); // ENTRY("vRegConsumeXmitsNew"_Stz);
  
  ASSERT (m_pRegSet->m_xpConsumeXmits.bIsNull());
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  m_pRegSet->m_xpConsumeXmits = new ConsumeXmits(pOp->nLeftEosMarkers());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ConsumeXmits[].pulse -- handle consume of pulse marker                     */
/*----------------------------------------------------------------------------*/
void Step::vRegConsumeXmitsPulse(void)
{
  ENTRY(); // ENTRY("vRegConsumeXmitsPulse"_Stz);
  
  ConsumeXmits* pConsumeXmits = _pConsumeXmits(*m_pRegSet->m_xpConsumeXmits);
  
  pConsumeXmits->vIncPulse();
  
  m_pDataStack->vPushInt(pConsumeXmits->bPulseDone());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ConsumeXmits[].pulsed -- handle consume of pulsed marker                   */
/*----------------------------------------------------------------------------*/
void Step::vRegConsumeXmitsPulsed(void)
{
  ENTRY(); // ENTRY("vRegConsumeXmitsPulsed"_Stz);
  
  ConsumeXmits* pConsumeXmits = _pConsumeXmits(*m_pRegSet->m_xpConsumeXmits);
  
  pConsumeXmits->vIncPulsed();
  
  m_pDataStack->vPushInt(pConsumeXmits->bPulsedDone());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ConsumeXmits[].start -- handle consume of start marker                     */
/*----------------------------------------------------------------------------*/
void Step::vRegConsumeXmitsStart(void)
{
  ENTRY(); // ENTRY("vRegConsumeXmitsStart"_Stz);
  
  ConsumeXmits* pConsumeXmits = _pConsumeXmits(*m_pRegSet->m_xpConsumeXmits);
  
  bool bStartDone = pConsumeXmits->bStartDone();
  pConsumeXmits->vIncStart();
  
  m_pDataStack->vPushInt(bStartDone);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ConsumeXmits[].pulsed -- handle consume of pulsed marker                   */
/*----------------------------------------------------------------------------*/
void Step::vRegConsumeXmitsStreamDef(void)
{
  ENTRY(); // ENTRY("vRegConsumeXmitsStreamDef"_Stz);
  
  ConsumeXmits* pConsumeXmits = _pConsumeXmits(*m_pRegSet->m_xpConsumeXmits);
  
  bool bStreamDefDone = pConsumeXmits->bStreamDefDone();
  
  pConsumeXmits->vIncStreamDef();
  
  m_pDataStack->vPushInt(bStreamDefDone);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
