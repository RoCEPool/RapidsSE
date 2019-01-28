/*============================================================================*/
/* Plan.cpp - plan                                                            */
/*============================================================================*/

#include "StorageEngine.h"
#include "Plans.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Plan* Plans::_pPlan(void* aAddr)
{
  Plan* pPlan = reinterpret_cast<Plan*>(aAddr);
  
  RASSERT_TYPE(pPlan,CN_PLAN);

  return pPlan;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Plan* Plans::_pPlan(i8 iAddr)
{
  Plan* pPlan = reinterpret_cast<Plan*>(iAddr);
  
  RASSERT_TYPE(pPlan,CN_PLAN);
  
  return pPlan;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Plan::operator new(size_t)
{
  Plan* pPlan = reinterpret_cast<Plan*>(HeapMem::pAllocate(CN_PLAN));
  
  return pPlan;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Plan::operator new(size_t, Plan* pPlan)
{
  return pPlan;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Plan::operator delete(void* pPlanAddress)
{
  Plan* pPlan = reinterpret_cast<Plan*>(pPlanAddress);
  
  HeapMem::vFree(pPlan);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Plan::Plan(void)
: Object(CN_PLAN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);
  
  vInit();
    
  _checkInitialised_(CN_PLAN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* inialise                                                                   */
/*----------------------------------------------------------------------------*/
void Plan::vInit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);
  
  m_iPlanId = ++gpAttachMap->m_ixPlanId;
  
  //m_sName
  //m_xDic
  //m_sLambda
  
  m_pRegList       = nullptr;
  m_pExecutingTask = nullptr;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Plan::~Plan(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Plan::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Plan* pPlan = _pPlan(pObject);
  
  vTrace("planId %lld.%lld name %s\n"_Stz,
         pPlan->m_iPlanId >> 48,
         pPlan->m_iPlanId & 0x00FFFFFFFFFFFFFF,
         pPlan->m_sName.aChar());
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
  
  m_sLambda.vConcat(sText);
}

/*----------------------------------------------------------------------------*/
/* get execute task                                                           */
/*----------------------------------------------------------------------------*/
Tasks::Task* Plan::pExecutingTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);
  
  return m_pExecutingTask;
}

/*----------------------------------------------------------------------------*/
/* set execute task                                                           */
/*----------------------------------------------------------------------------*/
void Plan::vExecutingTask(Tasks::Task* pExecutingTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);
  
  m_pExecutingTask = pExecutingTask;
}

/*----------------------------------------------------------------------------*/
/* get plan id                                                                */
/*----------------------------------------------------------------------------*/
i8 Plan::iPlanId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);

  return m_iPlanId;
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
/* planning has finished                                                      */
/*----------------------------------------------------------------------------*/
void Plan::vFinished(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);
  
  while (m_pRegList) {
    LocalSet* pLocalSet = m_pRegList;
    m_pRegList = m_pRegList->pNext();
    
    delete pLocalSet;
  }  
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
/* nullify pointers (invoked before sending plan to anothetr node)            */
/*----------------------------------------------------------------------------*/
void Plan::vNullifyPointers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);
  
  m_pRegList = nullptr;
}

/*----------------------------------------------------------------------------*/
/* push register frame                                                        */
/*----------------------------------------------------------------------------*/
void Plan::vLocalSetPush(LocalSet* pLocalSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);
  
  pLocalSet->vNext(m_pRegList);
  
  m_pRegList = pLocalSet;
}

/*----------------------------------------------------------------------------*/
/* get top register frame                                                     */
/*----------------------------------------------------------------------------*/
LocalSet* Plan::pLocalSetTop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);
    
  return m_pRegList;
}

/*----------------------------------------------------------------------------*/
/* show this plan                                                             */
/*----------------------------------------------------------------------------*/
void Plan::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLAN,_FOOTER_);

  vTrace("  plan %p id %lld.%lld %s\n"_Stz,
         this,
         m_iPlanId >> 48,
         m_iPlanId & 0x00FFFFFFFFFFFFFF,
         m_sName.aChar());
  
  LocalSet* pLocalSet = m_pRegList;
  
  while(pLocalSet) {
    pLocalSet = pLocalSet->m_pNext;
    if (pLocalSet)
      vTrace("<-"_Stz);
  }
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Plan)


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
