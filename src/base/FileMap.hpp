/*============================================================================*/
/* FileMap.hpp -- FileMap                                                          */
/*============================================================================*/

#pragma once

#include "BASE.h"

namespace BASE
{
  class FileMap : public Object
  {
  public:
    Stz* m_szFilePath;
    i8 m_iFileSize;
    i8 m_iCreated;

    // Store start and end address of pageable area?

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    FileMap(void);
    ~FileMap(void);

    void vInitialise(void);
    static PoolBuffer* pToPoolBuffer(i8 i_PoolBuffer);

  private:

    Footer* aFooter(void);
  };
  FileMap* _pFileMap(void* aAddr);
}
