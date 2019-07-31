/*============================================================================*/
/* Globals.cpp -- Globals                                                     */
/*============================================================================*/

#include "StorageEngine.h"

Arguments     gxArguments;
PromptCatalog gxPromptCatalog;

AttachMap*    gpAttachMap   = nullptr;
BufferPool*   gpBufferPool  = nullptr;
Log*          gpLog         = nullptr;
Header*       gaSeShmRegion = nullptr;
File*         gpTraceFile   = nullptr;

bool gbSHM = false;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
