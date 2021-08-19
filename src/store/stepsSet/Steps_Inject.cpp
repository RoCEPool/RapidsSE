/*============================================================================*/
/* Steps_Inject.cpp - fault injection step group                              */
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
void Step::vAddSteps_Inject(void)
{
  vAddStep("debug::bloat"_Stz,        &Step::vInjectBloat);
  vAddStep("debug::bloatCleanup"_Stz, &Step::vInjectBloatCleanup);
  vAddStep("debug::sigsegv"_Stz,      &Step::vInjectSigsegv);
  vAddStep("debug::throw"_Stz,        &Step::vInjectThrow);
}

/*----------------------------------------------------------------------------*/
/* debug::bloat -- hog memory and bloat                                       */
/*----------------------------------------------------------------------------*/
void Step::vInjectBloat(void)
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
void Step::vInjectBloatCleanup(void)
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
/* debug::sigsegv -- inject a sigsegv signal                                  */
/*----------------------------------------------------------------------------*/
void Step::vInjectSigsegv(void)
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
void Step::vInjectThrow(void)
{
  ENTRY();
  
  NEVER_REACH;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
