/*============================================================================*/
/* ExtendedText.hpp -- Extended (multi-buffer) Text                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "ExtendedTextBuffer.hpp"
#include "String.hpp"

namespace STORE
{
  class ExtendedText : public Object
  {
  public:
    i8 m_nTextBlocks;
    ExtendedTextBuffer* m_pExtTextBuffer;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ExtendedText(void);
    ~ExtendedText(void) noexcept(false);

    static void vRepair(Object* pObject);

    void vConcat(Stz* szSubStr);
    void vCopy(ExtendedText* pExtText_arg);
    void vCopy(Stz* szStr);

    void vEmpty(void);

    ExtendedTextBuffer* pExtTextBuffer(i8 i_TextBuffer);
    void vExtTextBuffer(i8 i_TextBuffer, ExtendedTextBuffer* pExtTextBuffer);

    bool bRetrieve(String* pString);

    i8 nTextBlocks(void);
    Stz* szTextBlock(i8 i_TextBlock);

  protected:
    Footer* aFooter(void);
  };

  ExtendedText* _pExtendedText(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
