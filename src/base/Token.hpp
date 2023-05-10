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
    i8    m_i_Token; // which token
    eTN   m_eToken;  // kind of token
    Stz*  m_szChars; // the charsed characters of this token
    i8    m_iLineNo; // line number where term can be found in input
    i8    m_iColNo;  // column number where term can be found in input

    TokenVec* m_p_ExoTknVec; // which token vector this 
  
    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Token(void);
    Token(Token* pToCopy, TokenVec* pCaller);
   ~Token(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    static bool bStartCppInteger(char cArg);
    static bool bStartCppName(char cArg);
    static bool bStartCppOther(char cArg);
    static bool bStartCppString(char cArg);
    static bool bEndCppString(char cArg);

    static bool bStartCodeBlock(char* azArg);
    static bool bStartSenInteger(char cArg);
    static bool bStartSenName(char cArg);
    static bool bStartSenString(char cArg);
    static bool bStartSenTag(char cArg);
    static bool bEndSenString(char cArg);


    static eTN eCollectCodeBlock(char** aazCrs, Stz* szString);
    static eTN eCollectSenName(char** aazCrs, Stz* szString);
    static eTN eCollectSenNumber(char** aazCrs, Stz* szString);
    static eTN eCollectSenString(char** aazCrs, Stz* szString);
    static eTN eCollectSenTag(char** aazCrs, Stz* szString);

    static eTN eCollectSenToken(char** aazCrs, Stz* szString);

    void  vUpdateChars(Stz* szArg, TokenVec* pExoTknVec);
    void  vCopyToken_restricted(Token* pToken_src, TokenVec* pExoTknVec);
    void  vRippleCopy_restricted(Token* pToken_src, i8 nBytesAdjust,
                                 TokenVec* pExoTknVec);
    void  vInitToken_restricted(i8 i_Token, TokenVec* pExoTknVec);
    void  vSetChars_restricted(Stz* szArg, TokenVec* pExoTknVec);

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
    void  vShow(void);
    void  vStripSingleQuotes(void);
    i8    i_Token(void);
    i8    iToI8(void);
    eTN   eToken(void);
    void  vToken(eTN eToken);
  
    Footer* aFooter(void);
  };

  Token* _pToken(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
