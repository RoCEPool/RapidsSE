/*============================================================================*/
/* Args.hpp - command line arguments                                     */
/*============================================================================*/
#pragma once

namespace Arguments
{
  class Args : public Object
  {
    friend class Oblique::AM;
    friend class StorageEngine::Engine;
    friend class LexicalAnalysis::Charser;
    friend class LexicalAnalysis::Input;

    friend class Prompt::SePrompt;

  public:
  
    // configuration of this node
    s256  m_sCommandsDir;  // commands dir
    i8    m_iConnectCode;  // connect code (not yet used)
    i8    m_nDegOfPar;     // degree of parallelism
    s32   m_sGridName;     // name of the grid
    s32   m_sNodeName;     // this node name
    void* m_aRegionAddr;   // use this address for region
    s32   m_sRegionName;   // name of shared memory region
    s32   m_sRegionPrefix; // prefix of the shared memory region name
    i8    m_iRegionSize;   // size of region in bytes
    s256  m_sSenDir;       // sen dir
    i8    m_nTablePtns;    // partitions per table
  
    i8    m_iLiftNo;
    
    // grid configuration at boot
    i8    m_nBootNodes;
    struct {
      s32 m_sNodeName;     // name of node
      s32 m_sIPAddress;    // ip address of node
      i8  m_iPortNo;       // port number
    } m_xBootNode[C_MAX_NODES];
    i8   m_iThisNode;      // this node no in the m_xBootNode list       //! FIX

    TagSet m_oArgTagSet;  // arguments tag set
  
    // command
    bool m_bAddNode;     // add node
    bool m_bAppendTrace; // append to trace file
    bool m_bCommand;     // run command
    bool m_bHelp;        // display help
    bool m_bInitial;     // run command is an initial command
    bool m_bPrompt;      // run the se> prompt
    bool m_bSethScript;  // run in scripting mode (like prompt but no prompt)
    bool m_bStart;       // start system
                         //   (shm not exist, shm create, root thread & task)
    bool m_bStatus;      // get status
    bool m_bStop;        // force stop system
                         //   (kill tasks & threads, shm unmap & unlink)
    bool m_bUnlink;      // unlink shm region only

    // options
    s256 m_sCommand;         // command to run
    i8   m_iCondCode;        // conditional compilation cond value
    bool m_bConfig;          // print configuration information
    bool m_bDebugIndex;      // debug indexes
    bool m_bDebugMode;       // debug mode
    bool m_bDeveloper;       // user is a developer
    bool m_bDontFree;        // dont actually free objects back to heap
    i8   m_nDummyThreads;    // number of dummy threads
    i8   m_nDummySleep;      // sleep period for dummy threads
    i8   m_nDummyLoop;       // loop count for dummy threads
    i8   m_nEntries;         // no of entries per buffer
    bool m_bFreeCB;          // but free CrwBuffers back to heap
    bool m_bGridTest;        // running a single machine multi-node test
    bool m_bHugePages;       // to use huge pages
    s256 m_sInitial;         // initial to run
    bool m_bJot;             // jotting
    bool m_bKillPrev;        // kill previous se's (temporary)
    bool m_bLock;            // lock shared memory pages in memory
    bool m_bLog;             // use shared log
    i8   m_nLvlLevels;       // maximum number of levels
    i8   m_iLvlRatio;        // level ratio
    bool m_bNoMunge;         // prompt not to munge
    i8   m_nMaxLoadErrors;   // maximum load errors
    i8   m_nMaxReadyLP;      // maximum ready lp queue
    bool m_bMetadata;        // print metadata information
    bool m_bMunged;          // load munged words
    i8   m_nOperands;        // maximum number of operands in the system
    i8   m_nPeriodicSleep;   // sleep period for periodic job
    bool m_bProgress;        // print initialisation progress
    i8   m_iPromptDebug;     // prompt debug mode
    bool m_bReload;          // relaod from checkpoint files
    bool m_bRestart;         // restart if system already started
    bool m_bRLUDiary;        // RLU diary
    bool m_bRLUTimer;        // RLU timer
    i8   m_nSubIngesters;    // number of subloaders
    i8   m_nSubLoaders;      // number of subloaders
    i8   m_nSupervisorSleep; // sleep period for supervisor job
    i8   m_nThreads;         // number of general threads
    bool m_bTouch;           // touch all shm pages during region create
    bool m_bTracepoint;      // print tracepoint
    bool m_bTracing;         // tracing
    bool m_bVerbose;         // print extra stuff
    bool m_bWelcome;         // print banner
    
    // must be last
    Footer m_xFooter;
    
  public:
    Args(void);
   ~Args(void) noexcept(false);
    
    static i8 nBurstArgs(s256 saArgv[], Stz* szArgStr);

    static void vLog(void);
  
    void vEnvironmentVariables(void);
    bool bParse(i8 argc, s256 szaArgv[]);
    void vPrint(void);
    void vPrintStartupInfo(void);
    void vPrintHelp(void);
    void vSanityCheck(void);
    void vScribble(void);
  
    bool bUserPrompt(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
