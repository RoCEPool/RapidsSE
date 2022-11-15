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
    static void vWriteDiskTest(Stz* sFilePath, i8 iNoOfGb);
    static void vWriteHeap(Heap* pHeap, File* pFile);
    static void vWriteHeapBuffers(Heap* pHeap, File* pFile);
    static void vWritePersistentHeaps(Stz* sFilePath);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
