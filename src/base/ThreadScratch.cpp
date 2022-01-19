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
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADSCRATCH,_FOOTER_);
  
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


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
