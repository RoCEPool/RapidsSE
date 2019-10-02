/*============================================================================*/
/* Steps_EnmSet.cpp - ename-set step group                                    */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_EnmSet(void)
{
  vAddStep("enmSet::new"_Stz,   &Step::vEnmSetNew);
  
  vAddStep("enmSet.addStr"_Stz, &Step::vEnmSetAddStr);
  vAddStep("enmSet.asStr"_Stz,  &Step::vEnmSetAsStr);
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


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
