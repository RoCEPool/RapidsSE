/*============================================================================*/
/* Steps_Fun.cpp - fun[] step group                                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "Op.hpp"
#include "Stream.hpp"
#include "Stage.hpp"

#include "LogicUnit.hpp"

//#define WITH_COMBINEDOP_SE endif

#define WITH_FLAT_STAGE1 endif
//#define WITH_FLAT_STAGE2 endif
//#define WITH_FLAT_STAGE3 endif
//#define WITH_FLAT_STAGE4 endif

#ifdef WITH_COMBINEDOP_SE
  #include "../combined/FlatQuery.hpp"
#endif

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Fun(void)
{
  vAddStep("CrwHandler[]"_Stz,      &Step::vCrwHandlerGet);
  vAddStep("CrwHandler[].get"_Stz,  &Step::vCrwHandlerGet);
  vAddStep("CrwHandler[].set"_Stz,  &Step::vCrwHandlerSet);

  vAddStep("Fun1[]"_Stz,      &Step::vFun1Get);
  vAddStep("Fun1[].get"_Stz,  &Step::vFun1Get);
  vAddStep("Fun1[].set"_Stz,  &Step::vFun1Set);

  vAddStep("Fun2[]"_Stz,      &Step::vFun2Get);
  vAddStep("Fun2[].get"_Stz,  &Step::vFun2Get);
  vAddStep("Fun2[].isNotNull"_Stz,  &Step::vFun2IsNotNull);
  vAddStep("Fun2[].set"_Stz,  &Step::vFun2Set);

  vAddStep("Fun3[]"_Stz,      &Step::vFun3Get);
  vAddStep("Fun3[].get"_Stz,  &Step::vFun3Get);
  vAddStep("Fun3[].set"_Stz,  &Step::vFun3Set);

  vAddStep("Fun4[]"_Stz,      &Step::vFun4Get);
  vAddStep("Fun4[].get"_Stz,  &Step::vFun4Get);
  vAddStep("Fun4[].set"_Stz,  &Step::vFun4Set);
}

/*----------------------------------------------------------------------------*/
/* CrwHandler[].get -- get CrwHandler                                          */
/*----------------------------------------------------------------------------*/
void Step::vCrwHandlerGet(void)
{
  ENTRY();

  m_pDataStack->vPushInt((i8)* m_pLogicUnit->m_afCrwHandler);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CrwHandler[].set -- set CrwHandler                                                     */
/*----------------------------------------------------------------------------*/
void Step::vCrwHandlerSet(void)
{
  ENTRY();

  i8 iFun = m_pDataStack->iPopInt(__func__);

  m_pLogicUnit->m_afCrwHandler = (bool (*)(void*)) iFun;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun1[].get -- get Fun1                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun1Get(void)
{
  ENTRY();

  m_pDataStack->vPushInt((i8)* m_pLogicUnit->m_afFun1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun1[].set -- set Fun1                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun1Set(void)
{
  ENTRY();

  i8 iFun = m_pDataStack->iPopInt(__func__);

  m_pLogicUnit->m_afFun1 = (bool (*)(void*)) iFun;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun2[].get -- get Fun2                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun2Get(void)
{
  ENTRY();

  m_pDataStack->vPushInt((i8)* m_pLogicUnit->m_afFun2);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun2[].isNotNull -- fun2 is not null                                       */
/*----------------------------------------------------------------------------*/
void Step::vFun2IsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_afFun2 ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun2[].set -- set Fun2                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun2Set(void)
{
  ENTRY();

  i8 iFun = m_pDataStack->iPopInt(__func__);

  m_pLogicUnit->m_afFun2 = (bool (*)(void*)) iFun;

  EXIT();
}


/*----------------------------------------------------------------------------*/
/* Fun3[].get -- get Fun3                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun3Get(void)
{
  ENTRY();

  m_pDataStack->vPushInt((i8)* m_pLogicUnit->m_afFun3);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun3[].set -- set Fun3                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun3Set(void)
{
  ENTRY();

  i8 iFun = m_pDataStack->iPopInt(__func__);

  m_pLogicUnit->m_afFun3 = (bool (*)(void*)) iFun;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun4[].get -- get Fun4                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun4Get(void)
{
  ENTRY();

  m_pDataStack->vPushInt((i8)* m_pLogicUnit->m_afFun4);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun4[].set -- set Fun4                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun4Set(void)
{
  ENTRY();

  i8 iFun = m_pDataStack->iPopInt(__func__);

  m_pLogicUnit->m_afFun4 = (bool (*)(void*)) iFun;

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
