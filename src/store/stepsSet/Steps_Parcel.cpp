/*============================================================================*/
/* Steps_Parcel.cpp -- parcel steps                                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

#include "Parcel.hpp"

#include "Op.hpp"
#include "OpSpec.hpp"
#include "Stage.hpp"
#include "StageSpec.hpp"
#include "Stream.hpp"
#include "Pipework.hpp"
#include "PtnMap.hpp"
#include "Node.hpp"
#include "Pipeline.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

extern SizeHeap gxaSizeHeap[];

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Parcel(void)
{
  vAddStep("parcel::delete"_Stz,          &Step::vParcelDeleteOld);
  vAddStep("parcel.demonNm@"_Stz,         &Step::vParcelDemonNmFetch);
  vAddStep("parcel.demonNm!"_Stz,         &Step::vParcelDemonNmStore);
  vAddStep("parcel.demonOut"_Stz,         &Step::vParcelDemonOut);
  vAddStep("parcel.jobid@"_Stz,           &Step::vParcelJobIdFetch);
  vAddStep("parcel.label"_Stz,            &Step::vParcelLabel);
  vAddStep("parcel.pipeOut"_Stz,          &Step::vParcelPipeOut);
  vAddStep("parcel.sendOut"_Stz,          &Step::vParcelSendOut);

  vAddStep("Parcel[]"_Stz,                &Step::vParcelGet);
  vAddStep("Parcel[]\\clear"_Stz,         &Step::vParcelGetAndClear);
  vAddStep("Parcel[].delete"_Stz,         &Step::vParcelDelete);
  vAddStep("Parcel[].isNotNull"_Stz,      &Step::vParcelIsNotNull);
  vAddStep("Parcel[].isNull"_Stz,         &Step::vParcelIsNull);
  vAddStep("Parcel[].set"_Stz,            &Step::vParcelSet);
}

/*----------------------------------------------------------------------------*/
/* parcel::delete -- delete parcel                                            */
/*----------------------------------------------------------------------------*/
void Step::vParcelDeleteOld(void)
{
  ENTRY();

  Parcel* pParcel = _pParcel(m_pDataStack->pPopPtr(__func__));
  delete pParcel;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* parcel.demonOut -- parcel out to each demon in grid                        */
/*----------------------------------------------------------------------------*/
void Step::vParcelDemonOut(void)
{
  ENTRY();
  
  // first case, just the same node
  
  //vTrace("vParcelDemonOut\n"_Stz);

  Parcel* pParcel = _pParcel(m_pDataStack->pPopPtr(__func__));
  
  Operand* pOperand = pParcel->pGetOperand();

  Stz* szDemonNm = pParcel->szDemonNm();
  
  //vTrace("vParcelPipeOut to demon %s\n"_Stz,szDemonNm);
  
  Word* pWord = AM::WORD::pFind(szDemonNm);
  Job* pJob = _pJob(pWord->pObject());
  
  
  // !!!!!!!!!!!!!!
  // move to AM
  
  Pipeline* pPipeline = pJob->pPipeline_job();
  Pipework* pPipework = pPipeline->pPipework();
  Stage*    pStage0   = pPipework->pGet(0);
  Stream*   pStream0  = pStage0->pGet(0);
  Op*       pOp0      = pStream0->pGet(0);

  pOp0->vAppendLeft(pOperand,this);
  // !!!!!!!!!!!!!!

  delete pParcel;
}

/*----------------------------------------------------------------------------*/
/* parcel.demonNmFetch -- fetch demon name                                    */
/*----------------------------------------------------------------------------*/
void Step::vParcelDemonNmFetch(void)
{
  ENTRY();

  Parcel* pParcel = _pParcel(m_pDataStack->pPopPtr(__func__));

  Stz* szDemonNm = pParcel->szDemonNm();

  m_pDataStack->vPushStz(szDemonNm);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* parcel.demonNmStore -- store demon name                                    */
/*----------------------------------------------------------------------------*/
void Step::vParcelDemonNmStore(void)
{
  ENTRY();

  Parcel* pParcel = _pParcel(m_pDataStack->pPopPtr(__func__));
  
  s256 sDemonNm;
  m_pDataStack->vPopStr(sDemonNm,__func__);

  pParcel->vDemonNm(sDemonNm);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* parcel.jobIdFetch -- fetch job id                                          */
/*----------------------------------------------------------------------------*/
void Step::vParcelJobIdFetch(void)
{
  ENTRY();

  Parcel* pParcel = _pParcel(m_pDataStack->pPopPtr(__func__));

  i8 iJobid = pParcel->iPipelineJobId();
  
  m_pDataStack->vPushInt(iJobid);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* parcel::label -- label parcel                                              */
/*----------------------------------------------------------------------------*/
void Step::vParcelLabel(void)
{
  ENTRY();

  Op* pOp = *m_pOp;
  OpSpec* pOpSpec = pOp->pOpSpec();;

  Parcel* pParcel = _pParcel(m_pDataStack->pPopPtr(__func__));

  pParcel->vLabel(pOp->pPipelineJob()->tJobId().iAsI8(),
                  pOpSpec->eParcelForward());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* parcel.pipeOut -- pipe out parcel out to next operator in pipeline         */
/*----------------------------------------------------------------------------*/
void Step::vParcelPipeOut(void)
{
  ENTRY();
  
  //vTrace("vParcelPipeOut\n"_Stz);

  Parcel* pParcel = _pParcel(m_pDataStack->pPopPtr(__func__));
  
  Operand* pOperand = pParcel->pGetOperand();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipeing out to a demon
  if (C_UNKNOWN == pParcel->iPipelineJobId()) {
    
    NEVER_REACH;
    
    Stz* szDemonNm = pParcel->szDemonNm();
    
    vTrace("vParcelPipeOut to demon %s\n"_Stz,szDemonNm);
    
    Word* pWord = AM::WORD::pFind(szDemonNm);
    Job* pJob = _pJob(pWord->pObject());
    
    
    // !!!!!!!!!!!!!!
    // move to AM
    
    Pipeline* pPipeline = pJob->pPipeline_job();
    Pipework* pPipework = pPipeline->pPipework();
    Stage*    pStage0   = pPipework->pGet(0);
    Stream*   pStand0   = pStage0->pGet(0);
    Op*       pOp0      = pStand0->pGet(0);

    pOp0->vAppendLeft(pOperand,this);
    // !!!!!!!!!!!!!!

    delete pParcel;
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // find the pipeline
  JobId tPipelineJob;
  tPipelineJob = pParcel->iPipelineJobId();
  Job* pJob = AM::pJob_FindParticipating(tPipelineJob);
  Pipeline* pPipeline = pJob->pPipeline_job();
  
  // get the next stage the send is supposed to send to
  Pipework* pFormWork = pPipeline->pPipework();
  Stage* pDstStage = pFormWork->pGet(pParcel->iDstStage());
  i8 nStreamPtns = pDstStage->nItems();

  // get the op to forward to depending on the cross topology
  Op* pDstOp = nullptr;
  
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pParcel->bOriginalParcel()) {
    //vTrace("bOriginalParcel\n"_Stz);
    pDstOp = pDstStage->pPipeDstOp(pParcel->tDstPtn(),0);
      
    // CAUTION NOTE: Cannot access LU of OP after piping out operand
    // On LINUX it moves very fast and the OP/Task/LU could disappear underneath it
    LogicUnit* pLogicUnit = pDstOp->pOpLogicUnit();
    if (pLogicUnit && pLogicUnit->m_xpStats.bIsNotNull()) {
      pLogicUnit->m_xpStats->m_nParcelRecvs++;
      pLogicUnit->m_xpStats->m_nObjectsRecv += pParcel->nItems();
    }

    pDstOp->vAppendLeft(pParcel,this);
    // do not touch pDstOp or pParcel
    EXIT();
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  Task* pCurTask = MY::pTask();
  
  eCN eOperandClass = pOperand->eGetClassNo();
  eRV eRecvInstructions = pParcel->eRecvInstructions();
  //vTrace("eRecvInstructions %s\n"_Stz,aRV[eRecvInstructions]);

  i8 iCurNode = MY::pNode()->iNodeNo();
  i8 nCopies = AM::PTNMAP::nPtnsPerNode();

  eCN eClass = pOperand->eGetClassNo();
  i8 nSize = gxaSizeHeap[eClass].m_nClassSize;

  RowBuffer* pRowBuffer_shared = nullptr;
  if (CN_ROWBUFFER == eOperandClass)
    pRowBuffer_shared = _pRowBuffer(pOperand);

  i8 nQueued = 0;

  switch (eRecvInstructions) {
      
    case RV_ALL_PTNS: {
      eYG eStageTopology = pDstStage->pStageSpec()->eStageTopology();

//      vTrace("RV_ALL_PTNS %s %s nStreamPtns %lld\n"_Stz
//             ,aYG[eStageTopology]
//             ,aCN[eClass]
//             ,nStreamPtns);

      // put a copy of operand onto all ptns
      
      //vTrace("eRecvInstructions %s\n"_Stz,aRV[eRecvInstructions]);

      if (pRowBuffer_shared)
        pRowBuffer_shared->vDeciderCopies(nCopies);
      
      
      for (i8 i_Stream = 0; i_Stream < nStreamPtns; i_Stream++) {
        Stream* pDstStream = pDstStage->pGet(i_Stream);

        if (YG_ALL_PARTITIONS == eStageTopology) {
          PtnId tDstPtn = pDstStream->tPtnId();
          i8 iDstNde = AM::pPtnMap()->iMappedNodeNo(tDstPtn);
          if (iDstNde != iCurNode)
            continue;
        }
        else {
          RASSERT(YG_ALL_PARTITIONS_ON_ORIGIN == eStageTopology);
        }
        
        Op* pDstOp = pDstStream->pGet(0);
//        vTrace("pDstOp %p pDstStream->tPtnId() %lld\n"_Stz
//               ,pDstOp
//               ,pDstStream->tPtnId().iAsI8());

        // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
              
        // put shared rowBuffer operand onto ptn
        
        if (pRowBuffer_shared) {
//          vTrace("i_Stream %lld pRowBuffer_shared %p\n"_Stz
//                 ,i_Stream,pRowBuffer_shared);
          AM::OP::vPipeToOp(pDstOp,pRowBuffer_shared,pCurTask);
          nQueued++;
          continue;
        }
        
        // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

        // put a copy of the operand onto ptn
        
        Operand* pDuplicateOperand = (Operand*)Heaps::pAllocate(eClass);
        MemLib::vCopy(pDuplicateOperand,pOperand,nSize,eClass);

        if (gxaSizeHeap[eClass].m_fpPostRecvFn)
          gxaSizeHeap[eClass].m_fpPostRecvFn(pDuplicateOperand);

//        vTrace("i_Stream %lld pDstOp %p pDuplicateOperand %p pCurTask %p\n"_Stz
//               ,i_Stream,pDstOp,pDuplicateOperand,pCurTask);
        AM::OP::vPipeToOp(pDstOp,pDuplicateOperand,pCurTask);
        nQueued++;
      }

      // delete the original operand (to be optimised)
      if (CN_ROWBUFFER != eOperandClass)
        AM::vDeleteOperand(pOperand);
      
      delete pParcel;

      if (nQueued != AM::PTNMAP::nPtnsPerNode()) {
        
        vTrace("🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪\n"
               "nQueued %lld OperandClass %s \n"
               "DstStage %lld nStreamPtns %lld \n"
               ""_Stz
               
               ,nQueued
               ,aCN[eClass]
               
               ,pDstStage->i_Stage()
               ,nStreamPtns
               );
        
        for (i8 i_Stream = 0; i_Stream < nStreamPtns; i_Stream++) {
          Stream* pDstStream = pDstStage->pGet(i_Stream);

          PtnId tDstPtn = pDstStream->tPtnId();
          vTrace("i_Stream %lld tDstPtn %lld\n"_Stz
                 ,i_Stream
                 ,tDstPtn.iAsI8());
        }
        
      }
      
      RASSERT(nQueued == AM::PTNMAP::nPtnsPerNode());
      
      break;
    }
    
    //  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~

    case RV_ONE_PTN:
//        vTrace("RV_ONE_PTN  tDstPtn %lld\n"_Stz
//               ,pParcel->tDstPtn().iAsI8());
      if (pOperand) {
        pDstOp = pDstStage->pPipeDstOp(pParcel->tDstPtn(),0);
        if (!pDstOp) {
          vTrace("huh? RV_ONE_PTN tDstPtn %lld\n"_Stz
                 ,pParcel->tDstPtn().iAsI8());
        }
        pDstOp->vAppendLeft(pOperand, this);
        nQueued++;
        // do not touch pDstOp or pParcel
        delete pParcel;
      }
      break;
      
    default:
      //vTrace("eRecvInstructions %s\n"_Stz,aRV[eRecvInstructions]);
      NEVER_REACH;
      break;
  }
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* parcel.sendOut -- send parcel out to next operator in pipeline             */
/*----------------------------------------------------------------------------*/
void Step::vParcelSendOut(void)
{
  ENTRY();
    
  Parcel* pParcel = _pParcel(m_pDataStack->pPopPtr(__func__));
  
  // revised job/op model
  Op* pCurrOp = *m_pOp;

  // CAUTION NOTE: Cannot access LU of OP after sending out operand
  // On LINUX it moves very fast and the OP/Task/LU could disappear underneath it
  LogicUnit* pLogicUnit = pCurrOp->pOpLogicUnit();
  if (pLogicUnit && pLogicUnit->m_xpStats.bIsNotNull()) {
    pLogicUnit->m_xpStats->m_nParcelSends++;
    pLogicUnit->m_xpStats->m_nObjectsSent += pParcel->nItems();
  }

  pParcel->vDistribute(pCurrOp);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Parcel[].delete -- parcel delete                                           */
/*----------------------------------------------------------------------------*/
void Step::vParcelDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_pLogicUnit->m_xpParcel);

  m_pLogicUnit->m_xpParcel = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Parcel[].get -- fetch parcel                                               */
/*----------------------------------------------------------------------------*/
void Step::vParcelGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpParcel);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Parcel[].getAndClear -- fetch parcel and set to nullptr                    */
/*----------------------------------------------------------------------------*/
void Step::vParcelGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpParcel);

  m_pLogicUnit->m_xpParcel = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Parcel[].isNotNull -- is parcel not null ?                                 */
/*----------------------------------------------------------------------------*/
void Step::vParcelIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpParcel.bIsNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Parcel[].isNull -- is parcel null ?                                        */
/*----------------------------------------------------------------------------*/
void Step::vParcelIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpParcel.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Parcel[].set -- set parcel                                                 */
/*----------------------------------------------------------------------------*/
void Step::vParcelSet(void)
{
  ENTRY();

  Parcel* pParcel = _pParcel(m_pDataStack->pPopPtr(__func__));

  ASSERT(! pParcel || m_pLogicUnit->m_xpParcel.bIsNull());

  m_pLogicUnit->m_xpParcel = _pParcel(pParcel);

  EXIT();
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
