/*============================================================================*/
/* PageFile.hpp -- Page File                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace BASE
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class PageFile : public Object
  {
  public:
    File*  m_pFile;               // Paging file pointer
    Stz*   m_szPFilePath;         // Page File path
    u8     m_uPFIndex;            // Index of Page File
    u8     m_uPFSize;             // Size of Page File
    u8     m_uFirstUsableByte;    // Offset from start of file to the first writable byte
    u8     m_uLastUsableByte;     // Offset from start of file to the last writable byte
    u8     m_uWilderness;         // Subset to the start of free space wilderness (0,1,2...)
    char    m_caBufferHeader[sizeof(Object)];
    Header* m_aHeader;
    Object* m_pObject;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    PageFile(Stz* sPath, u8 uIndex);
    ~PageFile(void);

    void vInitFooter(void);
    void vInitMagic(void);
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    void vInitialise(void);
#ifdef WITH_PAGING
    BufferId oNextFreeBufferId(void);
    u8 uNextFreeSubset(void);
    void vWriteDataBufferToPFile(DataBuffer* pDataBuffer);
    DataBuffer* pReadDataBufferFromPFile(BufferId oBufferId);
#endif

  private:
    Footer* aFooter(void);
  };

  PageFile* _pPageFile(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
