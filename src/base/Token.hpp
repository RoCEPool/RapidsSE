/*============================================================================*/
/* Token.hpp -- Token                                                         */
/*============================================================================*/
#pragma once

#include "eTN.h"

namespace BASE
{
  class Token : public Object
  {
  private:
    eTN   m_eToken;  // kind of token
    Stz*  m_szChars; // the charsed characters of this token
    i8    m_iLineNo; // line number where term can be found in input
    i8    m_iColNo;  // column number where term can be found in input

    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Token(void);
   ~Token(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

    static eTN eCollectSenToken(char** aazCrs, Stz* szString);
    static eTN eCollectCodeBlock(char** aazCrs, Stz* szString);
    static eTN eCollectSenName(char** aazCrs, Stz* szString);

    void  vAdjustChars(i8 nBytes);
    i8    nChars(void);
    char* azChars(void);
    Stz*  szChars(void);
    void  vChars(Stz* szChars);
    i8    iColNo(void);
    void  vColNo(i8 iColNo);
    bool  bHasChars(void);
    bool  bInvalidToken(void);
    bool  bIsArithmetic(void);
    bool  bIsAssign(void);
    bool  bIsBoolean(void);
    bool  bIsFloat(void);
    bool  bIsDecimal(void);
    bool  bIsInteger(void);
    bool  bIsGreaterThan(void);
    bool  bIsLessThan(void);
    bool  bIsLogical(void);
    bool  bIsName(void);
    bool  bIsNull(void);
    bool  bIsOther(void);
    bool  bIsPipeline(void);
    bool  bIsRelational(void);
    bool  bIsSemicolon(void);
    bool  bIsString(void);
    i8    iLineNo(void);
    void  vLineNo(i8 iLineNo);
    void  vReset(void);
    bool  bSame(Stz*);
    void  vSetNull(void);
    void  vShow(i8 iTknIdx);
    void  vStripSingleQuotes(void);
    i8    iToI8(void);
    eTN   eToken(void);
    void  vToken(eTN eToken);
  
    Footer* aFooter(void);

  private:
    static bool bStartSenInteger(char cArg);
    static bool bStartSenName(char cArg);
    static bool bStartSenString(char cArg);
    static bool bStartSenTag(char cArg);
    static bool bEndSenString(char cArg);

    static eTN eCollectSenNumber(char** aazCrs, Stz* szString);
    static eTN eCollectSenString(char** aazCrs, Stz* szString);
    static eTN eCollectSenTag(char** aazCrs, Stz* szString);
  };

  Token* _pToken(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
