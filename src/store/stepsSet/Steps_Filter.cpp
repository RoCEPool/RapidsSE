/*============================================================================*/
/* Steps_Filter.cpp -- Filter step group                                    */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"

#include "Response.hpp"
#include "Result.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Filter(void)
{
  vAddStep("filter[]"_Stz,                 &Step::vFilterGet);
  vAddStep("filter[].new"_Stz,             &Step::vFilterNew);
  vAddStep("filter[].filterRows"_Stz,      &Step::vFilterRows);
  vAddStep("filter[].set"_Stz,             &Step::vFilterSet);
}

/*----------------------------------------------------------------------------*/
/* filter[] -- return filter[]                                                */
/*----------------------------------------------------------------------------*/
void Step::vFilterGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*( m_pJob->pLogicUnit_job() )->m_xpFilter);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* filter::new -- new a filter                                                */
/*----------------------------------------------------------------------------*/
void Step::vFilterNew(void)
{
  ENTRY();

  ( m_pJob->pLogicUnit_job() )->m_xpFilter = new Filter;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* filter[].filterRows -- filter rows                                         */
/*----------------------------------------------------------------------------*/
void Step::vFilterRows(void)
{
  ENTRY();

  Filter* pFilter = *( m_pJob->pLogicUnit_job() )->m_xpFilter;

  StreamDef* pStreamDefOut = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  StreamDef* pStreamDefIn = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  s4K sFilter;
  m_pDataStack->vPopStr(sFilter, __func__);

  //eEC eError = pFilter->eFilterRows(pStreamDefIn, pStreamDefOut, sFilter);
  //m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* filter[].set -- set filter[]                                               */
/*----------------------------------------------------------------------------*/
void Step::vFilterSet(void)
{
  ENTRY();

  ( m_pJob->pLogicUnit_job() )->m_xpFilter = _pFilter(m_pDataStack->pPopPtr(__func__));

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
