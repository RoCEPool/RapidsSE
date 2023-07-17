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
    static const i8 C_EXTENDEDTEXTBUFFER = 96;

  public:
    i8 m_nBlocks;
    ExtendedTextBuffer* m_paExtTextBuffer[C_EXTENDEDTEXTBUFFER];

    i8 m_nBytes;
    i8 m_nLines;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ExtendedText(void);
    ~ExtendedText(void) noexcept(false);

    void vConcat(Stz* szStr);
    void vConcat(ExtendedText* pExtText_arg);
    void vCopy(ExtendedText* pExtText_arg);
    void vCopy(Stz* szStr);

    void vDump(void);

    void vEmpty(void);

    ExtendedTextBuffer* pExtTextBuffer(i8 i_TextBuffer);
    void vExtTextBuffer(i8 i_TextBuffer, ExtendedTextBuffer* pExtTextBuffer);

    bool bRetrieve(String* pString);

    i8 nTextBlocks(void);
    Stz* szTextBlock(i8 i_TextBlock);

    void vVerify(void);

  private:
    bool bAddBuffer(void);
    void vEnsureBuffer(void);
    void vResum(void);
    ExtendedTextBuffer* pTailBuffer(void);

  protected:
    Footer* aFooter(void);
  };

  ExtendedText* _pExtendedText(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
