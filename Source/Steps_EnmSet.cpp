/*============================================================================*/
/* Steps_EnmSet.cpp - ename-set step group                                    */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_EnmSet(void)
{
  vAddStep("enmSet.asStr"_Stz, &Step::vEnmSetAsStr);
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
