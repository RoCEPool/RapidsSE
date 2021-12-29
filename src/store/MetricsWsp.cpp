/*============================================================================*/
/* MetricsWsp.cpp -- Job Metrics                                              */
/*============================================================================*/

#include "BASE.h"
#include "MetricsWsp.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,MetricsWsp,CN_METRICSWSP)
__aFooterFunction(MetricsWsp)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MetricsWsp::MetricsWsp(void)
: Object(CN_METRICSWSP)
{
  _precondition_(_MAGIC_,CN_METRICSWSP,_FOOTER_);
  
  _checkInitialised_(CN_METRICSWSP);

  _postcondition_(_MAGIC_,CN_METRICSWSP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MetricsWsp::~MetricsWsp(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_METRICSWSP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot an embedded block                                                    */
/*----------------------------------------------------------------------------*/
void* MetricsWsp::aAddr(i8 d_Offset)
{
  _precondition_(_MAGIC_,CN_METRICSWSP,_FOOTER_);
  
  if (d_Offset >= m_xWsp.nSize())
    return nullptr;
  
  void* aAddr = m_xWsp.ayAddr(d_Offset);
  
  return aAddr;
}

/*----------------------------------------------------------------------------*/
/* allot an embedded block                                                    */
/*----------------------------------------------------------------------------*/
void* MetricsWsp::aAllot(i8 nBytes)
{
  _precondition_(_MAGIC_,CN_METRICSWSP,_FOOTER_);
  
  void* aAddr = m_xWsp.ayAllot(nBytes);
  
  return aAddr;
}

/*----------------------------------------------------------------------------*/
/* allot an embedded block                                                    */
/*----------------------------------------------------------------------------*/
i8 MetricsWsp::d_Offset(void* aAllot)
{
  _precondition_(_MAGIC_,CN_METRICSWSP,_FOOTER_);
  
  i8 d_Offset = m_xWsp.d_Offset(aAllot);
  
  return d_Offset;
}

/*----------------------------------------------------------------------------*/
/* allot an embedded block                                                    */
/*----------------------------------------------------------------------------*/
i8 MetricsWsp::iPctFree(void)
{
  _precondition_(_MAGIC_,CN_METRICSWSP,_FOOTER_);
  
  i8 iPct = IntLib::iPercent(m_xWsp.nFree(), m_xWsp.nSize());
  
  return iPct;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
