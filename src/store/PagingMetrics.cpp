/*============================================================================*/
/* PagingMetrics.cpp -- Paging Metrics                                        */
/*============================================================================*/

#include "BASE.h"
#include "PagingMetrics.hpp"
#include "DerefMetrics.hpp"
#include "Stream.hpp"
#include "Table.hpp"

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
  //m_oMetricsWsp;
  
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

/*----------------------------------------------------------------------------*/
/* add a DerefMetrics                                                         */
/*----------------------------------------------------------------------------*/
bool PagingMetrics::bAddDerefMetrics(i8 iObjectNo, eCN eClassNo,
                                     i8 iTsc, eDR eAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PAGINGMETRICS,_FOOTER_);
  
  if ( !m_oMetricsWsp.bHasSpace(sizeof(DerefMetrics)))
    return false;
  
  new(&m_oMetricsWsp) DerefMetrics(iObjectNo, eClassNo, iTsc, eAction);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* add a DerefMetrics                                                         */
/*----------------------------------------------------------------------------*/
void PagingMetrics::vReportDerefMetrics(Tub* pTub)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PAGINGMETRICS,_FOOTER_);
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the internal derefMetrics table
  
  Table* pJobMetricsTable = AM::pTableFind(C_TDEF_DEREFMETRICS);
  StreamDef* pStreamDef = pJobMetricsTable->pStreamDef();

  
  i8 d_MetricsCursor = C_UNKNOWN;

  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
