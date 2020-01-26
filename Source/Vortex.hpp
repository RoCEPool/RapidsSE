/*============================================================================*/
/* Vortex.hpp -- Vortex                                                       */
/*============================================================================*/
#pragma once

namespace Utilities
{
  class Vortex : public Object
  {
  public:
    File*  m_pVortexFile;
    s256   m_sSourcePath;
    Input* m_pInput;
  
    Token  m_xToken_ClassNm;
    Token  m_xToken_Other;
  
    s32    m_sVarNm;
    s32    m_sTypeNm;

    bool   m_bTraceTokens;
  
    i8     m_nClasses;
    i8     m_nMembers;
  
    i8     m_nTerms;
  
    EnameSet   m_oTerms;
  
    MetaGraph* m_pMetaGraph;
    ConstSet*  m_pConsSet;
    MetaClass* m_pMetaClass_cur;
    MemberSet* m_pMemberSet_cur;
    MemberVar* m_pMemberVar_cur;
  
    i8         m_nPointers;
    bool       m_bFunPtr;
    i8         m_nElements;
    i8         m_nBits;

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
    bool bCollectTerms(Stz* szEnd, bool bAddEnd=false);
    bool bGetToken(Token* pToken);
  
    void vParseAll(void);
    void vParseClassBase(void);
    void vParseClassBlock(void);
    void vParseClassFunction(void);
    void vParseClassMembers(void);
    void vParseConst(void);
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
