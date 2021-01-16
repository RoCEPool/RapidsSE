/*============================================================================*/
/* Steps_range.cpp -- range step group                                    */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"

#include "Index.hpp"

#include "Response.hpp"
#include "Result.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Range(void)
{
  vAddStep("range[]"_Stz,                 &Step::vRangeGet);
  vAddStep("range[].new"_Stz,             &Step::vRangeNew);
  vAddStep("range[].rangeBounds"_Stz,     &Step::vRangeBounds);
  vAddStep("range[].set"_Stz,             &Step::vRangeSet);
}

/*----------------------------------------------------------------------------*/
/* range[] -- return range[]                                                  */
/*----------------------------------------------------------------------------*/
void Step::vRangeGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*( m_pJob->pLogicUnit_job() )->m_xpRange);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* range[].]new -- new a range                                                */
/*----------------------------------------------------------------------------*/
void Step::vRangeNew(void)
{
  ENTRY();

  ( m_pJob->pLogicUnit_job() )->m_xpRange = new Range;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* range[].rangeBounds -- range bounds                                        */
/*----------------------------------------------------------------------------*/
void Step::vRangeBounds(void)
{
  ENTRY();

  Range* pRange = *( m_pJob->pLogicUnit_job() )->m_xpRange;

  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));

  s4K sRange;
  m_pDataStack->vPopStr(sRange, __func__);

  eEC eError = pRange->eRangeValues(pIndex, sRange);
  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* range[].set -- set range[]                                                 */
/*----------------------------------------------------------------------------*/
void Step::vRangeSet(void)
{
  ENTRY();

  ( m_pJob->pLogicUnit_job() )->m_xpRange = _pRange(m_pDataStack->pPopPtr(__func__));

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
