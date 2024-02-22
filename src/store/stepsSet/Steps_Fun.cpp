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

  vAddStep("PdsHandler[]"_Stz,      &Step::vPdsHandlerGet);
  vAddStep("PdsHandler[].get"_Stz,  &Step::vPdsHandlerGet);
  vAddStep("PdsHandler[].set"_Stz,  &Step::vPdsHandlerSet);

  vAddStep("IndexHandler[]"_Stz,      &Step::vIndexHandlerGet);
  vAddStep("IndexHandler[].get"_Stz,  &Step::vIndexHandlerGet);
  vAddStep("IndexHandler[].isNotNull"_Stz,  &Step::vIndexHandlerIsNotNull);
  vAddStep("IndexHandler[].set"_Stz,  &Step::vIndexHandlerSet);

  vAddStep("Fun3[]"_Stz,      &Step::vFun3Get);
  vAddStep("Fun3[].get"_Stz,  &Step::vFun3Get);
  vAddStep("Fun3[].set"_Stz,  &Step::vFun3Set);

  vAddStep("Fun4[]"_Stz,      &Step::vFun4Get);
  vAddStep("Fun4[].get"_Stz,  &Step::vFun4Get);
  vAddStep("Fun4[].set"_Stz,  &Step::vFun4Set);
}

/*----------------------------------------------------------------------------*/
/* CrwHandler[].get -- get CrwHandler                                         */
/*----------------------------------------------------------------------------*/
void Step::vCrwHandlerGet(void)
{
  ENTRY();

  m_pDataStack->vPushInt((i8)* m_pLogicUnit->m_afCrwHandler);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CrwHandler[].set -- set CrwHandler                                         */
/*----------------------------------------------------------------------------*/
void Step::vCrwHandlerSet(void)
{
  ENTRY();

  i8 iFun = m_pDataStack->iPopInt(__func__);

  m_pLogicUnit->m_afCrwHandler = (bool (*)(void*)) iFun;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsHandler[].get -- get PdsHandler                                         */
/*----------------------------------------------------------------------------*/
void Step::vPdsHandlerGet(void)
{
  ENTRY();

  m_pDataStack->vPushInt((i8)* m_pLogicUnit->m_afPdsHandler);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsHandler[].set -- set PdsHandler                                         */
/*----------------------------------------------------------------------------*/
void Step::vPdsHandlerSet(void)
{
  ENTRY();

  i8 iFun = m_pDataStack->iPopInt(__func__);

  m_pLogicUnit->m_afPdsHandler = (bool (*)(void*)) iFun;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexHandler[].get -- get IndexHandler                                     */
/*----------------------------------------------------------------------------*/
void Step::vIndexHandlerGet(void)
{
  ENTRY();

  m_pDataStack->vPushInt((i8)* m_pLogicUnit->m_afIndexHandler);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexHandler[].isNotNull -- IndexHandler is not null                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexHandlerIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_afIndexHandler ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexHandler[].set -- set IndexHandler                                     */
/*----------------------------------------------------------------------------*/
void Step::vIndexHandlerSet(void)
{
  ENTRY();

  i8 iFun = m_pDataStack->iPopInt(__func__);

  m_pLogicUnit->m_afIndexHandler = (bool (*)(void*)) iFun;

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
