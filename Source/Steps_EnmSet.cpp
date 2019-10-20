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
void Step::vEnmSetNew(void)
{
  ENTRY("vEnmSetNew"_Stz);
  
  EnameSet* pEnameSet = new EnameSet;
  
  m_pDataStack->vPushPointer(pEnameSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* EnameSet.addInt -- add a name-integer pair to ename-set                      */
/*----------------------------------------------------------------------------*/
void Step::vEnmSetAddInt(void)
{
  ENTRY("vEnmSetAddInt"_Stz);
  
  Object* pEnameSet_obj;
  if ( ! m_pDataStack->bPopObject(pEnameSet_obj) )
    THROW_DSTACK();
  EnameSet* pEnameSet = _pEnameSet(pEnameSet_obj);
  
  i8 iValue;
  if ( ! m_pDataStack->bPop(iValue) )
    THROW_DSTACK();
  
  s32 sName;
  if ( ! m_pDataStack->bPopS32(sName) )
    THROW_DSTACK();
  
  pEnameSet->vAdd(sName,iValue,TN_INTEGER);
  pEnameSet->vPrint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* EnameSet.addStr -- add a name-string pair to ename-set                       */
/*----------------------------------------------------------------------------*/
void Step::vEnmSetAddStr(void)
{
  ENTRY("vEnmSetAddStr"_Stz);
  
  Object* pEnameSet_obj;
  if ( ! m_pDataStack->bPopObject(pEnameSet_obj) )
    THROW_DSTACK();
  EnameSet* pEnameSet = _pEnameSet(pEnameSet_obj);

  s32 sValue;
  if ( ! m_pDataStack->bPopS32(sValue) )
    THROW_DSTACK();
  
  s32 sName;
  if ( ! m_pDataStack->bPopS32(sName) )
    THROW_DSTACK();
  
  pEnameSet->vAdd(sName,sValue,TN_STRING);
  pEnameSet->vPrint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* EnameSet.asStr -- get ename-set as a single string                           */
/*----------------------------------------------------------------------------*/
void Step::vEnmSetAsStr(void)
{
  ENTRY("vEnmSetAsStr"_Stz);
  
  Object* pEnameSet_obj;
  if ( ! m_pDataStack->bPopObject(pEnameSet_obj) )
    THROW_DSTACK();
  
  EnameSet* pEnameSet = _pEnameSet(pEnameSet_obj);
  
  s4K sStr;
  SP_EnmSet spEnameSet;
  sStr = spEnameSet.sz(pEnameSet);
  
  m_pDataStack->vPushS4K(sStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* EnameSet.getStr -- get str value from ename-set with given name              */
/*----------------------------------------------------------------------------*/
void Step::vEnmSetGetStr(void)
{
  ENTRY("vEnmSetGetStr"_Stz);
  
  Object* pEnameSet_obj;
  if ( ! m_pDataStack->bPopObject(pEnameSet_obj) )
    THROW_DSTACK();
  EnameSet* pEnameSet = _pEnameSet(pEnameSet_obj);
  
  s32 sName;
  if ( ! m_pDataStack->bPopS32(sName) )
    THROW_DSTACK();
  
  s48K sValue;
  sValue = pEnameSet->szValue(sName);
  
  m_pDataStack->vPushS48K(sValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* EnameSet.hasName -- test if ename-set has a pair with given name             */
/*----------------------------------------------------------------------------*/
void Step::vEnmSetHasName(void)
{
  ENTRY("vEnmSetHasName"_Stz);
  
  Object* pEnameSet_obj;
  if ( ! m_pDataStack->bPopObject(pEnameSet_obj) )
    THROW_DSTACK();
  EnameSet* pEnameSet = _pEnameSet(pEnameSet_obj);
  
  s32 sName;
  if ( ! m_pDataStack->bPopS32(sName) )
    THROW_DSTACK();

  bool bHas = pEnameSet->bHasName(sName);
  
  m_pDataStack->vPushBool(bHas);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
