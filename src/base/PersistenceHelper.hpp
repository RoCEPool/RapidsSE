/*============================================================================*/
/* PersistenceHelper.hpp -- Persistence Helper                                */
/*============================================================================*/
#pragma once

namespace BASE
{
  class PersistenceHelper
  {
  public:
    static void vReadBuffers(Stz* sFilePath);
    static void vWriteBuffers(Stz* sFilePath);
    static void vWriteHeap(Heap* pHeap, File* pFile);
    static void vWritePersistentHeapMem(Stz* sFilePath);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
