/*============================================================================*/
/* ThreadScratch.cpp -- Thread Scratch area                                   */
/*============================================================================*/

#include "BASE.h"

#include "ThreadScratch.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,ThreadScratch,CN_THREADSCRATCH)
__aNewFunction(ThreadScratch,CN_THREADSCRATCH)
__vDeleteFunction(ThreadScratch,CN_THREADSCRATCH)
__aFooterFunction(ThreadScratch)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ThreadScratch::ThreadScratch(void)
: Object(CN_THREADSCRATCH)
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADSCRATCH,_FOOTER_);

  m_pPagingMetrics = nullptr;
  
  _checkInitialised_(CN_THREADSCRATCH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_THREADSCRATCH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ThreadScratch::~ThreadScratch(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADSCRATCH,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* drop lock                                                                  */
/*----------------------------------------------------------------------------*/
void ThreadScratch::vDropLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADSCRATCH,_FOOTER_);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock                                                                  */
/*----------------------------------------------------------------------------*/
void ThreadScratch::vGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADSCRATCH,_FOOTER_);
  
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* get pPagingMetrics                                                         */
/*----------------------------------------------------------------------------*/
PagingMetrics* ThreadScratch::pPagingMetrics(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADSCRATCH,_FOOTER_);
  
  return m_pPagingMetrics;
}

/*----------------------------------------------------------------------------*/
/* set pPagingMetrics                                                         */
/*----------------------------------------------------------------------------*/
void ThreadScratch::vPagingMetrics(PagingMetrics* pPagingMetrics)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADSCRATCH,_FOOTER_);
  
  m_pPagingMetrics = pPagingMetrics;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
