/*============================================================================*/
/* Steps_Debug.cpp - debug step group                                         */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Debug(void)
{
  vAddStep("debug::bloat"_Stz,       &Step::vDebugBloat);
  vAddStep("debug::bloatCleanup"_Stz, &Step::vDebugBloatCleanup);
  vAddStep("debug::breakpoint"_Stz,  &Step::vDebugBreakpoint);
  vAddStep("debug::flag@"_Stz,       &Step::vDebugFlagFetch);
  vAddStep("debug::flag!"_Stz,       &Step::vDebugFlagStore);
  vAddStep("debug::trap"_Stz,        &Step::vDebugBreakpoint);
  vAddStep("debug::throw"_Stz,       &Step::vDebugThrow);
  
  vAddStep("debug::sigsegv"_Stz,     &Step::vDebugSigsegv);
}

/*----------------------------------------------------------------------------*/
/* debug::bloat -- hog memory and bloat                                       */
/*----------------------------------------------------------------------------*/
void Step::vDebugBloat(void)
{
  ENTRY();
  
  //AM::SE::vWhereAmI();

  RowBuffer* pRowBuffer = new RowBuffer();
  m_p_TaskLogicUnit->m_xpMemoryHog = pRowBuffer;
  i8 i = 0;

  while (true) {
    RowBuffer* pNewRowBuffer = new RowBuffer();
    i++;
    pRowBuffer->vNextRowBuffer(pNewRowBuffer);
    pRowBuffer = pNewRowBuffer;
  }

  vTrace("HOGGED %lld\n"_Stz, i);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* debug::bloatCleanup -- cleanup the bloat                                   */
/*----------------------------------------------------------------------------*/
void Step::vDebugBloatCleanup(void)
{
  ENTRY();

  RowBuffer* pRowBuffer = _pRowBuffer(*m_p_TaskLogicUnit->m_xpMemoryHog);
  RowBuffer* pNextRowBuffer;
  i8 i = 0;

  while (pRowBuffer) {
    pNextRowBuffer = pRowBuffer->pNextRowBuffer();
    delete pRowBuffer;
    i++;
    pRowBuffer = pNextRowBuffer;
  }

  vTrace("DELETED %lld\n"_Stz, i);

  m_p_TaskLogicUnit->m_xpMemoryHog = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* debug::trap -- call debug breakpoint                                       */
/*----------------------------------------------------------------------------*/
void Step::vDebugBreakpoint(void)
{
  ENTRY();

  BASE::vDebugPoint();
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
/* debug::flag@ -- fetch debug flag                                           */
/*----------------------------------------------------------------------------*/
void Step::vDebugFlagFetch(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_iDebugFlag);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* debug::flag! -- store debug flag                                           */
/*----------------------------------------------------------------------------*/
void Step::vDebugFlagStore(void)
{
  ENTRY();

  m_p_TaskLogicUnit->m_iDebugFlag = m_pDataStack->iPopInt(__func__);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* debug::sigsegv -- inject a sigsegv signal                                  */
/*----------------------------------------------------------------------------*/
void Step::vDebugSigsegv(void)
{
  ENTRY();
  
  Object* pObject = nullptr;
  
  pObject->vMemoryDump();
  
  NEVER_REACH;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* debug::throw -- throw an exception                                         */
/*----------------------------------------------------------------------------*/
void Step::vDebugThrow(void)
{
  ENTRY();
  
  NEVER_REACH;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
