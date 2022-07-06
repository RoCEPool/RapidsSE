/*============================================================================*/
/* Steps_NodeMonitor.cpp - Node Monitor step group                            */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "STORE.h"
#include "Grid.hpp"
#include "JobList.hpp"
#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_NodeMonitor(void)
{
  vAddStep("NodeMonitor::DummyCall"_Stz,     &Step::vNodeMonitorDummyCall);
  vAddStep("NodeMonitor::InspectGrid"_Stz,   &Step::vNodeMonitorInspectGrid);
}

/*----------------------------------------------------------------------------*/
/* NodeMonitor::DummyCall -- placeholder                                      */
/*----------------------------------------------------------------------------*/
void Step::vNodeMonitorDummyCall(void)
{
  ENTRY();
  vTrace("Test print\n"_Stz);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* NodeMonitor::InspectGrid - Check if nodes are alive                        */
/*----------------------------------------------------------------------------*/
void Step::vNodeMonitorInspectGrid(void)
{
  ENTRY();
  vTrace("Inspecting Grid...\n"_Stz);

  Grid* pGrid = AM::pGrid();

  SkipListNL* pNodes = pGrid->pSkipList();

  for (SkipNode *pSkipNode = pNodes->pFirst(0);
       pSkipNode;
       pSkipNode = pNodes->pNext(pSkipNode, 0)) {

    Node* pNode = _pNode(pSkipNode->m_pObject);
    vTrace("Inspecting Node %s\n"_Stz, pNode->szNodeNm());
    if(pNode->szNodeNm()->bSame("NODE2"_Stz))
    {
      pNode->vSetNodeStatus(5);
    }

    if (pNode && (pNode->iGetNodeStatus() != 0)) {

      vTrace("Node failure, status code: %d\n"_Stz, pNode->iGetNodeStatus());
      vTrace("Notifying jobs on current node:\n"_Stz);

      JobList* pJobList = AM::JLIST::pJobsList();
      pJobList->vGrabLock();

      for (SkipNode* pCursor = pJobList->pFirst();
           pCursor;
           pCursor = pJobList->pNext(pCursor)) {

        Job *pJob = _pJob(pCursor->m_pObject);

        vTrace("%s\n"_Stz, pJob->szJobNm());
      }
      pJobList->vDropLock();
      // Remove this node from node list to prevent triggering this again..
      // Or figure out a different way
      break;
    }
  }

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/