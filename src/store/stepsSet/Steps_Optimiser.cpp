/*============================================================================*/
/* Steps_Optimiser.cpp -- Optimiser step group                                */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Op.hpp"
#include "Job.hpp"

#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Optimiser(void)
{
  vAddStep("Optimiser[]"_Stz,              &Step::vOptimiserGet);
  vAddStep("Optimiser[].delete"_Stz ,      &Step::vOptimiserDelete);
  vAddStep("Optimiser[].dissect"_Stz,      &Step::vOptimiserDissect);
  vAddStep("Optimiser[].get"_Stz,          &Step::vOptimiserGet);
  vAddStep("Optimiser[].new"_Stz,          &Step::vOptimiserNew);
  vAddStep("Optimiser[].optimise"_Stz,     &Step::vOptimiserOptimise);
  vAddStep("Optimiser[].weave"_Stz,        &Step::vOptimiserWeave);
}


/*----------------------------------------------------------------------------*/
/* Optimiser[].delete -- delete optimiser                                     */
/*----------------------------------------------------------------------------*/
void Step::vOptimiserDelete(void)
{
  ENTRY();
    
  m_pLogicUnit->m_xpOptimiser.vDeleteComponent();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Optimiser[].dissect -- dissed unoptimised pipeline spec                    */
/*----------------------------------------------------------------------------*/
void Step::vOptimiserDissect(void)
{
  ENTRY();
    
  PipeSpec* pUnOptPlineSpec =
    _pPipeSpec(m_pDataStack->pPopPtr(__func__));

  m_pLogicUnit->m_xpOptimiser->vDissect(pUnOptPlineSpec);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Optimiser[].get -- get optimiser                                           */
/*----------------------------------------------------------------------------*/
void Step::vOptimiserGet(void)
{
  ENTRY();

  if (m_pLogicUnit->m_xpOptimiser.bIsNotNull())
    m_pDataStack->vPushPtr(* m_pLogicUnit->m_xpOptimiser);
  else
    m_pDataStack->vPushPtr(nullptr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Optimiser[].new -- new optimiser                                           */
/*----------------------------------------------------------------------------*/
void Step::vOptimiserNew(void)
{
  ENTRY();
    
  Job* pJob = nullptr;

  if (m_pJob.bIsNotNull()) {
    //RASSERT(! *m_pOp);
    pJob = *m_pJob;
  }
  else
  if (m_pOp.bIsNotNull()) {
    RASSERT(m_pJob.bIsNull());
    pJob = m_pOp->pJobOfOp();
  }
  else {
    NEVER_REACH;
  }

  m_pLogicUnit->m_xpOptimiser = new Optimiser(pJob->pJobTags());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Optimiser[].optimise -- optimise a pipeline                                */
/*----------------------------------------------------------------------------*/
void Step::vOptimiserOptimise(void)
{
  ENTRY();
    
  m_pLogicUnit->m_xpOptimiser->vOptimise();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Optimiser[].weave -- weave optimised pipeline statement                    */
/*----------------------------------------------------------------------------*/
void Step::vOptimiserWeave(void)
{
  ENTRY();
    
  Stz* szOptPlineStmt = m_pLogicUnit->m_xpOptimiser->szWeave();

  m_pDataStack->vPushStz(szOptPlineStmt);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
