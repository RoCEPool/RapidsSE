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
  
  i8 iSnapshotObjects[CN_ENUMS];
  BPA::vObjectSnapshot(&iSnapshotObjects[0]);
  
  if (AM::bMetadata()) {
    vTrace("metadata: (objectName VARCHAR, count INTEGER)\n"_Stz);
  }
  
  i8* iaObjectsInPool = pPrompt->iaObjectsInPool();
  
  for (i8 i_Object = 0 ; i_Object < CN_ENUMS; i_Object++) {
    
    eCN eClass = (eCN)i_Object;
    
    if (eClass == CN_FREEBUFFER || eClass == CN_HEAPBUFFER)
      continue;
    
    i8 iCount = iSnapshotObjects[eClass] - iaObjectsInPool[eClass];
    if ( iCount == 0 )
      continue;
    
    vTrace("row: ('%s',  %lld)\n"_Stz,
           aCN[eClass],
           iSnapshotObjects[eClass] - iaObjectsInPool[eClass]
           );
    iRowsProcessed++;
  }
  
  vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
  
  pPrompt->vTrackingMemory(false);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
