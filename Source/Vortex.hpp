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
    s256       m_sSourcePath;  // .h/hpp source path
    Input*     m_pInput;       // input  token stream
  
    MetaGraph* m_pMetaGraph;   // the metagraph that is created
    ConstSet*  m_p_ConsSet;    // set of static const i8's
    MetaClass* m_p_MetaClass;  // current working metaclass
    MemberSet* m_p_MemberSet;  // current working member set
    MemberVar* m_p_MemberVar;  // current working member var
  
    EnameSet   m_oTerms;       // working set of collected terms
    bool       m_bTraceTokens; // trace tokens
   
    bool       m_bInClass;     // currently in a class?
    s32        m_sClassNm;     // current namespace name
    bool       m_bInNamespace; // currently in a namespace?
    s32        m_sNamespaceNm; // current namespace name
    
    s4K        m_sPart;        // current namespace name

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
    bool bGetToken(Token& roToken);
    void vNewPart(void);
    void vRecognised(eCP ePart);
    bool bTermsAreFunction(void);
    bool bTermsAreVariable(void);
    void vThrowProblem(i8 i_Problem);

    bool bParseFile(void);
  
    void vParseClass(Stz* szTerm);
    void vParseClassBase(void);
    void vParseEnumPart(void);
    void vParseFriendPart(void);
    void vParseHashPart(void);
    void vParseMember(Stz* szTerm);
    void vParseNamespaceFunction(Stz* szSawTerm);
    void vParseNamespacePart(void);
    void vParseStaticPart(void);
    void vParseTemplatePart(void);
    void vParseUsingPart(void);
    void vParse_otherPart(Stz* szTerm);
  
    void vWithinClassBlock(void);
    void vWithinNamespace(void);


    void vProcAll(void);
    void vProcClassBase(void);
    void vProcClassBlock(void);
    void vProcClassFunction(void);
    void vProcClassMembers(void);
    void vProcConst(void);
    void vProcEnum(void);
    void vProcHashDefine(void);
    void vProcHashInclude(void);
    void vProcHashPragma(void);
    void vProcLabelledMember(void);
    void vProcMemberProper(Stz* szFirstTerm);
    bool bProcNamespace(void);
    void vProcStructArray(void);
    void vProcStructMember(void);
    void vProcUnionMember(void);
  
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
 
  
  
    bool bSkipLabel(Stz* szTerm);
    void vSkipTillSemicolon(void);
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
