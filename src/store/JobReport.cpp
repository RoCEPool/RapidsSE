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
/* metrics report                                                             */
/*----------------------------------------------------------------------------*/
eEC JobReport::eMetricsReport(i8 iJob, Tub* pTub)
{
  _static_
  
  // find the participant job
  
  Job* pJob = pFindParticipantJobNo(iJob);

  if ( ! pJob ) {
    JobId tJobId;
    tJobId = iJob;
    pJob = pFindParticipantJobId(tJobId);
  }

  if (! pJob) {
    vTrace("Did not jind job %lld\n"_Stz,iJob);
    return EC_NONE;
  }
  
  //vTrace("JobReport::eMetricsReport job =%p\n"_Stz,pJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the internal jobMetrics table
  
  Table* pJobMetricsTable = AM::pTableFind(C_TDEF_JOBMETRICS);
  StreamDef* pStreamDef = pJobMetricsTable->pStreamDef();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get a working buffer from the tub
  
  NamedBuffer* pNamedBuffer = _pNamedBuffer(pTub->pWorkingBuffer());

  if (! pNamedBuffer) {
    // new a named buffer
    pNamedBuffer = new NamedBuffer(pStreamDef);

    // add the named buffer to the tub
    pTub->vAddBuffer(pNamedBuffer);
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
          Row* pRow = pNamedBuffer->pAllotRow
                      (
                        C_VARGS_BEGIN
                       
                       ,iJob

                       ,pThisNode->iNodeId()
                       ,pThisNode->szNodeNm()

                       ,pPipelineMetrics->szPipelineNm()
                       ,pStageMetrics->i_Stage()
                       ,pStageMetrics->szStageNm()
                       ,pStreamMetrics->i_Stream()
                       ,pOpMetrics->i_Op()
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
            pNamedBuffer = new NamedBuffer(pStreamDef);

            // add the named buffer to the tub
            pTub->vAddBuffer(pNamedBuffer);
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
eEC JobReport::eNetDissectReport(i8 iJob, Tub* pTub)
{
  _static_
  
  
  // find the participant job
  Job* pJob = pFindParticipantJobNo(iJob);

  if ( ! pJob ) {
    JobId tJobId;
    tJobId = iJob;
    pJob = pFindParticipantJobId(tJobId);
  }
  
  // find the internal jobOperators table
  Table* pJobNetDissectTable = AM::pTableFind(C_TDEF_JOBNETDISSECT);
  StreamDef* pStreamDef = pJobNetDissectTable->pStreamDef();

  // get a working buffer from the tub
  NamedBuffer* pNamedBuffer = _pNamedBuffer(pTub->pWorkingBuffer());

  if (! pNamedBuffer) {
    // new a named buffer
    pNamedBuffer = new NamedBuffer(pStreamDef);

    // add the named buffer to the tub
    pTub->vAddBuffer(pNamedBuffer);
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
      Row* pRow = pNamedBuffer->pAllotRow
                  (
                    C_VARGS_BEGIN
                   
                   ,iJob

                   ,pThisNode->iNodeId()
                   ,pThisNode->szNodeNm()

                   ,pNode->iNodeId()
                   ,pNode->szNodeNm()
                   
                   ,"sndSkt"_Stz
                   ,aCN[i_Item]

                   ,pTotup->nCnt(i_Item)
                   ,pTotup->iSum(i_Item)

                   ,C_VARGS_END
                  );
      
      if (! pRow) {
        pNamedBuffer = new NamedBuffer(pStreamDef);

        // add the named buffer to the tub
        pTub->vAddBuffer(pNamedBuffer);
        goto L_RETRY_1a;
      }
    }
    
    
    pWhyTot = pSndSkt->pWhyTot();
    for (i8 i_Item = 0; i_Item < SY_ENUMS; i_Item++) {
      if (! pWhyTot->nCnt(i_Item))
        continue;
      
      L_RETRY_1b:
      Row* pRow = pNamedBuffer->pAllotRow
                  (
                    C_VARGS_BEGIN
                   
                   ,iJob

                   ,pThisNode->iNodeId()
                   ,pThisNode->szNodeNm()

                   ,pNode->iNodeId()
                   ,pNode->szNodeNm()
                   
                   ,"sndSkt"_Stz
                   ,aSY[i_Item]

                   ,pWhyTot->nCnt(i_Item)
                   ,pWhyTot->iSum(i_Item)

                   ,C_VARGS_END
                  );
      
      if (! pRow) {
        pNamedBuffer = new NamedBuffer(pStreamDef);

        // add the named buffer to the tub
        pTub->vAddBuffer(pNamedBuffer);
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
      Row* pRow = pNamedBuffer->pAllotRow
                  (
                    C_VARGS_BEGIN
                   
                   ,iJob

                   ,pThisNode->iNodeId()
                   ,pThisNode->szNodeNm()

                   ,pNode->iNodeId()
                   ,pNode->szNodeNm()
                   
                   ,"rcvSkt"_Stz
                   ,aCN[i_Item]

                   ,pTotup->nCnt(i_Item)
                   ,pTotup->iSum(i_Item)

                   ,C_VARGS_END
                  );
      
      if (! pRow) {
        pNamedBuffer = new NamedBuffer(pStreamDef);

        // add the named buffer to the tub
        pTub->vAddBuffer(pNamedBuffer);
        goto L_RETRY_2a;
      }
    }
    
    
    pWhyTot = pRcvSkt->pWhyTot();
    for (i8 i_Item = 0; i_Item < SY_ENUMS; i_Item++) {
      if (! pWhyTot->nCnt(i_Item))
        continue;
      
      L_RETRY_2b:
      Row* pRow = pNamedBuffer->pAllotRow
                  (
                    C_VARGS_BEGIN
                   
                   ,iJob

                   ,pThisNode->iNodeId()
                   ,pThisNode->szNodeNm()

                   ,pNode->iNodeId()
                   ,pNode->szNodeNm()
                   
                   ,"rcvSkt"_Stz
                   ,aSY[i_Item]

                   ,pWhyTot->nCnt(i_Item)
                   ,pWhyTot->iSum(i_Item)

                   ,C_VARGS_END
                  );
      
      if (! pRow) {
        pNamedBuffer = new NamedBuffer(pStreamDef);

        // add the named buffer to the tub
        pTub->vAddBuffer(pNamedBuffer);
        goto L_RETRY_2b;
      }

    }

  }

  
  
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* network report                                                             */
/*----------------------------------------------------------------------------*/
eEC JobReport::eNetworkReport(i8 iJob, Tub* pTub)
{
  _static_
  
  // find the participant job
  Job* pJob = pFindParticipantJobNo(iJob);

  if ( ! pJob ) {
    JobId tJobId;
    tJobId = iJob;
    pJob = pFindParticipantJobId(tJobId);
  }
  
  // find the internal jobOperators table
  Table* pJobNetworkTable = AM::pTableFind(C_TDEF_JOBNETWORK);
  StreamDef* pStreamDef = pJobNetworkTable->pStreamDef();

  // get a working buffer from the tub
  NamedBuffer* pNamedBuffer = _pNamedBuffer(pTub->pWorkingBuffer());

  if (! pNamedBuffer) {
    // new a named buffer
    pNamedBuffer = new NamedBuffer(pStreamDef);

    // add the named buffer to the tub
    pTub->vAddBuffer(pNamedBuffer);
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
    Row* pRow = pNamedBuffer->pAllotRow
                (
                  C_VARGS_BEGIN
                 
                 ,iJob

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
      pNamedBuffer = new NamedBuffer(pStreamDef);

      // add the named buffer to the tub
      pTub->vAddBuffer(pNamedBuffer);
      goto L_HACK;
    }

  }
  

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* operand report                                                             */
/*----------------------------------------------------------------------------*/
eEC JobReport::eOperandReport(i8 iJob, Tub* pTub)
{
  _static_

  // find the participant job
  Job* pJob = pFindParticipantJobNo(iJob);

  if ( ! pJob ) {
    JobId tJobId;
    tJobId = iJob;
    pJob = pFindParticipantJobId(tJobId);
  }

  i8 iParticipantTsc = pJob->iTscParticipant();
  
  // find the internal jobOperators table
  Table* pJobOperandsTable = AM::pTableFind(C_TDEF_JOBOPERANDS);
  StreamDef* pStreamDef = pJobOperandsTable->pStreamDef();

  // get a working buffer from the tub
  NamedBuffer* pNamedBuffer = _pNamedBuffer(pTub->pWorkingBuffer());

  if (! pNamedBuffer) {
    // new a named buffer
    pNamedBuffer = new NamedBuffer(pStreamDef);

    // add the named buffer to the tub
    pTub->vAddBuffer(pNamedBuffer);
  }

  Node*   pThisNode = MY::pNode();

  // get the pipeline
  Task* pTask = pJob->pTask();
  LogicUnit* pLogicUnit = pTask->pLogicUnit();
  Pipeline* pPipeline = (* pLogicUnit->m_xpPipeline);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // each stage in the pipeline
  Vertex* pPipeWork = pPipeline->pVertexRoot();
  for (i8 i_Stage = 0; i_Stage < pPipeWork->nItems(); i_Stage++) {
    Stage* pStage = pPipeWork->pDeck()->pGet(i_Stage);

    // each stream in the stage
    for (i8 i_Stream = 0; i_Stream < pStage->nItems(); i_Stream++) {
      Stream* pStream = pStage->pGet(i_Stream);

      // each op in each stream
    //for (i8 i_Op = pStream->nItems() - 1; i_Op >= 0; i_Op--) {
      for (i8 i_Op = 0; i_Op < pStream->nItems(); i_Op++) {
        Op* pOp = pStream->pGet(i_Op);
        if ( ! pOp->pOpLogicUnit() )
          continue;
        
        OperandWatch* pOperandWatch = pOp->pOperandWatch();
        for (i8 i_Operand = 0; i_Operand < EV_ENUMS; i_Operand++) {

          if (! pOperandWatch->nCnt(i_Operand))
            continue;
          
          L_HACK:
          Row* pRow = pNamedBuffer->pAllotRow
                      (
                        C_VARGS_BEGIN
                       
                       ,iJob
                       
                       ,pThisNode->iNodeId()
                       ,pThisNode->szNodeNm()
                       
                       ,pOp->pPipeline()->szPipelineNm()
                       ,pOp->szStageName()
                       ,pOp->pStage()->i_Stage()
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
            pNamedBuffer = new NamedBuffer(pStreamDef);

            // add the named buffer to the tub
            pTub->vAddBuffer(pNamedBuffer);
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
eEC JobReport::eOperatorReport(i8 iJob, Tub* pTub)
{
  _static_

  // find the participant job
  Job* pJob = pFindParticipantJobNo(iJob);

  if ( ! pJob ) {
    JobId tJobId;
    tJobId = iJob;
    pJob = pFindParticipantJobId(tJobId);
  }
  
  // no such job ?
  if (! pJob)
    return EC_NO_SUCH_JOB;

  // create the jobReport
  JobReport* pJobReport = new JobReport(iJob, pJob);

  // find the internal jobOperators table
  Table* pJobOperatorsTable = AM::pTableFind(C_TDEF_JOBOPERATORS);
  StreamDef* pStreamDef = pJobOperatorsTable->pStreamDef();

  // get a working buffer from the tub
  NamedBuffer* pNamedBuffer = _pNamedBuffer(pTub->pWorkingBuffer());

  if (! pNamedBuffer) {
    // new a named buffer
    pNamedBuffer = new NamedBuffer(pStreamDef);

    // add the named buffer to the tub
    pTub->vAddBuffer(pNamedBuffer);
  }
  
  Node*   pThisNode = MY::pNode();

  Task* pTask = pJob->pTask();
  LogicUnit* pLogicUnit = pTask->pLogicUnit();

  // get the pipeline
  Pipeline* pPipeline = (* pLogicUnit->m_xpPipeline);

  // each stage in the pipeline
  Vertex* pPipeWork = pPipeline->pVertexRoot();
  for (i8 i_Stage = 0; i_Stage < pPipeWork->nItems(); i_Stage++) {

    Stage* pStage = pPipeWork->pDeck()->pGet(i_Stage);

    // each stream in the stage
    for (i8 i_Stream = 0; i_Stream < pStage->nItems(); i_Stream++) {
      Stream* pStream = pStage->pGet(i_Stream);

      // each op in each stream
    //for (i8 i_Op = pStream->nItems() - 1; i_Op >= 0; i_Op--) {
      for (i8 i_Op = 0; i_Op < pStream->nItems(); i_Op++) {

        // get the stats for the op
        Op* pOp = pStream->pGet(i_Op);

        // skip operators that don't have a LU
        if ( ! pOp->pOpLogicUnit() )
          continue;

        Stats* pStats = *pOp->pOpLogicUnit()->m_xpStats;

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
        if (pNamedBuffer->nFreeBytes() < pNamedBuffer->nRowProperBytes() + 1) {

          // new a named buffer
          pNamedBuffer = new NamedBuffer(pStreamDef);

          // add athe named buffer to the tub
          pTub->vAddBuffer(pNamedBuffer);
        }

        // allot a row for the stats in the named buffer
        Row* pRow = pNamedBuffer->pAllotRow
                    (
                      C_VARGS_BEGIN
                                            
                     ,iJob
                      
                     ,pThisNode->iNodeId()
                     ,pThisNode->szNodeNm()
                     
                     ,pOp->pPipeline()->szPipelineNm()
                     ,pOp->szStageName()
                     ,pOp->pStage()->i_Stage()
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
                     ,sMaxBytes.azChar()
                     ,pStats->m_nReads
                     ,sBytesRead.azChar()
                     ,pStats->m_nWrites
                     ,sBytesWrite.azChar()
                     ,pStats->m_nParcelSends
                     ,pStats->m_nObjectsSent
                     ,pStats->m_nParcelRecvs
                     ,pStats->m_nObjectsRecv
                     ,C_VARGS_END
                    );
        
        RASSERT(pRow);
      }
    }
  }

  // delete the jobReport
  delete pJobReport;

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* find the job on the origin                                                 */
/*----------------------------------------------------------------------------*/
Job* JobReport::pFindOriginalJobId(JobId tJobId)
{
  _static_
  
  Job* pJob = nullptr;
  JobList* pRetiredJobs = AM::pRetiredJobs();
  
  pRetiredJobs->vGrabLock();
  
  for (SkipNode* pScan = pRetiredJobs->pFirst();
       pScan;
       pScan = pRetiredJobs->pNext(pScan)) {
    Job* pJobScan = _pJob(pScan->m_pObject);
    
    JobId tJobNo_test = pJobScan->tJobId();
//    vTrace("Scan %p %lld (%s)\n"_Stz
//           ,pJobScan
//           ,iJobNo
//           ,C_UNKNOWN == pJobScan->iParticipantNode()
//             ? "origin"_Stz
//             : "participant"_Stz
//           );
    
    if (tJobNo_test == tJobId && C_UNKNOWN == pJobScan->iParticipantNodeId()) {
      pJob = pJobScan;
    }
  }

  pRetiredJobs->vDropLock();

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
  JobList* pRetiredJobs = AM::pRetiredJobs();
  
  pRetiredJobs->vGrabLock();
  
  for (SkipNode* pScan = pRetiredJobs->pFirst();
       pScan;
       pScan = pRetiredJobs->pNext(pScan)) {
    Job* pJobScan = _pJob(pScan->m_pObject);
    
    i8 iJobNo_test = (i8)pJobScan->tJobId().uJobNo();
//    vTrace("Scan %p %lld (%s)\n"_Stz
//           ,pJobScan
//           ,iJobNo
//           ,C_UNKNOWN == pJobScan->iParticipantNode()
//             ? "origin"_Stz
//             : "participant"_Stz
//           );
    
    if (iJobNo_test == iJobNo && C_UNKNOWN == pJobScan->iParticipantNodeId()) {
      pJob = pJobScan;
    }
  }

  pRetiredJobs->vDropLock();

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
  JobList* pRetiredJobs = AM::pRetiredJobs();

  pRetiredJobs->vGrabLock();

  for (SkipNode* pScan = pRetiredJobs->pFirst();
       pScan;
       pScan = pRetiredJobs->pNext(pScan)) {
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

    if (tJobId_test == tJobId && C_UNKNOWN != pJobScan->iParticipantNodeId()) {
      if (pJobScan->szJobNm()->bSame("pipeline"_Stz))
        pJob = pJobScan;
    }
  }

  pRetiredJobs->vDropLock();

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
  JobList* pRetiredJobs = AM::pRetiredJobs();

  pRetiredJobs->vGrabLock();

  for (SkipNode* pScan = pRetiredJobs->pFirst();
       pScan;
       pScan = pRetiredJobs->pNext(pScan)) {
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

    if (iJobNo_test == iJobNo && C_UNKNOWN != pJobScan->iParticipantNodeId()) {
      if (pJobScan->szJobNm()->bSame("pipeline"_Stz))
        pJob = pJobScan;
    }
  }

  pRetiredJobs->vDropLock();

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
