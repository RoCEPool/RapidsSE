/*============================================================================*/
/* Steps_StreamOfOp.cpp -- Stream (of op) step group                          */
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
void Step::vAddSteps_StreamOfOp(void)
{
  vAddStep("[stream]"_Stz,                &Step::vStreamOfOpGet);

  vAddStep("[stream].dump"_Stz,           &Step::vStreamOfOpDump);
  vAddStep("[stream].findOp"_Stz,         &Step::vStreamOfOpFindOp);
}

/*----------------------------------------------------------------------------*/
/* [stream].dump -- dump this stream only                                     */
/*----------------------------------------------------------------------------*/
void Step::vStreamOfOpDump(void)
{
  ENTRY();
      
  Stream* pStream = m_pOp->pStream();
  
  pStream->vDump();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [stream].findOp -- find op by name                                         */
/*----------------------------------------------------------------------------*/
void Step::vStreamOfOpFindOp(void)
{
  ENTRY();
   
  s256 sOpNm;
  m_pDataStack->vPopStr(sOpNm,__func__);
  
  Stream* pStream = m_pOp->pStream();
  
  Op* pOp = pStream->pFindOp(sOpNm);
  
  m_pDataStack->vPushPtr(pOp);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [stream].get -- get current stream                                         */
/*----------------------------------------------------------------------------*/
void Step::vStreamOfOpGet(void)
{
  ENTRY();
   
  Stream* pStream = m_pOp->pStream();
  
  m_pDataStack->vPushPtr(pStream);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
