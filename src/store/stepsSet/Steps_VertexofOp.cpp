/*============================================================================*/
/* Steps_VertexOfOp.cpp -- Vertex (of op) step group                          */
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
void Step::vAddSteps_VertexOfOp(void)
{
  vAddStep("[vertex]"_Stz,                &Step::vVertexOfOpGet);

  vAddStep("[vertex].dump"_Stz,           &Step::vVertexOfOpDump);
}

/*----------------------------------------------------------------------------*/
/* [vertex].dump -- dump this vertex only                                     */
/*----------------------------------------------------------------------------*/
void Step::vVertexOfOpDump(void)
{
  ENTRY();
      
  //SP_Pipeline spSubPipe;
  
  // vTrace("%s\n"_Stz,spSubPipe.sz(* m_pLogicUnit->m_xpVertex,true));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [vertex].get -- get current vertex                                         */
/*----------------------------------------------------------------------------*/
void Step::vVertexOfOpGet(void)
{
  ENTRY();
   
//  if (m_pLogicUnit->m_xpVertex.bIsNull())
//    m_pDataStack->vPushPtr(nullptr);
//  else
//    m_pDataStack->vPushPtr(* m_pLogicUnit->m_xpVertex);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
