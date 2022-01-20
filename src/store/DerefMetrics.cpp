/*============================================================================*/
/* DerefMetrics.cpp -- De-Reference Metrics                                   */
/*============================================================================*/

#include "BASE.h"
#include "DerefMetrics.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,DerefMetrics,CN_DEREFMETRICS)
__aFooterFunction(DerefMetrics)

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* DerefMetrics::operator new(size_t nSize, MetricsWsp* pMetricsWsp)
{
  void* aEmbedded = pMetricsWsp->aAllot(nSize);
  
  return aEmbedded;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void DerefMetrics::operator delete(void* aAddr)
{
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
DerefMetrics::DerefMetrics(i8 iObjectNo, eCN eClassNo, i8 iTsc, eDR eAction)
: Metrics(CN_DEREFMETRICS)
{
  _precondition_(_MAGIC_,CN_DEREFMETRICS,_FOOTER_);

  m_iObjectNo = iObjectNo;
  m_eClassNo  = eClassNo;
  m_iTsc      = iTsc;
  m_eAction   = eAction;

  _checkInitialised_(CN_DEREFMETRICS);

  _postcondition_(_MAGIC_,CN_DEREFMETRICS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
DerefMetrics::~DerefMetrics(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_DEREFMETRICS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get eAction                                                                */
/*----------------------------------------------------------------------------*/
eDR DerefMetrics::eAction(void)
{
  _precondition_(_MAGIC_,CN_DEREFMETRICS,_FOOTER_);
  
  return m_eAction;
}

/*----------------------------------------------------------------------------*/
/* get eClassNo                                                               */
/*----------------------------------------------------------------------------*/
eCN DerefMetrics::eClassNo(void)
{
  _precondition_(_MAGIC_,CN_DEREFMETRICS,_FOOTER_);
  
  return m_eClassNo;
}

/*----------------------------------------------------------------------------*/
/* get iObjectNo                                                              */
/*----------------------------------------------------------------------------*/
i8 DerefMetrics::iObjectNo(void)
{
  _precondition_(_MAGIC_,CN_DEREFMETRICS,_FOOTER_);
  
  return m_iObjectNo;
}

/*----------------------------------------------------------------------------*/
/* get iTsc                                                                   */
/*----------------------------------------------------------------------------*/
i8 DerefMetrics::iTsc(void)
{
  _precondition_(_MAGIC_,CN_DEREFMETRICS,_FOOTER_);
  
  return m_iTsc;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
