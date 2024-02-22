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

  vAddStep("IndexHandler[]"_Stz,           &Step::vIndexHandlerGet);
  vAddStep("IndexHandler[].get"_Stz,       &Step::vIndexHandlerGet);
  vAddStep("IndexHandler[].isNotNull"_Stz, &Step::vIndexHandlerIsNotNull);
  vAddStep("IndexHandler[].set"_Stz,       &Step::vIndexHandlerSet);

  vAddStep("EosHandler[]"_Stz,      &Step::vEosHandlerGet);
  vAddStep("EosHandler[].get"_Stz,  &Step::vEosHandlerGet);
  vAddStep("EosHandler[].set"_Stz,  &Step::vEosHandlerSet);

  vAddStep("StreamDefHandler[]"_Stz,      &Step::vStreamDefHandlerGet);
  vAddStep("StreamDefHandler[].get"_Stz,  &Step::vStreamDefHandlerGet);
  vAddStep("StreamDefHandler[].set"_Stz,  &Step::vStreamDefHandlerSet);
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
/* EosHandler[].get -- get EosHandler                                         */
/*----------------------------------------------------------------------------*/
void Step::vEosHandlerGet(void)
{
  ENTRY();

  m_pDataStack->vPushInt((i8)* m_pLogicUnit->m_afEosHandler);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* EosHandler[].set -- set EosHandler                                         */
/*----------------------------------------------------------------------------*/
void Step::vEosHandlerSet(void)
{
  ENTRY();

  i8 iFun = m_pDataStack->iPopInt(__func__);

  m_pLogicUnit->m_afEosHandler = (bool (*)(void*)) iFun;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefHandler[].get -- get StreamDefHandler                             */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefHandlerGet(void)
{
  ENTRY();

  m_pDataStack->vPushInt((i8)* m_pLogicUnit->m_afStreamDefHandler);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefHandler[].set -- set StreamDefHandler                              */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefHandlerSet(void)
{
  ENTRY();

  i8 iFun = m_pDataStack->iPopInt(__func__);

  m_pLogicUnit->m_afStreamDefHandler = (bool (*)(void*)) iFun;

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
