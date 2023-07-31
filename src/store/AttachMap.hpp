/*============================================================================*/
/* AttachMap.hpp - map of addresses when process attaches                     */
/*============================================================================*/
#pragma once

#include "Resources.hpp" //?
#include "ReapLock.hpp"
#include "TraceMultiLock.hpp"
#include "Starter.hpp"
#include "EventHandlers.hpp"
#include "SysMetrics.hpp"
#include "CmdLineArgs.hpp"
#include "MarkerMgr.hpp"
#include "eAC.h"

namespace STORE { class InternalTables; class Jobs; class Numbering; }

namespace STORE
{
  static const u8 C_CREATED = 0x2444455441455243;   // $CREATED
  static const u8 C_READY   = 0x2424594441455224;   // $READY$$
  
  class AttachMap : public Object
  {
  friend class OBLIQUE::AM;
  friend class ENGINE::Engine;
  friend class PROMPT::SePrompt;

  private:
  
    union {
      u8    m_uAttachMapState;         // attachmap is ready
      char  m_caAttachMapState[8];
    };
  
    Birth m_oBirth;
    Now   m_oNow;
    i8    m_iSePid;
    Timer m_oSince;
    i8    m_iTimePoint;
  
    s256  m_sLogRegion;
    s256  m_sLogCsvFile;
    s256  m_sLogTxtFile;
  
    bool          m_bBootError;
    BufferMap*    m_pBufferMap;
    BufferPool*   m_pBufferPool;
    ClassFlags    m_oClassFlags;
    Atomic        m_ixCmdNo;
    Word*         m_pConstantRoot;
    Dic*          m_pDicRoot;
    i8            m_iEpoch;
    EventHandlers m_oEventHandlers;
    FileMap*      m_pFileMap;
    Format        m_oFormat;
    Grid*         m_pGrid;
    HashMap*      m_pHashMap;
    Heaps         m_xHeaps;
    InternalTables* m_pInternalTables;
    Jobs*         m_pJobs;
    Atomic        m_nxJobsActive;
    i8            m_nJobsThreshold;
    LglPtnMap*    m_pLglPtnMap;
    Locks         m_oLocks;
    MarkerMgr*    m_pMarkerMgr;
    Numbering*    m_pNumbering;
    ObjectSnapshot* m_pSnapshotAtTrackingMemory;  // object delta reporting
    ObjectSnapshot* m_pSnapshotAtMemoryLeaks;     // object delta reporting
    Atomic        m_nxOperands;
    Atomic        m_ixQueryNo;
    i8            m_iQuit;
    b8            m_bReady;
    ReapLock      m_oReapLock;
    Timer         m_oReferenceTimer;
    Resources     m_oResources;
    Schedule*     m_pSchedule;
    bool          m_bStarted;
    Starter*      m_pStarter;
    Steps*        m_pSteps;
    i8            m_iStop;
    bool          m_bSupervisorHalt;
    SysArgs       m_xSysArgs;
    SysMetrics*   m_pSysMetricsChain;
    b8            m_bSystemOpen;
    Word*         m_pTenants;
    TempName      m_oTempName;
    TraceLock      m_oTraceLock;
    TraceMultiLock m_oTraceMultiLock;
    TxSet*        m_pTxSet;
    Word*         m_pWordRoot;

    JobId         m_tAnswerDemonJobId;
    JobId         m_tAnswerWLDemonJobId;
    JobId         m_tCollectDemonJobId;
    JobId         m_tConnectorDemonJobId;
    JobId         m_tEventDemonJobId;
    JobId         m_tExitDemonJobId;
    JobId         m_tMetricsDemonJobId;
    JobId         m_tMonDemonJobId;
    JobId         m_tNodeMonDemonJobId;
    JobId         m_tPageInDemonJobId;
    JobId         m_tPageOutDemonJobId;
    JobId         m_tReaperDemonJobId;
    JobId         m_tRecurDemonJobId;
    JobId         m_tStatDemonJobId;
    JobId         m_tWakerDemonJobId;
    JobId         m_tSuperDemonJobId;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    AttachMap(bool bNoFree);
   ~AttachMap(void) noexcept(false);
    
    static void vSubSystem(TokenVec* pTokenVec);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    void vAttachMapState(u8 uState);
    u8   uAttachMapState(void);
  
    i8   iCmdNoNext(void);
    b1   bDisablePromptCommands(void);

    eAC  eFindApi(Stz* aApiNm);
  
    void vGetGlobals(void);
  
    void vIndexEntries(i8 nIndexEntries);
  
    Jobs* pJobs(void);
  
    MarkerMgr* pMarkerMgr(void);
  
    void vNextTempName(s32& sName);
  
    void vOutline(s4K& sOutline);
  
    i8   iSePid(void);
    void vPrintAttachMap(void);
    void vPrintStartupInfo(void);

    void vReapLockDrop(void);
    void vReapLockGrab(void);
    void vRecordArguments(CmdLineArgs* CmdLineArgs);
  
    void vSysMetricsAdd(SysMetrics* pSysMetrics);
    SysMetrics* pSysMetricsChain(void);

    void vTraceLockDrop(void);
    void vTraceLockGrab(void);
    void vTraceMultiLockDrop(void);
    void vTraceMultiLockGrab(void);

    Footer* aFooter(void);
  };
  
  AttachMap* _pAttachMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
