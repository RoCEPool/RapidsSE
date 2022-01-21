/*============================================================================*/
/* PagingMetrics.cpp -- Paging Metrics                                        */
/*============================================================================*/

#include "BASE.h"
#include "PagingMetrics.hpp"
#include "DerefMetrics.hpp"
#include "Stream.hpp"
#include "Table.hpp"
#include "NamedBuffer.hpp"

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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get a working buffer from the tub
  
  NamedBuffer* pNamedBuffer = _pNamedBuffer(pTub->pWorkingBuffer());
  
  if (! pNamedBuffer) {
    pNamedBuffer = new NamedBuffer(pStreamDef);
    pTub->vAddBuffer(pNamedBuffer);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 d_MetricsCursor = 0;

  while (DerefMetrics* pDerefMetrics = pGetDerefMetrics(d_MetricsCursor)) {
    
    L_RETRY:
    Row* pRow = pNamedBuffer->pAllotRow
                (
                  C_VARGS_BEGIN
                 
                 ,pDerefMetrics->iObjectNo()
                 ,aCN[pDerefMetrics->eClassNo()]
                 ,pDerefMetrics->iTsc()
                 ,aDR[pDerefMetrics->eAction()]

                 ,C_VARGS_END
                );
    
    if (! pRow) {
      pNamedBuffer = new NamedBuffer(pStreamDef);
      pTub->vAddBuffer(pNamedBuffer);
      goto L_RETRY;
    }

    
  } // while derefMetrics
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a DerefMetrics                                                         */
/*----------------------------------------------------------------------------*/
DerefMetrics* PagingMetrics::pGetDerefMetrics(i8& d_MetricsCursor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PAGINGMETRICS,_FOOTER_);
  
  DerefMetrics* pDerefMetrics =
    _pDerefMetrics(m_oMetricsWsp.aAddr(d_MetricsCursor));
  
  if (! pDerefMetrics)
    return nullptr;
  
  d_MetricsCursor += sizeof(DerefMetrics);
  
  return pDerefMetrics;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
