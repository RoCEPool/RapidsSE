/*============================================================================*/
/* PagingMetrics.cpp -- Paging Metrics                                        */
/*============================================================================*/

#include "BASE.h"
#include "PagingMetrics.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,PagingMetrics,CN_PAGINGMETRICS)
__aNewFunction(PagingMetrics,CN_PAGINGMETRICS)
__vDeleteFunction(PagingMetrics,CN_PAGINGMETRICS)
__aFooterFunction(PagingMetrics)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PagingMetrics::PagingMetrics(JobId tJobId)
: Operand(CN_PAGINGMETRICS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PAGINGMETRICS,_FOOTER_);
  
  m_pNext = nullptr;
  
  _checkInitialised_(CN_PAGINGMETRICS);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PAGINGMETRICS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PagingMetrics::~PagingMetrics(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PAGINGMETRICS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
