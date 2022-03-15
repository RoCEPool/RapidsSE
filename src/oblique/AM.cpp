/*============================================================================*/
/* AM.cpp -- AttachMap helper                                                 */
/*============================================================================*/


#include "BASE.h"


#include "eAK.h"
#include "eAR.h"
#include "eBO.h"
#include "eCC.h"
#include "eCP.h"
#include "eCX.h"
#include "eDL.h"
#include "eDS.h"
#include "eED.h"
#include "eFN.h"
#include "eIK.h"
#include "eJF.h"
#include "eLG.h"
#include "eMM.h"
#include "eMT.h"
#include "eNE.h"
#include "eOK.h"

#include "eOI.h"
#include "eOR.h"
#include "ePF.h"
#include "ePP.h"
#include "eYG.h"
#include "ePY.h"
#include "eYX.h"
#include "eRG.h"
#include "eRO.h"
#include "eRU.h"
#include "eST.h"
#include "eTP.h"
#include "eTT.h"
#include "eUK.h"

#include "CmdLineArgs.hpp"

#include "IndexDef.hpp"
#include "Marker.hpp"

#include "RowBuffer.hpp"

#include "Pot.hpp"
#include "InfoSet.hpp"
#include "StreamSet.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"

#include "Crw.hpp"

#include "PtnMap.hpp"

#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"

#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "RowOnce.hpp"
#include "IndexStream.hpp"
#include "Table.hpp"
#include "MetaGet.hpp"

#include "Node.hpp"
#include "Node_I.hpp"
#include "Grid.hpp"

#include "Arg.hpp"
#include "RelOp.hpp"
#include "Predicate.hpp"
#include "PredicateVec.hpp"
#include "AccessPath.hpp"

#include "Api.hpp"
#include "AttachMap.hpp"
#include "Constants.hpp"
#include "Engine.hpp"
#include "PipelineSpec.hpp"
#include "Job.hpp"
#include "JobList.hpp"
#include "Jobs.hpp"
#include "Session.hpp"
#include "Sessions.hpp"
#include "Parcel.hpp"
#include "Numbering.hpp"
#include "Output.hpp"

#include "TagConstants.h"


#include "LogicUnit.hpp"

#include "Pipeline.hpp"

#include "Op.hpp"

#include "PlanOperand.hpp"
#include "EnumGroups.h"

#include "Step.hpp"
#include "StreamDef.hpp"
#include "StreamDefRht.hpp"

#include "SP_Word.hpp"

#include "TraceLock.hpp"
#include "InternalTables.hpp"

#include "Version.h"

#include "MY.hpp"
#include "SP_Pointer.hpp"

#include "HeadsUp.hpp"

#include "PtnId.hpp"
#include "BucketMap_dyn.hpp"
#include "Stage.hpp"
#include "SysMetrics.hpp"

extern CmdLineArgs   gxCmdLineArgs;

extern AttachMap*    gpAttachMap;
extern BufferPool*   gpBufferPool;
extern File*         gpTraceFile;
extern Header*       gaSeShmRegion;
extern Index*        gpReadApiIndex;

extern SizeHeap      gxaSizeHeap[];



/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                  ATTACHMAP                                 */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* add sys metrics to attach map                                              */
/*----------------------------------------------------------------------------*/
void AM::ATTACHMAP::vSysMetricsAdd(SysMetrics* pSysMetrics)
{
  _static_
  
  gpAttachMap->vSysMetricsAdd(pSysMetrics);
}

/*----------------------------------------------------------------------------*/
/* get sys metrics chain from attach map                                      */
/*----------------------------------------------------------------------------*/
SysMetrics* AM::ATTACHMAP::pSysMetricsChain(void)
{
  _static_
  
  SysMetrics* pSysMetricsChain = gpAttachMap->pSysMetricsChain();
  
  return pSysMetricsChain;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                   BKTMAP                                   */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* convert bucket id to ptn id                                                */
/*----------------------------------------------------------------------------*/
PtnId AM::BKTMAP::tBktIdToLglPtnId(i8 iBktId)
{
  _static_
  
  PtnId tPtnId = gpAttachMap->m_pBucketMap->tBktToLglPtnId(iBktId);

  return tPtnId;
}

/*----------------------------------------------------------------------------*/
/* get bucket map                                                             */
/*----------------------------------------------------------------------------*/
BucketMap_dyn* AM::BKTMAP::pBucketMap(void)
{
  _static_
  
  return gpAttachMap->m_pBucketMap;
}

/*----------------------------------------------------------------------------*/
/* convert hash to bucket id                                                  */
/*----------------------------------------------------------------------------*/
i8 AM::BKTMAP::iHashToBktId(i8 iHash)
{
  _static_
  
  i8 iBktId = iHash % C_MAX_BUCKETS_IN_GRID;

  return iBktId;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                   DIC                                      */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* list all words in this dic                                                 */
/*----------------------------------------------------------------------------*/
void AM::DIC::vListContents(Dic* pDic)
{
  _static_
  
  pDic->vGrabRead();
  
  
  for (Word* pWord = pDic->pFirstWord(); pWord; pWord = pWord->pNextWord()) {

    Object* pObject = pWord->pObject();
    if (! pObject)
      continue;

    AM::WORD::vListContents(pWord,0);
  }
  
  pDic->vDropRead();
}

/*----------------------------------------------------------------------------*/
/* get root dictionary                                                        */
/*----------------------------------------------------------------------------*/
Dic* AM::DIC::pRoot(void)
{
  _static_
  
  return gpAttachMap->m_pDicRoot;
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                   DEMON                                    */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* pipe given row buffer to named demon                                       */
/*----------------------------------------------------------------------------*/
void AM::DEMON::vPipeTo(Stz* szDemonNm, Operand* pOperand)
{
  _static_

  // for reference: sen code from : OP_toStatDemon_Profile \handler
  
//  PipeIn[]
//  "__statDemon" find dup if
//    word.object@
//    pipeline.pipeout
//    nullptr to PipeIn[]
//  else
//    discard discard
//    PipeIn[].delete
//  then

  Dic* pRootDic = AM::DIC::pRoot();

  Word* pDemonWord = nullptr;
  
  while (! (pDemonWord = pRootDic->pFindWord(szDemonNm))) {
    vTrace("Sleeping till demon: %s\n"_Stz,szDemonNm);
    vNapMsec(3000,CN_PIPELINE);
  }
  
  Job* pPipelineJob = _pJob(pDemonWord->pObject());
  
  Pipeline* pPipeline = pPipelineJob->pPipeline_job();
  
  Op* pOp0 = pPipeline->pOp0();

  //Task* pThisTask = AM::pThisTLSTask();
  
  pOp0->vAppendLeft(pOperand,nullptr);
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                  ENGINE                                    */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define bDELETE_RULE(CLASSNO,FUNNM) (CLASSNO == eDeleter && szDelFunNm->bSame(FUNNM))

/*----------------------------------------------------------------------------*/
/* assert EE delete rules                                                     */
/*----------------------------------------------------------------------------*/
void AM::EE::vAssertEEDelete(eCN     eInstance,
                             Object* pInstance,
                             
                             eCN     eExpecting,
                             eCN     eDeleter,
                             Object* pDeleter,
                             Stz*    szDelFunNm,
                             bool    bTrace)
{
  _static_
  
  RASSERT(szDelFunNm);

  if (eExpecting != eInstance) {
    
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (false)
    {;}

  // -- -- -- -- -- -- -- -- -- -- -- --
  // formwork spec

  else if (CN_FORMWORKSPEC == eInstance) {
    if (bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[eInstance],eInstance,pInstance,szDelFunNm,pDeleter);

    RASSERT(   bDELETE_RULE(CN_PIPELINESPEC,  "~PipelineSpec"_Stz)
            );
  }

  // -- -- -- -- -- -- -- -- -- -- -- --
  // freeChunk
  
  else if (CN_FREECHUNK == eInstance) {
    if (bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[eInstance],eInstance,pInstance,szDelFunNm,pDeleter);
      
    NEVER_REACH;
  }

  // -- -- -- -- -- -- -- -- -- -- -- --
  // job

  else if (CN_JOB == eInstance) {
    if (bTrace)
      vTrace("del %12s:%-3lld %p "
             "from %s() "
             "by %12s:%-3lld %p\n"_Stz
             
             ,aCN[eInstance],(i8)eInstance,pInstance
             
             ,szDelFunNm
             
             ,aCN[eDeleter],(i8)eDeleter,pDeleter
             );

 
    if ( !
           (
            
            //bDELETE_RULE(CN_JOBLIST, "pFindJob"_Stz        )

            //|| bDELETE_RULE(CN_API,     "vDeleteImmediate"_Stz)
            //|| bDELETE_RULE(CN_API,     "vCallBootSystem"_Stz)
               bDELETE_RULE(CN_API,     "bReapJob"_Stz        )
            || bDELETE_RULE(CN_STEP,    "vJobDelete2pc"_Stz   )
           )
        )
      vDebugPoint();
    
    RASSERT(
            //bDELETE_RULE(CN_JOBLIST, "pFindJob"_Stz        )

            //|| bDELETE_RULE(CN_API,     "vDeleteImmediate"_Stz)
            //|| bDELETE_RULE(CN_API,     "vCallBootSystem"_Stz)
               bDELETE_RULE(CN_API,     "bReapJob"_Stz        )
            || bDELETE_RULE(CN_STEP,    "vJobDelete2pc"_Stz   )

//              || bDELETE_RULE(CN_AM,      "vJobDelete"_Stz      )
//              || bDELETE_RULE(CN_AM,      "~Task"_Stz           )
//              || bDELETE_RULE(CN_API,     "vDeleteJob"_Stz      )
//              || bDELETE_RULE(CN_STEP,    "vJobDelete"_Stz      )
            );
  }

  // -- -- -- -- -- -- -- -- -- -- -- --
  // logic unit

  else if (CN_LOGICUNIT == eInstance) {
    if (bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[eInstance],eInstance,pInstance,szDelFunNm,pDeleter);

    RASSERT(   bDELETE_RULE(CN_AM,  "vDelete"_Stz         )
            || bDELETE_RULE(CN_JOB, "~Job"_Stz            )
            || bDELETE_RULE(CN_JOB, "vResetForCascade"_Stz)
            || bDELETE_RULE(CN_OP,  "~Op"_Stz             )
            );
  }

  // -- -- -- -- -- -- -- -- -- -- -- --
  // op
  
  else if (CN_OP == eInstance) {
    if (bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[eInstance],eInstance,pInstance,szDelFunNm,pDeleter);
    
    RASSERT( bDELETE_RULE(CN_STREAM, "~Stream"_Stz ) );
  }

  // -- -- -- -- -- -- -- -- -- -- -- --
  // opSpec
  
  else if (CN_OPSPEC == eInstance) {
    if (bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[eInstance],eInstance,pInstance,szDelFunNm,pDeleter);
    
    RASSERT(   bDELETE_RULE(CN_FORMWORKSPEC, "vFirstSend"_Stz)
            || bDELETE_RULE(CN_FORMWORKSPEC, "vOtherSend"_Stz)
            || bDELETE_RULE(CN_STAGESPEC,    "~StageSpec"_Stz)
            );
  }
  
  // -- -- -- -- -- -- -- -- -- -- -- --
  // pipeline

  else if (CN_PIPELINE == eInstance) {
    if (bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[eInstance],eInstance,pInstance,szDelFunNm,pDeleter);
    
    RASSERT( bDELETE_RULE(CN_LOGICUNIT, "~LogicUnit"_Stz ) );
  }
  
  // -- -- -- -- -- -- -- -- -- -- -- --
  // pipelineSpec
    
  else if (CN_PIPELINESPEC == eInstance) {
    if (bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[eInstance],eInstance,pInstance,szDelFunNm,pDeleter);

    RASSERT(   bDELETE_RULE(CN_DYNAMO,   "pCompile"_Stz           )
            || bDELETE_RULE(CN_DYNAMO,   "vDynamoCompile"_Stz )
            || bDELETE_RULE(CN_PIPELINE, "~Pipeline"_Stz          )
            || bDELETE_RULE(CN_STEP,     "vPipelineSpecDelete"_Stz)
            );
  }

  // -- -- -- -- -- -- -- -- -- -- -- --
  // stage
  
  else if (CN_STAGE == eInstance) {
    if (bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[eInstance],eInstance,pInstance,szDelFunNm,pDeleter);

    RASSERT( bDELETE_RULE(CN_PIPELINE, "~Pipeline"_Stz ) );
  }

  // -- -- -- -- -- -- -- -- -- -- -- --
  // stageSpec
  
  else if (CN_STAGESPEC == eInstance) {
    if (bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[eInstance],eInstance,pInstance,szDelFunNm,pDeleter);

    RASSERT( bDELETE_RULE(CN_FORMWORKSPEC, "~FormworkSpec"_Stz) );
  }
  
  // -- -- -- -- -- -- -- -- -- -- -- --
  // stream
  
  else if (CN_STREAM == eInstance) {
    if (bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[eInstance],eInstance,pInstance,szDelFunNm,pDeleter);
    
    RASSERT( bDELETE_RULE(CN_STAGE, "~Stage"_Stz ) );
  }

  // -- -- -- -- -- -- -- -- -- -- -- --
  // task

  else if (CN_TASK == eInstance) {
    if (bTrace)
      vTrace("del %12s:%-3lld %p from %s() by %p\n"_Stz,
             aCN[eInstance],eInstance,pInstance,szDelFunNm,pDeleter);
      
    if ( ! (   bDELETE_RULE(CN_JOB,       "~Job"_Stz)
            || bDELETE_RULE(CN_OP,        "~Op"_Stz)
            || bDELETE_RULE(CN_SCHEDULER, "vTaskSelf_DeleteThisTask"_Stz)
           )
        )
        vDebugPoint();
        
    RASSERT(   bDELETE_RULE(CN_JOB,       "~Job"_Stz)
            || bDELETE_RULE(CN_OP,        "~Op"_Stz)
            || bDELETE_RULE(CN_SCHEDULER, "vTaskSelf_DeleteThisTask"_Stz)
            );
  }

  // -- -- -- -- -- -- -- -- -- -- -- --
  // unknown???
  
  else {
    vTrace("del UNRECOGNISED %p CLASS BEING DELETED\n"_Stz,pInstance);

    vTrace("eInstance=%lld:%s\n"_Stz,eInstance,aCN[eInstance]);

    vTrace("eDeleter=%lld:%s\n"_Stz,eDeleter,aCN[eDeleter]);

    vTrace("szDelFunNm=%s\n"_Stz,szDelFunNm);
    
    NEVER_REACH;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RASSERT(eExpecting == eInstance);

}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                  ENGINE                                    */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* call engine signal handler                                                 */
/*----------------------------------------------------------------------------*/
void AM::ENGINE::vSignalHandler(i8 iSignal)
{
  _static_
  
  Engine::vSignalHandler(iSignal);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                   GRID                                     */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* get node for given node number                                             */
/*----------------------------------------------------------------------------*/
Node* AM::GRID::pGetNode(i8 iNodeNo)
{  
  _static_
  
  Node* pNode = gpAttachMap->m_pGrid->pFindNode(iNodeNo);
    
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* get this                                                                   */
/*----------------------------------------------------------------------------*/
Node* AM::GRID::pGetThisNode(void)
{
  _static_
  
  Node* pNode = gpAttachMap->m_pGrid->pThisNode();
    
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* get the node number of nth node                                            */
/*----------------------------------------------------------------------------*/
i8 AM::GRID::iGetNthNodeNo(i8 i_Node)
{
  _static_
  
  i8 iNodeNo = 0;
  
  Node* pNode = gpAttachMap->m_pGrid->pGetNthNode(i_Node);
  
  if (pNode)
    iNodeNo = pNode->iNodeNo();
  
  return iNodeNo;
}

/*----------------------------------------------------------------------------*/
/* get number of known nodes                                                  */
/*----------------------------------------------------------------------------*/
i8 AM::GRID::nKnownNodes(void)
{
  _static_
  
  return gpAttachMap->m_pGrid->nKnownNodes();
}

/*----------------------------------------------------------------------------*/
/**
 * @brief Add new node into the cluster
 * @details When a new node comes up, add it to the cluster.
 * <p>
 * @param[in] iNodeNo  ID of this node
 * @param[in] pszName  name of this node
 * @param[in] pszAddr  Address this node runs at
 * @param[in] iPort   port number this node listens to
 *
 *
 * @return none
 *
 */
void AM::GRID::vNodeDetected(i8 iNodeNo, Stz* szNodeNm,
                       Stz* szIPAddr, i8 iPortNo,
                       Stz* szRegionNm)
{
  _static_
  
  if (AM::SYSTEM::bStarted()) {
    Node *pDetectedNode = AM::pGrid()->pFindNode(szNodeNm);
    
    // check to make sure we haven't added the detected node yet
    if (! pDetectedNode) {
      AM::pGrid()->vAddDetectedNode(iNodeNo, szNodeNm,
                                    szIPAddr, iPortNo,
                                    szRegionNm);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get this node number                                                       */
/*----------------------------------------------------------------------------*/
i8 AM::GRID::iThisNodeNo(void)
{
  _static_
  
  i8 iNodeNo = gpAttachMap->m_pGrid->iThisNodeNo();
    
  return iNodeNo;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                  HANDLERS                                  */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* get the default event handlers                                             */
/*----------------------------------------------------------------------------*/
EventHandlers* AM::HANDLERS::pEventHandlers(void)
{
  _static_
  
  return & gpAttachMap->m_oEventHandlers;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                   JOB                                      */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* has job got given tag?                                                     */
/*----------------------------------------------------------------------------*/
bool AM::JOB::bHasTag(Job* pJob, Stz* szTagNm)
{
  _static_
  
  TagSet* pJobTags = pJob->pJobTags();
  if (! pJobTags)
    return false;
  
  if (! pJobTags->bHasName(szTagNm))
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get job name                                                               */
/*----------------------------------------------------------------------------*/
Stz* AM::JOB::szJobNm(Job* pJob)
{
  _static_
  
  return pJob->szJobNm();
}


/*----------------------------------------------------------------------------*/
/* test if the job passes all pre-execution requirements                      */
/*----------------------------------------------------------------------------*/
eEC AM::JOB::ePreExecuteTest(Job* pJob)
{
  _static_

  if (! AM::SYSTEM::bPassesLowMem(pJob))
    return EC_LOW_MEMORY;

  if (! AM::SYSTEM::bPassesOpenSystem(pJob))
    return EC_SYSTEM_CLOSED;

  return EC_NONE;
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                  LOCK                                      */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* lock is about to be read unlocked                                         */
/*----------------------------------------------------------------------------*/
void AM::LOCK::vIntendReadUnlocked(Lock* pLock)
{
  _static_

  Task* pUnlockingTask = MY::pTask();
  if (! pUnlockingTask)
    return;
      
  LockSet* pReadLocks = pUnlockingTask->pReadLocks();
  
  pReadLocks->vRemove(pLock);
}

/*----------------------------------------------------------------------------*/
/* lock has just been read locked                                             */
/*----------------------------------------------------------------------------*/
void AM::LOCK::vJustReadLocked(Lock* pLock)
{
  _static_
  
  Task* pUnlockingTask = MY::pTask();
  if (! pUnlockingTask)
    return;
      
  LockSet* pReadLocks = pUnlockingTask->pReadLocks();

  pReadLocks->vAdd(pLock);
}

/*----------------------------------------------------------------------------*/
/* lock is about to be write unlocked                                         */
/*----------------------------------------------------------------------------*/
void AM::LOCK::vIntendWriteUnlocked(Lock* pLock)
{
  _static_

  Task* pUnlockingTask = MY::pTask();
  if (! pUnlockingTask)
    return;
      
  LockSet* pWriteLocks = pUnlockingTask->pWriteLocks();
  
  pWriteLocks->vRemove(pLock);
}

/*----------------------------------------------------------------------------*/
/* lock has just been write locked                                            */
/*----------------------------------------------------------------------------*/
void AM::LOCK::vJustWriteLocked(Lock* pLock)
{
  _static_
  
  Task* pLockingTask = MY::pTask();
  if (! pLockingTask)
    return;
      
  LockSet* pWriteLocks = pLockingTask->pWriteLocks();
  
  pWriteLocks->vAdd(pLock);
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                   LOG                                      */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


/*----------------------------------------------------------------------------*/
/* log entry into internal table                                              */
/*----------------------------------------------------------------------------*/
//void AM::LOG::vLogInternal(i8 iTable, ...)
//{
//  _static_
//
//  bool bSetLogMemory = false;
//  if (AM::TASK::bLogMemory()) {
//    bSetLogMemory = true;
//    AM::TASK::vLogMemory(false);
//  }
//
//  WriteBehind* pWriteBehind = AM::THREAD::pWriteBehind();
//  if (! pWriteBehind)
//    return;
//
//  StreamDef* pStreamDef = AM::pInternalTables()->pStreamDef((eTI)iTable);
//  if (! pStreamDef)
//    return;
//
//  pWriteBehind->vGrabLock();
//
//
//  va_list xVarArgs;
//  va_start(xVarArgs,iTable);
//
//  Row* pRow = pWriteBehind->pAllotRow((eTI)iTable);
//  pRow->vDeposit(xVarArgs);
//
//  va_end(xVarArgs);
//
//
//  pWriteBehind->vDropLock();
//
//  if (bSetLogMemory)
//    AM::TASK::vLogMemory(true);
//}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                LOGICUNIT                                   */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* add register in logic unit                                                 */
/*----------------------------------------------------------------------------*/
void AM::LOGICUNIT::vAddReg(LogicUnit* pLogicUnit, void* aReg)
{
  _static_

  pLogicUnit->vAddReg(static_cast< REG<Object,CN_NONE>* >(aReg));
  
}

/*----------------------------------------------------------------------------*/
/* delete given logic unit                                                    */
/*----------------------------------------------------------------------------*/
void AM::LOGICUNIT::vDelete(LogicUnit** ppLogicUnit)
{
  _static_
  

  //delete * ppLogicUnit;
  
  LogicUnit* pLogicUnit = * ppLogicUnit;
  * ppLogicUnit = nullptr;
  
  pLogicUnit->vGuardedDelete(CN_LOGICUNIT,&pLogicUnit,
                             CN_AM,nullptr,(Stz*)__func__);

}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                MASTERLOCK                                  */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* drop ddl read lock                                                         */
/*----------------------------------------------------------------------------*/
void AM::MASTERLOCK::vDropDdlReadLock(LogicUnit* pLogicUnit)
{
  _static_
  
  RASSERT(LH_HAVE_READ_LOCK == pLogicUnit->m_eDdlLocked);

  gpAttachMap->m_oLocks.m_oDdlMasterLock.vDropRead();
  
  pLogicUnit->m_eDdlLocked = LH_HAVE_NO_LOCK;
}

/*----------------------------------------------------------------------------*/
/* grab ddl read lock                                                         */
/*----------------------------------------------------------------------------*/
void AM::MASTERLOCK::vGrabDdlReadLock(LogicUnit* pLogicUnit)
{
  _static_
  
  RASSERT(LH_HAVE_NO_LOCK == pLogicUnit->m_eDdlLocked);
  
  gpAttachMap->m_oLocks.m_oDdlMasterLock.vGrabRead();
  
  pLogicUnit->m_eDdlLocked = LH_HAVE_READ_LOCK;
}

/*----------------------------------------------------------------------------*/
/* drop ddl write lock                                                        */
/*----------------------------------------------------------------------------*/
void AM::MASTERLOCK::vDropDdlWriteLock(LogicUnit* pLogicUnit)
{
  _static_
  
  RASSERT(LH_HAVE_WRITE_LOCK == pLogicUnit->m_eDdlLocked);
  
  gpAttachMap->m_oLocks.m_oDdlMasterLock.vDropWrite();
  
  pLogicUnit->m_eDdlLocked = LH_HAVE_NO_LOCK;
}

/*----------------------------------------------------------------------------*/
/* grab ddl write lock                                                        */
/*----------------------------------------------------------------------------*/
void AM::MASTERLOCK::vGrabDdlWriteLock(LogicUnit* pLogicUnit)
{
  _static_
  
  RASSERT(LH_HAVE_NO_LOCK == pLogicUnit->m_eDdlLocked);

  gpAttachMap->m_oLocks.m_oDdlMasterLock.vGrabWrite();
  
  pLogicUnit->m_eDdlLocked = LH_HAVE_WRITE_LOCK;
}

/*----------------------------------------------------------------------------*/
/* test for ddl lock and drop if currently locked by given logic unit         */
/*----------------------------------------------------------------------------*/
void AM::MASTERLOCK::vDdlLockTestDrop(LogicUnit* pLogicUnit)
{
  _static_
  
  if (LH_HAVE_READ_LOCK == pLogicUnit->m_eDdlLocked) {
    
    AM::MASTERLOCK::vDropDdlReadLock(pLogicUnit);
  }
  else
  if (LH_HAVE_WRITE_LOCK == pLogicUnit->m_eDdlLocked) {
    
    AM::MASTERLOCK::vDropDdlWriteLock(pLogicUnit);
  }
}

/*----------------------------------------------------------------------------*/
/* return ddl lock word (i.e. its current value)                              */
/*----------------------------------------------------------------------------*/
u8 AM::MASTERLOCK::uDdlLockWord(void)
{
  _static_
  
  u8 uLockWord = gpAttachMap->m_oLocks.m_oDdlMasterLock.uLockWord();
  
  return uLockWord;
}

/*----------------------------------------------------------------------------*/
/* drop reap lock                                                             */
/*----------------------------------------------------------------------------*/
void AM::MASTERLOCK::vDropReap(LogicUnit* pLogicUnit)
{
  _static_
  
  UNUSED(pLogicUnit);
  gpAttachMap->m_oReapLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab reap lock                                                             */
/*----------------------------------------------------------------------------*/
void AM::MASTERLOCK::vGrabReap(LogicUnit* pLogicUnit)
{
  _static_
  
  UNUSED(pLogicUnit);
  gpAttachMap->m_oReapLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* drop traceLine lock                                                       */
/*----------------------------------------------------------------------------*/
void AM::MASTERLOCK::vDropTraceLine(void)
{
  _static_
  
  gpAttachMap->vTraceLockDrop();
}

/*----------------------------------------------------------------------------*/
/* grab traceLine lock                                                        */
/*----------------------------------------------------------------------------*/
void AM::MASTERLOCK::vGrabTraceLine(void)
{
  _static_
  
  gpAttachMap->vTraceLockDrop();
}

/*----------------------------------------------------------------------------*/
/* drop traceMulti lock                                                       */
/*----------------------------------------------------------------------------*/
void AM::MASTERLOCK::vDropTraceMulti(void)
{
  _static_
  
  gpAttachMap->vTraceMultiLockDrop();
}

/*----------------------------------------------------------------------------*/
/* grab traceLine lock                                                        */
/*----------------------------------------------------------------------------*/
void AM::MASTERLOCK::vGrabTraceMulti(void)
{
  _static_
  
  gpAttachMap->vTraceMultiLockDrop();
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                METRICS                                     */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* add a deref metric sample                                                  */
/*----------------------------------------------------------------------------*/
void AM::METRICS::vDerefMetrics(i8 iObjectNo, eCN eClassNo, eDR eAction)
{
  _static_
 
  if (! AM::SYSARGS::bDerefMetrics())
    return;
  
  Thread* pThread = Thread::pThisTLSThread();
  
  ThreadScratch* pThreadScratch = pThread->pThreadScratch();

  i8 iTsc = Hardware::iReadTsc();
  
  pThreadScratch->vAddDerefMetrics(iObjectNo,eClassNo,iTsc,eAction);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                  NO                                        */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* <no_free>                                                                  */
/*----------------------------------------------------------------------------*/
bool AM::NO::bFree(void)
{
  _static_
  
  if (gpAttachMap->m_xSysArgs.m_oArgTags.bHasName(C_NO_FREE))
    return true;
  else
    return false;
}



/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                 OBJECT                                     */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* get next object number                                                     */
/*----------------------------------------------------------------------------*/
i8 AM::OBJECT::iNextNo(eCN eClassNo)
{
  _static_
  
  i8 iNo;
    
  if (gpAttachMap && gpAttachMap->m_pNumbering) {
    iNo = gpAttachMap->m_pNumbering->iNextNo(eClassNo);
    if (CN_JOB == eClassNo) {
      i8 iNodeNo = AM::GRID::iThisNodeNo();
      
      iNo = iNodeNo << 48 | iNo;
    }
  }
  else {
    RASSERT(CN_FILE == eClassNo);
    iNo = 0;
  }
  
  return iNo;
}

/*----------------------------------------------------------------------------*/
/* test if object is valid                                                    */
/*----------------------------------------------------------------------------*/
eEC AM::OBJECT::eValidObject(Object* pObject)
{
  _static_
  
  // valid address in buffer?
  if (! gpBufferPool->bValidAddress(pObject))
    return EC_OBJECT_ADDRESS_INVALID;
  
  // valid class, footer etc?
  eEC eError = pObject->eValidObject();
  if (EC_NONE != eError)
    return eError;

  // object is valid
  return EC_NONE;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                   OP                                       */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* append operand onto next operator in pipeline                              */
/*----------------------------------------------------------------------------*/
void AM::OP::vPipeToNextOp(Op* pCurrOp, Operand* pOperand,
                           Task* pTriggeringTask)
{
  _static_

  pCurrOp->vPipeToNextOp(pOperand,pTriggeringTask);
}


/*----------------------------------------------------------------------------*/
/* append operand onto next operator in pipeline                              */
/*----------------------------------------------------------------------------*/
void AM::OP::vPipeToOp(Op* pOp, Operand* pOperand,
                       Task* pTriggeringTask)
{
  _static_

  pOp->vPipeToThis(pOperand,pTriggeringTask);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                 OPERAND                                    */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* is operand an out-of-order operand?                                        */
/*----------------------------------------------------------------------------*/
bool AM::OPERAND::bIsOoO(Operand* pOperand)
{
  _static_
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is it a marker ?
  
  if (pOperand->bIsMarker()) {
  
    Marker* pMarker = _pMarker(pOperand);
    if (pMarker->eOpMarker() == OM_CANCELLING)
      return true;

    if (pMarker->eOpMarker() == OM_ERRORINGMARKER)
      return true;

    if (pMarker->eOpMarker() == OM_LEAVINGMARKER)
      return true;

    if (pMarker->eOpMarker() == OM_NEXTBUFFERMARKER)
      return true;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is it a infoset ?
  
  if (CN_INFOSET == pOperand->eGetClassNo()) {
    InfoSet* pInfoSet = _pInfoSet(pOperand);
    
    eOI eOpInfoSet = pInfoSet->eOpInfoSet();
    
//    if (OI_PROGRESSUNITS == eOpInfoSet)
//      return true;
//    if (OI_SEEKOFFSETS   == eOpInfoSet)
//      return true;
//    if (OI_TOTALUNITS    == eOpInfoSet)
//      return true;

  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  return false;
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                               PARCEL                                       */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* recv contents of parcel from given socket                                  */
/*----------------------------------------------------------------------------*/
void AM::PARCEL::vRecvContents(Parcel* pParcel, Socket* pSocket)
{
  _static_
  
  pParcel->vRecvContents(pSocket);
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                               POOL                                         */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* get free space in pool                                                     */
/*----------------------------------------------------------------------------*/
i8 AM::POOL::iFreeSpace(void)
{
  _static_
  
  i8 nFree = gpBufferPool->iFreeSpace();;
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* get percentage free space in pool                                          */
/*----------------------------------------------------------------------------*/
i8 AM::POOL::iPctFree(void)
{
  _static_
  
  i8 nFree  = iFreeSpace();
  i8 nTotal = iTotalSpace();
  
  i8 iPct = IntLib::iPercent(nFree,nTotal);

  
  return iPct;
}

/*----------------------------------------------------------------------------*/
/* get percentage used space in pool                                          */
/*----------------------------------------------------------------------------*/
i8 AM::POOL::iPctUsed(void)
{
  _static_
  
  i8 nUsed  = iUsedSpace();
  i8 nTotal = iTotalSpace();
  
  i8 iPct = IntLib::iPercent(nUsed,nTotal);
  
  return iPct;
}

/*----------------------------------------------------------------------------*/
/* get total space in pool                                                    */
/*----------------------------------------------------------------------------*/
i8 AM::POOL::iTotalSpace(void)
{
  _static_
  
  i8 nTotal = AM::iRegionSize();
  
  return nTotal;
}

/*----------------------------------------------------------------------------*/
/* get used space in pool                                                     */
/*----------------------------------------------------------------------------*/
i8 AM::POOL::iUsedSpace(void)
{
  _static_
  
  i8 nUsed = AM::iRegionSize()
               - gpBufferPool->iFreeSpace()
               - PoolHelper::iPoolFree()
             ;

  return nUsed;
}


/*----------------------------------------------------------------------------*/
/* get free chunks in data heap                                               */
/*----------------------------------------------------------------------------*/
i8 AM::POOL::nDataHeap(void)
{
  _static_

  i8 nBufferCount = gpAttachMap->m_xHeaps.Transient.m_pData->nBufferCount();
  
  return nBufferCount;
}

/*----------------------------------------------------------------------------*/
/* get free chunks in large heap                                              */
/*----------------------------------------------------------------------------*/
i8 AM::POOL::nLargeHeap(void)
{
  _static_

  i8 nBufferCount = gpAttachMap->m_xHeaps.Transient.m_pLarge->nBufferCount();
  
  return nBufferCount;
}

/*----------------------------------------------------------------------------*/
/* get free chunks in medium heap                                             */
/*----------------------------------------------------------------------------*/
i8 AM::POOL::nMediumHeap(void)
{
  _static_

  i8 nBufferCount = gpAttachMap->m_xHeaps.Transient.m_pMedium->nBufferCount();
  
  return nBufferCount;
}

/*----------------------------------------------------------------------------*/
/* get free chunks in small heap                                              */
/*----------------------------------------------------------------------------*/
i8 AM::POOL::nSmallHeap(void)
{
  _static_

  i8 nBufferCount = gpAttachMap->m_xHeaps.Transient.m_pSmall->nBufferCount();
  
  return nBufferCount;
}

/*----------------------------------------------------------------------------*/
/* get free chunks in tiny heap                                               */
/*----------------------------------------------------------------------------*/
i8 AM::POOL::nTinyHeap(void)
{
  _static_

  i8 nBufferCount = gpAttachMap->m_xHeaps.Transient.m_pTiny->nBufferCount();
  
  return nBufferCount;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                               PROMPT                                       */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* translate token vector into a pipeline statement                           */
/*----------------------------------------------------------------------------*/
bool AM::PROMPT::bToPipelineStmt(String* pAsPipelineStmt,
                                TokenVec* pFromTokenVec)
{
  _static_
  
  i8 nTkns = pFromTokenVec->nTokens();
  
  Stz* szOpNm;
  Stz* szOpArg;
  
  char cStageStart   = 'A';
  char cStageCurrent = 'A';

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pAsPipelineStmt->vCopy(
    "pipeline unnamed\n"
    "begin\n"
    ""_Stz);

  pAsPipelineStmt->vCatSprint(
    "  stage stage%c on origin :\n"
    "    start\n"
    "    | pulse once\n"
    "    | stageB\n"
    ""_Stz,
  cStageStart);

  ++ cStageCurrent;
  
  pAsPipelineStmt->vCatSprint(
    "  stage stage%c on all partitions :\n"
    ""_Stz,
    cStageCurrent);
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  for (i8 i_Tkn = 0; i_Tkn < nTkns; /*none*/) {

    szOpNm = pFromTokenVec->pGet(i_Tkn++)->szChars();

    // -- -- -- -- -- -- -- -- -- --
    
    if (szOpNm->bSame("scan"_Stz)) {
      
      szOpArg = pFromTokenVec->pGet(i_Tkn++)->szChars();

      pAsPipelineStmt->vCatSprint("    scan %s\n"_Stz,szOpArg);
      
      while (i_Tkn < nTkns) {
        szOpArg = pFromTokenVec->pGet(i_Tkn++)->szChars();
        if (szOpArg->bSame("|"_Stz))
          break;
      }
      
    }
    
    // -- -- -- -- -- -- -- -- -- --
    
    else
    if (szOpNm->bSame("filter"_Stz)) {
      
      pAsPipelineStmt->vCatSprint("    | filter 0"_Stz);
      pAsPipelineStmt->vCatSprint(" \""_Stz);
      
      while (i_Tkn < nTkns) {
        szOpArg = pFromTokenVec->pGet(i_Tkn++)->szChars();
        if (szOpArg->bSame("|"_Stz))
          break;
        pAsPipelineStmt->vCatSprint(" %s"_Stz,szOpArg);
      }
      
      pAsPipelineStmt->vCatSprint(" <>\"\n"_Stz);
    }
    
    // -- -- -- -- -- -- -- -- -- --
    
    else
    if (szOpNm->bSame("project"_Stz)) {
      
      pAsPipelineStmt->vCatSprint("    | project 0"_Stz);
      pAsPipelineStmt->vCatSprint(" \""_Stz);
      
      while (i_Tkn < nTkns) {
        szOpArg = pFromTokenVec->pGet(i_Tkn++)->szChars();
        if (szOpArg->bSame("|"_Stz))
          break;
        pAsPipelineStmt->vCatSprint("%s "_Stz,szOpArg);
      }
      
      pAsPipelineStmt->vCatSprint("\"\n"_Stz);
            
    }
    
    // -- -- -- -- -- -- -- -- -- --
    
    else
    if (szOpNm->bSame("aggreg"_Stz)) {
      
      pAsPipelineStmt->vCatSprint(
        "    | stage%c\n"
        ""_Stz,
        cStageCurrent + 1);

      ++ cStageCurrent;
      
      pAsPipelineStmt->vCatSprint(
        "  stage stage%c on origin :\n"
        ""_Stz,
        cStageCurrent);
      
      pAsPipelineStmt->vCatSprint("    aggreg 0"_Stz);
      pAsPipelineStmt->vCatSprint(" \""_Stz);
      
      while (i_Tkn < nTkns) {
        szOpArg = pFromTokenVec->pGet(i_Tkn++)->szChars();
        if (szOpArg->bSame("|"_Stz))
          break;
        pAsPipelineStmt->vCatSprint("%s "_Stz,szOpArg);
      }
      
      pAsPipelineStmt->vCatSprint("\"\n"_Stz);
            
    }
    
    // -- -- -- -- -- -- -- -- -- --
    
    else {
      
      return false;
    }
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pAsPipelineStmt->vCatSprint(
    "    | stage%c\n"
    ""_Stz,
    cStageCurrent + 1);
  
  ++ cStageCurrent;
  
  pAsPipelineStmt->vCatSprint(
    "  stage stage%c on origin :\n"
    "    output\n"
    "    | sink\n"
    "    | stage%c\n"
    ""_Stz,
    cStageCurrent, cStageStart);

  pAsPipelineStmt->vConcat(
    "end ;\n"
    ""_Stz);
  
  vTrace("bToPipelineStmt : \n%s\n"_Stz,pAsPipelineStmt->szStz());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pipe string out to prompt                                                  */
/*----------------------------------------------------------------------------*/
InfoSet* AM::PROMPT::pToPromptOut(Stz* szStz)
{
  _static_
  
  StreamDef* pStreamDef = AM::pInternalTables()->pStreamDef(TI_PROMPTOUT);
  if (! pStreamDef)
    return nullptr;

  InfoSet* pInfoSet = new InfoSet(pStreamDef);
  pInfoSet->vOpInfoSet(OI_PROMPTOUT);
  
  Row* pRow = pInfoSet->pAllotRow();
  
  pInfoSet->vDepositRow(pRow,C_VARGS_BEGIN,szStz,C_VARGS_END);
  
  return pInfoSet;
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                PTNMAP                                      */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* convert given hash to distributed partition number                         */
/*----------------------------------------------------------------------------*/
PtnId AM::PTNMAP::tHashToLglPtnId(i8 iHash)
{
  _static_

  return gpAttachMap->m_pPtnMap->tHashToLglPtnId(iHash);
}


/*----------------------------------------------------------------------------*/
/* get number of nodes as known by ptnmap                                     */
/*----------------------------------------------------------------------------*/
i8 AM::PTNMAP::nKnownNodes(void)
{
  _static_

  return gpAttachMap->m_pPtnMap->nKnownNodes();
}

/*----------------------------------------------------------------------------*/
/* get number of partitions in grid                                           */
/*----------------------------------------------------------------------------*/
i8 AM::PTNMAP::nPtnsInGrid(void)
{
  _static_

  return gpAttachMap->m_pPtnMap->nPtnsInGrid();
}

/*----------------------------------------------------------------------------*/
/* get number of partitions per node                                          */
/*----------------------------------------------------------------------------*/
i8 AM::PTNMAP::nPtnsPerNode(void)
{
  return gpAttachMap->m_pPtnMap->nPtnsPerNode();
}

/*----------------------------------------------------------------------------*/
/* convert given round robin to distributed partition number                  */
/*----------------------------------------------------------------------------*/
PtnId AM::PTNMAP::tRobinToDtdPtnStream(i8 iRobin)
{
  _static_
  
  return gpAttachMap->m_pPtnMap->tRobinToDtdPtnStream(iRobin);
}

/*----------------------------------------------------------------------------*/
/* convert given round robin to local partition number                        */
/*----------------------------------------------------------------------------*/
PtnId AM::PTNMAP::tRobinToLclPtnStream(i8 iRobin)
{
  _static_
  
  return gpAttachMap->m_pPtnMap->tRobinToLclPtnStream(iRobin);
}

/*----------------------------------------------------------------------------*/
/* convert given round robin to distributed partition number                  */
/*----------------------------------------------------------------------------*/
PtnId AM::PTNMAP::tRobinToNdePtnNo(i8 iRobin)
{
  _static_
  
  return gpAttachMap->m_pPtnMap->tRobinToNdePtnNo(iRobin);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                               SCHEDULER                                    */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* where                                                                      */
/*----------------------------------------------------------------------------*/
i8 AM::SCHEDULER::nIdleThreads(void)
{
  _static_
  
  Schedule* pSchedule = gpAttachMap->m_pSchedule;
  
  i8 nIdle = pSchedule->nIdleThreads();
  
  return nIdle;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                  SE                                        */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* where                                                                      */
/*----------------------------------------------------------------------------*/
void AM::SE::vWhereAmI(void)
{
  _static_
  
  printf("AM::SE::vWhereAmI()\n");
  
  HeadsUp oHeadsUp((Stz*)__func__);
  
//  SP_Pointer spThread;
//  Thread* pThread = MY::pThread();
//  vTrace("Thread %s\n"_Stz,spThread.sz(pThread));
//
//  SP_Pointer spTask;
  Task* pTask = MY::pTask();
//  vTrace("Task %s\n"_Stz,spTask.sz(pTask));
//
//  SP_Pointer spJob;
//  Job* pJob = MY::pJob();
//  vTrace("Job %s\n"_Stz,spJob.sz(pJob));
//
//  SP_Pointer spOp;
//  Op* pOp = MY::pOp();
//  vTrace("Op %s\n"_Stz,spOp.sz(pOp));
//
//  SP_Pointer spLogicUnit;
//  LogicUnit* pLogicUnit = MY::pLogicUnit();
//  vTrace("LogicUnit %s\n"_Stz,spLogicUnit.sz(pLogicUnit));
//
//  vTrace("\n"_Stz);
  
  if (EC_NONE == pTask->eValidObject()) {
    pTask->vDump();
  }
  
  
  AM::pSchedule()->vDumpBidingSet();
  AM::pSchedule()->vDumpExecutingSet();
  AM::pSchedule()->vDumpReadyQueue();
  AM::pSchedule()->vDumpSleepingQueue();
  AM::pSchedule()->vDumpWaitingSet();

  AM::pSchedule()->vThreadsReport();

  vTrace("\n"_Stz);
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                SESSION                                     */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* get the internal session subscript                                         */
/*----------------------------------------------------------------------------*/
i8 AM::SESSION::iInternalSession(void)
{
  _static_
  
  return gpAttachMap->m_pSessions->pInternalSession()->m_i_Session;
}

/*----------------------------------------------------------------------------*/
/* get the internal session                                                   */
/*----------------------------------------------------------------------------*/
Session* AM::SESSION::pInternalSession(void)
{
  _static_
  
  return gpAttachMap->m_pSessions->pInternalSession();
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                 SHOW                                       */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* show important point during boot?                                          */
/*----------------------------------------------------------------------------*/
bool AM::SHOW::bBoot(void)
{
  _static_
  
  if (gpAttachMap->m_xSysArgs.m_oArgTags.bHasName(C_SHOW_BOOT))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* show dir calls ?                                                           */
/*----------------------------------------------------------------------------*/
bool AM::SHOW::bDir(void)
{
  _static_
  
  if (gpAttachMap->m_xSysArgs.m_oArgTags.bHasName(C_SHOW_DIR))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* show dir call failures ?                                                   */
/*----------------------------------------------------------------------------*/
bool AM::SHOW::bDir_Failure(void)
{
  _static_
  
  if (gpAttachMap->m_xSysArgs.m_oArgTags.bHasName(C_SHOW_DIR_FAILURE))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* show inter-node connection?                                                */
/*----------------------------------------------------------------------------*/
bool AM::SHOW::bConnection(void)
{
  _static_
  
  if (gpAttachMap->m_xSysArgs.m_oArgTags.bHasName(C_SHOW_CONNECTION))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* show exception processing?                                                 */
/*----------------------------------------------------------------------------*/
bool AM::SHOW::bException(void)
{
  _static_
  
  if (gpAttachMap->m_xSysArgs.m_oArgTags.bHasName(C_SHOW_EXCEPTION))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* show file calls ?                                                          */
/*----------------------------------------------------------------------------*/
bool AM::SHOW::bFile(void)
{
  _static_
  
  if (gpAttachMap->m_xSysArgs.m_oArgTags.bHasName(C_SHOW_FILE))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* show file call failures ?                                                  */
/*----------------------------------------------------------------------------*/
bool AM::SHOW::bFile_Failure(void)
{
  _static_
  
  if (gpAttachMap->m_xSysArgs.m_oArgTags.bHasName(C_SHOW_FILE_FAILURE))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* show exception processing?                                                 */
/*----------------------------------------------------------------------------*/
bool AM::SHOW::bLowMem(void)
{
  _static_
  
  if (gpAttachMap->m_xSysArgs.m_oArgTags.bHasName(C_SHOW_LOWMEM))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* show exception processing?                                                 */
/*----------------------------------------------------------------------------*/
bool AM::SHOW::bLowMem_Recv(void)
{
  _static_
  
  if (gpAttachMap->m_xSysArgs.m_oArgTags.bHasName(C_SHOW_LOWMEM_RECV))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* show recurring information?                                                */
/*----------------------------------------------------------------------------*/
bool AM::SHOW::bRecurring(void)
{
  _static_
  
  if (gpAttachMap->m_xSysArgs.m_oArgTags.bHasName(C_SHOW_RECURRING))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* show socket calls ?                                                        */
/*----------------------------------------------------------------------------*/
bool AM::SHOW::bSocket(void)
{
  _static_
  
  if (gpAttachMap->m_xSysArgs.m_oArgTags.bHasName(C_SHOW_SOCKET))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* show socket call failures ?                                                */
/*----------------------------------------------------------------------------*/
bool AM::SHOW::bSocket_Failure(void)
{
  _static_
  
  if (gpAttachMap->m_xSysArgs.m_oArgTags.bHasName(C_SHOW_SOCKET_FAILURE))
    return true;
  else
    return false;
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                  STAGE                                     */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* get operator                                                               */
/*----------------------------------------------------------------------------*/
Op* AM::STAGE::pGetOp(Stage* pStage, PtnId tPtnId, i8 i_Op)
{
  _static_
  
  Op* pOp = pStage->pPipeDstOp(tPtnId,i_Op);
  
  return pOp;

}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                STARTER                                     */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* get the starter                                                            */
/*----------------------------------------------------------------------------*/
Starter* AM::STARTER::pStarter(void)
{
  _static_
  
  return gpAttachMap->m_pStarter;
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                  STR                                       */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void AM::STR::vReplaceTags(Stz* szDst, Stz* szSrc, TagSet* pTags)
{
  _static_
  
  szDst->vCopy(szSrc);
  
  i8 nEnames = pTags->nItems();
  for (i8 i_Tag = 0; i_Tag < nEnames; i_Tag++) {
    if (! pTags->bIsString(i_Tag))
      continue;
    
    Stz* szName  = pTags->szName(i_Tag);
    Stz* szValue = pTags->szValue(i_Tag);
    
    IGNORE_RETURN( StrLib::bReplace(szDst->azChar(),
                                    szName->azChar(),
                                    szValue->azChar()) );
  }

}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                SYSARGS                                     */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* get number of defines nodes as per command line                            */
/*----------------------------------------------------------------------------*/
i8 AM::SYSARGS::nDefinedNodes(void)
{
  _static_
  
  return gpAttachMap->m_xSysArgs.m_nDefinedNodes;
}


/*----------------------------------------------------------------------------*/
/* get collect deref metrics option                                           */
/*----------------------------------------------------------------------------*/
bool AM::SYSARGS::bDerefMetrics(void)
{
  _static_
  
  if (gpAttachMap)
    return gpAttachMap->m_xSysArgs.m_bDerefMetrics;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set collect deref metrics option                                           */
/*----------------------------------------------------------------------------*/
void AM::SYSARGS::vDerefMetrics(bool bDerefMetrics)
{
  _static_
  
  gpAttachMap->m_xSysArgs.m_bDerefMetrics = bDerefMetrics;
}


/*----------------------------------------------------------------------------*/
/* get collect job metrics option                                             */
/*----------------------------------------------------------------------------*/
bool AM::SYSARGS::bJobMetrics(void)
{
  _static_
  
  if (gpAttachMap)
    return gpAttachMap->m_xSysArgs.m_bJobMetrics;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set collect job metrics option                                             */
/*----------------------------------------------------------------------------*/
void AM::SYSARGS::vJobMetrics(bool bJobMetrics)
{
  _static_
  
  gpAttachMap->m_xSysArgs.m_bJobMetrics = bJobMetrics;
}



/*----------------------------------------------------------------------------*/
/* get lock tracking option                                                   */
/*----------------------------------------------------------------------------*/
bool AM::SYSARGS::bLockTracking(void)
{
  _static_
  
  if (gpAttachMap)
    return gpAttachMap->m_xSysArgs.m_bLockTracking;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set lock tracking option                                                   */
/*----------------------------------------------------------------------------*/
void AM::SYSARGS::vLockTracking(bool bLockTracking)
{
  _static_
  
  gpAttachMap->m_xSysArgs.m_bLockTracking = bLockTracking;
}



/*----------------------------------------------------------------------------*/
/* get mertics sleep                                                          */
/*----------------------------------------------------------------------------*/
i8 AM::SYSARGS::nMetricsSleep(void)
{
  _static_
  
  return gpAttachMap->m_xSysArgs.m_nMetricsSleep;
}

/*----------------------------------------------------------------------------*/
/* set mertics sleep                                                          */
/*----------------------------------------------------------------------------*/
void AM::SYSARGS::vMetricsSleep(i8 nMetricsSleep)
{
  _static_
  
  gpAttachMap->m_xSysArgs.m_nMetricsSleep = nMetricsSleep;
}






/*----------------------------------------------------------------------------*/
/* get operand yield                                                          */
/*----------------------------------------------------------------------------*/
i8 AM::SYSARGS::nOperandYield(void)
{
  _static_
  
  return gpAttachMap->m_xSysArgs.m_nOperandYield;
}

/*----------------------------------------------------------------------------*/
/* set network optimisation level                                             */
/*----------------------------------------------------------------------------*/
void AM::SYSARGS::vOperandYield(i8 nOperandYield)
{
  _static_
  
  gpAttachMap->m_xSysArgs.m_nOperandYield = nOperandYield;
}


/*----------------------------------------------------------------------------*/
/* get optimisation level                                                     */
/*----------------------------------------------------------------------------*/
i8 AM::SYSARGS::iOptimiserLvl(void)
{
  _static_
  
  return gpAttachMap->m_xSysArgs.m_iOptimiserLvl;
}

/*----------------------------------------------------------------------------*/
/* set optimisation level                                                     */
/*----------------------------------------------------------------------------*/
void AM::SYSARGS::vOptimiserLvl(i8 iOptimiserLvl)
{
  _static_
  
  gpAttachMap->m_xSysArgs.m_iOptimiserLvl = iOptimiserLvl;
}






/*----------------------------------------------------------------------------*/
/* get number of partitions per node as per command line                      */
/*----------------------------------------------------------------------------*/
i8 AM::SYSARGS::nPtnsPerNode(void)
{
  _static_
  
  return gpAttachMap->m_xSysArgs.m_nPtnsPerNode;
}

/*----------------------------------------------------------------------------*/
/* get collect sys metrics option                                             */
/*----------------------------------------------------------------------------*/
bool AM::SYSARGS::bSysMetrics(void)
{
  _static_
  
  if (gpAttachMap)
    return gpAttachMap->m_xSysArgs.m_bSysMetrics;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set collect sys metrics option                                             */
/*----------------------------------------------------------------------------*/
void AM::SYSARGS::vSysMetrics(bool bSysMetrics)
{
  _static_
  
  gpAttachMap->m_xSysArgs.m_bSysMetrics = bSysMetrics;
}

/*----------------------------------------------------------------------------*/
/* get number of general threads                                              */
/*----------------------------------------------------------------------------*/
i8 AM::SYSARGS::nThreads(void)
{
  return gpAttachMap->m_xSysArgs.m_nThreads;
}

/*----------------------------------------------------------------------------*/
/* inc number of general threads                                              */
/*----------------------------------------------------------------------------*/
void AM::SYSARGS::vThreadsInc(i8 iBy)
{
  gpAttachMap->m_xSysArgs.m_nThreads += iBy;
}

/*----------------------------------------------------------------------------*/
/* get sysargs tag set                                                        */
/*----------------------------------------------------------------------------*/
TagSet* AM::SYSARGS::pTagSet(void)
{
  _static_
  
  return & gpAttachMap->m_xSysArgs.m_oArgTags;
}

/*----------------------------------------------------------------------------*/
/* set verbose mode                                                           */
/*----------------------------------------------------------------------------*/
void AM::SYSARGS::vVerbose(bool bVerbose)
{
  _static_

  gpAttachMap->m_xSysArgs.m_bVerbose = bVerbose;
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                 SYSTEM                                     */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* get number of active jobs in the system                                    */
/*----------------------------------------------------------------------------*/
i8 AM::SYSTEM::nActiveJobs(void)
{
  _static_
    
  i8 nActiveJobs =   gpAttachMap->m_pJobs->pLocalJobs()->nJobs()
                   + gpAttachMap->m_pJobs->pOriginatingJobs()->nJobs()
                   + gpAttachMap->m_pJobs->pParticipatingJobs()->nJobs()
                   ;
  
  return nActiveJobs;
}

/*----------------------------------------------------------------------------*/
/* elapsed time (in nanoseconds) since boot                                   */
/*----------------------------------------------------------------------------*/
i8 AM::SYSTEM::nNanoseconds(void)
{
  _static_
  
  gpAttachMap->m_oSince.vUpdate();
  
  i8 nElapsed = gpAttachMap->m_oSince.iNanoseconds();
    
  return nElapsed;
}

/*----------------------------------------------------------------------------*/
/* passes the low-memory condition?                                           */
/*----------------------------------------------------------------------------*/
bool AM::SYSTEM::bPassesLowMem(Job* pJob)
{
  _static_

  if (! gpBufferPool->bLowMemory())
    return true;
  
  pJob->pJobTags()->vContentDump();
  
  if (pJob->bHasTag(C_TAG_LOWMEMORY))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* passes the open-system condition?                                          */
/*----------------------------------------------------------------------------*/
bool AM::SYSTEM::bPassesOpenSystem(Job* pJob)
{
  _static_

  if (bSystemOpen())
    return true;
    
  if (MY::pSession()->m_bSystemCloser)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get reap enabled?                                                          */
/*----------------------------------------------------------------------------*/
bool AM::SYSTEM::bReapEnabled(void)
{
  _static_
  
  if (! gpAttachMap)
    return false;

  return gpAttachMap->m_bReapEnabled;
}

/*----------------------------------------------------------------------------*/
/* set reap enabled?                                                          */
/*----------------------------------------------------------------------------*/
void AM::SYSTEM::vReapEnabled(bool bReapEnabled)
{
  _static_

  gpAttachMap->m_bReapEnabled = bReapEnabled;
}

/*----------------------------------------------------------------------------*/
/* get system started                                                         */
/*----------------------------------------------------------------------------*/
bool AM::SYSTEM::bStarted(void)
{
  _static_
  
  if (! gpAttachMap)
    return false;

  return gpAttachMap->m_bStarted;
}

/*----------------------------------------------------------------------------*/
/* set system started                                                         */
/*----------------------------------------------------------------------------*/
void AM::SYSTEM::vStarted(bool bStarted)
{
  _static_

  gpAttachMap->m_bStarted = bStarted;
}

/*----------------------------------------------------------------------------*/
/* is system open?                                                            */
/*----------------------------------------------------------------------------*/
bool AM::SYSTEM::bSystemOpen(void)
{
  _static_
  
  if (! gpAttachMap)
    return false;

  return gpAttachMap->m_bSystemOpen;
}

/*----------------------------------------------------------------------------*/
/* set system open                                                            */
/*----------------------------------------------------------------------------*/
void AM::SYSTEM::vSystemOpen(bool bSystemOpen)
{
  _static_

  gpAttachMap->m_bSystemOpen = bSystemOpen;
}

/*----------------------------------------------------------------------------*/
/* is the system ready?                                                       */
/*----------------------------------------------------------------------------*/
bool AM::SYSTEM::bReady(void)
{
  _static_

  return gpAttachMap->m_bReady;
}

/*----------------------------------------------------------------------------*/
/* Writes out a file that indicates that the system has booted and is ready   */
/*----------------------------------------------------------------------------*/
void AM::SYSTEM::vSystemReady(void)
{
  _static_

  i8 iPid = AM::iPid();
  s32 sPid;
  sPid.vSprint("%lld\n"_Stz, iPid);

  // Path is "../" because we have already changed the cwd
  // to the commands subdirectory
  const Stz* C_READY_FILE_PATH = "../se.ready"_Stz;

  if (File::bExists(const_cast<Stz*>(C_READY_FILE_PATH), false)) {
    File::bRemove(const_cast<Stz*>(C_READY_FILE_PATH));
  }

  File pReadyFile(gpAttachMap);
  b1 bOpen = pReadyFile.bOpenWrite(const_cast<Stz*>(C_READY_FILE_PATH));

  if (bOpen) {
    // Write out the pid
    pReadyFile.bWriteBytes(sPid.azChar(), sPid.nLength());

  } else {
    vTrace("Unable to open the ready file: %s\n"_Stz, C_READY_FILE_PATH);
  }

  pReadyFile.vClose();
  
  gpAttachMap->m_bReady = true;
  
  if (AM::SHOW::bBoot()) {
    vTrace("SYSTEM READY: pid %lld; refer %s\n"_Stz
           ,iPid
           ,C_READY_FILE_PATH);
  }
}

/*----------------------------------------------------------------------------*/
/* get system uptime (in seconds) since boot                                   */
/*----------------------------------------------------------------------------*/
i8 AM::SYSTEM::nUptimeInSeconds(void)
{
  _static_
  
  gpAttachMap->m_oSince.vUpdate();
  
  i8 nSeconds = gpAttachMap->m_oSince.iSeconds();
    
  return nSeconds;
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                    Task                                    */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* get task dic stack                                                         */
/*----------------------------------------------------------------------------*/
DicStack* AM::TASK::pDicStack(void)
{
  _static_
  
  Task* pTask = pThisTLSTask();
  if (! pTask)
    return nullptr;
  
  DicStack* pDicStack = pTask->pDicStack();
  
  return pDicStack;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void AM::TASK::vRetire(Task* pTask)
{
  _static_
  
  if (Op* pOp = pTask->pOp()) {
    // retiring op of job
    Job* pJob = pOp->pJob();
    pJob->vDecActiveOps();
  }
  else
  if (Job* pJob = pTask->pJob_task()) {
    // retiring job
    //pJob->vReapable(true);
//    if (pJob->bIsPipelineLocal()) {
//      Op* pOp = pJob->pSingularOp();
//    }
  }
  else {
    NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* trigger biding task                                                        */
/*----------------------------------------------------------------------------*/
void AM::TASK::vTriggerBiding(Task* pTaskBiding, Task* pTriggeringTask)
{
  _static_
  
  Scheduler::vTaskCross_TriggerBiding(pTaskBiding,pTriggeringTask);
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                   THREAD                                   */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* dedicated thread vector                                                    */
/*----------------------------------------------------------------------------*/
ThreadVec* AM::THREAD::pDedicatedThreadVec(void)
{
  _static_
  
  ThreadVec* pThreadVec = AM::pSchedule()->pDedicatedThreadVec();
  
  return pThreadVec;
}

/*----------------------------------------------------------------------------*/
/* delete thread                                                              */
/*----------------------------------------------------------------------------*/
void AM::THREAD::vDeleteThread(Thread* pThread)
{
  _static_
    
  delete pThread;
}

/*----------------------------------------------------------------------------*/
/* general thread vector                                                      */
/*----------------------------------------------------------------------------*/
ThreadVec* AM::THREAD::pGeneralThreadVec(void)
{
  _static_
  
  ThreadVec* pThreadVec = AM::pSchedule()->pGeneralThreadVec();
  
  return pThreadVec;
}

/*----------------------------------------------------------------------------*/
/* new thread                                                                 */
/*----------------------------------------------------------------------------*/
Thread* AM::THREAD::pNewThread(void)
{
  _static_
  
  Thread* pThread = new Thread;
  
  return pThread;
}

/*----------------------------------------------------------------------------*/
/* get this thread via tls                                                    */
/*----------------------------------------------------------------------------*/
Thread* AM::THREAD::pThisThread(void)
{
  _static_
  
  Thread* pThread = Thread::pThisTLSThread();
  
  return pThread;
}

/*----------------------------------------------------------------------------*/
/* get this threadNo via tls                                                  */
/*----------------------------------------------------------------------------*/
i8 AM::THREAD::iThisThreadNo(void)
{
  _static_

  Thread* pThread = Thread::pThisTLSThread();
  i8 iThreadNo = pThread->iThreadNo();
  
  return iThreadNo;
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                    TRACE                                   */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* long verbose format                                                        */
/*----------------------------------------------------------------------------*/
bool AM::TRACE::bVBlong(void)
{
  _static_
  
  return gxCmdLineArgs.m_xSysArgs.m_bVBlong;
}

/*----------------------------------------------------------------------------*/
/* verbose trace given tag?                                                   */
/*----------------------------------------------------------------------------*/
bool AM::TRACE::bVerbose(Stz* szVBtag)
{
  _static_
  
  TagSet* pTags;
  
  if (! gpAttachMap) {
  
    if (gxCmdLineArgs.m_xSysArgs.m_bVerbose)
      return true;
    
    pTags = & gxCmdLineArgs.m_xSysArgs.m_oArgTags;
    if (pTags) {
      if (pTags->bHasName(szVBtag))
        return true;
    }

    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (gpAttachMap->m_xSysArgs.m_bVerbose)
    return true;
  
  //if (! gpAttachMap->m_xSysArgs.m_bDeveloper)
  //  return false;

  pTags = & gpAttachMap->m_xSysArgs.m_oArgTags;
  if (pTags) {
    if (pTags->bHasName(szVBtag))
      return true;
  }

  Job* pJob = MY::pJob();
  Op* pOp = MY::pOp();
  
  if (! pJob)
    return false;


  pTags = pJob->pJobTags();
  if (pTags) {
    if (pTags->bHasName(szVBtag))
      return true;
  }
  
  pTags = AM::pDbgTagSet();
  if (pTags) {
    if (pTags->bHasName(szVBtag))
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* verbose trace given tags?                                                  */
/*----------------------------------------------------------------------------*/
bool AM::TRACE::bVerbose(Stz* szVBtag1, Stz* szVBtag2)
{
  _static_
  
  if (AM::TRACE::bVerbose(szVBtag1))
    return true;
  
  if (AM::TRACE::bVerbose(szVBtag2))
    return true;

  return false;
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                    TIMER                                   */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* get reference timer                                                        */
/*----------------------------------------------------------------------------*/
Timer* AM::TIMER::pReferenceTimer(void)
{
  _static_
  
  return & gpAttachMap->m_oReferenceTimer;
}

/*----------------------------------------------------------------------------*/
/* get since timer                                                            */
/*----------------------------------------------------------------------------*/
Timer* AM::TIMER::pSinceTimer(void)
{
  _static_
  
  return & gpAttachMap->m_oSince;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                   Word                                     */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* disassemble a COLON                                                         */
/*----------------------------------------------------------------------------*/
void AM::WORD::vDisassemble(Word* pWord, DicStack* pDicStack, CodeCell* aInxPtr)
{
  _static_
  
  s48K sString;
  
  SP_Word spWord;
  vTrace("\n"_Stz);
  vTrace("%s\n"_Stz,spWord.sz(pWord));
 
  CodeCell* aDoes = pWord->aEntryCell();
  
  Word* pDoesWord = CodeBlock::pToWord(aDoes);
  
  vTrace("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n"_Stz);

  AM::vWordSeeDoes(pDoesWord,aDoes,pDicStack,aInxPtr);
  
  vTrace("=============================================================\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* find given word                                                            */
/*----------------------------------------------------------------------------*/
Word* AM::WORD::pFind(Stz* szWordNm)
{
  _static_
  
  Dic* pRootDic = AM::DIC::pRoot();
  
  Word* pWord = pRootDic->pFindWord(szWordNm);
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* list contents of word                                                      */
/*----------------------------------------------------------------------------*/
void AM::WORD::vListContents(Word* pWord, i8 i_Depth)
{
  _static_
  
  Word* pKind = nullptr;
  
  Object* pObject = pWord->pObject();
  if (! pObject)
    return;
  
  Word* pCategory = pWord->pCategory();

  if (CN_DIC == pObject->eGetClassNo())
    pKind = ((Dic*)pObject)->pDicKind();
  
  vTrace("%s%s %s %s\n"_Stz,
         Stz::szTab(i_Depth),
         pWord->szWordNm(),
         pCategory ? pCategory->szWordNm() : "n/a"_Stz,
         pKind ? pKind->szWordNm() : "n/a"_Stz
         );

  if (CN_DIC == pObject->eGetClassNo())
    ((Dic*)pObject)->vListContents(i_Depth+1);
  else
  if (CN_TABLE == pObject->eGetClassNo())
    ((Table*)pObject)->vListContents(i_Depth+1);
  else
  if (CN_INDEX == pObject->eGetClassNo())
    ((Index*)pObject)->vListContents(i_Depth+1);

}




/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

Stz*  AM::szVersionString(void) { return const_cast<Stz*>(C_VERSION_STRING); }
i8    AM::iVersionNumber(void)  { return C_VERSION_NUMBER; }

Stz*  AM::szFormatString(void)  { return const_cast<Stz*>(C_VERSION_STRING);  }  // For now
i8    AM::iFormatNumber(void)   { return C_VERSION_NUMBER;  }                    // For now

// Arg
bool AM::bDebugIndex(void)    { return gpAttachMap->m_xSysArgs.m_bDebugIndex;        }
bool AM::bDebugMode(void)     { return gpAttachMap->m_xSysArgs.m_bDebugMode;         }
bool AM::bDeveloper(void)     { return gpAttachMap->m_xSysArgs.m_bDeveloper;         }
i8   AM::nDummyLoop(void)     { return gpAttachMap->m_xSysArgs.m_nDummyLoop;         }
i8   AM::nDummySleep(void)    { return gpAttachMap->m_xSysArgs.m_nDummySleep;        }
i8   AM::nDummyThreads(void)  { return gpAttachMap->m_xSysArgs.m_nDummyThreads;      }
i8   AM::nMaxLoadErrors(void) { return gpAttachMap->m_xSysArgs.m_nMaxLoadErrors;     }
i8   AM::nMaxReadyLP(void)    { return gpAttachMap->m_xSysArgs.m_nMaxReadyLP;        }
bool AM::bMunged(void)        { return gpAttachMap->m_xSysArgs.m_bMunged;            }
Stz* AM::szRegionName(void)   { return gpAttachMap->m_xSysArgs.m_sRegionName.szStz();}
bool AM::bRLUDiary(void)      { return gpAttachMap->m_xSysArgs.m_bRLUDiary;          }
bool AM::bRLUTimer(void)      { return gpAttachMap->m_xSysArgs.m_bRLUTimer;          }
i8   AM::iRegionSize(void)    { return gpAttachMap->m_xSysArgs.m_iRegionSize;        }
i8   AM::nThreads(void)       { return gpAttachMap->m_xSysArgs.m_nThreads;           }


TagSet* AM::pDbgTagSet(void)  { return & gpAttachMap->m_xSysArgs.m_oArgTags;       }

bool AM::bDbgTag(Stz* szTagNm) {
  
  if (! AM::bDebugMode())
    return false;
  
  bool bHasDbgTag = gpAttachMap->m_xSysArgs.m_oArgTags.bHasName(szTagNm);
  
  return bHasDbgTag;
}

Stz* AM::szCommand(void)      { return gxCmdLineArgs.m_xSysArgs.m_sCommand.szStz();             }
Stz* AM::szInitial(void)      { return gxCmdLineArgs.m_xSysArgs.m_sInitial.szStz();             }

// file names
Stz* AM::szLogRegion(void)    { return gpAttachMap->m_sLogRegion;             }
Stz* AM::szLogCsvFile(void)   { return gpAttachMap->m_sLogCsvFile;            }
Stz* AM::szLogTxtFile(void)   { return gpAttachMap->m_sLogTxtFile;            }

//
Sessions* AM::pSessions(void) { return gpAttachMap->m_pSessions;  }


bool AM::bBootError(void)     { return gpAttachMap->m_bBootError;             }
i8   AM::nBuckets(void)       { return C_MAX_BUCKETS_IN_GRID;                         }
BufferPool* AM::pBufferPool(void) { return gpAttachMap->m_pBufferPool;        }
ClassFlags* AM::pClassFlags(void)  { return & gpAttachMap->m_oClassFlags;      }

JobList* AM::pDemonJobs(void) {
  return gpAttachMap->m_pJobs->pDemonJobs();                                }
MasterLock* AM::pDicLock(void)      { return & gpAttachMap->m_oLocks.m_oDicMasterLock;            }
i8   AM::iElapsedSsec(void)   { return gpAttachMap->m_oBirth.iElapsedSsec();  }
i8   AM::iEpoch(void)         { return gpAttachMap->m_iEpoch;                 }
Format* AM::pFmt(void)        { return & gpAttachMap->m_oFormat;          }
Grid* AM::pGrid(void)         { return gpAttachMap ? gpAttachMap->m_pGrid : nullptr;                  }
Heaps* AM::pHeaps(void)   { return & gpAttachMap->m_xHeaps;             }
i8   AM::nIndexEntries(void)  { return gpAttachMap->m_xSysArgs.m_nIndexEntries;      }
JobList* AM::pLocalJobs(void) {
  return gpAttachMap->m_pJobs->pLocalJobs();                                }
Locks* AM::pLocks(void) { return & gpAttachMap->m_oLocks;   }
bool AM::bLogging(void)       { return false;                  }
i8   AM::iNextEpoch(void)     { return ++ gpAttachMap->m_iEpoch;              }
Stz* AM::szNodeNm(void)       {
  return gpAttachMap->m_pGrid->pThisNode()->szNodeNm();  }
JobList* AM::pOriginatingJobs(void) {
  return gpAttachMap->m_pJobs->pOriginatingJobs(); }
JobList* AM::pParticipatingJobs(void) {
  return gpAttachMap->m_pJobs->pParticipatingJobs(); }
JobList* AM::pRetiredJobs(void) {
  return gpAttachMap->m_pJobs->pRetiredJobs(); }
i8   AM::iPid(void)           { return gpAttachMap->m_iPid;                   }

void AM::vProcessExit(void)   { return Process::vExitFailure();                      }

i8   AM::iSystemUsec(void)    { return Process::iSystemUsec();                      }
i8   AM::iUserUsec(void)      { return Process::iUserUsec();  };
i8   AM::iResidentSize(void)  { return Process::iResidentSize();  };

bool AM::bProgress(void)      { return gpAttachMap->m_xSysArgs.m_bProgress;              }
bool AM::bMetadata(void)      { return gpAttachMap->m_xSysArgs.m_bMetadata;          }

i8   AM::iNextTxId(void)      { return gpAttachMap->m_pTxSet->iNextTxId();    }

Schedule* AM::pSchedule(void)     { return gpAttachMap->m_pSchedule;                }
Resources* AM::pResources(void) {
  return & gpAttachMap->m_oResources; }

Word* AM::pConstantRoot(void) { return gpAttachMap->m_pConstantRoot;          }
Word* AM::pWordRoot(void)     { return gpAttachMap->m_pWordRoot;              }
Statistics* AM::pStatistics(void)   { return & gpAttachMap->m_oStatistics;    }
Steps* AM::pSteps(void)   { return gpAttachMap->m_pSteps;      }

MasterLock* AM::pTccLock(void)      { return & gpAttachMap->m_oLocks.m_oTccMasterLock;            }
Word* AM::pTenants(void)      { return gpAttachMap->m_pTenants;               }
i8   AM::iThisNodeNo(void)    { return gpAttachMap->m_pGrid->iThisNodeNo();     }
void AM::vThreadBegins(void)  { ++ gpAttachMap->m_pSchedule->m_ixThreads; }
i8   AM::nThreadsBegun(void)  { return * gpAttachMap->m_pSchedule->m_ixThreads; }
bool AM::bTracepoint(void)    { return gpAttachMap->m_xSysArgs.m_bTracepoint;        }
TxSet* AM::pTxSet(void)       { return gpAttachMap->m_pTxSet;                 }

void AM::vSendNode(Op* pOp, i8 i_SendNode) {
  pOp->vSendNode(i_SendNode);
}

//
void AM::vAckWaitingTask(Task* pTaskWaiting,
                         Task* pTaskAcking,
                         i8& nAcksWereRemaining) {
  IGNORE_RETURN(Scheduler::bTaskCross_AckWaiting(pTaskWaiting,
                                                 pTaskAcking,
                                                 nAcksWereRemaining));
}
void AM::vAddConstant(Stz* szName,Stz* szValue) {
  Constants::vAddConstant(szName,szValue);
}
void AM::vAttachMap(Header* aHeader) {
  gpAttachMap = static_cast<AttachMap*>(aHeader);
}

Queue* AM::pOutputQueue(Session* pSession) {
    
  return pSession->m_pOutput->m_pOutputQueue;
}


void AM::vBootError(bool bBootError) {
  gpAttachMap->m_bBootError = bBootError;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
void AM::vDeleteOperand(Operand* pOperand)
{
  
  eCN eClass = pOperand->eGetClassNo();
  
  switch(eClass) {
      
    case CN_CRWBUFFER: {
      CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);
      
      // permament crw buffers not to be deleted
      if (pCrwBuffer->bIsPermanent())
        break;
      
      delete pCrwBuffer;
      break;
    }
      
    case CN_CSVBUFFER: {
      CsvBuffer* pCsvBuffer = _pCsvBuffer(pOperand);
      delete pCsvBuffer;
      break;
    }

    case CN_EACHROWBUFFER: {
      EachRowBuffer* pEachRowBuffer = _pEachRowBuffer(pOperand);
      delete pEachRowBuffer;
      break;
    }

    case CN_ENTBUFFER: {
      // should never be called for permanent ent buffers
      EntBuffer* pEntBuffer = _pEntBuffer(pOperand);
      RASSERT(! pEntBuffer->bIsPermanent());
      delete pEntBuffer;
      break;
    }
      
    case CN_INDEXDEF: {
      IndexDef* pIndexDef = _pIndexDef(pOperand);
      delete pIndexDef;
      break;
    }
      
    case CN_INFOSET: {
      InfoSet* pInfoSet = _pInfoSet(pOperand);
      delete pInfoSet;
      break;
    }
      
    case CN_MARKER: {
      Marker* pMarker = _pMarker(pOperand);
      delete pMarker;
      break;
    }

    case CN_NAMEDBUFFER: {
      NamedBuffer* pNamedBuffer = _pNamedBuffer(pOperand);
      // permament row buffers not to be deleted
      if (pNamedBuffer->bIsPermanent())
        break;
      delete pNamedBuffer;
      break;
    }

    case CN_PLANOPERAND: {
      PlanOperand* pPlanOperand = _pPlanOperand(pOperand);
      delete pPlanOperand;
      break;
    }
    case CN_PROFILE: {
      Profile* pProfile = _pProfile(pOperand);
      delete pProfile;
      break;
    }
      
    case CN_ROWBUFFER: {
      RowBuffer* pRowBuffer = _pRowBuffer(pOperand);
      // permament row buffers not to be deleted
      if (pRowBuffer->bIsPermanent())
        break;
      pRowBuffer->vDecidedDelete();
      break;
    }
      
    case CN_STREAMDEF: {
      StreamDef* pStreamDef = _pStreamDef(pOperand);
      delete pStreamDef;
      break;
    }

    case CN_STREAMDEFRHT: {
      StreamDefRht* pStreamDefRht = _pStreamDefRht(pOperand);
      delete pStreamDefRht;
      break;
    }
      
    case CN_STREAMSET: {
      StreamSet* pStreamSet = _pStreamSet(pOperand);
      delete pStreamSet;
      break;
    }

    case CN_METAGET: {
      MetaGet* pMetaGet = _pMetaGet(pOperand);
      delete pMetaGet;
      break;
    }

    case CN_BKTMAPGET: {
      BktMapGet* pBktMapGet = _pBktMapGet(pOperand);
      delete pBktMapGet;
      break;
    }
/*
    case CN_PARCEL: {
      Parcel* pParcel = _pParcel(pOperand);
      vTrace(">>>>vDeleteOperand delete pParcel, called by Step::vPipeinDelete as PipeIn[].delete\n"_Stz);
      delete pParcel;
      break;
    }
*/
    case CN_TABLE: {
      break;
    }

    case CN_GRID: {
     break;
    }

    case CN_BUCKETMAP_dyn: {
      break;
    }

    case CN_BUCKETGET: {
      //vTrace(">>>>vDeleteOperand delete bucketget\n"_Stz);
      break;
    }

    case CN_BUCKETINFO: {
      //vTrace(">>>>vDeleteOperand delete bucketinfo\n"_Stz);
      break;
    }

    case CN_PTNSTREAM: {
      break;
    }
      
    default: {
      vTrace("vDeleteOperand %ld Error: %s\n"_Stz,eClass, aCN[eClass]->azChar());
      RASSERT(0);
    }
  }
}
void AM::vDropTaskLock(Task* pTask, Stz* szFunc) {
  pTask->vDropTaskLock(szFunc);
}
void AM::vRootConstant(Word* pConstantRoot) {
  gpAttachMap->m_pConstantRoot = pConstantRoot;
}
void AM::vRootWord(Word* pWordRoot) {
  gpAttachMap->m_pWordRoot = pWordRoot;
}
void AM::vGrabTaskLock(Task* pTask, Stz* szFunc) {
  pTask->vGrabTaskLock(szFunc);
}
void AM::vIndexEntries(i8 nIndexEntries) {
  gpAttachMap->m_xSysArgs.m_nIndexEntries = nIndexEntries;
}
void AM::vNextTempName(s32& sName) {
  gpAttachMap->m_oTempName.vNextName(sName);
}
void AM::vSetAcksRmg(Task* pTask, i8 nAcks) {
  pTask->vAcksRmg(nAcks);
}
void AM::vSteps(Steps* pSteps) {
  gpAttachMap->m_pSteps = pSteps;
}
void AM::vTenants(Word *pTenants) {
  gpAttachMap->m_pTenants = pTenants;
}

//
bool AM::bAttached(void) {
  if (gpAttachMap)
    return true;
  else
    return false;
}

void AM::vAppendJobTags(Job* pJob, TagSet* pTagSet)
{
  pJob->pJobTags()->vCopy(pTagSet);
}

eCN AM::eJobClass(Job* pJob)
{
  return pJob->eGetClassNo();
}


Session* AM::pJobSession(Job* pJob)
{
  i8 iSessionId = pJob->m_i_Session;
  
  Session* pSession = nullptr;
  
  if (iSessionId != C_UNKNOWN)
    pSession = AM::pSessions()->pGetSession(iSessionId);

  return pSession;
}


TagSet* AM::pJobTags(Job* pJob)
{
  return pJob->pJobTags();
}

bool AM::bJobWasReceived(Job* pJob)
{
  return pJob->bWasReceived();
}


void AM::vDumpApi(void) {
  pSessions()->vDump();
}
Task* AM::pExecutingTask(Job* pJob) {
  Task* pTask = pJob->pJobTask();
  return pTask;
}

i8 AM::nLvlLevels(void) {
  return gpAttachMap->m_xSysArgs.m_nLvlLevels;
}
i8 AM::iLvlRatio(void) {
  return gpAttachMap->m_xSysArgs.m_iLvlRatio;
}

bool AM::bParseCmd(TokenVec* pTknVec, TagSet* pExtraTagSet)
{
  return CmdHelper::bParseCmd(pTknVec,pExtraTagSet);
}


Job* AM::pPipelineJob(void)
{
  Op* pOp = pThisTLSTask()->pOp();
  
  Job* pPipelineJob = pOp->pPipeline_op()->pPipelineJob();
  
  return pPipelineJob;
}



void AM::vPoolAnalyze(void)   { return PoolHelper::vPoolAnalyze(); }

void AM::vPrintArguments(void) {
  gxCmdLineArgs.vPrint();
}
void AM::vPrintAttachMap(void) {
  gpAttachMap->vPrintAttachMap();
}
void AM::vPrintStartupInfo(void) {
  gxCmdLineArgs.vPrintStartupInfo();
}

PtnMap* AM::pPtnMap(void) {
  return gpAttachMap->m_pPtnMap;
}



i8 AM::nRndSkipLevel(void) {
  
  i8 nLvlLevels = AM::nLvlLevels();
  i8 iLvlRatio  = AM::iLvlRatio();

  i8 nLevels;
  if (AM::bDeveloper()) {
    Task* pTask = Task::pThisTLSTask();
    nLevels =
      RndLib::iSkipLevel(nLvlLevels,
                         iLvlRatio,
                         MY::pLogicUnit()->m_i_NextRnd);
  }
  else {
    nLevels = RndLib::iSkipLevel(nLvlLevels,iLvlRatio);
  }
  RASSERT(nLevels >= 1 && nLevels <= nLvlLevels);
  return nLevels;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


Table* AM::pTableFind(Stz* szTableNm)
{
  Table* pTable = nullptr;

  DicStack* pDicStack = AM::TASK::pDicStack();
  
  if (pDicStack && pDicStack->pTop()) {
    pTable = Table::pFindTable(pDicStack,szTableNm);
  }
  else {
    pDicStack = new DicStack;
    
    if (pDicStack->bTest("rapidsse"_Stz,"public"_Stz,szTableNm))
      pTable = pDicStack->pTable();
    
    delete pDicStack;
  }
  
  return pTable;
}

StreamDef* AM::pStreamDefFind(Stz* szTableNm) {
  
  Table* pTable = AM::pTableFind(szTableNm);
  if (! pTable)
    return nullptr;
  
  StreamDef* pStreamDef = pTable->pStreamDef();

  return pStreamDef;
}

i8 AM::iTaskAcksRmg_assert_only_(Task* pTask) { return pTask->iAcksRmg_assert_only_(); }
void AM::vTaskDump(void) {
  if (AM::pThisTLSTask())
    AM::pThisTLSTask()->vDump();
}





void AM::vTaskDump(Task* pTask)
{
  Job* pJob = pTask->pJob_task();
    
  if (! pJob)
    return;
  
  AM::vJobContentDump(pJob);
  
  if (pJob->pJobLogicUnit()) {
    if (pJob->pJobLogicUnit()->m_xpOp.bIsNotNull())
      pJob->pJobLogicUnit()->m_xpOp->vDump();
  
    //pJob->pJobLogicUnit()->vContentDump();
  }

}







LogicUnit* AM::pTaskOpLogicUnit(Task* pTask) {
  Op* pOp = pTask->pOp();
  return pOp->pOpLogicUnit();
}

LogicUnit* AM::pTaskLogicUnit(Task* pTask) {
  Job* pJob = pTask->pJob_task();
  return pJob->pJobLogicUnit();
}


void AM::vTaskFullTrace(Task* pTask)
{
  Job* pJob = pTask->pJob_task();

  // dump api state
  //AM::vDumpApi();

  //pTask->vContentDump();
  
  if (pJob)
    AM::vJobContentDump(pJob);
  
  //pJob->pJobLogicUnit()->vContentDump();
  
  AM::pGrid()->vDsv();
}



i8 AM::iTaskQueue(Task* pTask) {
  eQK eQueue = pTask->eQueue();
  return eQueue;
}
i8 AM::iTaskState(Task* pTask) {
  eTS eState = pTask->eState();
  return eState;
}

t8 AM::tThisTLSPespective(void)
{
  Tx* pTx = pThisTLSTx();
  if (! pTx)
    return C_UNKNOWN;

  t8 tPerspective = pTx->tPerspective();
  
  return tPerspective;
}

//STORE::Task* AM::pThisTLSTask(void) {
//  Thread* pThread = Thread::pThisTLSThread();
//  if (! pThread)
//    return nullptr;
//  Task* pTask = pThread->pTask();
//  return pTask;
//}
LogicUnit* AM::pThisTLSLogicUnit(void) {
  Task* pTask = Task::pThisTLSTask();
  Job* pJob = pTask->pJob_task();
  LogicUnit* pLogicUnit = pJob->pJobLogicUnit();

  return pLogicUnit;
}

Task* AM::pThisTLSTask(void) {
  Task* pTask = Task::pThisTLSTask();
  return pTask;
}


Tx* AM::pThisTLSTx(void) {

  Task* pTask = pThisTLSTask();
  if (! pTask)
    return nullptr;
  
  Job* pJob = pTask->pJob_task();
  LogicUnit* pLogicUnit = pJob->pJobLogicUnit();

  Tx* pTx = *pLogicUnit->m_xpTx;
  return pTx;
}

PsxThd* AM::pThisTLSPsxThd(void) {
  Thread* pThread = Thread::pThisTLSThread();
  PsxThd* pPsxThd =pThread->pPsxThd();
  return pPsxThd;
}

HeapBuffer* AM::pToHeapBuffer(i8 i_Buffer) {
  
  HeapBuffer* pHeapBuffer =
    reinterpret_cast<HeapBuffer*>(gpBufferPool->pToPoolBuffer(i_Buffer));
  
  return pHeapBuffer;

}



Object* AM::pToLargeObect(i8 i_Buffer) {
    
  HeapBuffer* pHeapBuffer = pToHeapBuffer(i_Buffer);
  
  Object* pObject = reinterpret_cast<Object*>(pHeapBuffer->aFirstChunk());

  return pObject;

}

void AM::vWelcomeText(s256& sWelcome) {
  Engine::vGetWelcome(sWelcome);
}
void AM::vCopyrightText(s256& sCopyright) {
  Engine::vGetCopyright(sCopyright);
}

Crv* AM::aCrv(DataBuffer* pDataBuffer, i8 iItem) {
  CrvBuffer* pCrvBuffer = _pCrvBuffer(pDataBuffer);
  Crv* aCrv = pCrvBuffer->aGetCrv(iItem);
  return aCrv;
}

void AM::vWriteLog(void) {
  
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get pSession given i_Session                                               */
/*----------------------------------------------------------------------------*/
Session* AM::pSessionGet(i8 i_Origin,  i8 i_Session)
{
  if (i_Origin != AM::iThisNodeNo())
    return nullptr;
  
  if (C_UNKNOWN == i_Session)
    return nullptr;
  
  Session* pSession = AM::pSessions()->pGetSession(i_Session);

  return pSession;
}

/*----------------------------------------------------------------------------*/
/* spawn job                                                                  */
/*----------------------------------------------------------------------------*/
void AM::vSpawnTaskDedicated_job(Job* pJob)
{
  Api::vSpawnTaskDedicated_job(pJob);
}

/*----------------------------------------------------------------------------*/
/* spawn job                                                                  */
/*----------------------------------------------------------------------------*/
void AM::vSpawnTaskDedicated_op(Op* pOp)
{
  Api::vSpawnTaskDedicated_op(pOp);
}

/*----------------------------------------------------------------------------*/
/* spawn task for given job                                                   */
/*----------------------------------------------------------------------------*/
void AM::vSpawnTask_job(Job* pJob)
{
  Api::vSpawnTask_job(pJob);
}

/*----------------------------------------------------------------------------*/
/* spawn task for given op                                                    */
/*----------------------------------------------------------------------------*/
void AM::vSpawnTask_op(Op* pOp)
{
  Api::vSpawnTask_op(pOp);
}

/*----------------------------------------------------------------------------*/
/* spawn job                                                                  */
/*----------------------------------------------------------------------------*/
bool AM::bSpawnTask_session(Job* pJob)
{
  bool bResult = Api::bSpawnTask_session(pJob);
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* spawn job                                                                  */
/*----------------------------------------------------------------------------*/
void AM::vSpawnTask_receivedJob(Job* pJob)
{
  Api::vSpawnTask_receivedJob(pJob);
}

/*----------------------------------------------------------------------------*/
/* spawn job                                                                  */
/*----------------------------------------------------------------------------*/
void AM::vSpawnTask_demonJob(Job* pJob)
{
  Api::vSpawnTask_demonJob(pJob);
}

/*----------------------------------------------------------------------------*/
/* spawn job                                                                  */
/*----------------------------------------------------------------------------*/
void AM::vScheduleNextPhase(Job* pParticipantJob, Task* pActiviating2PcTask)
{
  Api::vScheduleNextPhase(pParticipantJob,pActiviating2PcTask);
}

/*----------------------------------------------------------------------------*/
/* quick config summary                                                       */
/*----------------------------------------------------------------------------*/
Stz* AM::szSummary(s256& sSummary)
{
  Now spNow;
  
  sSummary.vSprint("%lldN%lldP%lldT %s %s"_Stz,
                   AM::GRID::nKnownNodes(),
                   AM::PTNMAP::nPtnsPerNode(),
                   AM::nThreads(),
                   AM::szBuild(),
                   spNow.sz()
                   );
  
  return sSummary.szStz();
}

/*----------------------------------------------------------------------------*/
/* quick config summary usefule for dir name                                  */
/*----------------------------------------------------------------------------*/
Stz* AM::szSummaryDir(s256& sSummary, Stz* szNote)
{
  Now spNow;
  
  sSummary.vSprint("%s_%lldN%lldP%lldT"_Stz,
                   szNote,
                   AM::GRID::nKnownNodes(),
                   AM::PTNMAP::nPtnsPerNode(),
                   AM::nThreads()
                   );
  
  return sSummary.szStz();
}

/*----------------------------------------------------------------------------*/
/* write to trace file etc                                                    */
/*----------------------------------------------------------------------------*/
void AM::vWriteTrace(Stz* aFormat, ...)
{
  va_list xVarArgs;
  
  va_start(xVarArgs,aFormat);
  AM::vWriteTrace_vargs(aFormat,xVarArgs);
  va_end(xVarArgs);

}

/*----------------------------------------------------------------------------*/
/* write to trace file etc                                                    */
/*----------------------------------------------------------------------------*/
void AM::vWriteTrace_vargs(Stz* aFormat, va_list xArgs)
{
  static char caFormatted[C_1MB] = {'\0'};
  
  bool bUseLock = (gpAttachMap ? true : false);
  bool bLowMem  = false; //gpBufferPool ? gpBufferPool->bLowMemory() : false;
  
  if (bUseLock)
    gpAttachMap->vTraceLockGrab();
  //else
  //  printf("NOT LOCKING TRACE\n");
  
  // produce formatted line
  vsnprintf(caFormatted,sizeof(caFormatted) - 3,aFormat->azChar(),xArgs);
  
  // print formatted line
  fprintf(stdout,"%s%s",bLowMem ? "LOW-MEMORY! " : "",caFormatted);
  fflush(stdout);
  
  if (gpTraceFile) {
    gpTraceFile->vSeekEnd();
    gpTraceFile->vPrint("%s%s"_Stz,bLowMem ? "LOW-MEMORY! " : "",caFormatted);
    gpTraceFile->vFlush();
  }
  
  StrLib::vStrip(caFormatted,'\n');
    
  if (bUseLock)
    gpAttachMap->vTraceLockDrop();
}

/*----------------------------------------------------------------------------*/
/* write to trace file etc                                                    */
/*----------------------------------------------------------------------------*/
void AM::vWriteTrace_vargs(File* pFile, Stz* aFormat, va_list xArgs)
{
  static char caFormatted[C_1MB] = {'\0'};
  
  bool bUseLock = (gpAttachMap ? true : false);
  bool bLowMem  = gpBufferPool ? gpBufferPool->bLowMemory() : false;
  
  if (bUseLock)
    gpAttachMap->vTraceLockGrab();

  // produce formatted line
  vsnprintf(caFormatted,sizeof(caFormatted) - 3,aFormat->azChar(),xArgs);
  
  // print formatted line
  fprintf(stdout,"%s%s",bLowMem ? "LOW-MEMORY! " : "",caFormatted);
  fflush(stdout);
  
  if (gpTraceFile) {
    gpTraceFile->vSeekEnd();
    gpTraceFile->vPrint("%s%s"_Stz,bLowMem ? "LOW-MEMORY! " : "",caFormatted);
    gpTraceFile->vFlush();
  }
  
  if (pFile) {
    pFile->vPrint("%s"_Stz,caFormatted);
    pFile->vFlush();
  }
  
  StrLib::vStrip(caFormatted,'\n');
    
  if (bUseLock)
    gpAttachMap->vTraceLockDrop();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* nap for given number of milliseconds (no known calling thread)             */
/*----------------------------------------------------------------------------*/
void AM::vNapMsec(i8 nMilliSeconds, eCN eCallingClass)
{
  struct timespec xReq;
  struct timespec xRem;
  
  if (nMilliSeconds >= C_1K) {
    xReq.tv_nsec = (nMilliSeconds % C_1K) * C_1M;
    xReq.tv_sec  = DIV(nMilliSeconds,C_1K);
  }
  else {
    xReq.tv_nsec = nMilliSeconds * C_1M;
    xReq.tv_sec  = 0;
  }
  
#ifdef WITH_STATISTICS_COUNTERS
  Statistics* pStats = &gpAttachMap->m_oStatistics;
  pStats->Thread.m_nSleeps++;
  pStats->Thread.m_iaSleeps[eCallingClass]++;
  pStats->Thread.m_iaDuration[eCallingClass] += nMilliSeconds * C_1K;
#endif
    
  i8 rCode = nanosleep(&xReq,&xRem);
  
  if (rCode && errno != EINTR) {
    vTrace("AM::vNap error; errno=%lld\n"_Stz,errno);
  }
}

/*----------------------------------------------------------------------------*/
/* nap for given number of microseconds (no known calling thread)             */
/*----------------------------------------------------------------------------*/
void AM::vNapUsec(i8 nMicroSeconds, eCN eCallingClass)
{
  struct timespec xReq;
  struct timespec xRem;
  
  if (nMicroSeconds >= C_1M) {
    xReq.tv_nsec = (nMicroSeconds % C_1M) * C_1K;
    xReq.tv_sec  = DIV(nMicroSeconds,C_1M);
  }
  else {
    xReq.tv_nsec = nMicroSeconds * C_1K;
    xReq.tv_sec  = 0;
  }
  
#ifdef WITH_STATISTICS_COUNTERS
  Statistics* pStats = &gpAttachMap->m_oStatistics;
  pStats->Thread.m_nSleeps++;
  pStats->Thread.m_iaSleeps[eCallingClass]++;
  pStats->Thread.m_iaDuration[eCallingClass] += nMilliSeconds * C_1K;
#endif
    
  i8 rCode = nanosleep(&xReq,&xRem);
  
  if (rCode && errno != EINTR) {
    vTrace("AM::vNap error; errno=%lld\n"_Stz,errno);
  }
}

/*----------------------------------------------------------------------------*/
/* duplicate the operand ( marker / streamDef )                               */
/*----------------------------------------------------------------------------*/
Operand* AM::pDuplicateOperand(Operand* pOperand)
{
  if (pOperand->eGetClassNo() == CN_MARKER) {
    Marker* pMarker = _pMarker(pOperand);
    Marker* pNewMarker = new Marker(pMarker->eOpMarker());
    return pNewMarker;
  }
  else {
    ASSERT(pOperand->eGetClassNo() == CN_STREAMDEF ||
           pOperand->eGetClassNo() == CN_STREAMDEFRHT);

    if (pOperand->eGetClassNo() == CN_STREAMDEF) {
      StreamDef* pStreamDef = _pStreamDef(pOperand);
      StreamDef* pNewStreamDef = new StreamDef(pStreamDef);
      return pNewStreamDef;
    }
    else {
      ASSERT(pOperand->eGetClassNo() == CN_STREAMDEFRHT);
      StreamDefRht* pStreamDefRht = _pStreamDefRht(pOperand);
      StreamDefRht* pNewStreamDefRht = new StreamDefRht(pStreamDefRht);
      return pNewStreamDefRht;
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* new a universal job for op                                                 */
/*----------------------------------------------------------------------------*/
Stz* AM::pOperandName(Operand* pOperand)
{
  if (pOperand->bIsMarker())
    return aOM[(_pMarker(pOperand))->eOpMarker()];
  
  if (pOperand->bIsInfoSet())
    return aOI[(_pInfoSet(pOperand))->eOpInfoSet()];
  else
    return aCN[pOperand->eGetClassNo()];
}

/*----------------------------------------------------------------------------*/
/* set the number of operands in the system                                   */
/*----------------------------------------------------------------------------*/
i8 AM::nOperandsArg(void)
{
  return gpAttachMap->m_xSysArgs.m_nOperands;
}

/*----------------------------------------------------------------------------*/
/* set the number of operands in the system                                   */
/*----------------------------------------------------------------------------*/
void AM::vOperandsArg(i8 nOperands)
{
  gpAttachMap->m_xSysArgs.m_nOperands = nOperands;
}

/*----------------------------------------------------------------------------*/
/* decrement the number of operands in the system                             */
/*----------------------------------------------------------------------------*/
void AM::vOperandsDec(void)
{
  -- gpAttachMap->m_nxOperands;
}

/*----------------------------------------------------------------------------*/
/* get the number of operands in the system                                   */
/*----------------------------------------------------------------------------*/
i8 AM::nOperandsGet(void)
{
  i8 nOperands = * gpAttachMap->m_nxOperands;
  
  return nOperands;
}

/*----------------------------------------------------------------------------*/
/* increment the number of operands in the system                             */
/*----------------------------------------------------------------------------*/
void AM::vOperandsInc(void)
{
  ++ gpAttachMap->m_nxOperands;
}

/*----------------------------------------------------------------------------*/
/* what kind of operand                                                       */
/*----------------------------------------------------------------------------*/
i8 AM::iOperandZkKind(Operand* pOperand)
{
  eZK eOperand = ZK_NONE;
  
  eCN eClass_opd = pOperand->eGetClassNo();
  
  switch (eClass_opd) {

    case CN_BUCKETMAP_dyn: eOperand = ZK_D_BUCKETMAP_dyn; break;
    case CN_CRWBUFFER: eOperand = ZK_D_CRWBUFFER; break;
    case CN_CSVBUFFER: eOperand = ZK_D_CSVBUFFER; break;
    case CN_EACHROWBUFFER: eOperand = ZK_D_EACHROWBUFFER; break;
    case CN_ENTBUFFER: eOperand = ZK_D_ENTBUFFER; break;
    case CN_INDEX:     eOperand = ZK_D_INDEX;  break;
    case CN_INDEXDEF:  eOperand = ZK_D_INDEXDEF;  break;
      
    case CN_INFOSET:
    {
      InfoSet* pInfoSet = _pInfoSet(pOperand);
      
      eOI eInfoSet = pInfoSet->eOpInfoSet();
      switch(eInfoSet) {
        case OI_CALLPIPELINE:   eOperand = ZK_I_CALLPIPELINE;   break;
        case OI_COMMANDERRORS:  eOperand = ZK_I_COMMANDERRORS;  break;
        case OI_COUNTINFO:      eOperand = ZK_I_COUNTINFO;      break;
        case OI_EXECERRORS:     eOperand = ZK_I_EXECERRORS;     break;
        case OI_LOADRESULT:     eOperand = ZK_I_LOADRESULT;     break;
        case OI_LOCKSTATS:      eOperand = ZK_I_LOCKSTATS;      break;
        case OI_NODEINFO:       eOperand = ZK_I_NODEINFO;       break;
        case OI_PATHINFO:       eOperand = ZK_I_PATHINFO;       break;
        case OI_PROGRESSUNITS:  eOperand = ZK_I_PROGRESSUNITS;  break;
        case OI_PROMPTOUT:      eOperand = ZK_I_PROMPTOUT;      break;
        case OI_RETURNPIPELINE: eOperand = ZK_I_RETURNPIPELINE; break;
        case OI_SEEKOFFSETS:    eOperand = ZK_I_SEEKOFFSETS;    break;
        case OI_TOTALUNITS:     eOperand = ZK_I_TOTALUNITS;     break;
        default:
          RASSERT(0);
      }
      break;
    }

    case CN_JOB:                eOperand = ZK_D_JOB;            break;
    case CN_JOBMETRICS:         eOperand = ZK_D_JOBMETRICS;     break;

    case CN_MARKER:
    {
      Marker* pMarker = _pMarker(pOperand);

      eOM eMarker = pMarker->eOpMarker();
      switch(eMarker) {
        case OM_CANCELMARKER:       eOperand = ZK_M_CANCEL;       break;
        case OM_CANCELLING:         eOperand = ZK_M_CANCELLING;   break;
        case OM_CANCELLEDMARKER:    eOperand = ZK_M_CANCELLED;    break;
        case OM_CLEANING:           eOperand = ZK_M_CLEANING;     break;
        case OM_CLEANED:            eOperand = ZK_M_CLEANED;      break;
        case OM_DATAPULSE:          eOperand = ZK_M_DATAPULSE;    break;
        case OM_DESTROYMARKER:      eOperand = ZK_M_DESTROY;      break;
        case OM_EACHSCANEOSMARKER:  eOperand = ZK_M_EACHSCANEOS;  break;
        case OM_ENDMARKER:          eOperand = ZK_M_END;          break;
        case OM_ENDEDMARKER:        eOperand = ZK_M_ENDED;        break;
        case OM_ENDINGMARKER:       eOperand = ZK_M_ENDING;       break;
        case OM_ENDCRWRHT:          eOperand = ZK_M_ENDCRWRHT;    break;
        case OM_ENDROWRHT:          eOperand = ZK_M_ENDROWRHT;    break;
        case OM_EOSMARKER:          eOperand = ZK_M_EOS;          break;
        case OM_ERRORMARKER:        eOperand = ZK_M_ERROR;        break;
        case OM_ERROREDMARKER:      eOperand = ZK_M_ERRORED;      break;
        case OM_ERRORINGMARKER:     eOperand = ZK_M_ERRORING;     break;
        case OM_EXITING:            eOperand = ZK_M_EXITING;      break;
        case OM_EXITED:             eOperand = ZK_M_EXITED;       break;
        case OM_FLUSH:              eOperand = ZK_M_FLUSH;        break;
        case OM_LEAVEMARKER:        eOperand = ZK_M_LEAVE;        break;
        case OM_LEAVINGMARKER:      eOperand = ZK_M_LEAVING;      break;
        case OM_LEFTMARKER:         eOperand = ZK_M_LEFT;         break;
        case OM_LEFTOUTER_CRWLFT:   eOperand = ZK_M_LEFTOUTER_CRWLFT; break;
        case OM_LEFTOUTER_ROWLFT:   eOperand = ZK_M_LEFTOUTER_ROWLFT; break;
        case OM_NEXTBUFFERMARKER:   eOperand = ZK_M_NEXTBUFFER;   break;
        case OM_STARTEDMARKER:      eOperand = ZK_M_STARTED;      break;
        case OM_STARTCRWRHT:        eOperand = ZK_M_STARTCRWRHT;  break;
        case OM_STARTMARKER:        eOperand = ZK_M_START;        break;
        case OM_STARTROWRHT:        eOperand = ZK_M_STARTROWRHT;  break;
        case OM_STREAMDEFRHT:       eOperand = ZK_M_STREAMDEFRHT; break;
        case OM_TRIGGER:            eOperand = ZK_M_TRIGGER;      break;
        default:
          RASSERT(0);
      }
      break;
    }

    case CN_NAMEDBUFFER:  eOperand = ZK_D_NAMEDBUFFER;    break;
    case CN_OP:           eOperand = ZK_D_OP;             break;
    case CN_OTHEROPERAND: eOperand = ZK_D_OTHEROPERAND;   break;
    case CN_PAGINGMETRICS:eOperand = ZK_D_PAGINGMETRICS;  break;
    case CN_PLANOPERAND:  eOperand = ZK_D_PLANOPERAND;    break;
    case CN_PROFILE:      eOperand = ZK_D_PROFILE;        break;
    case CN_ROWBUFFER:    eOperand = ZK_D_ROWBUFFER;      break;
    //case CN_SRTBUFFER:    eOperand = ZK_D_SRTBUFFER;      break;
    case CN_STREAMDEF:    eOperand = ZK_D_STREAMDEF;      break;
    case CN_STREAMDEFRHT: eOperand = ZK_D_STREAMDEFRHT;   break;
    case CN_STREAMSET:    eOperand = ZK_D_STREAMSET;      break;
    case CN_METAGET:      eOperand = ZK_D_METAGET;        break;
    case CN_PARCEL:       eOperand = ZK_D_PARCEL;         break;
    case CN_TABLE:        eOperand = ZK_D_TABLE;          break;
    case CN_GRID:         eOperand = ZK_D_GRID;           break;
    case CN_BKTMAPGET:       eOperand = ZK_D_BKTMAPGET;      break;
    case CN_BUCKETGET:       eOperand = ZK_D_BUCKETGET;      break;
    case CN_BUCKETINFO:       eOperand = ZK_D_BUCKETINFO;      break;
    case CN_TABLEPTN:     eOperand = ZK_D_TABLEPTN;      break;
    //case CN_BUCKET:     eOperand = ZK_D_BUCKET;      break;

    default:
      vTrace("iOperandZkKind() what class:%lld %s\n"_Stz,eClass_opd, aCN[eClass_opd]);
      RASSERT(0);
      break;
  }
  
  return (i8)eOperand;
}

/*----------------------------------------------------------------------------*/
/* increment the query no and return                                          */
/*----------------------------------------------------------------------------*/
i8 AM::iQueryNoInc(void)
{
  i8 iQueryNo = ++ gpAttachMap->m_ixQueryNo;
  return iQueryNo;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* things to additinally check at boot                                        */
/*----------------------------------------------------------------------------*/
void AM::vBootCheck(void)
{
  // No of entries in eCN , aCN and gpDump should match
  i8 iEntriesECN = CN_ENUMS;
  i8 iEntriesPCN = 0;
  i8 iEntriesGCLASSSIZE = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vTrace("  aCN[] has CN_ENUMS\n"_Stz);
  
  bool bFoundEnum = false;
  // find number of entries in aCN
  for (i8 i_Class = 0; i_Class <= CN_ENUMS; i_Class++) {
    if (aCN[i_Class]->bEqual("ENUMS"_Stz)) {
      bFoundEnum = true;
      break;
    }
    iEntriesPCN++;
  }
  
  if (!bFoundEnum)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "Enum for CN_ENUMS not found in aCN\n"_Stz);
  
  if (iEntriesECN != iEntriesPCN) {
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "\n!!!ALERT!!! No of entries in aCN (%lld) "
           "do not match the no in eCN (%lld) -- missing comma??\n\n"_Stz,
           iEntriesPCN, iEntriesECN);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vTrace("  gxaSizeHeap[] matches aCN[]\n"_Stz);
  
  bFoundEnum = false;
  // find number of entries in gxaSizeHeap
  for (i8 i_Class = 0; i_Class <= CN_ENUMS; i_Class++) {
    if (gxaSizeHeap[i_Class].m_nClassSize == -1) {
      bFoundEnum = true;
      break;
    }
    iEntriesGCLASSSIZE++;
  }
    
  if (!bFoundEnum) {
    vTrace("Enum for CN_ENUM not found in gxaSizeHeap\n"_Stz);

    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "Enum for CN_ENUM not found in gxaSizeHeap\n"_Stz);
  }
  
  if (iEntriesECN != iEntriesGCLASSSIZE) {
    vTrace("\n!!!ALERT!!! No of entries in gxaSizeHeap (%lld) "
           "do not match the no in eCN (%lld)\n\n"_Stz,
           iEntriesGCLASSSIZE, iEntriesECN);
    
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "\n!!!ALERT!!! No of entries in gxaSizeHeap (%lld) "
           "do not match the no in eCN (%lld)\n\n"_Stz,
           iEntriesGCLASSSIZE, iEntriesECN);
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vTrace("  Heap assignment matches class size\n"_Stz);
  
  // check sizes are less than pool block size
  for (i8 iClass = 0; iClass <= CN_ENUMS; iClass++) {
    
    eCN eClass = (eCN)iClass;
    i8  iClassSize = gxaSizeHeap[eClass].m_nClassSize;
    UNUSED(iClassSize);

    switch (gxaSizeHeap[eClass].m_eWhichHeap) {
        
      case HT_INVALID:
        continue;
        
      case HT_P_TINY:
      case HT_T_TINY:
        ASSERT(iClassSize < C_SIZE_TINY_OBJECT);
        continue;
        
      case HT_P_SMALL:
      case HT_T_SMALL:
        ASSERT(iClassSize < C_SIZE_SMALL_OBJECT);
        continue;
        
      case HT_P_MEDIUM:
      case HT_T_MEDIUM:
        ASSERT(iClassSize < C_SIZE_MEDIUM_OBJECT);
        continue;
        
      case HT_P_LARGE:
      case HT_T_LARGE:
        ASSERT(iClassSize < C_SIZE_LARGE_OBJECT);
        continue;
                
      case HT_T_DATA:
        ASSERT(iClassSize < C_SIZE_DATA_OBJECT);
        continue;
                
      default:
        ASSERT(0);
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
}

/*----------------------------------------------------------------------------*/
/* get build string                                                           */
/*----------------------------------------------------------------------------*/
Stz* AM::szBuild(void)
{
#ifdef WITH_DEBUG_VERSION
  return "debug"_Stz;
#endif
#ifdef WITH_RELEASE_VERSION
  return "release"_Stz;
#endif
  NEVER_REACH
}
  
void AM::vFilePrint(File* pFile, Stz* szFormat,...) {
  
  va_list xArgs;
  va_start(xArgs,szFormat);
  pFile->vPrint(xArgs, szFormat);
  va_end(xArgs);
  
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

i8 AM::iRdTickingDevice(void) { return TickingDevice::iGetTick();                  }
i8 AM::iAMTimePoint(void) { return gpAttachMap->m_iTimePoint;                  }


bool AM::bInBufferPool(Object* pObject) {
  return gpBufferPool->bInBufferPool(pObject);
}

InternalTables* AM::pInternalTables(void) {
  return gpAttachMap->m_pInternalTables;
}


void AM::vPrintGlobals(void)
{
  printf("gxCmdLineArgs     @%p\n",&gxCmdLineArgs);
  printf("\n");

  printf("gpAttachMap     @%p : %p\n",&gpAttachMap,  gpAttachMap);
  printf("gpBufferPool    @%p : %p\n",&gpBufferPool, gpBufferPool);
  printf("gaSeShmRegion   @%p : %p\n",&gaSeShmRegion,gaSeShmRegion);
  printf("gpTraceFile     @%p : %p\n",&gpTraceFile,  gpTraceFile);
}

Index* AM::pReadApiIndex(void) { return gpReadApiIndex; }
void   AM::vReadApiIndex(Index* pIndex) { gpReadApiIndex = pIndex; }

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

void AM::vJobContentDump(Job* pJob) { pJob->vPrintJobContents(); }

void AM::vJobDelete(Job** ppJob, Object* pDeleter, Stz* szFunc) {
  
  Job* pJob = * ppJob;
  
  pJob->vGuardedDelete(CN_JOB,&pJob,
                       CN_AM,pDeleter,szFunc);
  
  * ppJob = nullptr;
}





bool AM::bTaskIsLocked(Task* pTask) {
  bool bLocked = pTask->bIsLocked();
  return bLocked;
}

Job* AM::pTaskJob(void) {
  return pThisTLSTask()->pJob_task();
}
Job* AM::pTaskJob(Task* pTask) {
  if (pTask)
    return pTask->pJob_task();
  else
    return nullptr;
}

void AM::vJobObjectName(Job* pJob, Stz* szObjectName) {
  pJob->pJobTags()->vInsert(C_TAG_TABLENM,szObjectName);
}






////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* get RU_Cpu                                                                 */
/*----------------------------------------------------------------------------*/
RU_Cpu* AM::pRU_Cpu(void)
{
  Task* pTask = AM::pThisTLSTask();
  
  if (! pTask)
    return nullptr;
  
  Profile* pProfile = pTask->pProfile();
  if (! pProfile)
    return nullptr;
  
  RU_Cpu* pRU_Cpu = pProfile->pRU_Cpu();
  return pRU_Cpu;
}

/*----------------------------------------------------------------------------*/
/* get RU_Memory                                                              */
/*----------------------------------------------------------------------------*/
RU_Memory* AM::pRU_Memory(void)
{
  Task* pTask = AM::pThisTLSTask();
  
  if (! pTask)
    return nullptr;
  
  Profile* pProfile = pTask->pProfile();
  if (! pProfile)
    return nullptr;
  
  RU_Memory* pRU_Memory = pProfile->pRU_Memory();
  return pRU_Memory;
}

/*----------------------------------------------------------------------------*/
/* allocInc                                                                   */
/*----------------------------------------------------------------------------*/
void AM::vAllocInc(RU_Memory* pRU_Memory, eCN eAllocClass)
{
  pRU_Memory->vAllocInc(eAllocClass);
}

/*----------------------------------------------------------------------------*/
/* FreeInc                                                                    */
/*----------------------------------------------------------------------------*/
void AM::vFreeInc(RU_Memory* pRU_Memory, eCN eAllocClass)
{
  pRU_Memory->vFreeInc(eAllocClass);
}

/*----------------------------------------------------------------------------*/
/* dump routine of word                                                       */
/*----------------------------------------------------------------------------*/
void AM::vWordSeeDoes(Word* pDoesWord, CodeCell* aDoes,
                      DicStack* pDicStack, CodeCell* aInxPtr)
{
  CodeCell* aLastCell = pDoesWord->aLastCell();
  for (CodeCell* aCell = aDoes; aCell < aLastCell; ) {
    
    fpSTEPFN fpStepFn = *aCell->m_afpStepFn;
    Word* pStepWord = pDicStack->pLocateStepWord(fpStepFn);
    Stz* szStepNm = pStepWord->szWordNm();
    
    // -- -- -- -- -- -- -- -- -- --
    // step is: jump / jump false / jump true
    
    if (fpStepFn == &Step::vSenJump ||
        fpStepFn == &Step::vSenJumpFalse ||
        fpStepFn == &Step::vSenJumpTrue ) {
      
      vTrace("%p: %s"_Stz,aCell,szStepNm);
      aCell++;
      
      vTrace(" %p\n"_Stz,aCell->m_aJumpTo);
      aCell++;
      
      goto L_Continue;
    }
    
    // -- -- -- -- -- -- -- -- -- --
    // step is: push bcdval
    
    if (fpStepFn == &Step::vPushBcdVal) {
      
      vTrace("%p: %s"_Stz,aCell,szStepNm);
      aCell++;
      
      TypedCell* aTypedCell = _aTypedCell(aCell);
      vTrace(" %lld\n"_Stz,aTypedCell->m_wxValueCell.iGetColVal());
      
      i8 iBytes = aTypedCell->nStackUsage();
      aCell = aAdd<CodeCell*>(aCell,iBytes);
      
      goto L_Continue;
    }
    
    // -- -- -- -- -- -- -- -- -- --
    // step is: push bolval
    
    if (fpStepFn == &Step::vPushBolVal) {
      
      vTrace("%p: %s"_Stz,aCell,szStepNm);
      aCell++;
      
      TypedCell* aTypedCell = _aTypedCell(aCell);
      vTrace(" %s\n"_Stz
             ,aTypedCell->m_wxValueCell.iGetColVal() ? "true" : "false");
      
      i8 iBytes = aTypedCell->nStackUsage();
      aCell = aAdd<CodeCell*>(aCell,iBytes);
      
      goto L_Continue;
    }
    
    // -- -- -- -- -- -- -- -- -- --
    // step is: push fltval
    
    if (fpStepFn == &Step::vPushFltVal) {
      
      vTrace("%p: %s"_Stz,aCell,szStepNm);
      aCell++;
      
      TypedCell* aTypedCell = _aTypedCell(aCell);
      vTrace(" %g\n"_Stz,aTypedCell->m_wxValueCell.dGetColVal());
      
      i8 iBytes = aTypedCell->nStackUsage();
      aCell = aAdd<CodeCell*>(aCell,iBytes);
      
      goto L_Continue;
    }
    
    // -- -- -- -- -- -- -- -- -- --
    // step is: push intval
    
    if (fpStepFn == &Step::vPushIntVal) {
      
      vTrace("%p: %s"_Stz,aCell,szStepNm);
      aCell++;
      
//      IntVal* aIntVal = _aIntVal(aCell);
      TypedCell* aTypedCell = _aTypedCell(aCell);
      vTrace(" %lld\n"_Stz,aTypedCell->m_wxValueCell.iGetColVal());
      
      i8 iBytes = aTypedCell->nStackUsage();
      aCell = aAdd<CodeCell*>(aCell,iBytes);
      
      goto L_Continue;
    }
    
    // -- -- -- -- -- -- -- -- -- --
    // step is: push ptrval
    
    if (fpStepFn == &Step::vPushPtrVal) {
      
      vTrace("%p: %s"_Stz,aCell,szStepNm);
      aCell++;
      
      TypedCell* aTypedCell = _aTypedCell(aCell);
      eCN eClassNo_obj = aTypedCell->pObj()->eGetClassNo();

      if (CN_WORD == eClassNo_obj) {
        Word* pWord = _pWord(aTypedCell->pObj());
        vTrace(" %p %s\n"_Stz,pWord,pWord->szWordNm());
      }
      else {
        vTrace(" %p %s\n"_Stz,aCN[eClassNo_obj]);
      }
      
      i8 iBytes = aTypedCell->nStackUsage();
      aCell = aAdd<CodeCell*>(aCell,iBytes);
      
      goto L_Continue;
    }
    
    // -- -- -- -- -- -- -- -- -- --
    // step is: push stzval
    
    if (fpStepFn == &Step::vPushStzVal) {
      
      vTrace("%p: %s"_Stz,aCell,szStepNm);
      aCell++;
      
      TypedCell* aTypedCell = _aTypedCell(aCell);
      vTrace(" \"%s\"\n"_Stz,aTypedCell->szStz());
      
      i8 iBytes = aTypedCell->nStackUsage();
      aCell = aAdd<CodeCell*>(aCell,iBytes);
      
      goto L_Continue;
    }
    
    // -- -- -- -- -- -- -- -- -- --
    // step is: enter routine
    
    if (fpStepFn == &Step::vSenDoColon) {
      
      CodeCell* aEnterCell = *(CodeCell**)(aCell->m_afpStepFn + 1);
      Word* pEnterWord = CodeBlock::pToWord(aEnterCell);

      vTrace("%p: %s %s\n"_Stz,
             aCell,
             szStepNm,
             pEnterWord->szWordNm());
      aCell++;
      
      goto L_Continue;
    }
    
    // -- -- -- -- -- -- -- -- -- --
    // step is: call step
  
    vTrace("%p: %s\n"_Stz,aCell,szStepNm);
    aCell++;
    
    L_Continue:
    
    if (aInxPtr == aCell)
      vTrace(" <===\n"_Stz);
  }
  
}

static Stz* aEG[]
{
  "EG_NONE"_Stz,
  
  "EG_AA"_Stz,
  "EG_AC"_Stz,
  "EG_AK"_Stz,
  "EG_AN"_Stz,
  "EG_AT"_Stz,

  "EG_BO"_Stz,

  "EG_CC"_Stz,
  "EG_CM"_Stz,
  "EG_CN"_Stz,
  "EG_CP"_Stz,
  "EG_CS"_Stz,
  "EG_CT"_Stz,
  "EG_CX"_Stz,

  "EG_DA"_Stz,
  "EG_DC"_Stz,
  "EG_DL"_Stz,
  "EG_DM"_Stz,
  "EG_DS"_Stz,
  
  "EG_EA"_Stz,
  "EG_EC"_Stz,
  "EG_ED"_Stz,
  "EG_EK"_Stz,
  "EG_EO"_Stz,
  "EG_EP"_Stz,

  "EG_FN"_Stz,

  "EG_GA"_Stz,
  
  "EG_HT"_Stz,
  "EG_HS"_Stz,
  "EG_HT"_Stz,

  "EG_IK"_Stz,
  "EG_IS"_Stz,
  "EG_IT"_Stz,

  "EG_JF"_Stz,
  "EG_JK"_Stz,
  "EG_JL"_Stz,
  "EG_JP"_Stz,
  "EG_JS"_Stz,
  
  "EG_KC"_Stz,
  "EG_KE"_Stz,
  "EG_KK"_Stz,

  "EG_LA"_Stz,
  "EG_LN"_Stz,
  
  "EG_MA"_Stz,
  "EG_MC"_Stz,
  "EG_MM"_Stz,
  "EG_MT"_Stz,

  "EG_NE"_Stz,

  "EG_NS"_Stz,
  
  "EG_OA"_Stz,
  "EG_OI"_Stz,
  "EG_OK"_Stz,
  "EG_OM"_Stz,
  "EG_OR"_Stz,

  "EG_PA"_Stz,
  "EG_PF"_Stz,
  "EG_PK"_Stz,
  "EG_YG"_Stz,
  "EG_PP"_Stz,
  "EG_PT"_Stz,
  "EG_YX"_Stz,
  "EG_PY"_Stz,
  
  "EG_QK"_Stz,

  "EG_RA"_Stz,
  "EG_RE"_Stz,
  "EG_RG"_Stz,
  "EG_RO"_Stz,
  "EG_RS"_Stz,
  "EG_RU"_Stz,
  
  "EG_SC"_Stz,
  "EG_SI"_Stz,
  "EG_SK"_Stz,
  "EG_ST"_Stz,

  "EG_TA"_Stz,
  "EG_TN"_Stz,
  "EG_TP"_Stz,
  "EG_TR"_Stz,
  "EG_TS"_Stz,
  "EG_TT"_Stz,
  
  "EG_UF"_Stz,
  "EG_UK"_Stz,

  "EG_VA"_Stz,

  "EG_WK"_Stz,

  "EG_XA"_Stz,
  "EG_XK"_Stz,
  "EG_XP"_Stz,
  
  "EG_ZK"_Stz,
  
  "EG_ENUMS"_Stz,
};
  

/*----------------------------------------------------------------------------*/
/* get enum text                                                              */
/*----------------------------------------------------------------------------*/
Stz* AM::szEnumText(i8 i_EnumGroup, i8 i_Enum)
{
  switch (i_EnumGroup) {
      
    case EG_AA: return aAA[i_Enum];
    case EG_AC: return aAC[i_Enum];
    case EG_AK: return aAK[i_Enum];
    //case EG_AN: return aAN[i_Enum];
    case EG_AT: return aAT[i_Enum];
             
    case EG_BO: return aBO[i_Enum];
      
    case EG_CC: return aCC[i_Enum];
    case EG_CM: return aCM[i_Enum];
    case EG_CN: return aCN[i_Enum];
    case EG_CP: return aCP[i_Enum];
    //case EG_CS: return aCS[i_Enum];
    case EG_CT: return aCT[i_Enum];
    case EG_CX: return aCX[i_Enum];

    case EG_DA: return aDA[i_Enum];
    case EG_DC: return aDC[i_Enum];
    case EG_DL: return aDL[i_Enum];
    case EG_DM: return aDM[i_Enum];
    case EG_DS: return aDS[i_Enum];
      
    case EG_EA: return aEA[i_Enum];
    case EG_EC: return aEC[i_Enum];
    case EG_ED: return aED[i_Enum];
    case EG_EO: return aEO[i_Enum];
    case EG_EP: return aEP[i_Enum];

    case EG_FN: return aFN[i_Enum];

    case EG_GA: return aGA[i_Enum];

    case EG_HA: return aHA[i_Enum];
    case EG_HS: return aHS[i_Enum];
    case EG_HT: return aHT[i_Enum];
      
    case EG_IK: return aIK[i_Enum];
    case EG_IS: return aIS[i_Enum];
    case EG_IT: return aIT[i_Enum];
      
    case EG_JF: return aJF[i_Enum];
    case EG_JK: return aJK[i_Enum];
    case EG_JL: return aJL[i_Enum];
    case EG_JP: return aJP[i_Enum];
    case EG_JS: return aJS[i_Enum];
      
    case EG_KC: return aKC[i_Enum];
    case EG_KE: return aKE[i_Enum];
    case EG_KK: return aKK[i_Enum];

    case EG_LA: return aLA[i_Enum];
    case EG_LG: return aLG[i_Enum];
    case EG_LH: return aLH[i_Enum];
      
    case EG_MA: return aMA[i_Enum];
    case EG_MC: return aMC[i_Enum];
    case EG_MM: return aMM[i_Enum];
    case EG_MT: return aMT[i_Enum];

    case EG_NE: return aNE[i_Enum];
    case EG_NS: return aNS[i_Enum];
      
    case EG_OA: return aOA[i_Enum];
    case EG_OI: return aOI[i_Enum];
    case EG_OK: return aOK[i_Enum];
    case EG_OM: return aOM[i_Enum];
    case EG_OR: return aOR[i_Enum];

    case EG_PA: return aPA[i_Enum];
    case EG_PF: return aPF[i_Enum];
    case EG_PK: return aPK[i_Enum];
    case EG_PP: return aPP[i_Enum];
    case EG_PT: return aPT[i_Enum];
    case EG_PY: return aPY[i_Enum];

    case EG_QK: return aQK[i_Enum];

    case EG_RA: return aRA[i_Enum];
    case EG_RE: return aLE[i_Enum];
    case EG_RG: return aRG[i_Enum];
    case EG_RO: return aRO[i_Enum];
    case EG_RS: return aRS[i_Enum];
    case EG_RU: return aRU[i_Enum];
      
    case EG_SC: return aSC[i_Enum];
    case EG_SI: return aSI[i_Enum];
    case EG_SK: return aSK[i_Enum];
    case EG_ST: return aST[i_Enum];

    case EG_TA: return aTA[i_Enum];
    case EG_TN: return aTN[i_Enum];
    case EG_TP: return aTP[i_Enum];
    case EG_TR: return aTR[i_Enum];
    case EG_TS: return aTS[i_Enum];
    case EG_TT: return aTT[i_Enum];
      
    case EG_UF: return aUF[i_Enum];
    case EG_UK: return aUK[i_Enum];

    case EG_VA: return aVA[i_Enum];

    case EG_WK: return aWK[i_Enum];

    case EG_XA: return aXA[i_Enum];
    case EG_XK: return aXK[i_Enum];
    case EG_XP: return aXP[i_Enum];

    case EG_YG: return aYG[i_Enum];
    case EG_YX: return aYX[i_Enum];

    case EG_ZK: return aZK[i_Enum];

    default:
      vTrace("AM::szEnumText: Unknown enum group %lld\n"_Stz, i_EnumGroup);
      return nullptr;
  }
}


/*----------------------------------------------------------------------------*/
/* set format from given tag set                                              */
/*----------------------------------------------------------------------------*/
bool AM::bFormatSetFromTagSet(Format* pFormat, TagSet* pTagSet)
{
  s32 sTagValue;
  i8 iTagValue;
  i8 i_Tag;

  // - - - - - - - - - - - - - - - - - - - - -
  // C_RECORD_TERMINATOR
  i_Tag = pTagSet->i_Find(C_TAG_LD_RECORD_TERMINATOR);
  if (i_Tag != C_UNKNOWN) {
    sTagValue = pTagSet->szValue(i_Tag);
    pFormat->m_cRecordTerminator = *sTagValue.azChar();
    if (pFormat->m_cRecordTerminator == '0')
      pFormat->m_cRecordTerminator = '\0';
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // C_RECORD_ENDS
  i_Tag = pTagSet->i_Find(C_TAG_LD_RECORD_ENDS);
  if (i_Tag != C_UNKNOWN) {
    sTagValue = pTagSet->szValue(i_Tag);
    char cRecordEnds = *sTagValue.azChar();
    
    // is record_ends 1 character ?
    if (sTagValue.nLength() == 1) {
      if (cRecordEnds == '0') {
        vTrace("Invalid load : record_ends can only be \\n, \\r or \\r\\n\n"_Stz);
        return false;
      }
      else if (cRecordEnds == '\n') {
        pFormat->m_eLineEnd = LE_NL;
      }
      else if (cRecordEnds == '\r') {
        pFormat->m_eLineEnd = LE_CR;
      }
      else {
        vTrace("Invalid load : record_ends can only be \\n, \\r or \\r\\n\n"_Stz);
        return false;
      }
    }
    // record_ends is 2 characters
    else if (sTagValue.nLength() == 2) {
      
      if (sTagValue.bSame("\r\n"_Stz)) {
        pFormat->m_eLineEnd = LE_CRNL;
      }
      else {
        vTrace("Invalid load : record_ends can only be \\n, \\r or \\r\\n\n"_Stz);
        return false;
      }
    }
    else {
      vTrace("Invalid load : record_ends can only be \\n, \\r or \\r\\n\n"_Stz);
      return false;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // C_FIELD_DELIMITER
  i_Tag = pTagSet->i_Find(C_TAG_LD_FIELD_DELIMITER);
  if (i_Tag != C_UNKNOWN) {
    sTagValue = pTagSet->szValue(i_Tag);
    if (sTagValue.bSame("\\t"_Stz))
      pFormat->m_cFieldDelimiter = '\t';
    else
      pFormat->m_cFieldDelimiter = *sTagValue.azChar();
    if (pFormat->m_cFieldDelimiter == '0')
      pFormat->m_cFieldDelimiter = '\0';
  }

  // - - - - - - - - - - - - - - - - - - - - -
  // C_ENCLOSED_BY_LEADING
  i_Tag = pTagSet->i_Find(C_TAG_LD_ENCLOSED_BY_LEADING);
  if (i_Tag != C_UNKNOWN) {
    sTagValue = pTagSet->szValue(i_Tag);
    pFormat->m_cEnclosedBy_Leading = *sTagValue.azChar();
    if (pFormat->m_cEnclosedBy_Leading == '0')
      pFormat->m_cEnclosedBy_Leading = '\0';
        
    // default set trailing same as leading
    pFormat->m_cEnclosedBy_Trailing =
    pFormat->m_cEnclosedBy_Leading;
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // C_ENCLOSED_BY_TRAILING
  i_Tag = pTagSet->i_Find(C_TAG_LD_ENCLOSED_BY_TRAILING);
  if (i_Tag != C_UNKNOWN) {
    sTagValue = pTagSet->szValue(i_Tag);
    // if trailing is specified, leading should also have been specified
    if (pFormat->m_cEnclosedBy_Trailing == '\0') {
      if (! AM::bMetadata())
        vTrace("Invalid load : enclosed_by_trailing specified but no enclosed_by_leading\n"_Stz);
      return false;
    }
    pFormat->m_cEnclosedBy_Trailing = *sTagValue.azChar();
    if (pFormat->m_cEnclosedBy_Trailing == '0')
      pFormat->m_cEnclosedBy_Trailing = '\0';
  }
  //
  //    DATE_FORMAT='YYYY.MM.DD’
  //
  //
  // - - - - - - - - - - - - - - - - - - - - -
  // C_DATE_FORMAT
  i_Tag = pTagSet->i_Find(C_TAG_LD_DATE_FORMAT);
  if (i_Tag != C_UNKNOWN) {
    sTagValue = pTagSet->szValue(i_Tag);
    // extract date format
    if ( ! pFormat->bExtractDateFormat(sTagValue.szStz()) ) {
      if ( ! AM::bMetadata())
        vTrace("Invalid load : enclosed_by_trailing specified but no enclosed_by_leading\n"_Stz);
      return false;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // C_ESCAPE_CHAR
  i_Tag = pTagSet->i_Find(C_TAG_LD_ESCAPE_CHAR);
  if (i_Tag != C_UNKNOWN) {
    sTagValue = pTagSet->szValue(i_Tag);
    pFormat->m_cEscapeChar = *sTagValue.azChar();
    if (pFormat->m_cEscapeChar == '0')
      pFormat->m_cEscapeChar = '\0';
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // C_MAX_ERRORS
  i_Tag = pTagSet->i_Find(C_TAG_LD_MAX_ERRORS);
  if (i_Tag != C_UNKNOWN) {
    iTagValue = pTagSet->iValue(i_Tag);
    
    pFormat->m_nMaxErrors = iTagValue;
  }
        
  return true;
}









/*----------------------------------------------------------------------------*/
/* add job to demon list                                                      */
/*----------------------------------------------------------------------------*/
void AM::vJob_AddDemon(Job* pJob)
{
  AM::pDemonJobs()->vInsertJob(pJob);
}

/*----------------------------------------------------------------------------*/
/* add job to local list                                                      */
/*----------------------------------------------------------------------------*/
void AM::vJob_AddLocal(Job* pJob)
{
  AM::pLocalJobs()->vInsertJob(pJob);
}

/*----------------------------------------------------------------------------*/
/* add job to originating list                                                */
/*----------------------------------------------------------------------------*/
void AM::vJob_AddOriginating(Job* pJob)
{
  AM::pOriginatingJobs()->vInsertJob(pJob);
}

/*----------------------------------------------------------------------------*/
/* add job to participating list                                              */
/*----------------------------------------------------------------------------*/
void AM::vJob_AddParticipating(Job* pJob)
{
  AM::pParticipatingJobs()->vInsertJob(pJob);
}

/*----------------------------------------------------------------------------*/
/* add job to retired list                                                    */
/*----------------------------------------------------------------------------*/
void AM::vJob_AddRetired(Job* pJob)
{
  AM::pRetiredJobs()->vInsertJob(pJob);
}

/*----------------------------------------------------------------------------*/
/* dump demon job list                                                        */
/*----------------------------------------------------------------------------*/
void AM::vJob_DumpDemon(void)
{
  AM::pDemonJobs()->vListJobs();
}

/*----------------------------------------------------------------------------*/
/* dump local job list                                                        */
/*----------------------------------------------------------------------------*/
void AM::vJob_DumpLocal(void)
{
  AM::pLocalJobs()->vListJobs();
}

/*----------------------------------------------------------------------------*/
/* dump originating job list                                                  */
/*----------------------------------------------------------------------------*/
void AM::vJob_DumpOriginating(void)
{
  AM::pOriginatingJobs()->vListJobs();
}

/*----------------------------------------------------------------------------*/
/* dump participating job list                                                */
/*----------------------------------------------------------------------------*/
void AM::vJob_DumpParticipating(void)
{
  AM::pParticipatingJobs()->vListJobs();
}

/*----------------------------------------------------------------------------*/
/* find job in originating list                                               */
/*----------------------------------------------------------------------------*/
Job* AM::pJob_FindOriginating(JobId tJobId)
{
  Job* pJob = AM::pOriginatingJobs()->pFindJob(tJobId);
    
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* find job in participating list                                             */
/*----------------------------------------------------------------------------*/
Job* AM::pJob_FindParticipating(JobId tJobId)
{
  Job* pJob = AM::pParticipatingJobs()->pFindJob(tJobId);
  
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* remove job from local list                                                 */
/*----------------------------------------------------------------------------*/
void AM::vJob_RemoveLocal(Job* pJob)
{
  AM::pLocalJobs()->vRemoveJob(pJob);
}

/*----------------------------------------------------------------------------*/
/* remove job from originating list                                           */
/*----------------------------------------------------------------------------*/
void AM::vJob_RemoveOriginating(Job* pJob)
{
  AM::pOriginatingJobs()->vRemoveJob(pJob);
}

/*----------------------------------------------------------------------------*/
/* remove job from participating list                                         */
/*----------------------------------------------------------------------------*/
void AM::vJob_RemoveParticipating(Job* pJob)
{
  AM::pParticipatingJobs()->vRemoveJob(pJob);
}

/*----------------------------------------------------------------------------*/
/* remove job from retired list                                               */
/*----------------------------------------------------------------------------*/
void AM::vJob_RemoveRetired(Job* pJob)
{
  AM::pRetiredJobs()->vRemoveJob(pJob);
}

/*----------------------------------------------------------------------------*/
/* update heartbeat info for a given node by ID                               */
/*----------------------------------------------------------------------------*/
void AM::vPutHeartbeat(i8 iNode, const Heartbeat& xHeartbeat)
{
  if (AM::SYSTEM::bStarted()) {
    Node *pThisNode = AM::pGrid()->pFindNode(iNode);

    if (pThisNode) {
      pThisNode->vPutHeartbeat(xHeartbeat);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* update heartbeat for a given node by name                                  */
/*----------------------------------------------------------------------------*/
bool AM::bPutHeartbeat(Stz* pszName, const Heartbeat& xHeartbeat )
{
  if (AM::SYSTEM::bStarted()) {
    Node *pThisNode = AM::pGrid()->pFindNode(pszName);

    if (pThisNode) {
      pThisNode->vPutHeartbeat(xHeartbeat);
      return true;
    }
  }
  return false;
}

/*----------------------------------------------------------------------------*/
/* get heartbeat info for a given node ID                                     */
/*----------------------------------------------------------------------------*/
Heartbeat AM::xGetHeartbeat(i8 iNode)
{
  Node *pThisNode = AM::SYSTEM::bStarted() ? AM::pGrid()->pFindNode(iNode) : nullptr;

  if (!pThisNode) {
    vThrow(FK_FATAL, __func__, __FILE__, __LINE__,
           "Cannot find Node %lld in the cluster"_Stz,
           iNode);
  }
  return pThisNode->xGetHeartbeat();
}

/*----------------------------------------------------------------------------*/
/* get heartbeat info for a give node                                         */
/*----------------------------------------------------------------------------*/
Heartbeat AM::xGetHeartbeat(Stz* pszName)
{
  Node *pThisNode = AM::SYSTEM::bStarted() ? AM::pGrid()->pFindNode(pszName) : nullptr;;

  if (!pThisNode) {
    vThrow(FK_FATAL, __func__, __FILE__, __LINE__,
           "Cannot find Node %s in the cluster"_Stz,
           pszName->azChar());
  }

  return pThisNode->xGetHeartbeat();

}

/**
 * @brief Remove dead node from the cluster
 * @details When a node went down, remove it from the cluster.
 * <p>
 * @param[in] pszName  name of this node
 *
 *
 * @return none
 * @Todo  Must verify that we remove and delete the node here.
 */
void AM::vRemoveNode(Stz* pszName)
{
  if (AM::SYSTEM::bStarted()) {
    Node *pThisNode = AM::pGrid()->pFindNode(pszName);
    // Check to make sure we have this node in the cluster/grid.
    if (pThisNode) {

      AM::pGrid()->vRemoveNode(pThisNode); // BootNodes is decr in this function
      //delete pThisNode;

    }
  }
}


/*----------------------------------------------------------------------------*/
/* get supervisor halt flag                                                   */
/*----------------------------------------------------------------------------*/
bool AM::bSupervisorHalt(void)
{
  return gpAttachMap->m_bSupervisorHalt;
}

/*----------------------------------------------------------------------------*/
/* set supervisor halt flag                                                   */
/*----------------------------------------------------------------------------*/
void AM::vSupervisorHalt(bool bHalt)
{
  gpAttachMap->m_bSupervisorHalt = bHalt;
}



/*----------------------------------------------------------------------------*/

void where(void)
{
  AM::SE::vWhereAmI();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
