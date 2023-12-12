/*============================================================================*/
/* JobReport.cpp -- JobReport                                                 */
/*============================================================================*/

#include "BASE.h"
#include "JobReport.hpp"
#include "JobList.hpp"
#include "Job.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "RowOnce.hpp"
#include "IndexStream.hpp"
#include "Table.hpp"
#include "Pipeline.hpp"
#include "Vertex.hpp"
#include "Stage.hpp"
#include "Deck.hpp"
#include "LogicUnit.hpp"
#include "Stream.hpp"
#include "Op.hpp"


#include "Node.hpp"
#include "Node_I.hpp"
#include "Grid.hpp"
#include "Socket.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,JobReport,CN_JOBREPORT)
__aNewFunction(JobReport,CN_JOBREPORT)
__vDeleteFunction(JobReport,CN_JOBREPORT)
__aFooterFunction(JobReport)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobReport::JobReport(i8 iJobNo, Job* pJob)
: Object(CN_JOBREPORT)
{
  _precondition_(_MAGIC_,CN_JOBREPORT,_FOOTER_);

  m_iJobNo   = iJobNo;
  m_tJobId   = C_UNKNOWN;
  m_pJob     = pJob;
  m_pJobTags = nullptr;

  m_i_pu_test   = C_UNKNOWN;
  m_i_pu_subset = C_UNKNOWN;
  m_i_pu_t      = C_UNKNOWN;
  m_i_pu_ft     = C_UNKNOWN;
  m_i_pu_j      = C_UNKNOWN;
  m_i_pu_fj     = C_UNKNOWN;
  m_i_pu_groups = C_UNKNOWN;

  m_iReponseTime = C_UNKNOWN;
  m_iMagnitude   = C_UNKNOWN;
  
  _checkInitialised_(CN_JOBREPORT);

  _postcondition_(_MAGIC_,CN_JOBREPORT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
JobReport::~JobReport(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_JOBREPORT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* all report                                                                 */
/*----------------------------------------------------------------------------*/
eEC JobReport::eAllReport(i8 iJob, Tub* pTub)
{
  _static_
  UNUSED(iJob);
  UNUSED(pTub);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* metrics report                                                             */
/*----------------------------------------------------------------------------*/
eEC JobReport::eMetricsReport(Job* pJob, Tub* pTub)
{
  _static_
  
  // find the participant job
  
//  Job* pJob = pFindParticipantJobNo(iJob);
//
//  if ( ! pJob ) {
//    JobId tJobId;
//    tJobId = iJob;
//    pJob = pFindParticipantJobId(tJobId);
//  }
//
//  if (! pJob) {
//    vTrace("Did not jind job %lld\n"_Stz,iJob);
//    return EC_NONE;
//  }
  
  //vTrace("JobReport::eMetricsReport job =%p\n"_Stz,pJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the internal jobMetrics table
  
  Table* pJobMetricsTable = AM::pTableFind(C_TDEF_JOBMETRICS);
  StreamDef* pStreamDef = pJobMetricsTable->pStreamDef();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get a working buffer from the tub
  
  RowBuffer* pRowBuffer = _pRowBuffer(pTub->pWorkingBuffer());

  if (! pRowBuffer) {
    // new a named buffer
		pRowBuffer = new RowBuffer(pStreamDef, BT_JOBMETRICS);

    // add the named buffer to the tub
    pTub->vAddBuffer(pRowBuffer);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Node* pThisNode = MY::pNode();

  for (JobMetrics* pJobMetrics = pJob->pJobMetricsChain();
       pJobMetrics;
       pJobMetrics = pJobMetrics->pNext()) {
    
    i8 d_MetricsCursor = C_UNKNOWN;

    PipelineMetrics* pPipelineMetrics
                       = pJobMetrics->pGetPipelineMetrics(d_MetricsCursor);
    
    while (CN_STAGEMETRICS == pJobMetrics->eNextClassNo(d_MetricsCursor)) {
      StageMetrics* pStageMetrics
                      = pJobMetrics->pGetStageMetrics(d_MetricsCursor);

      while (CN_STREAMMETRICS == pJobMetrics->eNextClassNo(d_MetricsCursor)) {
        StreamMetrics* pStreamMetrics
                         = pJobMetrics->pGetStreamMetrics(d_MetricsCursor);

        while (CN_OPMETRICS == pJobMetrics->eNextClassNo(d_MetricsCursor)) {
          OpMetrics* pOpMetrics
                       = pJobMetrics->pGetOpMetrics(d_MetricsCursor);

          L_RETRY:
          Row* pRow = pRowBuffer->pAllotRow
                      (
                        C_VARGS_BEGIN
                       
                       ,pJobMetrics->tJobId().uJobNo()

                       ,pThisNode->iNodeId()
                       ,pThisNode->szNodeNm()

                       ,pPipelineMetrics->szPipelineNm()
                       ,pStageMetrics->i_Stage()
                       ,pStageMetrics->szStageNm()
                       ,pStreamMetrics->i_Stream()
                       ,pOpMetrics->szOpNm()
                       
                       ,pJobMetrics->nRefTimer()
                       ,pJobMetrics->nMilliseconds()
                       ,pJobMetrics->nThreadsIdle()
                       ,pJobMetrics->iPctFree()

                       ,pOpMetrics->nPotSize()
                       ,pOpMetrics->nTmpIndexSize()
                       ,pOpMetrics->nQueued()

                       ,C_VARGS_END
                      );
          
          if (! pRow) {
            pRowBuffer = new RowBuffer(pStreamDef, BT_JOBMETRICS);

            // add the named buffer to the tub
            pTub->vAddBuffer(pRowBuffer);
            goto L_RETRY;
          }

        } // while opMetrics
        
      } // while streamMetrics

    } // stageMetrics

  } // for jobMetrics
       
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  return EC_NONE;

}

/*----------------------------------------------------------------------------*/
/* network dissect report                                                     */
/*----------------------------------------------------------------------------*/
eEC JobReport::eNetDissectReport(Job* pJob, Tub* pTub)
{
  _static_
  
  
  // find the participant job
//  Job* pJob = pFindParticipantJobNo(iJob);
//
//  if ( ! pJob ) {
//    JobId tJobId;
//    tJobId = iJob;
//    pJob = pFindParticipantJobId(tJobId);
//  }
//
  // find the internal netDissect table
  Table* pJobNetDissectTable = AM::pTableFind(C_TDEF_JOBNETDISSECT);
  StreamDef* pStreamDef = pJobNetDissectTable->pStreamDef();

  // get a working buffer from the tub
	RowBuffer* pRowBuffer = _pRowBuffer(pTub->pWorkingBuffer());

  if (! pRowBuffer) {
    // new a row buffer
    pRowBuffer = new RowBuffer(pStreamDef, BT_JOBNETDISSECT);

    // add the row buffer to the tub
    pTub->vAddBuffer(pRowBuffer);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Node*   pNode;
  Socket* pSndSkt;
  Socket* pRcvSkt;
  Totup*  pTotup;
  Totup*  pWhyTot;

  Node*   pThisNode = MY::pNode();

  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {
    pNode = (*oNode_I);
    pSndSkt =  pNode->pSendSocket();
    pRcvSkt =  pNode->pRecvSocket();

    // -- -- -- -- -- -- -- -- -- -- -- --
    // sndskt
    
    pTotup = pSndSkt->pTotup();
    for (i8 i_Item = 0; i_Item < CN_ENUMS; i_Item++) {
      if (! pTotup->nCnt(i_Item))
        continue;
      
      L_RETRY_1a:
      Row* pRow = pRowBuffer->pAllotRow
                  (
                    C_VARGS_BEGIN
                   
                   ,pJob->tJobId().uJobNo()

                   ,pThisNode->iNodeId()
                   ,pThisNode->szNodeNm()

                   ,pNode->iNodeId()
                   ,pNode->szNodeNm()
                   
                   ,"sndSkt"_Stz
                   ,aCN[i_Item]

                   ,pTotup->nCnt(i_Item)
                   ,pTotup->iBytes(i_Item)

                   ,C_VARGS_END
                  );
      
      if (! pRow) {
				pRowBuffer = new RowBuffer(pStreamDef, BT_JOBNETDISSECT);

        // add the row buffer to the tub
        pTub->vAddBuffer(pRowBuffer);
        goto L_RETRY_1a;
      }
    }
    
    
    pWhyTot = pSndSkt->pWhyTot();
    for (i8 i_Item = 0; i_Item < SY_ENUMS; i_Item++) {
      if (! pWhyTot->nCnt(i_Item))
        continue;
      
      L_RETRY_1b:
      Row* pRow = pRowBuffer->pAllotRow
                  (
                    C_VARGS_BEGIN
                   
                   ,pJob->tJobId().uJobNo()

                   ,pThisNode->iNodeId()
                   ,pThisNode->szNodeNm()

                   ,pNode->iNodeId()
                   ,pNode->szNodeNm()
                   
                   ,"sndSkt"_Stz
                   ,aSY[i_Item]

                   ,pWhyTot->nCnt(i_Item)
                   ,pWhyTot->iBytes(i_Item)

                   ,C_VARGS_END
                  );
      
      if (! pRow) {
				pRowBuffer = new RowBuffer(pStreamDef, BT_JOBNETDISSECT);

        // add the row buffer to the tub
        pTub->vAddBuffer(pRowBuffer);
        goto L_RETRY_1b;
      }

    }
    
    // -- -- -- -- -- -- -- -- -- -- -- --
    // rcvskt
    
    pTotup = pRcvSkt->pTotup();
    for (i8 i_Item = 0; i_Item < CN_ENUMS; i_Item++) {
      if (! pTotup->nCnt(i_Item))
        continue;
      
      L_RETRY_2a:
      Row* pRow = pRowBuffer->pAllotRow
                  (
                    C_VARGS_BEGIN
                   
                   ,pJob->tJobId().uJobNo()

                   ,pThisNode->iNodeId()
                   ,pThisNode->szNodeNm()

                   ,pNode->iNodeId()
                   ,pNode->szNodeNm()
                   
                   ,"rcvSkt"_Stz
                   ,aCN[i_Item]

                   ,pTotup->nCnt(i_Item)
                   ,pTotup->iBytes(i_Item)

                   ,C_VARGS_END
                  );
      
      if (! pRow) {
				pRowBuffer = new RowBuffer(pStreamDef, BT_JOBNETDISSECT);

        // add the named buffer to the tub
        pTub->vAddBuffer(pRowBuffer);
        goto L_RETRY_2a;
      }
    }
    
    
    pWhyTot = pRcvSkt->pWhyTot();
    for (i8 i_Item = 0; i_Item < SY_ENUMS; i_Item++) {
      if (! pWhyTot->nCnt(i_Item))
        continue;
      
      L_RETRY_2b:
      Row* pRow = pRowBuffer->pAllotRow
                  (
                    C_VARGS_BEGIN
                   
                   ,pJob->tJobId().uJobNo()

                   ,pThisNode->iNodeId()
                   ,pThisNode->szNodeNm()

                   ,pNode->iNodeId()
                   ,pNode->szNodeNm()
                   
                   ,"rcvSkt"_Stz
                   ,aSY[i_Item]

                   ,pWhyTot->nCnt(i_Item)
                   ,pWhyTot->iBytes(i_Item)

                   ,C_VARGS_END
                  );
      
      if (! pRow) {
				pRowBuffer = new RowBuffer(pStreamDef, BT_JOBNETDISSECT);

        // add the named buffer to the tub
        pTub->vAddBuffer(pRowBuffer);
        goto L_RETRY_2b;
      }

    }

  }

  
  
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* network report                                                             */
/*----------------------------------------------------------------------------*/
eEC JobReport::eNetworkReport(Job* pJob, Tub* pTub)
{
  _static_
  
  // find the participant job
//  Job* pJob = pFindParticipantJobNo(iJob);
//
//  if ( ! pJob ) {
//    JobId tJobId;
//    tJobId = iJob;
//    pJob = pFindParticipantJobId(tJobId);
//  }
  
  // find the internal jobNetwork table
  Table* pJobNetworkTable = AM::pTableFind(C_TDEF_JOBNETWORK);
  StreamDef* pStreamDef = pJobNetworkTable->pStreamDef();

  // get a working buffer from the tub
	RowBuffer* pRowBuffer = _pRowBuffer(pTub->pWorkingBuffer());

  if (! pRowBuffer) {
    // new a row buffer
		pRowBuffer = new RowBuffer(pStreamDef, BT_JOBNETWORK);

    // add the row buffer to the tub
    pTub->vAddBuffer(pRowBuffer);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Node*   pNode;
  Socket* pSndSkt;
  Socket* pRcvSkt;

  Node*   pThisNode = MY::pNode();

  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {
    pNode = (*oNode_I);
    pSndSkt =  pNode->pSendSocket();
    pRcvSkt =  pNode->pRecvSocket();

    L_HACK:
    Row* pRow = pRowBuffer->pAllotRow
                (
                  C_VARGS_BEGIN
                 
                 ,pJob->tJobId().uJobNo()

                 ,pThisNode->iNodeId()
                 ,pThisNode->szNodeNm()

                 ,pNode->iNodeId()
                 ,pNode->szNodeNm()
                 ,pNode->szIPAddr()
                 ,pNode->iPortNo()
                 
                 ,pSndSkt->nSends()
                 ,pSndSkt->nBytesSent()
                 ,pSndSkt->iSendBufferSize()

                 ,pRcvSkt->nRecvs()
                 ,pRcvSkt->nBytesRcvd()
                 ,pRcvSkt->iRecvBufferSize()

                 ,C_VARGS_END
                );
    
    if (! pRow) {
			pRowBuffer = new RowBuffer(pStreamDef, BT_JOBNETWORK);

      // add the named buffer to the tub
      pTub->vAddBuffer(pRowBuffer);
      goto L_HACK;
    }

  }
  

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* operand report                                                             */
/*----------------------------------------------------------------------------*/
eEC JobReport::eOperandReport(Job* pJob, Tub* pTub)
{
  _static_

  // find the participant job
//  Job* pJob = pFindParticipantJobNo(iJob);
//
//  if ( ! pJob ) {
//    JobId tJobId;
//    tJobId = iJob;
//    pJob = pFindParticipantJobId(tJobId);
//  }

  i8 iParticipantTsc = pJob->iTscParticipant();
  
  // find the internal jobOpererands table
  Table* pJobOperandsTable = AM::pTableFind(C_TDEF_JOBOPERANDS);
  StreamDef* pStreamDef = pJobOperandsTable->pStreamDef();

  // get a working buffer from the tub
	RowBuffer* pRowBuffer = _pRowBuffer(pTub->pWorkingBuffer());

  if (! pRowBuffer) {
    // new a named buffer
		pRowBuffer = new RowBuffer(pStreamDef, BT_JOBOPERANDS);

    // add the named buffer to the tub
    pTub->vAddBuffer(pRowBuffer);
  }

  Node*   pThisNode = MY::pNode();

  // get the pipeline
//  Task* pTask = pJob->pTask();
//  LogicUnit* pLogicUnit = pTask->pLogicUnit();
//  Pipeline* pPipeline = (* pLogicUnit->m_xpPipeline);

  Pipeline* pPipeline = pJob->pMetricsPipeline();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // each stage in the pipeline
  Vertex* pPipeWork = pPipeline->pVertexRoot();
  for (i8 i_Stage = 0; i_Stage < pPipeWork->pDeck()->nItems(); i_Stage++) {
    Stage* pStage = pPipeWork->pDeck()->pGet(i_Stage);

    // each stream in the stage
    for (i8 i_Stream = 0; i_Stream < pStage->nItems(); i_Stream++) {
      Stream* pStream = pStage->pGet(i_Stream);

      // each op in each stream
    //for (i8 i_Op = pStream->nItems() - 1; i_Op >= 0; i_Op--) {
      for (i8 i_Op = 0; i_Op < pStream->nItems(); i_Op++) {
        Op* pOp = pStream->pGet(i_Op);
        
        OperandWatch* pOperandWatch = pOp->pOperandWatch();

        //if ( ! pOperandWatch )
        //  continue;

        for (i8 i_Operand = 0; i_Operand < EV_ENUMS; i_Operand++) {

          if (! pOperandWatch->nCnt(i_Operand))
            continue;
          
          L_HACK:
          Row* pRow = pRowBuffer->pAllotRow
                      (
                        C_VARGS_BEGIN
                       
                       ,pJob->tJobId().uJobNo()
                       
                       ,pThisNode->iNodeId()
                       ,pThisNode->szNodeNm()
                       
                       ,pOp->pPipeline()->szPipelineNm()
                       ,pOp->pStage()->i_Stage()
                       ,pOp->szStageName()
                       ,pOp->tLglPtn().iAsI8()
                       ,pOp->szOpNm()

                       ,i_Operand
                       ,aEV[i_Operand]
                       
                       ,pOperandWatch->nCnt(i_Operand)
                       
                       ,pOperandWatch->iFirstUs(i_Operand,iParticipantTsc)
                       ,pOperandWatch->iLastUs(i_Operand,iParticipantTsc)

                       ,pOperandWatch->iFirstUs(i_Operand)
                       ,pOperandWatch->iLastUs(i_Operand)
                       
                       ,C_VARGS_END
                      );
          
          if (! pRow) {
            pRowBuffer = new RowBuffer(pStreamDef, BT_JOBOPERANDS);

            // add the named buffer to the tub
            pTub->vAddBuffer(pRowBuffer);
            goto L_HACK;
          }

        } // operand
      } // op
    } // stream
  } // stage
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* operator report                                                            */
/*----------------------------------------------------------------------------*/
eEC JobReport::eOperatorReport(Job* pJob, Tub* pTub)
{
  _static_

  // find the participant job
//  Job* pJob = pFindParticipantJobNo(iJob);
//
//  if ( ! pJob ) {
//    JobId tJobId;
//    tJobId = iJob;
//    pJob = pFindParticipantJobId(tJobId);
//  }
  
//  // no such job ?
//  if (! pJob)
//    return EC_NO_SUCH_JOB;

  // create the jobReport
  //JobReport* pJobReport = new JobReport(iJob, pJob);

  // find the internal jobOperators table
  Table* pJobOperatorsTable = AM::pTableFind(C_TDEF_JOBOPERATORS);
  StreamDef* pStreamDef = pJobOperatorsTable->pStreamDef();

  // get a working buffer from the tub
  RowBuffer* pRowBuffer = _pRowBuffer(pTub->pWorkingBuffer());

  if (! pRowBuffer) {
    // new a row buffer
		pRowBuffer = new RowBuffer(pStreamDef, BT_JOBOPERATORS);

    // add the named buffer to the tub
    pTub->vAddBuffer(pRowBuffer);
  }
  
  Node*   pThisNode = MY::pNode();

//  Task* pTask = pJob->pTask();
//  LogicUnit* pLogicUnit = pTask->pLogicUnit();
//
//  // get the pipeline
//  Pipeline* pPipeline = (* pLogicUnit->m_xpPipeline);
   Pipeline* pPipeline = pJob->pMetricsPipeline();

  // each stage in the pipeline
  Vertex* pPipeWork = pPipeline->pVertexRoot();
  for (i8 i_Stage = 0; i_Stage < pPipeWork->pDeck()->nItems(); i_Stage++) {

    Stage* pStage = pPipeWork->pDeck()->pGet(i_Stage);

    // each stream in the stage
    for (i8 i_Stream = 0; i_Stream < pStage->nItems(); i_Stream++) {
      Stream* pStream = pStage->pGet(i_Stream);

      // each op in each stream
    //for (i8 i_Op = pStream->nItems() - 1; i_Op >= 0; i_Op--) {
      for (i8 i_Op = 0; i_Op < pStream->nItems(); i_Op++) {

        // get the stats for the op
        Op* pOp = pStream->pGet(i_Op);

        // get the OP metrics stats
        Stats* pStats = pOp->pMetricsStats();

        // skip operators that don't have a metrics stats
        if ( ! pStats )
          continue;

        Thread* pThread = Thread::pThisTLSThread();

        pThread->pPsxThd()->vSleepMsec(100,CN_WORD);


        s32 sBytesAlloc;
        MagLib::vByteMag(pStats->m_nBytesAlloc, sBytesAlloc);

        s32 sBytesFree;
        MagLib::vByteMag(pStats->m_nFrees, sBytesFree);

        s32 sMaxBytes;
        MagLib::vByteMag(pStats->m_nMaxBytes, sMaxBytes);

        s32 sBytesRead;
        MagLib::vByteMag(pStats->m_nBytesRead, sBytesRead);

        s32 sBytesWrite;
        MagLib::vByteMag(pStats->m_nBytesWrite, sBytesWrite);

        // is their space in the buffer ?
        if (pRowBuffer->nFreeBytes() < pRowBuffer->nRowProperBytes() + 1) {

          // new a row buffer
					pRowBuffer = new RowBuffer(pStreamDef, BT_JOBOPERATORS);

          // add the row buffer to the tub
          pTub->vAddBuffer(pRowBuffer);
        }

        // allot a row for the stats in the named buffer
        Row* pRow = pRowBuffer->pAllotRow
                    (
                      C_VARGS_BEGIN
                                            
                     ,pJob->tJobId().uJobNo()
                      
                     ,pThisNode->iNodeId()
                     ,pThisNode->szNodeNm()
                     
                     ,pOp->pPipeline()->szPipelineNm()
                     ,pOp->pStage()->i_Stage()
                     ,pOp->szStageName()
                     ,pOp->tLglPtn().iAsI8()
                     ,pOp->szOpNm()
                     ,pStats->iMicroseconds()
                     ,pStats->m_nMaxQs
                     ,pStats->m_nOperands
                     ,pStats->m_nBuffIn
                     ,pStats->m_nBuffOut
                     ,pStats->m_nNewRowBuffer
                     ,pStats->m_nDelRowBuffer
                     ,pStats->m_nNewCsvBuffer

                    ,pStats->m_nDelCsvBuffer
                    ,pStats->m_nAllocs
                    ,sBytesAlloc.azChar()
                    ,pStats->m_nFrees
                    ,sBytesFree.azChar()
                   /*
                   ,sMaxBytes.azChar()
                   ,pStats->m_nReads
                   ,sBytesRead.azChar()
                   ,pStats->m_nWrites
                   ,sBytesWrite.azChar()
                   ,pStats->m_nParcelSends
                   ,pStats->m_nObjectsSent
                   ,pStats->m_nParcelRecvs
                   ,pStats->m_nObjectsRecv
                   */
                     ,C_VARGS_END
                    );
        
        RASSERT(pRow);
      }
    }
  }

  // delete the jobReport
  //delete pJobReport;

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* find the job on the origin                                                 */
/*----------------------------------------------------------------------------*/
Job* JobReport::pFindOriginalJobId(JobId tJobId)
{
  _static_
  
  Job* pJob = nullptr;
  JobList* pExitedJobs = AM::pExitedJobs();

  pExitedJobs->vGrabLock();
  
  for (SkipNode* pScan = pExitedJobs->pFirst();
       pScan;
       pScan = pExitedJobs->pNext(pScan)) {
    Job* pJobScan = _pJob(pScan->m_pObject);
    
    JobId tJobNo_test = pJobScan->tJobId();
//    vTrace("Scan %p %lld (%s)\n"_Stz
//           ,pJobScan
//           ,iJobNo
//           ,C_UNKNOWN == pJobScan->iParticipantNode()
//             ? "origin"_Stz
//             : "participant"_Stz
//           );
    
    if (tJobNo_test == tJobId && C_UNKNOWN == pJobScan->iDistNodeId()) {
      pJob = pJobScan;
    }
  }

  pExitedJobs->vDropLock();

  if (pJob) {
//    vTrace("Found %p %lld (%s)\n"_Stz
//           ,m_pJob
//           ,(i8)m_pJob->tJobId().uJobNo()
//           ,C_UNKNOWN == m_pJob->iParticipantNode()
//             ? "origin"_Stz
//             : "participant"_Stz
//           );
  }

  return pJob;

}

/*----------------------------------------------------------------------------*/
/* find the job on the origin                                                 */
/*----------------------------------------------------------------------------*/
Job* JobReport::pFindOriginalJobNo(i8 iJobNo)
{
  _static_

  //vTrace("bFindParts: %lld\n"_Stz,m_iJobNo);
  
  
  // form jobId from NodeId.m_jobNo
  
  Job* pJob = nullptr;
  JobList* pExitedJobs = AM::pExitedJobs();

  pExitedJobs->vGrabLock();
  
  for (SkipNode* pScan = pExitedJobs->pFirst();
       pScan;
       pScan = pExitedJobs->pNext(pScan)) {
    Job* pJobScan = _pJob(pScan->m_pObject);
    
    i8 iJobNo_test = (i8)pJobScan->tJobId().uJobNo();
//    vTrace("Scan %p %lld (%s)\n"_Stz
//           ,pJobScan
//           ,iJobNo
//           ,C_UNKNOWN == pJobScan->iParticipantNode()
//             ? "origin"_Stz
//             : "participant"_Stz
//           );
    
    if (iJobNo_test == iJobNo && C_UNKNOWN == pJobScan->iDistNodeId()) {
      pJob = pJobScan;
    }
  }

  pExitedJobs->vDropLock();

  if (pJob) {
//    vTrace("Found %p %lld (%s)\n"_Stz
//           ,m_pJob
//           ,(i8)m_pJob->tJobId().uJobNo()
//           ,C_UNKNOWN == m_pJob->iParticipantNode()
//             ? "origin"_Stz
//             : "participant"_Stz
//           );
  }

  return pJob;
}

/*----------------------------------------------------------------------------*/
/* find the participant job on the origin                                     */
/*----------------------------------------------------------------------------*/
Job* JobReport::pFindParticipantJobId(JobId tJobId)
{
  _static_

  Job* pJob = nullptr;
  JobList* pExitedJobs = AM::pExitedJobs();

  pExitedJobs->vGrabLock();

  for (SkipNode* pScan = pExitedJobs->pFirst();
       pScan;
       pScan = pExitedJobs->pNext(pScan)) {
    Job* pJobScan = _pJob(pScan->m_pObject);

    JobId tJobId_test = pJobScan->tJobId();
//    vTrace("Scan %p (%s) tJobNo_test %lld  : yJobNo %lld\n"_Stz
//           ,pJobScan
//           ,C_UNKNOWN == pJobScan->iParticipantNode()
//             ? "origin"_Stz
//             : "participant"_Stz
//           ,tJobId_test.iAsI8()
//           ,tJobId.iAsI8()
//           );

    if (tJobId_test == tJobId && C_UNKNOWN != pJobScan->iDistNodeId()) {
      if (pJobScan->szJobNm()->bSame("pipeline"_Stz))
        pJob = pJobScan;
    }
  }

  pExitedJobs->vDropLock();

  if (pJob) {
//    vTrace("Found %p %lld (%s)\n"_Stz
//           ,m_pJob
//           ,(i8)m_pJob->tJobId().uJobNo()
//           ,C_UNKNOWN == m_pJob->iParticipantNode()
//             ? "origin"_Stz
//             : "participant"_Stz
//           );
  }

  return pJob;
}

/*----------------------------------------------------------------------------*/
/* find the participant job on the origin                                     */
/*----------------------------------------------------------------------------*/
Job* JobReport::pFindParticipantJobNo(i8 iJobNo)
{
  _static_

  Job* pJob = nullptr;
  JobList* pExitedJobs = AM::pExitedJobs();

  pExitedJobs->vGrabLock();

  for (SkipNode* pScan = pExitedJobs->pFirst();
       pScan;
       pScan = pExitedJobs->pNext(pScan)) {
    Job* pJobScan = _pJob(pScan->m_pObject);

    i8 iJobNo_test = (i8)pJobScan->tJobId().uJobNo();
//    vTrace("Scan %p (%s) iJobNo_test %lld  : iJobNo %lld\n"_Stz
//           ,pJobScan
//           ,C_UNKNOWN == pJobScan->iParticipantNode()
//             ? "origin"_Stz
//             : "participant"_Stz
//           ,iJobNo_test
//           ,iJobNo
//           );

    if (iJobNo_test == iJobNo && C_UNKNOWN != pJobScan->iDistNodeId()) {
      if (pJobScan->szJobNm()->bSame("pipeline"_Stz))
        pJob = pJobScan;
    }
  }

  pExitedJobs->vDropLock();

  if (pJob) {
//    vTrace("Found %p %lld (%s)\n"_Stz
//           ,m_pJob
//           ,(i8)m_pJob->tJobId().uJobNo()
//           ,C_UNKNOWN == m_pJob->iParticipantNode()
//             ? "origin"_Stz
//             : "participant"_Stz
//           );
  }

  return pJob;
}

/*----------------------------------------------------------------------------*/
/*-- Private ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
