/*============================================================================*/
/* Vortex.hpp -- Vortex                                                       */
/*============================================================================*/
#pragma once

namespace Utilities
{
  static const i8 C_VOREX_TERMS = 256;

  class Vortex : public Object
  {
  public:
    File*  m_pVortexFile;
    s256   m_sSourcePath;
    Input* m_pInput;
    Token  m_xToken_ClassNm;
    Token  m_xToken_Other;
  
    bool   m_bTraceTokens;
  
    i8     m_nClasses;
    i8     m_nMembers;
  
    i8     m_nTerms;
    s32    m_saTerm[C_VOREX_TERMS];

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
 
    Vortex(void);
   ~Vortex(void) noexcept(false);

  public:
    void vClose(void);
    bool bOpen(Stz* szVortexPath);
    void vProcessSource(Stz* szSourcePath);

  private:
    void vAddTerm(Stz* szTerm);
    bool bGetToken(Token* pToken);
  
    void vParseAllClasses(void);
    void vParseClassBase(void);
    void vParseClassBlock(void);
    void vParseClassFunction(void);
    void vParseClassMembers(void);
    bool bParseLabelledMember(void);
    void vParseMemberProper(void);
    void vParseStructArray(void);
    void vParseStructMember(void);
    void vParseUnionMember(void);

  private:
    Footer* aFooter(void);
  };

  Vortex* _pVortex(void* aAddr);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
