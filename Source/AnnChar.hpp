/*============================================================================*/
/* AnnChar.hpp -- Annotated Char                                              */
/*============================================================================*/
#pragma once

namespace LexicalAnalysis
{
  class AnnChar {
  public:
    char m_cChar;  // the char
    i8   m_iLine;  // line number
    i8   m_iCol;   // column number
    
    bool bIsBlank(void);
    bool bIsCloseBracket(void);
    bool bIsColon(void);
    bool bIsDigit(void);
    bool bIsDot(void);
    bool bIsDoubleQuote(void);
    bool bIsEOF(void);
    bool bIsEqual(void);
    bool bIsHex(void);
    bool bIsLessThan(void);
    bool bIsGreaterThan(void);
    bool bIsMinus(void);
    bool bIsNull(void);
    bool bIsOpenBracket(void);
    bool bIsPipe(void);
    bool bIsPlus(void);
    bool bIsSingleQuote(void);
    bool bIsWhite(void);
    bool bIsX(void);

    void vReset(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
