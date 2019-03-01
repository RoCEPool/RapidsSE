/*============================================================================*/
/* Prompt_numbers.cpp -- numbers report                                       */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_numbers (
  "Prompt_numbers"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "numbers"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_numbers,      // prompt function
  true,                 // enabled

  true,                 // visible
  "fun help"_Stz        // help
);

extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_numbers(SePrompt* pPrompt)
{
  vTrace("\n"_Stz);

  vTrace("  C_MAGIC  = 0x%llx\n"_Stz,C_MAGIC);
  vTrace("  C_FOOTER = 0x%llx\n"_Stz,C_FOOTER);
  vTrace("\n"_Stz);
  
  vTrace("  C_SCRIBBLE_PATTERN = 0x%llx\n"_Stz,C_SCRIBBLE_PATTERN);
  vTrace("\n"_Stz);
  
  vTrace("  C_DATUM_NULL = %lld\n"_Stz,C_DATUM_NULL);
  vTrace("  C_DATUM_MINUS_INFINITY = %lld\n"_Stz,C_DATUM_MINUS_INFINITY);
  vTrace("  C_DATUM_PLUS_INFINITY = %lld\n"_Stz,C_DATUM_PLUS_INFINITY);
  vTrace("\n"_Stz);
  
  vTrace("  C_I8_MIN %&lld = %lld\n"_Stz,C_I8_MIN);
  vTrace("  C_I8_MIN %%llu = %llu\n"_Stz,(u8)C_I8_MIN);
  vTrace("  C_I8_MAX %&lld = %lld\n"_Stz,C_I8_MAX);
  vTrace("  C_I8_MAX %%llu = %llu\n"_Stz,(u8)C_I8_MAX);
  vTrace("\n"_Stz);
  
  vTrace("  LLONG_MIN = %lld\n"_Stz,LLONG_MIN);
  vTrace("  LLONG_MIN = %p\n"_Stz,  LLONG_MIN);
  vTrace("  LLONG_MAX = %lld\n"_Stz,LLONG_MAX);
  vTrace("  LLONG_MAX = %p\n"_Stz,  LLONG_MAX);
  vTrace("\n"_Stz);
  
  vTrace("  DBL_MIN %%e = %e\n"_Stz,DBL_MIN);
  vTrace("  DBL_MIN %%g = %g\n"_Stz,DBL_MIN);
  vTrace("  DBL_MIN %%f = %f\n"_Stz,DBL_MIN);
  vTrace("  DBL_MAX %%e = %e\n"_Stz,DBL_MAX);
  vTrace("  DBL_MAX %%g = %g\n"_Stz,DBL_MAX);
  vTrace("  DBL_MAX %%f = %f\n"_Stz,DBL_MAX);
  vTrace("\n"_Stz);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
