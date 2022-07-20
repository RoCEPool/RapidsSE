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

  Grid* pGrid = AM::pGrid();
  pGrid->vGrabRead();

  SkipListNL* pNodes = pGrid->pSkipList();

  if(0 == pGrid->bHealthy()) {
    vTrace("Inspecting Grid...\n"_Stz);

    for (SkipNode *pSkipNode = pNodes->pFirst(0);
         pSkipNode;
         pSkipNode = pNodes->pNext(pSkipNode, 0)) {

      Node *pNode = _pNode(pSkipNode->m_pObject);
      vTrace("Inspecting %s - %s\n"_Stz,
             pNode->szNodeNm(),
             pNode->eState() == NS_ACTIVE ? "running"_Stz : "not running"_Stz);

      if (pNode && (pNode->eState() == NS_INACTIVE)) {

        vTrace("Node failure, status code: %d\n"_Stz, pNode->iGetNodeStatus());
        vTrace("Notifying jobs on current node:\n"_Stz);


        // Notify Origin jobs
        JobList *pOriginJobList = AM::pOriginatingJobs();
        pOriginJobList->vGrabLock();

        for (SkipNode *pCursor = pOriginJobList->pFirst();
             pCursor;
             pCursor = pOriginJobList->pNext(pCursor)) {

          Job *pJob = _pJob(pCursor->m_pObject);

          vTrace("%s\n"_Stz, pJob->szJobNm());
        }
        pOriginJobList->vDropLock();


        // Notify participant jobs
        JobList* pPtpJobList = AM::pParticipatingJobs();
        pPtpJobList->vGrabLock();

        for (SkipNode *pCursor = pPtpJobList->pFirst();
             pCursor;
             pCursor = pPtpJobList->pNext(pCursor)) {

          Job *pJob = _pJob(pCursor->m_pObject);

          vTrace("%s\n"_Stz, pJob->szJobNm());
        }
        pPtpJobList->vDropLock();
        break;
      }
    }
  }

  pGrid->vDropRead();

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/