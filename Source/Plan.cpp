/*============================================================================*/
/* Plan.cpp - plan                                                            */
/*============================================================================*/

#include "Plans.h"

extern PlanList* gpPlanList;
extern Atomic    gzPlanId;

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
  
//  Plan* pPlan = this;
//  gpPlanList->vAppend(pPlan);

  //gpPlanList->vDump();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* inialise                                                                   */
/*----------------------------------------------------------------------------*/
void Plan::vInit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);
  
  m_uPlanId = ++gzPlanId;
  
  //m_sName
  //m_xDic
  //m_sLambda
  
  m_pRegList = nullptr;
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
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Plan::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_PLAN);
  ASSERT(pObject->eClass() == CN_PLAN);
  
  Plan* pPlan = reinterpret_cast<Plan*>(pObject);
  
  vTrace("planId %lld.%lld name %s\n",
         pPlan->m_uPlanId >> 48,
         pPlan->m_uPlanId & 0x00FFFFFFFFFFFFFF,
         pPlan->m_sName.a());
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append text to lambda                                                      */
/*----------------------------------------------------------------------------*/
void Plan::vAppendToLambda(s48K& sText)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);
  
  String::vConcat(m_sLambda.a(), sText.a());
}

/*----------------------------------------------------------------------------*/
/* get plan id                                                                */
/*----------------------------------------------------------------------------*/
u8 Plan::uPlanId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);

  return m_uPlanId;
}

/*----------------------------------------------------------------------------*/
/* get plan name                                                              */
/*----------------------------------------------------------------------------*/
s32& Plan::sName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);
  
  return m_sName;
}

/*----------------------------------------------------------------------------*/
/* set plan name                                                              */
/*----------------------------------------------------------------------------*/
void Plan::vName(s32& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);
  
  m_sName = sName;
}

/*----------------------------------------------------------------------------*/
/* get plan name                                                              */
/*----------------------------------------------------------------------------*/
Dic* Plan::pDic(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);
  
  return &m_xDic;
}

/*----------------------------------------------------------------------------*/
/* get lambda text                                                            */
/*----------------------------------------------------------------------------*/
s48K& Plan::sLambda(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);
  
  return m_sLambda;
}

/*----------------------------------------------------------------------------*/
/* set lambda text                                                            */
/*----------------------------------------------------------------------------*/
void Plan::vLambda(s48K& sLambda)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);
  
  m_sLambda = sLambda;
}

/*----------------------------------------------------------------------------*/
/* show this plan                                                             */
/*----------------------------------------------------------------------------*/
void Plan::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);

  vTrace("  plan %p id %lld.%lld %s\n",
         this,
         m_uPlanId >> 48,
         m_uPlanId & 0x00FFFFFFFFFFFFFF,
         m_sName.a());
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
