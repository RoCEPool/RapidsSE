/*============================================================================*/
/* Steps_LogicUnit.cpp -- LogicUnit step group                                */
/*============================================================================*/

#include "Steps.h"

extern EnvFlags gxEnvFlags;;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_LogicUnit(void)
{
  vAddStep("LogicUnit[].dynFun"_Stz,        &Step::vLogicUnitDynFun);
  vAddStep("LogicUnit[].dynFunArgs"_Stz,    &Step::vLogicUnitDynFunArgs);
  vAddStep("LogicUnit[].genCode"_Stz,       &Step::vLogicUnitGenCode);
    
  vAddStep("LogicUnit[].callAgain@"_Stz,    &Step::vLogicUnitCallAgainFetch);
  vAddStep("LogicUnit[].callAgain!"_Stz,    &Step::vLogicUnitCallAgainStore);
  vAddStep("LogicUnit[].calledAgain@"_Stz,  &Step::vLogicUnitCalledAgainFetch);
  vAddStep("LogicUnit[].calledAgain!"_Stz,  &Step::vLogicUnitCalledAgainStore);

  vAddStep("LogicUnit[].dev@"_Stz,          &Step::vLogicUnitDevFetch);
  vAddStep("LogicUnit[].dev!"_Stz,          &Step::vLogicUnitDevStore);
  
  vAddStep("LogicUnit[].handler@"_Stz,      &Step::vLogicUnitHandlerFetch);
  vAddStep("LogicUnit[].handler!"_Stz,      &Step::vLogicUnitHandlerStore);
  
  vAddStep("LogicUnit[].init"_Stz,          &Step::vLogicUnitInit);
  
  vAddStep("LogicUnit[].transform"_Stz,     &Step::vLogicUnitTransform);
  vAddStep("LogicUnit[].transformArgs"_Stz, &Step::vLogicUnitTransformArgs);
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].dynFun -- call dynamic function created by LogicCompiler       */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDynFun(void)
{
  ENTRY();
  
  i8 iRes = m_pLogicUnit->iDynFun(0,C_UNKNOWN);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].dynFun -- call dynamic function created by LogicCompiler       */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDynFunArgs(void)
{
  ENTRY();
  
  i8 nArgs = m_pDataStack->iPopInt(__func__);

  if (nArgs) {
    RASSERT(1 == nArgs);
  }
  i8 iArg0 = m_pDataStack->iPopInt(__func__);

  i8 iRes = m_pLogicUnit->iDynFun(nArgs,iArg0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].genCode -- generate code for given test                        */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitGenCode(void)
{
  ENTRY();
  
  i8 iTestNo = m_pDataStack->iPopInt(__func__);
      
  m_pLogicUnit->vGenCode(iTestNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].callAgain@ --                                                  */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCallAgainFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xRluCommon.m_bCallAgain);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].callAgain! --                                                  */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCallAgainStore(void)
{
  ENTRY();

  i8 iCallAgain = m_pDataStack->iPopInt(__func__);
  
  m_pLogicUnit->m_xRluCommon.m_bCallAgain = iCallAgain;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].calledAgain@ --                                                */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCalledAgainFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xRluCommon.m_bCalledAgain);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].calledAgain! --                                                */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCalledAgainStore(void)
{
  ENTRY();

  i8 iCallAgain = m_pDataStack->iPopInt(__func__);
  
  m_pLogicUnit->m_xRluCommon.m_bCalledAgain = iCallAgain;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].dev@ --                                                        */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDevFetch(void)
{
  ENTRY();

  i8 i_Dev = m_pDataStack->iPopInt(__func__);
  
  void* aDev = m_pLogicUnit->m_aaDev[i_Dev];
  
  m_pDataStack->vPushPtr((Object*)aDev);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].dev! --                                                        */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitDevStore(void)
{
  ENTRY();

  i8 i_Dev = m_pDataStack->iPopInt(__func__);
  
  void* aDev = m_pDataStack->pPopPtr(__func__);
  
  m_pLogicUnit->m_aaDev[i_Dev] = aDev;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].handler@ --                                                    */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitHandlerFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpHandler);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].handler! --                                                    */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitHandlerStore(void)
{
  ENTRY();

  Word* pHandler = _pWord(m_pDataStack->pPopPtr(__func__));
  
  m_pLogicUnit->m_xpHandler = pHandler;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].init -- init the logic unit                                    */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitInit(void)
{
  ENTRY();
  
  m_pLogicUnit->vInit();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].transform -- transform                                         */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitTransform(void)
{
  ENTRY();
  
  i8 bResult = m_pLogicUnit->iTransform(0,C_UNKNOWN);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].transformArgs -- transform (with arguments)                    */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitTransformArgs(void)
{
  ENTRY();
  
  i8 nArgs = m_pDataStack->iPopInt(__func__);

  if (nArgs) {
    RASSERT(1 == nArgs);
  }
  i8 iArg0 = m_pDataStack->iPopInt(__func__);

  i8 bResult = m_pLogicUnit->iTransform(nArgs,iArg0);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
