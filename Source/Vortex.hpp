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
    MetaBlock* m_p_MemberSet;  // current working member set
    MemberVar* m_p_MemberVar;  // current working member var
  
    EnameSet   m_oTerms;       // working set of collected terms
    bool       m_bTraceTokens; // trace tokens
   
    bool       m_bInClass;     // currently in a class?
    s32        m_sClassNm;     // current namespace name
    bool       m_bInNamespace; // currently in a namespace?
    s32        m_sNamespaceNm; // current namespace name
    
    s4K        m_sPart;        // current part

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
    bool bParseFile(void);
  
    void vParseFileEnum(Stz* szSawTerm);
    void vParseFileFun(Stz* szSawTerm);
    void vParseFileHash(Stz* szSawTerm);
    void vParseFileNspace(Stz* szSawTerm);
    void vParseFileUsing(Stz* szSawTerm);

    void vParseHashDefine(Stz* szSawTerm);
    void vParseHashInclude(Stz* szSawTerm);
    void vParseHashPragma(Stz* szSawTerm);

    void vParseNspaceClass(Stz* szSawTerm);
    void vParseNspaceFun(Stz* szSawTerm);
    void vParseNspaceStatic(Stz* szSawTerm);
    void vParseNspaceStruct(Stz* szSawTerm);
    void vParseNspaceTemplate(Stz* szSawTerm);
    void vParseNspaceUnion(Stz* szSawTerm);
  
    void vParseClassBase(Stz* szSawTerm);
    void vParseClassBlock(Stz* szSawTerm);
  
    void vParseBlockFriend(Stz* szSawTerm);
    bool bParseBlockLabel(Stz* szSawTerm);
    void vParseBlockMember(Stz* szSawTerm);

    void vParseMemberFunVar(Stz* szSawTerm);
    void vParseMemberStatic(Stz* szSawTerm);
    void vParseMemberStruct(Stz* szSawTerm);
    void vParseMemberTemplate(Stz* szSawTerm);
    void vParseMemberUnion(Stz* szSawTerm);
  
    void vAddTerm(Stz* szTerm);
    void vAscendFromClass(void);
    void vAscendFromNamepace(void);
    bool bCollectTerms(Stz* szEnd, bool bAddEnd=false);
    void vDescendIntoClass(Stz* szName);
    void vDescendIntoNamepace(Stz* szName);
    bool bGetToken(Token& roToken);
    void vRecognised(eCP ePart);
    void vResetPart(void);
    //void vSkipTillSemicolon(void);
    void vSkipTillSemicolon(Stz* szTerm);
    void vSkipUnrecognised(Stz* szTerm);
    bool bTermsAreFunction(void);
    bool bTermsAreVariable(void);
    void vThrowProblem(i8 i_Problem);



    // - - - - - - - - - - - - - - - - - -
    // remove
  
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
 
  private:
  
    Footer* aFooter(void);
  };

  Vortex* _pVortex(void* aAddr);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
