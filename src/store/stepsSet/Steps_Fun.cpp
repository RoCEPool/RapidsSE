/*============================================================================*/
/* Steps_Fun.cpp - fun[] step group                                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Fun(void)
{
  vAddStep("Fun0[]"_Stz,      &Step::vFun0Get);
  vAddStep("Fun0[].call"_Stz, &Step::vFun0Call);
  vAddStep("Fun0[].get"_Stz,  &Step::vFun0Get);
  vAddStep("Fun0[].set"_Stz,  &Step::vFun0Set);
  
  vAddStep("Fun1[]"_Stz,      &Step::vFun1Get);
  vAddStep("Fun1[].call"_Stz, &Step::vFun1Call);
  vAddStep("Fun1[].get"_Stz,  &Step::vFun1Get);
  vAddStep("Fun1[].set"_Stz,  &Step::vFun1Set);

  vAddStep("Fun2[]"_Stz,      &Step::vFun2Get);
  vAddStep("Fun2[].call"_Stz, &Step::vFun2Call);
  vAddStep("Fun2[].get"_Stz,  &Step::vFun2Get);
  vAddStep("Fun2[].set"_Stz,  &Step::vFun2Set);
}

/*----------------------------------------------------------------------------*/
/* Fun0[].call -- call Fun0                                                   */
/*----------------------------------------------------------------------------*/
void Step::vFun0Call(void)
{
  ENTRY();
   
  bool (*afFun)(void*) = m_p_TaskLogicUnit->m_afFun0;

  bool b = (*afFun)(nullptr);
  if (! b)
    vTrace("Fun0 returned false\n"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun0[].get -- get Fun0                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun0Get(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr((Object*)m_p_TaskLogicUnit->m_afFun0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun0[].set -- set Fun0                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun0Set(void)
{
  ENTRY();
    
  void* afFun = m_pDataStack->pPopPtr(__func__);
  
  m_p_TaskLogicUnit->m_afFun0 = (bool (*)(void*)) afFun;

  EXIT();
}




/*----------------------------------------------------------------------------*/
/* Fun1[].call -- call Fun1                                                   */
/*----------------------------------------------------------------------------*/
void Step::vFun1Call(void)
{
  ENTRY();
   
  bool (*afFun)(void*) = m_p_TaskLogicUnit->m_afFun1;

  bool b = (*afFun)(nullptr);
  if (! b)
    vTrace("Fun1 returned false\n"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun1[].get -- get Fun1                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun1Get(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr((Object*)m_p_TaskLogicUnit->m_afFun1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun1[].set -- set Fun1                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun1Set(void)
{
  ENTRY();
    
  void* afFun = m_pDataStack->pPopPtr(__func__);
  
  m_p_TaskLogicUnit->m_afFun1 = (bool (*)(void*)) afFun;

  EXIT();
}



/*----------------------------------------------------------------------------*/
/* Fun2[].call -- call Fun2                                                   */
/*----------------------------------------------------------------------------*/
void Step::vFun2Call(void)
{
  ENTRY();
   
  bool (*afFun)(void*) = m_p_TaskLogicUnit->m_afFun2;

  bool b = (*afFun)(nullptr);
  if (! b)
    vTrace("Fun2 returned false\n"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun2[].get -- get Fun2                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun2Get(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr((Object*)m_p_TaskLogicUnit->m_afFun2);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun2[].set -- set Fun2                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun2Set(void)
{
  ENTRY();
    
  void* afFun = m_pDataStack->pPopPtr(__func__);
  
  m_p_TaskLogicUnit->m_afFun2 = (bool (*)(void*)) afFun;

  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
