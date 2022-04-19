/*============================================================================*/
/* FileMap.hpp -- FileMap                                                          */
/*============================================================================*/

#pragma once

#include "BASE.h"
#include "Resources.hpp" //?
#include "ReapLock.hpp"
#include "TraceMultiLock.hpp"
#include "Starter.hpp"
//#include "EventHandlers.hpp"
//#include "SysMetrics.hpp"

namespace BASE
{
  class FileMap : public Object
  {
  public:
    Stz* m_szFilePath;
    i8 m_iFileSize;
    i8 m_iCreated;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    FileMap(void);
    ~FileMap(void);


    void vInitialise(bool bTouch, bool bProgress);
  private:

    Footer* aFooter(void);
  };
  FileMap* _pFileMap(void* aAddr);
}
