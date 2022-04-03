/*============================================================================*/
/* Steps_Vertex.cpp -- Vertex step group                                      */
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
void Step::vAddSteps_Vertex(void)
{
  vAddStep("Vertex[]"_Stz,                &Step::vVertexGet);
  vAddStep("Vertex[].set"_Stz,            &Step::vVertexSet);
  
  vAddStep("Vertex[].dump"_Stz,           &Step::vVertexDump);
}

/*----------------------------------------------------------------------------*/
/* Vertex[].dump -- dump this subpipeline only                                */
/*----------------------------------------------------------------------------*/
void Step::vVertexDump(void)
{
  ENTRY();
      
  SP_Pipeline spSubPipe;
  
  // vTrace("%s\n"_Stz,spSubPipe.sz(* m_pLogicUnit->m_xpVertex,true));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Vertex[].get -- get subpipeline                                            */
/*----------------------------------------------------------------------------*/
void Step::vVertexGet(void)
{
  ENTRY();
   
  if (m_pLogicUnit->m_xpVertex.bIsNull())
    m_pDataStack->vPushPtr(nullptr);
  else
    m_pDataStack->vPushPtr(* m_pLogicUnit->m_xpVertex);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Vertex[].set -- set subpipeline                                            */
/*----------------------------------------------------------------------------*/
void Step::vVertexSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpVertex =
    _pVertex(m_pDataStack->pPopPtr(__func__));

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
