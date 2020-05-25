/*============================================================================*/
/* Prompt_constants.cpp -- important constants report                         */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_constants (
  PF_CONSTANTS,            // prompt function
  "Prompt_constants"_Stz,  // name of the prompt function
  "comment"_Stz,           // comment about the function
  "constants"_Stz,         // the prompt command
  nullptr,                 // the second word
  bPrompt_constants,       // prompt function
  true,                    // enabled

  true,                    // visible
  "constants"_Stz,         // help
  ""_Stz                   // description
);


/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_constants(SePrompt* pPrompt)
{
  vTrace("  C_LOW_BUFFERS       = %lld\n"_Stz,C_LOW_BUFFERS);
  vTrace("  C_MAX_BCD_PRECISION = %lld\n"_Stz,C_MAX_BCD_PRECISION);
  vTrace("  C_MAX_COLUMNS       = %lld\n"_Stz,C_MAX_COLUMNS);
  vTrace("  C_MAX_FLT_PRECISION = %lld\n"_Stz,C_MAX_FLT_PRECISION);
  vTrace("  C_MAX_INT_PRECISION = %lld\n"_Stz,C_MAX_INT_PRECISION);
  vTrace("  C_MAX_NODES         = %lld\n"_Stz,C_MAX_NODES);
  vTrace("  C_MAX_PARTITIONS    = %lld\n"_Stz,C_MAX_PARTITIONS);
  vTrace("  C_MAX_RECORD        = %lld\n"_Stz,C_MAX_RECORD);
  vTrace("  C_MAX_SESSIONS      = %lld\n"_Stz,C_MAX_SESSIONS);
  vTrace("  C_MAX_SHM_PERCENT   = %lld\n"_Stz,C_MAX_SHM_PERCENT);
  vTrace("  C_MAX_PATH          = %lld\n"_Stz,C_MAX_PATH);
  vTrace("  C_MAX_SLOTS         = %lld\n"_Stz,C_MAX_SLOTS);
  vTrace("  C_MAX_THREADS       = %lld\n"_Stz,C_MAX_THREADS);
  vTrace("  C_MAX_TOKENS        = %lld\n"_Stz,C_MAX_TOKENS);
  vTrace("  C_MAX_VCH_PRECISION = %lld\n"_Stz,C_MAX_FLT_PRECISION);
  vTrace("  C_MIN_SHM_REGION    = %lld\n"_Stz,C_MIN_SHM_REGION);
  vTrace("  C_WSP_DATACHUNK     = %lld\n"_Stz,C_WSP_DATACHUNK);
  vTrace("  C_WSP_STACK         = %lld\n"_Stz,C_WSP_STACK);
  vTrace("\n"_Stz);

  vTrace("  C_SIZE_SMALL_OBJECT  = %lld\n"_Stz,C_SIZE_SMALL_OBJECT);
  vTrace("  C_SIZE_MEDIUM_OBJECT = %lld\n"_Stz,C_SIZE_MEDIUM_OBJECT);
  vTrace("  C_SIZE_LARGE_OBJECT  = %lld\n"_Stz,C_SIZE_LARGE_OBJECT);
  vTrace("  C_SIZE_POOL_BUFFER   = %lld\n"_Stz,C_SIZE_POOL_BUFFER);
  vTrace("\n"_Stz);
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
