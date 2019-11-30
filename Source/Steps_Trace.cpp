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
void Step::vTraceOff(void)
{
  ENTRY(); // ENTRY("vTraceOff"_Stz);
  
  m_pTask->m_iTraceMask = 0;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* turn tracing on                                                            */
/*----------------------------------------------------------------------------*/
void Step::vTraceOn(void)
{
  ENTRY(); // ENTRY("vTraceOn"_Stz);
  
  m_pTask->m_iTraceMask = C_TRACE_STEPS;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* trace point                                                                */
/*----------------------------------------------------------------------------*/
void Step::vTracePoint(void)
{
  ENTRY(); // ENTRY("vTracePoint"_Stz);
  
  s256 sPointName;
  
  if ( ! m_pDataStack->bPopStr(sPointName) ) THROW_DSTACK();
  
  if (AM::bTracepoint())
    m_pTask->vTracePoint(sPointName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* set tracing                                                                */
/*----------------------------------------------------------------------------*/
void Step::vTraceSet(void)
{
  ENTRY(); // ENTRY("vTraceSet"_Stz);
  
  i8 iTrace;
  
  if ( ! m_pDataStack->bPopInt(iTrace) ) THROW_DSTACK();
  
  m_pTask->m_iTraceMask = iTrace;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
