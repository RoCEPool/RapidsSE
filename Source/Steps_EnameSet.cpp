/*============================================================================*/
/* Steps_EnameSet.cpp - ename-set step group                                  */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_EnameSet(void)
{
  vAddStep("enameSet::new"_Stz,    &Step::vEnameSetNew);
  
  vAddStep("enameSet.addInt"_Stz,  &Step::vEnameSetAddInt);
  vAddStep("enameSet.addStr"_Stz,  &Step::vEnameSetAddStr);
  vAddStep("enameSet.asStr"_Stz,   &Step::vEnameSetAsStr);
  vAddStep("enameSet.getInt"_Stz,  &Step::vEnameSetGetInt);
  vAddStep("enameSet.getStr"_Stz,  &Step::vEnameSetGetStr);
  vAddStep("enameSet.hasName"_Stz, &Step::vEnameSetHasName);
}

/*----------------------------------------------------------------------------*/
/* EnameSet::new -- new ename-set                                             */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetNew(void)
{
  ENTRY("vEnameSetNew"_Stz);
  
  EnameSet* pEnameSet = new EnameSet;
  
  m_pDataStack->vPushPointer(pEnameSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* EnameSet.addInt -- add a name-integer pair to ename-set                    */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetAddInt(void)
{
  ENTRY("vEnameSetAddInt"_Stz);
  
  Object* pEnameSet_obj;
  if ( ! m_pDataStack->bPopObject(pEnameSet_obj) )
    THROW_DSTACK();
  EnameSet* pEnameSet = _pEnameSet(pEnameSet_obj);
  
  i8 iValue;
  if ( ! m_pDataStack->bPopI8(iValue) )
    THROW_DSTACK();
  
  s32 sName;
  if ( ! m_pDataStack->bPopS32(sName) )
    THROW_DSTACK();
  
  pEnameSet->vAdd(sName,iValue);
  pEnameSet->vPrint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* EnameSet.addStr -- add a name-string pair to ename-set                     */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetAddStr(void)
{
  ENTRY("vEnameSetAddStr"_Stz);
  
  Object* pEnameSet_obj;
  if ( ! m_pDataStack->bPopObject(pEnameSet_obj) )
    THROW_DSTACK();
  EnameSet* pEnameSet = _pEnameSet(pEnameSet_obj);

  s256 sValue;
  if ( ! m_pDataStack->bPopS256(sValue) )
    THROW_DSTACK();
  
  s32 sName;
  if ( ! m_pDataStack->bPopS32(sName) )
    THROW_DSTACK();
  
  pEnameSet->vAdd(sName,sValue);
  //pEnameSet->vPrint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* EnameSet.asStr -- get ename-set as a single string                         */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetAsStr(void)
{
  ENTRY("vEnameSetAsStr"_Stz);
  
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
/* EnameSet.getInt -- get int value from ename-set with given name            */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetGetInt(void)
{
  ENTRY("vEnameSetGetInt"_Stz);
  
  Object* pEnameSet_obj;
  if ( ! m_pDataStack->bPopObject(pEnameSet_obj) )
    THROW_DSTACK();
  EnameSet* pEnameSet = _pEnameSet(pEnameSet_obj);
  
  s32 sName;
  if ( ! m_pDataStack->bPopS32(sName) )
    THROW_DSTACK();
  
  i8 iValue;
  iValue = pEnameSet->iValue(sName);
  
  m_pDataStack->vPushI8(iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* EnameSet.getStr -- get str value from ename-set with given name            */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetGetStr(void)
{
  ENTRY("vEnameSetGetStr"_Stz);
  
  Object* pEnameSet_obj;
  if ( ! m_pDataStack->bPopObject(pEnameSet_obj) )
    THROW_DSTACK();
  EnameSet* pEnameSet = _pEnameSet(pEnameSet_obj);
  
  s32 sName;
  if ( ! m_pDataStack->bPopS32(sName) )
    THROW_DSTACK();
  
  s4K sValue;
  sValue = pEnameSet->szValue(sName);
  
  m_pDataStack->vPushS4K(sValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* EnameSet.hasName -- test if ename-set has a pair with given name           */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetHasName(void)
{
  ENTRY("vEnameSetHasName"_Stz);
  
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
