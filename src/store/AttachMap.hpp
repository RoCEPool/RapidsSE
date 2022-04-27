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
#include "eAC.h"

namespace STORE { class InternalTables; class Jobs; class Numbering; }

namespace STORE
{
  static const u8 C_CREATED = 0x2444455441455243;   // $CREATED
  static const u8 C_READY   = 0x2424594441455224;   // $READY$$
  
  class AttachMap : public Object
  {
  friend class OBLIQUE::AM;
  friend class Engine;
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
  
    s256  m_sTraceFile;
    s256  m_sLogRegion;
    s256  m_sLogCsvFile;
    s256  m_sLogTxtFile;
  
    bool          m_bBootError;
    BucketMap*    m_pBucketMap;
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
#ifdef WITH_HASHMAP
    HashMap*      m_pHashMap;
#endif
    Heaps         m_xHeaps;
    InternalTables* m_pInternalTables;
    Jobs*         m_pJobs;
    Atomic        m_nxJobsActive;
    i8            m_nJobsThreshold;
    LglPtnMap*    m_pLglPtnMap;
    Locks         m_oLocks;
    Numbering*    m_pNumbering;
    Atomic        m_nxOperands;
    Atomic        m_ixQueryNo;
    i8            m_iQuit;
    b8            m_bReady;
    b8            m_bReapEnabled;
    ReapLock      m_oReapLock;
    Timer         m_oReferenceTimer;
    Resources     m_oResources;
    Schedule*     m_pSchedule;
    bool          m_bStarted;
    Starter*      m_pStarter;
    Statistics    m_oStatistics;
    Steps*        m_pSteps;
    i8            m_iStop;
    bool          m_bSupervisorHalt;
    SysArgs       m_xSysArgs;
    SysMetrics*   m_pSysMetricsChain;
    b8            m_bSystemOpen;
    Word*         m_pTenants;
    TempName      m_oTempName;
    File*         m_pTraceFile;
    TraceLock      m_oTraceLock;
    TraceMultiLock m_oTraceMultiLock;
    TxSet*        m_pTxSet;
    Word*         m_pWordRoot;

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

    eAC  eFindApi(Stz* aApiNm);
  
    void vGetGlobals(void);
  
    void vIndexEntries(i8 nIndexEntries);
  
    void vNextTempName(s32& sName);
  
    void vOutline(s4K& sOutline);
  
    i8   iSePid(void);
    void vPrintAttachMap(void);
    void vPrintStartupInfo(void);
  
    bool bReapEnabled(void);
    void vReapEnabled(bool bReapEnabled);
    void vReapLockDrop(void);
    void vReapLockGrab(void);
    void vRecordArguments(CmdLineArgs* CmdLineArgs);
  
    void vSysMetricsAdd(SysMetrics* pSysMetrics);
    SysMetrics* pSysMetricsChain(void);

    File* pTraceFile(void);
    void  vTraceFile(File* pTraceFile);
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
