/*============================================================================*/
/* Steps_Cmd.cpp - cmd step group                                             */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "LogicUnit.hpp"
#include "Job.hpp"
#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Cmd(void)
{
  vAddStep("[Job.Cmd]"_Stz,        &Step::vCmdFetch);
  vAddStep("[Job.Cmd].fetch"_Stz,  &Step::vCmdFetch);
  vAddStep("[Job.Cmd].print"_Stz,  &Step::vCmdPrint);
  vAddStep("[Job.Cmd].store"_Stz,  &Step::vCmdStore);
  vAddStep("[Job.Cmd].update"_Stz, &Step::vCmdUpdate);
}

/*----------------------------------------------------------------------------*/
/* [Job.Cmd].fetch -- get cmd[]                                               */
/*----------------------------------------------------------------------------*/
void Step::vCmdFetch(void)
{
  ENTRY();

  if (Stz* szValue = m_pJob->szCmdText()) {
    m_pDataStack->vPushStz(szValue);
  }
  else {
    THROW("Step::vCmdFetch : unknown cmd text\n"_Stz);
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [Job.Cmd].print -- print cmd[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vCmdPrint(void)
{
  ENTRY();

  if (m_pJob->szCmdText())
    vTrace("%s\n"_Stz,(m_pJob->szCmdText()));
  else
    vTrace("<NO CMD TEXT>\n"_Stz);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [Job.Cmd].store -- set cmd[]                                               */
/*----------------------------------------------------------------------------*/
void Step::vCmdStore(void)
{
  ENTRY();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [Job.Cmd].update -- update cmd[]                                           */
/*----------------------------------------------------------------------------*/
void Step::vCmdUpdate(void)
{
  ENTRY();

  s48K sCmdValue;
  m_pDataStack->vPopStr(sCmdValue,__func__);

  m_pJob->vCmdText(sCmdValue);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
