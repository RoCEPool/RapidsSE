/*============================================================================*/
/* PlanList.cpp - plan List                                                   */
/*============================================================================*/

#include "Plans.h"

extern Heap*        gpSmallHeap;
extern Heap*        gpMediumHeap;
extern PlanList*    gpPlanList;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
PlanList* Plans::_pPlanList(void* pAddr)
{
  PlanList* pPlanList = reinterpret_cast<PlanList*>(pAddr);
  
  RASSERT( !pPlanList || CN_PLANLIST == pPlanList->eClass() );
  
  return pPlanList;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* PlanList::operator new(size_t iSize)
{
  RASSERT(iSize);

  void* pObject = Heap::pAllocate(CN_PLANLIST);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* PlanList::operator new(size_t iSize, PlanList* pPlanList)
{
  RASSERT(iSize);
  return pPlanList;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void PlanList::operator delete(void* pPlanListAddress)
{
  PlanList* pPlanList = reinterpret_cast<PlanList*>(pPlanListAddress);
  
  Heap::vFree(pPlanList);
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
PlanList::~PlanList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANLIST,_FOOTER_);
  
  ASSERT( ! gpPlanList );
  
  gpPlanList = this;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void PlanList::vContentDump(void)
{
  vDump(CN_PLANLIST, this, pSingleTab());
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void PlanList::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_PLANLIST);
  ASSERT(pObject->eClass() == CN_PLANLIST);
  
  PlanList* pPlanList = reinterpret_cast<PlanList*>(pObject);
  
  vTrace("\n");
}

/*----------------------------------------------------------------------------*/
/* find plan based on id                                                      */
/*----------------------------------------------------------------------------*/
Plan* PlanList::pFind(u8 uPlanId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANLIST,_FOOTER_);
  
  Plan* pPlan = nullptr;
  
  m_zLock.vGrabRead();
  
  for (pPlan = m_pHead; pPlan; pPlan = pPlan->pNextNode())
    if (pPlan->uPlanId() == uPlanId)
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
  
  vTrace("  planList %p count %lld\n",
         this,
         iCount());
  
  for (pPlan = m_pHead; pPlan; pPlan = pPlan->pNextNode())
    pPlan->vShow();
  
  m_zLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* PlanList::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANLIST);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
