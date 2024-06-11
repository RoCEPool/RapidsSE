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
  bool bGridHealthy = true;

  SkipListNL* pNodes = pGrid->pSkipList();
  SkipNode *pCursor;

  if ( ! pGrid->bHealthy() ) {
    vTrace("Inspecting Grid...\n"_Stz);

    for (SkipNode *pSkipNode = pNodes->pFirst(0);
         pSkipNode;
         pSkipNode = pNodes->pNext(pSkipNode, 0)) {

      Node *pNode = _pNode(pSkipNode->m_pObject);
      vTrace("Inspecting %s - %s\n"_Stz,
             pNode->szNodeNm(),
             pNode->eState() == NS_ACTIVE ? "running"_Stz : "not running"_Stz);

      if (pNode && (pNode->eState() == NS_INACTIVE)) {

         bGridHealthy = false;

       /**
        ** PARTICIPANT JOBS HANDLING TO BE CODED
        **
        **
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // Notify participant jobs

        JobList *pPtpJobList = AM::pParticipatingJobs();

        pPtpJobList->vGrabWriteLock();

        pCursor = pPtpJobList->pFirst();

        while (pCursor) {

          Job *pJob = _pJob(pCursor->m_pObject);

          //if (pJob->eJobError() != EC_NONE) {
          //  continue;
          //}

          // get the next job
          pCursor = pPtpJobList->pNext(pCursor);

          pPtpJobList->vDropWriteLock();

          JobId tJobId = pJob->tDistId();
          vTrace("**************Aborting Participating Job %04lld.%04lld ; %s\n"_Stz,
                 pJob->tDistId().uNodeId(),
                 pJob->tDistId().uJobNo(),
                 aJP[pJob->eJobPhase()]);

          AM::JOB::vPtpJobNodeFailure(tJobId);
          pPtpJobList->vGrabWriteLock();
        }
        pPtpJobList->vDropWriteLock();

      **/

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // Notify Originating jobs


        JobList *pOriginJobList = AM::pOriginatingJobs();

        pOriginJobList->vGrabLock();

        pCursor = pOriginJobList->pFirst();

        while (pCursor) {

          Job *pJob = _pJob(pCursor->m_pObject);

          // get the next job
          pCursor = pOriginJobList->pNext(pCursor);

          pOriginJobList->vDropLock();

          JobId tJobId = pJob->tOriginId();
          vTrace("**************Aborting Originating Job %04lld.%04lld ; %s\n"_Stz,
                 pJob->tOriginId().uNodeId(),
                 pJob->tOriginId().uJobNo(),
                 aJP[pJob->eJobPhase()]);

          AM::JOB::vOriginJobNodeFailure(tJobId);
          pOriginJobList->vGrabLock();
        }
        pOriginJobList->vDropLock();
      }
    }

    }

  pGrid->vDropRead();

  m_oDataStack.vPushInt(bGridHealthy);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/