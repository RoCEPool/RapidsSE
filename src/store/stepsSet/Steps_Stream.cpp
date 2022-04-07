/*============================================================================*/
/* Steps_Stream.cpp -- Stream step group                                      */
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
void Step::vAddSteps_Stream(void)
{
  vAddStep("Stream[]"_Stz,     &Step::vStreamGet);
  vAddStep("Stream[].get"_Stz, &Step::vStreamGet);
  vAddStep("Stream[].set"_Stz, &Step::vStreamSet);
}

/*----------------------------------------------------------------------------*/
/* Stream[].get -- get stream                                                 */
/*----------------------------------------------------------------------------*/
void Step::vStreamGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(* m_pLogicUnit->m_xpStream);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Stream[].set -- set stream                                                 */
/*----------------------------------------------------------------------------*/
void Step::vStreamSet(void)
{
  ENTRY();

  Stream* pStream = _pStream(m_pDataStack->pPopPtr(__func__));

  m_pLogicUnit->m_xpStream = pStream;

  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
