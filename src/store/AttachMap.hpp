/*============================================================================*/
/* AttachMap.hpp - map of addresses when process attaches                     */
/*============================================================================*/
#pragma once

#include "Resources.hpp" //?
#include "ReapLock.hpp"

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
    i8    m_iPid;
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
    Word*         m_pConstantRoot;
    Dic*          m_pDicRoot;
    i8            m_iEpoch;
    Format        m_oFormat;
    Grid*         m_pGrid;
    Heaps         m_xHeaps;
    InternalTables* m_pInternalTables;
    Jobs*         m_pJobs;
    Locks         m_oLocks;
    Numbering*    m_pNumbering;
    Atomic        m_nxOperands;
    PtnMap*       m_pPtnMap;
    i8            m_iQuit;
    b8            m_bReady;
    b8            m_bReapEnabled;
    Resources     m_oResources;
    Schedule*     m_pSchedule;
    Sessions*     m_pSessions;
    bool          m_bStarted;
    Statistics    m_oStatistics;
    Steps*        m_pSteps;
    i8            m_iStop;
    bool          m_bSupervisorHalt;
    SysArgs       m_xSysArgs;
    b8            m_bSystemOpen;
    Word*         m_pTenants;
    TempName      m_oTempName;
    File*         m_pTraceFile;
    TraceLock     m_oTraceLock;
    TxSet*        m_pTxSet;
    Word*         m_pWordRoot;
    Atomic        m_ixQueryNo;
  
  private:
    ReapLock      m_oReapLock;

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
    eAC  eFindApi(Stz* aApiNm);
    void vOutline(s4K& sOutline);
    void vGetGlobals(void);
    void vIndexEntries(i8 nIndexEntries);
    void vNextTempName(s32& sName);
    i8   iPid(void);
    void vPrintAttachMap(void);
    void vPrintStartupInfo(void);
  
    bool  bReapEnabled(void);
    void  vReapEnabled(bool bReapEnabled);
    ReapLock* pReapLock(void);

    void vRecordArguments(CmdLineArgs* CmdLineArgs);
    Sessions* pSessions(void);
  
    File* pTraceFile(void);
    void  vTraceFile(File* pTraceFile);
    TraceLock* pTraceLock(void);

    Footer* aFooter(void);
  };
  
  AttachMap* _pAttachMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
