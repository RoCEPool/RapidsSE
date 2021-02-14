/*============================================================================*/
/* AttachMap.hpp - map of addresses when process attaches                     */
/*============================================================================*/
#pragma once

#include "Resources.hpp" //?

namespace STORE { class ObjectNumbering; }

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
  
    i8    m_iPid;
    Birth m_oBirth;
    Now   m_oNow;
    i8    m_iTimePoint;
    
    s256  m_sTraceFile;
    s256  m_sLogRegion;
    s256  m_sLogCsvFile;
    s256  m_sLogTxtFile;
  
    SysArgs m_xSysArgs;
        
    bool          m_bBootError;
    BucketMap*    m_pBucketMap;
    BufferMap*    m_pBufferMap;
    BufferPool*   m_pBufferPool;
    EnumFlags     m_oClassFlags;
    i8            m_iEpoch;
    Format        m_oFormat;
    Grid*         m_pGrid;
    HeapMem       m_xHeapMem;
    Article*      m_pLexiconArticle;
    LockTracker   m_oLockTracker;
    ObjectNumbering* m_pObjectNumbering;
    Atomic        m_nxOperands;
    PtnMap*       m_pPtnMap;
    i8            m_iQuit;
    Resources     m_oResources;
    Article*      m_pRootArcticle;
    Word*         m_pRootConstant;
    Dic*          m_pRootDic;
    Word*         m_pRootWord;
    Schedule*     m_pSchedule;
    Sessions*     m_pSessions;
    Statistics    m_oStatistics;
    StepTiming*   m_pStepTiming;
    i8            m_iStop;
    Word*         m_pTenants;
    TempName      m_oTempName;
    TxSet*        m_pTxSet;
  
    struct {
      
      JobList*  m_pDemonJobs;
      JobList*  m_pLocalJobs;
      JobList*  m_pOriginatingJobs;
      JobList*  m_pParticipatingJobs;
      
      bool m_bSupervisorHalt;

    } m_xStore;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    AttachMap(void);
    ~AttachMap(void) noexcept(false);
    
    static void vSubSystem(SelfFn* aSelfFn, TokenVec* pTokenVec);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    void vAttachMapState(u8 uState);
    u8   uAttachMapState(void);
    eAC  eFindApi(Stz* aApiNm);
    void vFocalOutline(s4K& sOutline);
    void vGetGlobals(void);
    void vIndexEntries(i8 nIndexEntries);
    void vNextTempName(s32& sName);
    i8   iPid(void);
    void vPrintAttachMap(void);
    void vPrintStartupInfo(void);
  
    void vRecordArguments(CmdLineArgs* CmdLineArgs);
    Sessions* pSessions(void);

    Footer* aFooter(void);
  };
  
  AttachMap* _pAttachMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
