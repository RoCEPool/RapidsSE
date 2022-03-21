/*============================================================================*/
/* Steps_Stage.cpp -- Stage step group                                        */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

#include "Node_I.hpp"
#include "Op.hpp"
#include "Pipeline.hpp"
#include "StageSpec.hpp"

#include "Job.hpp"
#include "LogicUnit.hpp"
#include "Stream.hpp"
#include "Stage.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Stage(void)
{
  vAddStep("Stage[]"_Stz,     &Step::vStageGet);
  vAddStep("Stage[].get"_Stz, &Step::vStageGet);
  vAddStep("Stage[].set"_Stz, &Step::vStageSet);
}

/*----------------------------------------------------------------------------*/
/* Stage[].get -- get stage                                                   */
/*----------------------------------------------------------------------------*/
void Step::vStageGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(* m_pLogicUnit->m_xpStage);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Stage[].set -- set stage                                                   */
/*----------------------------------------------------------------------------*/
void Step::vStageSet(void)
{
  ENTRY();

  Stage* pStage = _pStage(m_pDataStack->pPopPtr(__func__));

  m_pLogicUnit->m_xpStage = pStage;

  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
