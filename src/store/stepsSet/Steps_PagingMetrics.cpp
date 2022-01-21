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
  vAddStep("pagingMetrics.delete"_Stz,    &Step::vPagingMetricsDelete);
  vAddStep("pagingMetrics.update"_Stz,    &Step::vPagingMetricsUpdate);
}


/*----------------------------------------------------------------------------*/
/* pagingMetrics.delete -- delete paging mertics                              */
/*----------------------------------------------------------------------------*/
void Step::vPagingMetricsDelete(void)
{
  ENTRY();

  PagingMetrics* pPagingMetrics = _pPagingMetrics(m_pDataStack->pPopPtr(__func__));

  EXIT();
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
