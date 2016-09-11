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
    s256 m_sRegionName; // name of shared memory region
    i8   m_iRegionSize; // size of region in bytes
    i8   m_nPartitions; // partitions per node
    
    // commands
    s256 m_sCommand;    // command to run
    
    bool m_bCommand;    // run command
    bool m_bDocol;      // run the docol> interpreter
    bool m_bHelp;       // display help
    bool m_bList;       // list shm regions using /tmp
    bool m_bPrompt;     // run the se> prompt
    bool m_bStart;      // start system (shm not exist, shm create, root thread & task)
    bool m_bStatus;     // get status
    bool m_bStop;       // force stop system (kill tasks & threads, shm unmap & unlink)
    
    // options
    bool m_bBgTasks;    // start background tasks
    bool m_bVerbose;    // print extra stuff
    bool m_bWelcome;    // print banner
    s256 m_sFolder;     // working folder
    
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