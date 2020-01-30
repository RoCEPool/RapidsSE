/*============================================================================*/
/* Vortex.hpp -- Vortex                                                       */
/*============================================================================*/
#pragma once

namespace Utilities
{
  class Vortex : public Object
  {
  public:
    File*       m_pOutputFile;  // output file
    s256        m_sSourcePath;  // .h/hpp source path
    Input*      m_pInput;       // input  token stream
    bool        m_bTraceTokens; // trace tokens

    MetaGraph*  m_pMetaGraph;   // the metagraph that is created
    MetaBlock*  m_p_MetaBlock;  // current working meta block
    MetaMember* m_p_MetaMember; // current working meta member
    ConstSet*   m_p_ConstSet;   // set of static const i8's

    EnameSet    m_oOtherTerms;  // working set of collected terms

    EnameSet    m_oFunctionTerms;   // working set of collected terms
    EnameSet    m_oMemberTerms;     // working set of collected terms
    EnameSet    m_oTplArgTerms;     // working set of collected terms
   
    bool        m_bInClass;     // currently in a class?
    s32         m_sClassNm;     // current class name
    bool        m_bInNamespace; // currently in a namespace?
    s32         m_sNamespaceNm; // current namespace name
    
    s4K         m_sPart;        // current part

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
  
    // - + - + - + - + - + - + - + - + - + - + - + - +
    // top-down parsing
  
    bool bParseFile(void);
  
    void vParseFileClass(Stz* szSawTerm);
    void vParseFileConst(Stz* szSawTerm);
    void vParseFileEnum(Stz* szSawTerm);
    void vParseFileFunction(Stz* szSawTerm);
    void vParseFileHash(Stz* szSawTerm);
    void vParseFileNamespace(Stz* szSawTerm);
    void vParseFileOther(Stz* szSawTerm);
    void vParseFileStatic(Stz* szSawTerm);      // -> other
    void vParseFileTemplate(Stz* szSawTerm);
    void vParseFileUsing(Stz* szSawTerm);

    void vParseHashDefine(Stz* szSawTerm);
    void vParseHashIf(Stz* szSawTerm);
    void vParseHashInclude(Stz* szSawTerm);
    void vParseHashPragma(Stz* szSawTerm);

    void vParseNamespaceClass(Stz* szSawTerm);
    void vParseNamespaceFunction(Stz* szSawTerm);
    void vParseNamespaceStatic(Stz* szSawTerm);      // -> other
    void vParseNamespaceStruct(Stz* szSawTerm);
    void vParseNamespaceTemplate(Stz* szSawTerm);
    void vParseNamespaceTypedef(Stz* szSawTerm);
    void vParseNamespaceUnion(Stz* szSawTerm);
  
    // - + - + - + - + - + - + - + - + - + - + - + - +
    // class, block, member parsing

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
  
    // - + - + - + - + - + - + - + - + - + - + - + - +
    // term collection
  
    bool bCollectOtherTerms(Stz* szSawTerm);
  
    bool bCollectMemberTerms(Stz* szSawTerm);
    bool bCollectFunctionTerms(Stz* szSawTerm, Token& roFunNm, eFN& reFun);
    bool bCollectTemplateArgs(Stz* szSawTerm);

    // - + - + - + - + - + - + - + - + - + - + - + - +
    // term skipping
  
    void vSkipBalancedClosingBrace(Stz* szSawTerm);
    void vSkipTill(Stz* szTerm, Stz* szTill);
    void vSkipUnrecognised(Stz* szTerm);

    // - + - + - + - + - + - + - + - + - + - + - + - +
    // term testing
  
    bool bTermsAreFunction(void);
    bool bTermsAreVariable(void);

    // - + - + - + - + - + - + - + - + - + - + - + - +
    // blocks
  
    void vNewTopBlock(Stz* szBlockNm, eMB m_eBlock);
    void vPopNestedBlock(void);
    void vPushNestedBlock(Stz* szBlockNm, eMB eBlock);

    // - + - + - + - + - + - + - + - + - + - + - + - +
    // support
  
    void vAddPart(Stz* szTerm);
    void vAddTerm(Stz* szTerm);
    void vAscendFromClass(void);
    void vAscendFromNamepace(void);
    void vDescendIntoClass(Stz* szClassNm);
    void vDescendIntoNamepace(Stz* szNamespaceNm);
    bool bGetNextToken(Token& roToken);
    void vRecognised(eCP ePart);
    void vResetFunVarTerms(void);
    void vResetPart(void);
    void vThrowProblem(i8 i_Problem);

   private:
  
    Footer* aFooter(void);
  };

  Vortex* _pVortex(void* aAddr);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
