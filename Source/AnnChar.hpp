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
    
    bool bIs(char cChar);
    bool bIsAmpersand(void);
    bool bIsAsterisk(void);
    bool bIsAt(void);
    bool bIsBackQuote(void);
    bool bIsBackSlash(void);
    bool bIsBlank(void);
    bool bIsCircumflex(void);
    bool bIsCloseBrace(void);
    bool bIsCloseBracket(void);
    bool bIsCloseParenthesis(void);
    bool bIsColon(void);
    bool bIsDollar(void);
    bool bIsDot(void);
    bool bIsDoubleQuote(void);
    bool bIsEOF(void);
    bool bIsEqual(void);
    bool bIsExclamation(void);
    bool bIsHash(void);
    bool bIsHex(void);
    bool bIsLessThan(void);
    bool bIsGreaterThan(void);
    bool bIsMinus(void);
    bool bIsNull(void);
    bool bIsOpenBrace(void);
    bool bIsOpenBracket(void);
    bool bIsOpenParenthesis(void);
    bool bIsPercent(void);
    bool bIsPipe(void);
    bool bIsPlus(void);
    bool bIsQuestion(void);
    bool bIsSemicolon(void);
    bool bIsSingleQuote(void);
    bool bIsSlash(void);
    bool bIsTilde(void);
    bool bIsUnderscore(void);
    bool bIsWhite(void);
    
    bool bIsAlpha(void);
    bool bIsDigit(void);
    bool bIsX(void);

    void vReset(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
