/*============================================================================*/
/* Prompt_config.cpp -- simple configuration report                           */
/*============================================================================*/

#include "StorageEngine.h"

extern Args gxArgs;

PromptFun __Prompt_config (
  PF_CONFIG,            // prompt function
  "Prompt_config"_Stz,  // name of the prompt function
  "comment"_Stz,        // comment about the function
  "config"_Stz,         // the prompt command
  nullptr,              // the second word
  bPrompt_config,       // prompt function
  true,                 // enabled

  true,                 // visible
  "config"_Stz,         // help
  ""_Stz                // description
);


/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_config(SePrompt* pPrompt)
{
  s32 sShmSize;
  MagLib::vByteMag(AM::iRegionSize(), sShmSize);
  
  s32 sFreeSpace;
  MagLib::vByteMag(gpBufferPool->iFreeSpace(), sFreeSpace);
  
  vTrace("  Node %s ShmLib %s Free %s Nodes %lld TablePtns %lld Tasking %lld : %s\n"_Stz,
         gxArgs.m_sNodeName.azChar(),
         sShmSize.azChar(),
         sFreeSpace.azChar(),
         gxArgs.m_nBootNodes,
         gxArgs.m_nPtnsPerTable,
         gxArgs.m_nThreads,

         AM::pGrid()->bHasStarted() ? "cluster started"
                                    : "cluster not started"
         );
    
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
