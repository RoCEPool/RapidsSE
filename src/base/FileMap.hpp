/*============================================================================*/
/* FileMap.hpp -- FileMap                                                     */
/*============================================================================*/

#pragma once

#include "BASE.h"
#include "PageFile.hpp"

namespace BASE
{
  class FileMap : public Object
  {
  public:
    Lock       m_okLock;
    Stz*       m_szFilePath;
    i8         m_iFileSize;
    i8         m_iCreated;
    PageFile*  m_pPageFile; // TODO Arjun: Update to list
#ifndef WITH_PAGING
    i8         m_iSynthBufferId; // Use synthetic buffer id when paging is disabled;
#endif
    // Store start and end address of pageable area?

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    FileMap(void);
    ~FileMap(void);

    DataBuffer* pDataBufferFromDisk(BufferId oBufferId);
    BufferId oGetNewBufferId(void);

    void vToDisk(Operand* pOperand);
    void vToDisk(DataBuffer* pDataBuffer);
    static PoolBuffer* pToPoolBuffer(i8 i_PoolBuffer);

    void vGrabWriteLock(void);
    void vDropWriteLock(void);
    void vGrabReadLock(void);
    void vDropReadLock(void);

    void vStartPageIn(void);
    void vEndPageIn(void);

    void vStartPageOut(void);
    void vEndPageOut(void);

  private:

    Footer* aFooter(void);
  };
  FileMap* _pFileMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/