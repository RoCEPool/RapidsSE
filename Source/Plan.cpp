/*============================================================================*/
/* Plan.cpp - plan                                                            */
/*============================================================================*/

#include "Plans.h"

extern PlanList* gpPlanList;
extern Atomic    gzPlanNumber;
/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Plan* Plans::_pPlan(void* pAddr)
{
  Plan* pPlan = reinterpret_cast<Plan*>(pAddr);
  
  RASSERT( !pPlan || CN_PLAN == pPlan->eClass() );

  return pPlan;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Plan* Plans::_pPlan(i8 iAddr)
{
  Plan* pPlan = reinterpret_cast<Plan*>(iAddr);
  
  RASSERT( !pPlan || CN_PLAN == pPlan->eClass() );
  
  return pPlan;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Plan::operator new(size_t iSize)
{
  RASSERT(iSize);
  ASSERT(gpPlanList);
  
  Plan* pPlan = reinterpret_cast<Plan*>(Heap::pAllocate(CN_PLAN));
  
  return pPlan;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Plan::operator new(size_t iSize, Plan* pPlan)
{
  RASSERT(iSize);
  ASSERT(gpPlanList);
  
  return pPlan;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Plan::operator delete(void* pPlanAddress)
{
  Plan* pPlan = reinterpret_cast<Plan*>(pPlanAddress);
  
  Heap::vFree(pPlan);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Plan::Plan(void)
: Object(CN_PLAN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);

  ASSERT(gpPlanList);
  
  vInit();
    
  _checkInitialised_(CN_PLAN);
  
  Plan* pPlan = this;
  gpPlanList->vAppend(pPlan);
//  gpTaskList->vDump();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Plan::~Plan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);
  
  Plan* pPlan = this;
  gpPlanList->vRemove(pPlan);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
void Plan::vInit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);
  
  m_iPlanNo = ++gzPlanNumber;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Plan::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_PLAN);
  ASSERT(pObject->eClass() == CN_PLAN);
  
  Plan* pPlan = reinterpret_cast<Plan*>(pObject);
  
  vTrace("\n");
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Plan::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLAN);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
