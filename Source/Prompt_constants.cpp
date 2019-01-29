/*============================================================================*/
/* Prompt_constants.cpp -- important constants report                         */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_constants (
  "Prompt_constants"_Stz,  // name of the prompt function
  "comment"_Stz,           // comment about the functopm
  "constants"_Stz,         // the prompt command
  nullptr,                 // the second word
  bPrompt_constants,       // prompt function
  true,                    // enabled

  true,                    // visible
  "fun help"_Stz           // help
);

extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_constants(SePrompt* pPrompt)
{
  vTrace("  C_LOW_BUFFERS       = %lld\n"_Stz,C_LOW_BUFFERS);
  vTrace("  C_MAX_PRECISION_BCD = %lld\n"_Stz,C_MAX_PRECISION_BCD);
  vTrace("  C_MAX_COLUMNS       = %lld\n"_Stz,C_MAX_COLUMNS);
  vTrace("  C_MAX_NODES         = %lld\n"_Stz,C_MAX_NODES);
  vTrace("  C_MAX_PARTITIONS    = %lld\n"_Stz,C_MAX_PARTITIONS);
  vTrace("  C_MAX_RECORD        = %lld\n"_Stz,C_MAX_RECORD);
  vTrace("  C_MAX_SESSION       = %lld\n"_Stz,C_MAX_SESSIONS);
  vTrace("  C_MIN_REGION        = %lld\n"_Stz,C_MIN_REGION);
  vTrace("  C_MEMORY_PERCENT    = %lld\n"_Stz,C_MEMORY_PERCENT);
  vTrace("  C_PATH_MAX          = %lld\n"_Stz,C_PATH_MAX);
  vTrace("  C_SLOTS_PER_NODE    = %lld\n"_Stz,C_SLOTS_PER_NODE);
  vTrace("  C_WSP_DATACHUNK     = %lld\n"_Stz,C_WSP_DATACHUNK);
  vTrace("  C_WSP_STACK         = %lld\n"_Stz,C_WSP_STACK);
  vTrace("  C_TOKENS            = %lld\n"_Stz,C_TOKENS);
  vTrace("\n"_Stz);


  vTrace("  C_SIZE_SMALL_OBJECT  = %lld\n"_Stz,C_SIZE_SMALL_OBJECT);
  vTrace("  C_SIZE_MEDIUM_OBJECT = %lld\n"_Stz,C_SIZE_MEDIUM_OBJECT);
  vTrace("  C_SIZE_LARGE_OBJECT  = %lld\n"_Stz,C_SIZE_LARGE_OBJECT);
  vTrace("  C_SIZE_JUMBO_OBJECT  = %lld\n"_Stz,C_SIZE_JUMBO_OBJECT);
  vTrace("  C_SIZE_POOL_BUFFER   = %lld\n"_Stz,C_SIZE_POOL_BUFFER);
  vTrace("\n"_Stz);
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
