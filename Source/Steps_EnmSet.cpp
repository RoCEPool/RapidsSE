/*============================================================================*/
/* Steps_EnmSet.cpp - ename-set step group                                    */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_EnmSet(void)
{
  vAddStep("EnameSet::new"_Stz,    &Step::vEnmSetNew);
  
  vAddStep("EnameSet.addInt"_Stz,  &Step::vEnmSetAddInt);
  vAddStep("EnameSet.addStr"_Stz,  &Step::vEnmSetAddStr);
  vAddStep("EnameSet.asStr"_Stz,   &Step::vEnmSetAsStr);
  vAddStep("EnameSet.getStr"_Stz,  &Step::vEnmSetGetStr);
  vAddStep("EnameSet.hasName"_Stz, &Step::vEnmSetHasName);
}

/*----------------------------------------------------------------------------*/
/* EnameSet::new -- new ename-set                                               */
/*----------------------------------------------------------------------------*/
void Step::vEnmSetNew(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vEnmSetNew"_Stz);
  
  EnameSet* pEnameSet = new EnameSet;
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pEnameSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* EnameSet.addInt -- add a name-integer pair to ename-set                      */
/*----------------------------------------------------------------------------*/
void Step::vEnmSetAddInt(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vEnmSetAddInt"_Stz);
  
  Object* pEnameSet_obj;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pEnameSet_obj) )
    THROW_INVALID_DSTACK("vEnmSetAddStr"_Stz);
  EnameSet* pEnameSet = _pEnameSet(pEnameSet_obj);
  
  i8 iValue;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iValue) )
    THROW_INVALID_DSTACK("vEnmSetAddStr"_Stz);
  
  s32 sName;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("vEnmSetAddStr"_Stz);
  
  pEnameSet->vAdd(sName,iValue,TN_INTEGER);
  pEnameSet->vPrint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* EnameSet.addStr -- add a name-string pair to ename-set                       */
/*----------------------------------------------------------------------------*/
void Step::vEnmSetAddStr(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vEnmSetAddStr"_Stz);
  
  Object* pEnameSet_obj;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pEnameSet_obj) )
    THROW_INVALID_DSTACK("vEnmSetAddStr"_Stz);
  EnameSet* pEnameSet = _pEnameSet(pEnameSet_obj);

  s32 sValue;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sValue) )
    THROW_INVALID_DSTACK("vEnmSetAddStr"_Stz);
  
  s32 sName;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("vEnmSetAddStr"_Stz);
  
  pEnameSet->vAdd(sName,sValue,TN_STRING);
  pEnameSet->vPrint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* EnameSet.asStr -- get ename-set as a single string                           */
/*----------------------------------------------------------------------------*/
void Step::vEnmSetAsStr(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vEnmSetAsStr"_Stz);
  
  Object* pEnameSet_obj;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pEnameSet_obj) )
    THROW_INVALID_DSTACK("vEnmSetAsStr"_Stz);
  
  EnameSet* pEnameSet = _pEnameSet(pEnameSet_obj);
  
  s4K sStr;
  SP_EnmSet spEnameSet;
  sStr = spEnameSet.sz(pEnameSet);
  
  mpTask->m_pForth->m_pDataStack->vPushS4K(sStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* EnameSet.getStr -- get str value from ename-set with given name              */
/*----------------------------------------------------------------------------*/
void Step::vEnmSetGetStr(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vEnmSetGetStr"_Stz);
  
  Object* pEnameSet_obj;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pEnameSet_obj) )
    THROW_INVALID_DSTACK("vEnmSetGetStr"_Stz);
  EnameSet* pEnameSet = _pEnameSet(pEnameSet_obj);
  
  s32 sName;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("vEnmSetGetStr"_Stz);
  
  s48K sValue;
  sValue = pEnameSet->szValue(sName);
  
  mpTask->m_pForth->m_pDataStack->vPushS48K(sValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* EnameSet.hasName -- test if ename-set has a pair with given name             */
/*----------------------------------------------------------------------------*/
void Step::vEnmSetHasName(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vEnmSetHasName"_Stz);
  
  Object* pEnameSet_obj;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pEnameSet_obj) )
    THROW_INVALID_DSTACK("vEnmSetHasName"_Stz);
  EnameSet* pEnameSet = _pEnameSet(pEnameSet_obj);
  
  s32 sName;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("vEnmSetHasName"_Stz);

  bool bHas = pEnameSet->bHasName(sName);
  
  mpTask->m_pForth->m_pDataStack->vPushBool(bHas);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
