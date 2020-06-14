/*============================================================================*/
/* Descend.hpp -- token descender                                             */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Descend;

  struct DescendDsp
  {
    s32  m_sName;
    bool (*m_pFunc)(Descend*, Stmt_SubQry*);
    bool m_bSubQry;
  };
}

/*----------------------------------------------------------------------------*/

namespace CompilerStream
{
  class Descend : public Object
  {
    static const i8 C_MAX_DESCENDDSP = 64;

  private:
  
    i8 m_nDescend;
    DescendDsp m_xaDescend[C_MAX_DESCENDDSP];
 
    
    i8 m_iNesting;
 
    TokenVec* m_pTokenVec;
  
    Stmt* m_pStmt;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void* aAddress);
    
    Descend(TokenVec* pArgVec, Stmt* pStmt);
   ~Descend(void) noexcept(false);
  
    i8     iCurrToken(void);
    Token* pCurrToken(void);
  
    Token* pFirstToken(void);
    void   vFirstToken(void);

    Token* pLastToken(void);
    void   vLastToken(void);

    bool   bMoreTokens(void);
  
    void   vNestDec(void);
    void   vNestInc(void);
    i8     iNesting(void);

    void   vNextToken(void);
    Token* pNextToken(void);
  
    void   vPrevToken(void);
    Token* pPrevToken(void);
  
    //
  
    bool bCallFunc(Stmt_SubQry* pSubQry);
    bool bIsDescendParse(bool& bSubQry);
    i8   iFindDescend(Stz* szChars);

    Stmt* pStmt(void);

  private:
    void vInit(void);

    Footer* aFooter(void);
  };

  Descend* _pDescend(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
