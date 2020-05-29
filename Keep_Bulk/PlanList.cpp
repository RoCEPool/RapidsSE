/*============================================================================*/
/* PlanList.cpp - plan List                                                   */
/*============================================================================*/

#include "Plans.h"

#include "StorageEngine.h"
extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
PlanList* Plans::_pPlanList(void* aAddr)
{
  PlanList* pPlanList = reinterpret_cast<PlanList*>(aAddr);
  
  RASSERT_TYPE(pPlanList,CN_PLANLIST);
  
  return pPlanList;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* PlanList::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_PLANLIST);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* PlanList::operator new(size_t, PlanList* pPlanList)
{
  return pPlanList;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void PlanList::operator delete(void* pPlanListAddress)
{
  PlanList* pPlanList = reinterpret_cast<PlanList*>(pPlanListAddress);
  
  HeapMem::vFree(pPlanList);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PlanList::PlanList(void)
: Object(CN_PLANLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANLIST,_FOOTER_);
  
  _checkInitialised_(CN_PLANLIST);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PLANLIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PlanList::~PlanList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANLIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void PlanList::vContentDump(void)
{
  vDump(CN_PLANLIST, this, aSingleTab());
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void PlanList::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  PlanList* pPlanList = _pPlanList(pObject);
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* find plan based on id                                                      */
/*----------------------------------------------------------------------------*/
Plan* PlanList::pFind(i8 iPlanId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANLIST,_FOOTER_);
  
  Plan* pPlan = nullptr;
  
  m_zLock.vGrabRead();
  
  for (pPlan = m_pHead; pPlan; pPlan = pPlan->pNextNode())
    if (pPlan->iPlanId() == iPlanId)
      break;

  m_zLock.vDropRead();

  return pPlan;
}

/*----------------------------------------------------------------------------*/
/* display contents of plan list                                              */
/*----------------------------------------------------------------------------*/
void PlanList::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANLIST,_FOOTER_);

  Plan* pPlan = nullptr;
  
  m_zLock.vGrabRead();
  
  vTrace("  planList %p count %lld\n"_Stz,
         this,
         iCount());
  
  for (pPlan = m_pHead; pPlan; pPlan = pPlan->pNextNode())
    pPlan->vShow();
  
  m_zLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

_aFooter_(PlanList)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
