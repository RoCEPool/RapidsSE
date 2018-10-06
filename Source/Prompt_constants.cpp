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

extern Arguments   gxArguments;
extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_constants(SePrompt* pPrompt)
{
  vTrace("  C_LOADCHUNK_LENGTH  = %lld\n"_Stz,C_LOADCHUNK_LENGTH);
  vTrace("  C_LOW_BUFFERS       = %lld\n"_Stz,C_LOW_BUFFERS);
  vTrace("  C_MAX_PRECISION_BCD = %lld\n"_Stz,C_MAX_PRECISION_BCD);
  vTrace("  C_MAX_COLUMNS       = %lld\n"_Stz,C_MAX_COLUMNS);
  vTrace("  C_MAX_RECORD        = %lld\n"_Stz,C_MAX_RECORD);
  vTrace("  C_PATH_MAX          = %lld\n"_Stz,C_PATH_MAX);
  vTrace("  C_STACK_LENGTH      = %lld\n"_Stz,C_STACK_LENGTH);
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
