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
void Step::vTraceOff(Forth* pForth)
{
  ENTRY("vTraceOff"_Stz);
  
  pForth->m_pTask->m_iTracing = 0;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* turn tracing on                                                            */
/*----------------------------------------------------------------------------*/
void Step::vTraceOn(Forth* pForth)
{
  ENTRY("vTraceOn"_Stz);
  
  pForth->m_pTask->m_iTracing = C_TRACE_STEPS;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* trace point                                                                */
/*----------------------------------------------------------------------------*/
void Step::vTracePoint(Forth* pForth)
{
  ENTRY("vTracePoint"_Stz);
  
  s256 sPointName;
  
  if ( ! pForth->m_pDataStack->bPop(sPointName) )
    THROW_DSTACK();
  
  if (AM::bTracepoint())
    pForth->m_pTask->vTracePoint(sPointName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* set tracing                                                                */
/*----------------------------------------------------------------------------*/
void Step::vTraceSet(Forth* pForth)
{
  ENTRY("vTraceSet"_Stz);
  
  i8 iTrace;
  
  if ( ! pForth->m_pDataStack->bPop(iTrace) )
    THROW_DSTACK();
  
  pForth->m_pTask->m_iTracing = iTrace;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
