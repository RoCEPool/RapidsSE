/*============================================================================*/
/* Prompt_memoryObjects.cpp -- memory objects                                 */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_memoryObjects (
  PF_ENUMS,                    // prompt function
  "Prompt_memoryObjects"_Stz,  // name of the prompt function
  "memory objects"_Stz,        // comment about the function
  "memoryObjects"_Stz,         // the prompt command
  nullptr,                     // the second word
  bPrompt_memoryObjects,       // prompt function
  true,                        // enabled

  true,                        // visible
  "memoryObjects"_Stz,         // help
  ""_Stz                       // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_memoryObjects(SePrompt* pPrompt)
{
  i8 iRowsProcessed = 0;
  
  if ( ! pPrompt->bTrackingMemory() ) {
    vTrace("error: Memory not being tracked; Run trackMemory;\n"_Stz);
    return true;
  }
  
  if (AM::bMetadata()) {
    vTrace("metadata: (objectName VARCHAR, count INTEGER, heap VARCHAR)\n"_Stz);
  }
  
  vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
