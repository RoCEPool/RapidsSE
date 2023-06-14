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
  vAddStep("[Job.Cmd].print"_Stz,  &Step::vCmdPrint);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
