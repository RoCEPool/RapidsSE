/*============================================================================*/
/* Prompt_config.cpp -- simple configuration report                           */
/*============================================================================*/

#include "Prompt.h"
#include "AM.hpp"

PromptDesc __Prompt_config (
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
  
  /*
  vTrace("  Node %s Shm %s Free %s Nodes %lld Slots %lld Threads %lld SubIngesters %lld SubLoaders %lld : %s\n"_Stz,
         gxArguments.m_sNodeName.aChar(),
         sShmSize.aChar(),
         sFreeSpace.aChar(),
         gxArguments.m_nNodes,
         gxArguments.m_nSlotsPerNode,
         gxArguments.m_nThreads,
         gxArguments.m_nSubIngesters,
         gxArguments.m_nSubLoaders,
         AM::pGrid()->m_bHasStarted ? "cluster started"
                                            : "cluster not started"
         );
  
  */
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
