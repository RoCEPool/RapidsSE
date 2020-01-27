/*============================================================================*/
/* Vortex.hpp -- Vortex                                                       */
/*============================================================================*/
#pragma once

namespace Utilities
{
  class Vortex : public Object
  {
  public:
    File*      m_pOutputFile;  // output file
    s256       m_sSourcePath;  // .ho/hpp source path
    Input*     m_pInput;       // input  token stream
  
    MetaGraph* m_pMetaGraph;   // the metagraph that is created
    ConstSet*  m_p_ConsSet;    // set of static const i8's
    MetaClass* m_p_MetaClass;  // current working metaclass
    MemberSet* m_p_MemberSet;  // current working member set
    MemberVar* m_p_MemberVar;  // current working member var
  
    EnameSet   m_oTerms;       // working set of collected terms
    bool       m_bTraceTokens; // trace tokens
   
    s32        m_sNamespaceNm; // current namespace name
      
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
    void vBeginRecognise(void);
    void vEndRecognise(bool bCorrectly);
    bool bCollectTerms(Stz* szEnd, bool bAddEnd=false);
    bool bGetToken(Token* pToken);
  
    void vDetectAll(void);
  
    void vParseAll(void);
    void vParseClassBase(void);
    void vParseClassBlock(void);
    void vParseClassFunction(void);
    void vParseClassMembers(void);
    void vParseConst(void);
    void vParseEnum(void);
    void vParseHashDefine(void);
    void vParseHashInclude(void);
    void vParseHashPragma(void);
    bool bParseLabelledMember(void);
    void vParseMemberProper(Stz* szFirstTerm);
    bool bParseNamespace(void);
    void vParseStructArray(void);
    void vParseStructMember(void);
    void vParseUnionMember(void);
  
    bool bRecogniseClass(Stz* szTerm);
    bool bRecogniseConst(Stz* szTerm);
    bool bRecogniseEnum(Stz* szTerm);
    bool bRecogniseFriend(Stz* szTerm);
    bool bRecogniseFunction(Stz* szTerm);
    bool bRecogniseHash(Stz* szTerm);
    bool bRecogniseLabel(Stz* szTerm);
    bool bRecogniseMbrOrFun(Stz* szTerm);
    bool bRecogniseNamespace(Stz* szTerm);
    bool bRecogniseStruct(Stz* szTerm);
    bool bRecogniseTemplate(Stz* szTerm);
    bool bRecogniseUnion(Stz* szTerm);
    bool bRecogniseUsing(Stz* szTerm);
 
  
  
    void vSkipTillSemicolon(Stz* szTerm);
    void vSkipUnrecognised(Stz* szTerm);

  private:
  
    Footer* aFooter(void);
  };

  Vortex* _pVortex(void* aAddr);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
