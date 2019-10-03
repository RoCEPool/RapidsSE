/*============================================================================*/
/* Steps_EnmSet.cpp - ename-set step group                                    */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_EnmSet(void)
{
  vAddStep("enmSet::new"_Stz,    &Step::vEnmSetNew);
  
  vAddStep("enmSet.addInt"_Stz,  &Step::vEnmSetAddInt);
  vAddStep("enmSet.addStr"_Stz,  &Step::vEnmSetAddStr);
  vAddStep("enmSet.asStr"_Stz,   &Step::vEnmSetAsStr);
  vAddStep("enmSet.getStr"_Stz,  &Step::vEnmSetGetStr);
  vAddStep("enmSet.hasName"_Stz, &Step::vEnmSetHasName);
}

/*----------------------------------------------------------------------------*/
/* enmSet::new -- new ename-set                                               */
/*----------------------------------------------------------------------------*/
void Step::vEnmSetNew(MP_Task mpTask)
{
  ENTRY("vEnmSetNew"_Stz);
  
  EnmSet* pEnmSet = new EnmSet;
  
  mpTask->m_pDataStack->vPushPointer(pEnmSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* enmSet.addInt -- add a name-integer pair to ename-set                      */
/*----------------------------------------------------------------------------*/
void Step::vEnmSetAddInt(MP_Task mpTask)
{
  ENTRY("vEnmSetAddInt"_Stz);
  
  Object* pEnmSet_obj;
  if ( ! mpTask->m_pDataStack->bPop(pEnmSet_obj) )
    THROW_INVALID_DSTACK("vEnmSetAddStr"_Stz);
  EnmSet* pEnmSet = _pEnmSet(pEnmSet_obj);
  
  i8 iValue;
  if ( ! mpTask->m_pDataStack->bPop(iValue) )
    THROW_INVALID_DSTACK("vEnmSetAddStr"_Stz);
  
  s32 sName;
  if ( ! mpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("vEnmSetAddStr"_Stz);
  
  pEnmSet->vAdd(sName,iValue,TN_INTEGER);
  pEnmSet->vPrint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* enmSet.addStr -- add a name-string pair to ename-set                       */
/*----------------------------------------------------------------------------*/
void Step::vEnmSetAddStr(MP_Task mpTask)
{
  ENTRY("vEnmSetAddStr"_Stz);
  
  Object* pEnmSet_obj;
  if ( ! mpTask->m_pDataStack->bPop(pEnmSet_obj) )
    THROW_INVALID_DSTACK("vEnmSetAddStr"_Stz);
  EnmSet* pEnmSet = _pEnmSet(pEnmSet_obj);

  s32 sValue;
  if ( ! mpTask->m_pDataStack->bPop(sValue) )
    THROW_INVALID_DSTACK("vEnmSetAddStr"_Stz);
  
  s32 sName;
  if ( ! mpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("vEnmSetAddStr"_Stz);
  
  pEnmSet->vAdd(sName,sValue,TN_STRING);
  pEnmSet->vPrint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* enmSet.asStr -- get ename-set as a single string                           */
/*----------------------------------------------------------------------------*/
void Step::vEnmSetAsStr(MP_Task mpTask)
{
  ENTRY("vEnmSetAsStr"_Stz);
  
  Object* pEnmSet_obj;
  if ( ! mpTask->m_pDataStack->bPop(pEnmSet_obj) )
    THROW_INVALID_DSTACK("vEnmSetAsStr"_Stz);
  
  EnmSet* pEnmSet = _pEnmSet(pEnmSet_obj);
  
  s4K sStr;
  SP_EnmSet spEnmSet;
  sStr = spEnmSet.sz(pEnmSet);
  
  mpTask->m_pDataStack->vPushS4K(sStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* enmSet.getStr -- get str value from ename-set with given name              */
/*----------------------------------------------------------------------------*/
void Step::vEnmSetGetStr(MP_Task mpTask)
{
  ENTRY("vEnmSetGetStr"_Stz);
  
  Object* pEnmSet_obj;
  if ( ! mpTask->m_pDataStack->bPop(pEnmSet_obj) )
    THROW_INVALID_DSTACK("vEnmSetGetStr"_Stz);
  EnmSet* pEnmSet = _pEnmSet(pEnmSet_obj);
  
  s32 sName;
  if ( ! mpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("vEnmSetGetStr"_Stz);
  
  s48K sValue;
  sValue = pEnmSet->szValue(sName);
  
  mpTask->m_pDataStack->vPushS48K(sValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* enmSet.hasName -- test if ename-set has a pair with given name             */
/*----------------------------------------------------------------------------*/
void Step::vEnmSetHasName(MP_Task mpTask)
{
  ENTRY("vEnmSetHasName"_Stz);
  
  Object* pEnmSet_obj;
  if ( ! mpTask->m_pDataStack->bPop(pEnmSet_obj) )
    THROW_INVALID_DSTACK("vEnmSetHasName"_Stz);
  EnmSet* pEnmSet = _pEnmSet(pEnmSet_obj);
  
  s32 sName;
  if ( ! mpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("vEnmSetHasName"_Stz);

  bool bHas = pEnmSet->bHasName(sName);
  
  mpTask->m_pDataStack->vPushBool(bHas);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
