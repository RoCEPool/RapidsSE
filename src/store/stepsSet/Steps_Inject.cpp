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
  vAddStep("inject::segV"_Stz,         &Step::vInjectSegViolation);
  vAddStep("inject::sigfpe"_Stz,       &Step::vInjectFPE);
  vAddStep("inject::sigill"_Stz,       &Step::vInjectILL);
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

/*----------------------------------------------------------------------------*/
/* inject::segV -- display segV message                                       */
/*----------------------------------------------------------------------------*/
void Step::vInjectSegViolation(void)
{
  ENTRY();

  s256 pDumpBuffer = (char*)">>>> Segmentation Violation <<<<<";

  m_pDataStack->vPushStr(pDumpBuffer);

  // Segmentation Violation
  //vTrace("\n====>>>  Segmentation Violation\n\n"_Stz);
  int *p = (int *) 0x12345678;  // some unknown address, to cause seg fault
  *p = 0;



  EXIT();
}



/*----------------------------------------------------------------------------*/
/* inject::sigfpe -- floating-point exception                                 */
/*----------------------------------------------------------------------------*/
void Step::vInjectFPE(void)
{
  ENTRY();

  s256 pDumpBuffer = (char*)">>>> Divide by zero <<<<<";

  m_pDataStack->vPushStr(pDumpBuffer);

  // Experiment code
  // Violations
  //vTrace("\n====>>>  Divide by zero\n"_Stz);
  int i = 114;
  int j = 1;
  int k = 0;
  k = i / (j-j);

  //raise(SIGFPE);


  EXIT();
}

/*----------------------------------------------------------------------------*/
/* inject::sigill -- corruption in the code or to an attempt to execute data  */
/*----------------------------------------------------------------------------*/
void Step::vInjectILL(void)
{
  ENTRY();

  s256 pDumpBuffer = (char*)">>>> Illegal Instruction <<<<<";

  m_pDataStack->vPushStr(pDumpBuffer);

  // Experiment code

  // this would cause ILL
  //BUT: after a few tries, m_xpInfoSetTable.bIsNull(); thrown in ~LogicUnit (LogicUnit.cpp:315)
  char* pC=nullptr;
  strcpy(pC,"abc");
  printf("%s\n", pC);

  //raise(SIGILL);



  EXIT();
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
