/*============================================================================*/
/* Prompt_config.cpp -- simple configuration report                           */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_config (
  "Prompt_config"_Stz,  // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "config"_Stz,         // the prompt command
  nullptr,              // the second word
  bPrompt_config,       // prompt function
  true,                 // enabled

  true,                 // visible
  "fun help"_Stz        // help
);

extern Arguments   gxArguments;
extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_config(SePrompt* pPrompt)
{
  s32 sShmSiz;
   Magnitudes::vMagnitude(gpAttachMap->m_iRegionSize, sShmSiz);
  
  vTrace("  Node: %s Shm %s Nodes %lld Slots %lld Threads %lld SubLoaders %lld : %s\n"_Stz,
         gxArguments.m_sNodeName.aChar(),
         sShmSiz.aChar(),
         gxArguments.m_nNodes,
         gxArguments.m_nSlots,
         gxArguments.m_nThreads,
         gxArguments.m_nSubLoaders,
         gpAttachMap->m_pGrid->m_bHasStarted ? "cluster started"
                                            : "cluster not started"
         );
  
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
