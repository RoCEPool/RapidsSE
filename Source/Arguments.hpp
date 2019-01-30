/*============================================================================*/
/* Arguments.hpp - command line arguments                                     */
/*============================================================================*/
#pragma once

namespace Commands      { class Command; class Parser; }
namespace StorageEngine { class Engine; }

namespace StorageEngine
{
  class Arguments : public Object
  {
    friend class      Commands::Command;
    friend class StorageEngine::Engine;
    friend class      Commands::Parser;

  private:
  
    // configuration
    s256  m_sCommandsDir;  // commands dir
    i8    m_nJumboBuffers; // number of jumbo buffers
    s32   m_sNodeName;     // this node name
    void* m_aRegionAddr;   // use this address for region
    s32   m_sRegionName;   // name of shared memory region
    s32   m_sRegionPrefix; // prefix of the shared memory region name
    i8    m_iRegionSize;   // size of region in bytes
    s256  m_sSenDir;       // sen dir
    i8    m_nSlotsPerNode; // partition slots per node
    
    // node configuration
    i8    m_nNodes;
    struct {
      s32 m_sNodeName;     // name of node
      s32 m_sIPAddress;    // ip address of node
      i8  m_iPortNo;       // port number
    } m_xNodes[C_MAX_NODES];
  
    s256 m_sCommand;    // command to run
    
    // commands
    bool m_bCommand;    // run command
    bool m_bHelp;       // display help
    bool m_bPrompt;     // run the se> prompt
    bool m_bScript;     // run in scripting mode (like prompt but no prompt)
    bool m_bStart;      // start system (shm not exist, shm create, root thread & task)
    bool m_bStatus;     // get status
    bool m_bStop;       // force stop system (kill tasks & threads, shm unmap & unlink)
    bool m_bUnlink;     // unlink shm region only

    // options
    bool m_bActivityMap; // use activity map
    bool m_bConfig;     // print configuration information
    bool m_bDeveloper;  // user is a developer
    bool m_bDontFree;   // dont actually free objects back to heap
    i8   m_nDummyThreads; // number of dummy threads
    i8   m_nDummySleep;   // sleep period for dummy threads
    i8   m_nDummyLoop;    // loop count for dummy threads
    i8   m_nEntries;    // no of entries per buffer
    bool m_bFreeRB;     // but free RowBuffers back to heap
    bool m_bLock;       // lock shared memory pages in memory
    bool m_bHugePages;  // to use huge pages
    bool m_bKillPrev;   // kill previous se's (temporary)
    bool m_bMetadata;   // print metadata information
    bool m_bMunged;     // load munged words
    i8   m_nPeriodicSleep; // sleep period for periodic job
    bool m_bProgress;   // print initialisation progress
    bool m_bReload;     // relaod from checkpoint files
    bool m_bRestart;    // restart if system already started
    i8   m_nSubIngesters; // number of subloaders
    i8   m_nSubLoaders;   // number of subloaders
    i8   m_iThisNode;     // this node no in the m_xNode list              //! FIX
    i8   m_nThreads;      // number of general threads
     bool m_bTouch;         // touch all shm pages during region create
    bool m_bTracepoint;    // print tracepoint
    bool m_bTracing;       // tracing
    bool m_bVerbose;       // print extra stuff
    bool m_bWelcome;       // print banner

    // working
    i8   m_nJumboTook;  // number of pool buffers taken by jumbo buffers
    i8   m_iJumboSize;  // size of the jumbo heap
    
    // must be last
    Footer m_xFooter;
    
  public:
    Arguments(void);
    ~Arguments(void) noexcept(false);
    
    static void vMark(void);
  
    void vAddConstants(void);
    void vEnvironmentVariables(void);
    bool bParse(int argc, Stz* argv[]);
    void vPrint(void);
    void vPrintStartupInfo(void);
    void vPrintHelp(void);
    void vSanityCheck(void);
    void vScribble(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
