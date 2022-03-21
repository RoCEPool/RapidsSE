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

#include <signal.h>

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Inject(void)
{
  vAddStep("inject::bloat"_Stz,        &Step::vInjectBloat);
  vAddStep("inject::bloatCleanup"_Stz, &Step::vInjectBloatCleanup);
  vAddStep("inject::sigsegv"_Stz,      &Step::vInjectSigsegv);
  vAddStep("inject::sigsegv_dbg"_Stz,  &Step::vInjectSigsegv_dbg);
  vAddStep("inject::throw"_Stz,        &Step::vInjectThrow);
}

/*----------------------------------------------------------------------------*/
/* inject::bloat -- hog memory and bloat                                       */
/*----------------------------------------------------------------------------*/
void Step::vInjectBloat(void)
{
  ENTRY();
  
  //AM::SE::vWhereAmI();

  RowBuffer* pRowBuffer = new RowBuffer();
  m_pLogicUnit->m_xpMemoryHog = pRowBuffer;
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
/* inject::bloatCleanup -- cleanup the bloat                                   */
/*----------------------------------------------------------------------------*/
void Step::vInjectBloatCleanup(void)
{
  ENTRY();

  RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpMemoryHog);
  RowBuffer* pNextRowBuffer;

  while (pRowBuffer) {
    pNextRowBuffer = pRowBuffer->pNextRowBuffer();
    delete pRowBuffer;
    pRowBuffer = pNextRowBuffer;
  }

  m_pLogicUnit->m_xpMemoryHog = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* inject::sigsegv -- inject a sigsegv signal                                 */
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
/* inject::sigsegv_dbg -- inject a sigsegv signal                             */
/*----------------------------------------------------------------------------*/
void Step::vInjectSigsegv_dbg(void)
{
  ENTRY();
  
  AM::ENGINE::vSignalHandler(SIGSEGV);
  
  NEVER_REACH;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* inject::throw -- throw an exception                                         */
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
