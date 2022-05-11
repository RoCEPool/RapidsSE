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
  private:
    File*  m_pFile;
    Stz*   m_szPFilePath;
    u8     m_uPFSize;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    PageFile(Stz* sPath);
    ~PageFile(void);

    void vCheckFooter(void);
    void vCheckMagic(void);
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    void vInitialise();
    void vWriteDataBufferToPFile(RowBuffer* pRowBuffer);
    static void vToDisk(Operand* pOperand);

  private:
    Footer* aFooter(void);
  };

  PageFile* _pPageFile(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
