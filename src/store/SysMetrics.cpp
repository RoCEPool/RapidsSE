/*============================================================================*/
/* SysMetrics.cpp -- Job Metrics                                              */
/*============================================================================*/

#include "BASE.h"
#include "SysMetrics.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,SysMetrics,CN_SYSMETRICS)
__aNewFunction(SysMetrics,CN_SYSMETRICS)
__vDeleteFunction(SysMetrics,CN_SYSMETRICS)
__aFooterFunction(SysMetrics)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SysMetrics::SysMetrics(void)
: Operand(CN_SYSMETRICS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SYSMETRICS,_FOOTER_);

  m_iPctFree      = C_UNKNOWN;
  m_nRefTimer     = C_UNKNOWN;
  m_nThreadsIdle  = C_UNKNOWN;

  m_nLargeHeap    = C_UNKNOWN;
  m_nMediumHeap   = C_UNKNOWN;
  m_nSmallHeap    = C_UNKNOWN;
  m_nTinyHeap     = C_UNKNOWN;

  m_pNext = nullptr;

  _checkInitialised_(CN_SYSMETRICS);
  
  vCollect();

  _postcondition_(_ADDRESS_,_MAGIC_,CN_SYSMETRICS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SysMetrics::~SysMetrics(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SYSMETRICS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* repair the int stack                                                       */
/*----------------------------------------------------------------------------*/
void SysMetrics::vRepair(Object* pObject)
{
  _static_

}



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* collect job metrics                                                        */
/*----------------------------------------------------------------------------*/
void SysMetrics::vCollect(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SYSMETRICS,_FOOTER_);

  m_iPctFree = AM::POOL::iPctFree();

  Timer* pTimer = AM::TIMER::pReferenceTimer();
  pTimer->vUpdate();
  m_nRefTimer = pTimer->iMilliseconds();

  pTimer = AM::TIMER::pSinceTimer();
  pTimer->vUpdate();
  m_nSince = pTimer->iMilliseconds();

  m_nThreadsIdle = AM::SCHEDULER::nIdleThreads();
}



/*----------------------------------------------------------------------------*/
/* get next sys metrics                                                       */
/*----------------------------------------------------------------------------*/
SysMetrics* SysMetrics::pNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SYSMETRICS,_FOOTER_);

  return m_pNext;
}

/*----------------------------------------------------------------------------*/
/* link to next sys metrics                                                   */
/*----------------------------------------------------------------------------*/
void SysMetrics::vNext(SysMetrics* pNext)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SYSMETRICS,_FOOTER_);

  m_pNext = pNext;
}


/*----------------------------------------------------------------------------*/
/* get pool percentage free                                                   */
/*----------------------------------------------------------------------------*/
i8 SysMetrics::iPctFree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SYSMETRICS,_FOOTER_);

  return m_iPctFree;
}

/*----------------------------------------------------------------------------*/
/* get reference timer                                                        */
/*----------------------------------------------------------------------------*/
i8 SysMetrics::nRefTimer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SYSMETRICS,_FOOTER_);

  return m_nRefTimer;
}

/*----------------------------------------------------------------------------*/
/* get since timer                                                            */
/*----------------------------------------------------------------------------*/
i8 SysMetrics::nSince(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SYSMETRICS,_FOOTER_);

  return m_nSince;
}

/*----------------------------------------------------------------------------*/
/* get threads idle                                                           */
/*----------------------------------------------------------------------------*/
i8 SysMetrics::nThreadsIdle(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SYSMETRICS,_FOOTER_);

  return m_nThreadsIdle;
}



/*----------------------------------------------------------------------------*/
/* get large heap                                                             */
/*----------------------------------------------------------------------------*/
i8 SysMetrics::nLargeHeap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SYSMETRICS,_FOOTER_);

  return m_nLargeHeap;
}

/*----------------------------------------------------------------------------*/
/* get medium heap                                                            */
/*----------------------------------------------------------------------------*/
i8 SysMetrics::nMediumHeap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SYSMETRICS,_FOOTER_);

  return m_nMediumHeap;
}

/*----------------------------------------------------------------------------*/
/* get small heap                                                             */
/*----------------------------------------------------------------------------*/
i8 SysMetrics::nSmallHeap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SYSMETRICS,_FOOTER_);

  return m_nSmallHeap;
}

/*----------------------------------------------------------------------------*/
/* get tiny heap                                                              */
/*----------------------------------------------------------------------------*/
i8 SysMetrics::nTinyHeap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SYSMETRICS,_FOOTER_);

  return m_nTinyHeap;
}



/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
