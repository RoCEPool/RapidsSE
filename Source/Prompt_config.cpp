/*============================================================================*/
/* Prompt_config.cpp -- simple configuration report                           */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_config (
  PF_CONFIG,            // prompt function
  "Prompt_config"_Stz,  // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "config"_Stz,         // the prompt command
  nullptr,              // the second word
  bPrompt_config,       // prompt function
  true,                 // enabled

  true,                 // visible
  "config"_Stz          // help
);

extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_config(SePrompt* pPrompt)
{
  s32 sShmSize;
  Magnitudes::vByteMag(gpAttachMap->Arg.m_iRegionSize, sShmSize);
  
  s32 sFreeSpace;
  Magnitudes::vByteMag(gpBufferPool->iFreeSpace(), sFreeSpace);
  
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
         gpAttachMap->m_pGrid->m_bHasStarted ? "cluster started"
                                            : "cluster not started"
         );
  
  */
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
