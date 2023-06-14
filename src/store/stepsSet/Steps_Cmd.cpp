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
/* Cmd[].fetch -- get cmd[]                                                   */
/*----------------------------------------------------------------------------*/
void Step::vCmdFetch(void)
{
  ENTRY();

  if (m_pJob) {
    TagSet* pTags = m_pJob->pJobTags();
    Stz* szValue = pTags->szValue(C_TAG_CMD);
    if (! szValue)
      THROW("Step::vCmdFetch : unknown tag\n"_Stz);
    m_pDataStack->vPushStz(szValue);
  }
  else {
    THROW("Step::vCmdFetch : unknown tag\n"_Stz);
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Cmd[].print -- print cmd[]                                                 */
/*----------------------------------------------------------------------------*/
void Step::vCmdPrint(void)
{
  ENTRY();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Cmd[].store -- set cmd[]                                                   */
/*----------------------------------------------------------------------------*/
void Step::vCmdStore(void)
{
  ENTRY();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Cmd[].update -- update cmd[]                                               */
/*----------------------------------------------------------------------------*/
void Step::vCmdUpdate(void)
{
  ENTRY();

  if (m_pJob) {
    TagSet* pTags = m_pJob->pJobTags();
    Stz* szValue = pTags->szValue(C_TAG_CMD);
    if (! szValue || ! szValue->nLength())
      THROW("Step::vCmdFetch : unknown tag\n"_Stz);

    m_pJob->vCmdText(szValue);
  }
  else {
    THROW("Step::vCmdFetch : unknown tag\n"_Stz);
  }

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
