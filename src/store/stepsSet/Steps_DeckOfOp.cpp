/*============================================================================*/
/* Steps_DeckOfOp.cpp -- Deck (of op) step group                              */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

#include "Grid.hpp"
#include "Op.hpp"
#include "Stream.hpp"
#include "Stage.hpp"
#include "Vertex.hpp"
#include "Pipeline.hpp"
#include "VertexSpec.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

#include "Marker.hpp"

#include "SP_Pipeline.hpp"
#include "SP_PipeSpec.hpp"

#include "Dynamo.hpp"
#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_DeckOfOp(void)
{
  vAddStep("[deck]"_Stz,                &Step::vDeckOfOpGet);

  vAddStep("[deck].dump"_Stz,           &Step::vDeckOfOpDump);
}

/*----------------------------------------------------------------------------*/
/* [deck].dump -- dump this subpipeline only                                  */
/*----------------------------------------------------------------------------*/
void Step::vDeckOfOpDump(void)
{
  ENTRY();
      
  SP_Pipeline spSubPipe;
  
  // vTrace("%s\n"_Stz,spSubPipe.sz(* m_pLogicUnit->m_xpVertex,true));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [deck].get -- get subpipeline                                              */
/*----------------------------------------------------------------------------*/
void Step::vDeckOfOpGet(void)
{
  ENTRY();
   
  if (m_pLogicUnit->m_xpVertex.bIsNull())
    m_pDataStack->vPushPtr(nullptr);
  else
    m_pDataStack->vPushPtr(* m_pLogicUnit->m_xpVertex);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
