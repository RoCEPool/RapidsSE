/*============================================================================*/
/* Prompt_config.cpp -- simple configuration report                           */
/*============================================================================*/

#include "Prompt.h"

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
  
  vTrace("  Node %s ShmLib %s Free %s Nodes %lld Slots %lld Threads %lld SubIngesters %lld SubLoaders %lld : %s\n"_Stz,
         gxArgs.m_sNodeName.azChar(),
         sShmSize.azChar(),
         sFreeSpace.azChar(),
         gxArgs.m_nNodes,
         gxArgs.m_nSlotsPerNode,
         gxArgs.m_nThreads,
         gxArgs.m_nSubIngesters,
         gxArgs.m_nSubLoaders,
         AM::pGrid()->bHasStarted() ? "cluster started"
                                    : "cluster not started"
         );
    
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
