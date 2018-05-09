/*============================================================================*/
/* Arguments.hpp - command line arguments                                     */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  class Arguments : public Object
  {
  public:
    // configuration
    void* m_aRegionAddr; // use this address for region
    s256  m_sRegionName; // name of shared memory region
    s256  m_sRegionPrefix; // prefix of the shared memory region name
    i8    m_iRegionSize; // size of region in bytes
    i8    m_nSlots;      // partitions per node
    s256  m_sNodeName;   // this node name
    i8    m_nNodes;      // out of this number of nodes
    i8    m_iThisNode;   // this node no in the m_xNode list
    bool  m_bTracepoint; // print tracepoint
    s256  m_sCommands;   // commands dir
    s256  m_sSen;        // sen dir
    
    // commands
    s256 m_sCommand;    // command to run
    
    bool m_bCommand;    // run command
    bool m_bDocol;      // run the docol> interpreter
    bool m_bHelp;       // display help
    bool m_bPrompt;     // run the se> prompt
    bool m_bScript;     // run in scripting mode (like prompt but no prompt)
    bool m_bStart;      // start system (shm not exist, shm create, root thread & task)
    bool m_bStatus;     // get status
    bool m_bStop;       // force stop system (kill tasks & threads, shm unmap & unlink)
    bool m_bUnlink;     // unlink shm region only
    bool m_bLock;       // lock shared memory
    i8   m_iLevels;     // index levels
    i8   m_iFrequency;  // index level frequency

    // options
    bool m_bBgTasks;    // start background tasks
    bool m_bConfig;     // print configuration information
    s256 m_sFolder;     // working folder
    bool m_bHugePages;  // to use huge pages
    bool m_bProgress;   // print initialisation progress
    bool m_bRestart;    // restart if system already started
    bool m_bTouch;      // touch all shm pages during region create 
    bool m_bTracing;    // tracing
    bool m_bVerbose;    // print extra stuff
    bool m_bWelcome;    // print banner
    i8   m_iSleep;      // sleep period for yield (temporary)
    bool  m_bMetadata;  // print metadata information
    bool  m_bMunged;    // load munged words
    bool  m_bKillPrev;  // kill previous se's (temporary)
    bool  m_bDontFree;  // dont actually free objects back to heap
    bool  m_bFreeRB;    // but free RowBuffers back to heap
    bool  m_bReload;    // relaod from checkpoint files
    i8    m_nEntries;   // no of entries per buffer
    i8    m_nSplitRatio; // split ratio of indexes
    i8    m_nMaxQueue;   // max queue partition thread can build up
  
    bool  m_bStrictMode;// are we in strict (or relaxed mode)?
  
    // working
    i8   m_nJumboBuffers; // number of jumbo buffers
    i8   m_nJumboTook;    // number of pool buffers taken by jumbo buffers
    i8   m_iJumboSize;    // size of the jumbo heap
    
    struct {
      s256 m_sNodeName;
      s256 m_sIPAddress;
      i8   m_iPortNo;
    } m_xNodes[C_MAX_NODES];
    
    // must be last
    Footer m_xFooter;
    
  public:
    Arguments(void);
    ~Arguments(void);
    
    void vEnvironmentVariables(void);
    
    bool bParse(int argc, Stz* argv[]);
  
    void vPrintStartupInfo(void);
    
    void vPrintHelp(void);
  
    void vScribble(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
