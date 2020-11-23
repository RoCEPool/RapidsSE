/*============================================================================*/
/* Globals.cpp -- Globals                                                     */
/*============================================================================*/

#include "StorageEngine.h"

Args          gxArgs;
EnvFlags      gxEnvFlags;
FnCatalog     gxFnCatalog;

AttachMap*    gpAttachMap   = nullptr;
BufferPool*   gpBufferPool  = nullptr;
Log*          gpLog         = nullptr;
Header*       gaSeShmRegion = nullptr;
File*         gpTraceFile   = nullptr;

bool gbSHM = false;
Stz* gsBuildGitHash;
Stz* gsBuildDate;
Stz* gsBuildHostName;
Stz* gsBuildUserName;
Stz* gsBuildBranch;

char giProgressPoint    = 1;
i8   giProgressSubPoint = 0;

pthread_key_t giTLS_TaskKey   = C_UNKNOWN;
pthread_key_t giTLS_ThreadKey = C_UNKNOWN;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
