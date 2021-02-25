/*============================================================================*/
/* PlannerLock.cpp -- Planner Lock                                            */
/*============================================================================*/

#include "BASE.h"

#include "PlannerLock.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,PlannerLock,CN_PLANNERLOCK)
//__aNewFunction(PlannerLock,CN_PLANNERLOCK)
//__vDeleteFunction(PlannerLock,CN_PLANNERLOCK)
__aFooterFunction(PlannerLock);

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PlannerLock::PlannerLock(void)
: MasterLock(CN_PLANNERLOCK)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_PLANNERLOCK,_FOOTER_);
   
  _checkInitialised_(CN_PLANNERLOCK);
  
  //_postcondition_(_ADDRESS_,_MAGIC_,CN_PLANNERLOCK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PlannerLock::~PlannerLock(void) noexcept(false)
{
  printf("~PlannerLock~PlannerLock~PlannerLock~PlannerLock~PlannerLock\n");
  
  //_precondition_(_ADDRESS_,_MAGIC_,CN_PLANNERLOCK,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
