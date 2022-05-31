/*============================================================================*/
/* FileMap.hpp -- FileMap                                                     */
/*============================================================================*/

#pragma once

#include "BASE.h"
#include "PageFile.hpp"

namespace BASE
{
  class FileMap : public PoolBuffer
  {
  public:
    Stz* m_szFilePath;
    i8 m_iFileSize;
    i8 m_iCreated;
#ifdef WITH_PAGEFILE
    PageFile* m_pPageFile; // TODO Arjun: Update to list
#endif
    // Store start and end address of pageable area?

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    FileMap(void);
    ~FileMap(void);

    RowBuffer* pFromDisk(Operand* pOperand);
#ifdef WITH_BUFFER_ID
    BufferId oGetNewBufferId(i8 iPoolBufferNo);
#endif
    void vToDisk(Operand* pOperand);
    static PoolBuffer* pToPoolBuffer(i8 i_PoolBuffer);

  private:

    Footer* aFooter(void);
  };
  FileMap* _pFileMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/