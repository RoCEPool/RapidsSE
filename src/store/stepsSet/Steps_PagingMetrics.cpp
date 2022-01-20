/*============================================================================*/
/* Steps_PageMetrics.cpp - page metrics step group                            */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "PagingMetrics.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_PagingMetrics(void)
{
  vAddStep("pagingMetrics.update"_Stz,    &Step::vPagingMetricsUpdate);
}


/*----------------------------------------------------------------------------*/
/* pagingMetrics.update -- update paging mertics                              */
/*----------------------------------------------------------------------------*/
void Step::vPagingMetricsUpdate(void)
{
  ENTRY();

  PagingMetrics* pPagingMetrics = _pPagingMetrics(m_pDataStack->pPopPtr(__func__));

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
