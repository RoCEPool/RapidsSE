/*============================================================================*/
/* Steps_Trace.cpp - trace step group                                         */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Trace(void)
{
  vAddStep("traceOff"_Stz,         &Step::vTraceOff);
  vAddStep("traceOn"_Stz,          &Step::vTraceOn);
  vAddStep("tracePoint"_Stz,       &Step::vTracePoint);
  vAddStep("trace!"_Stz,           &Step::vTraceSet);
}

/*----------------------------------------------------------------------------*/
/* turn tracing off                                                           */
/*----------------------------------------------------------------------------*/
void Step::vTraceOff(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vTraceOff"_Stz);
  
  mpTask->m_iTracing = 0;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* turn tracing on                                                            */
/*----------------------------------------------------------------------------*/
void Step::vTraceOn(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vTraceOn"_Stz);
  
  mpTask->m_iTracing = C_TRACE_STEPS;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* trace point                                                                */
/*----------------------------------------------------------------------------*/
void Step::vTracePoint(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vTracePoint"_Stz);
  
  s256 sPointName;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sPointName) )
    THROW_DSTACK();
  
  if (AM::bTracepoint())
    mpTask->vTracePoint(sPointName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* set tracing                                                                */
/*----------------------------------------------------------------------------*/
void Step::vTraceSet(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vTraceSet"_Stz);
  
  i8 iTrace;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iTrace) )
    THROW_DSTACK();
  
  mpTask->m_iTracing = iTrace;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
