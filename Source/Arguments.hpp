/*============================================================================*/
/* Arguments.hpp - command line arguments                                     */
/*============================================================================*/

#pragma once

#include "StorageEngine.h"

namespace StorageEngine
{
  class Arguments : public Header
  {
  public:
    // configuration
    void* m_pRegionAddr; // use this address for region
    s256  m_sRegionName; // name of shared memory region
    i8    m_iRegionSize; // size of region in bytes
    i8    m_nPartitions; // partitions per node
    i8    m_iNode;       // this node number
    i8    m_nNodes;      // out of this number of nodes
    i8    m_iPort;       // port number
    bool  m_bTracepoint; // print tracepoint
    s256  m_sCommands;   // commands dir
    
    // commands
    s256 m_sCommand;    // command to run
    
    bool m_bCommand;    // run command
    bool m_bDocol;      // run the docol> interpreter
    bool m_bHelp;       // display help
    bool m_bPrompt;     // run the se> prompt
    bool m_bStart;      // start system (shm not exist, shm create, root thread & task)
    bool m_bStatus;     // get status
    bool m_bStop;       // force stop system (kill tasks & threads, shm unmap & unlink)
    bool m_bUnlink;     // unlink shm region only
    
    // options
    bool m_bBgTasks;    // start background tasks
    bool m_bConfig;     // print configuration information
    s256 m_sFolder;     // working folder
    bool m_bHugePages;  // to use huge pages
    bool m_bProgress;   // print initialisation progress
    bool m_bRestart;    // restart if system already started
    bool m_bTouch;      // touch all shm pages during region create 
    bool m_bTracing;    // tracing
    bool m_bUnitTests;  // run unit tests
    bool m_bVerbose;    // print extra stuff
    bool m_bWelcome;    // print banner
    
    // seitf options
    i8   m_iPartition;  // which partition
    
    // must be last
    Footer m_xFooter;
    
  public:
    Arguments(void);
    ~Arguments(void);
    
    void vEnvironmentVariables(void);
    
    bool bParse(int argc, const char* argv[]);
    
    void vPrint(void);
    
    void vPrintHelp(void);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
