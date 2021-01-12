/*============================================================================*/
/* Steps_Enter.cpp - enter step group                                         */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Enter(void)
{
  vAddStep("enterRoutine"_Stz,    &Step::vEnterRoutine);
}

/*----------------------------------------------------------------------------*/
/* enterRoutine -- enter word routine    (this is the does step for a : word) */
/*----------------------------------------------------------------------------*/
void Step::vEnterRoutine(void)
{
  ENTRY();
  
  CodeCell* aPrevCellOfCurrentCodeBlock = m_aInxPtr - 1;
  

  fpSTEPFN* aAddressOf__m_fpIntrinsic =
    (fpSTEPFN*) aPrevCellOfCurrentCodeBlock->m_afpStepFn;
                 

  CodeCell** aAddressOf__m_aInxPtr =
    (CodeCell**) (aAddressOf__m_fpIntrinsic + 1);
  
  
  CodeCell* aStartCellOfCalledCodeBlock = * aAddressOf__m_aInxPtr;
  
  
  m_pReturnStack->vPushFip(m_aInxPtr);
  
  
  m_aInxPtr = aStartCellOfCalledCodeBlock;
  

   if ( (m_iTraceMask & C_TRACE_ROUTINES) )
    Task::vTracePoint(">"_Stz);
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
//void Step::vEnterRoutine(void)
//{
//  ENTRY();
//
//  fpSTEPFN* afpEntryCell =
//    (fpSTEPFN*) ((m_aInxPtr - 1)->m_afpStepFn + 1);
//
//  m_pReturnStack->vPushFip(m_aInxPtr);
//  m_aInxPtr = *(CodeCell**)afpEntryCell;
//
//   if ( (m_iTraceMask & C_TRACE_ROUTINES) )
//    Task::vTracePoint(">"_Stz);
//
//  EXIT();
//}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
