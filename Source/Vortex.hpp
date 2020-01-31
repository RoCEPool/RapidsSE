/*============================================================================*/
/* Vortex.hpp -- Vortex                                                       */
/*============================================================================*/
# pragma once

namespace Utilities
{
  class Vortex : public Object
  {
  public:
  
    // input section
  
    s256        m_sSourcePath;   // .h/hpp source path
    Input*      m_pInput;        // input token stream of source file
    bool        m_bTrace;        // trace activity

    // output section
  
    File*       m_pOutputFile;   // output file
    MetaGraph*  m_pMetaGraph;    // the metagraph that is created
  
    // working section
  
    s4K         m_sFragment;     // top-down fragment of tokens

    bool        m_bInClass;      // currently in a class?
    s32         m_sClassNm;      // current class name
    bool        m_bInNamespace;  // currently in a namespace?
    s32         m_sNamespaceNm;  // current namespace name
    
    MetaBlock*  m_p_MetaBlock;   // current working meta block
    MetaMember* m_p_MetaMember;  // current working meta member
    ConstSet*   m_p_ConstSet;    // set of static const i8's

    EnameSet    m_oOtherTerms;   // working set of collected terms
    EnameSet    m_oFunctionTerms;// working set of collected terms
    EnameSet    m_oMemberTerms;  // working set of collected terms
    EnameSet    m_oTplArgTerms;  // working set of collected terms
    
    // correctness section
  
    Footer m_xFooter;            // must be last
  
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
    // machinary
  
    bool bDiscardNextToken(void);
    void vGetExpectedToken(Stz* szExpect); // -> vDisardExpectedToken()
    bool bGetNextTerm(Token& roTerm);
    bool bGetNextToken(Token& roToken);
    void vGetNextToken(Token& roToken);
    void vFragmentAppend(Stz* szTerm);
    void vFragmentMarker(Stz* szTerm);
    void vFragmentReset(void);
    void vPartRecognised(eCP ePart);
    void vThrowProblem(i8 i_Problem);

    // - + - + - + - + - + - + - + - + - + - + - + - +
    // block management
  
    void vBlockNewTop(Stz* szBlockNm, eMB m_eBlock);
    void vBlockPopNested(void);
    void vBlockPushNested(Stz* szBlockNm, eMB eBlock);
  
    void vClassAscend(void);
    void vClassDescend(Stz* szClassNm);
    void vNamepaceAscend(void);
    void vNamepaceDescend(Stz* szNamespaceNm);

    // - + - + - + - + - + - + - + - + - + - + - + - +
    // top-down parsing
  
    bool bParseFile(void);
  
    void vParseFileClass(Stz* szSawTerm);
    void vParseFileConst(Stz* szSawTerm);
    void vParseFileEnum(Stz* szSawTerm);
    void vParseFileHash(Stz* szSawTerm);
    void vParseFileNamespace(Stz* szSawTerm);
    void vParseFileOther(Stz* szSawTerm);
    void vParseFileStatic(Stz* szSawTerm);           // -> other
    void vParseFileTemplate(Stz* szSawTerm);
    void vParseFileUsing(Stz* szSawTerm);

    void vParseHashDefine(Stz* szSawTerm);
    void vParseHashIf(Stz* szSawTerm);
    void vParseHashInclude(Stz* szSawTerm);
    void vParseHashPragma(Stz* szSawTerm);

    void vParseNamespaceClass(Stz* szSawTerm);
    void vParseNamespaceFunction(Stz* szSawTerm);    // -> other
    void vParseNamespaceOther(Stz* szSawTerm);
    void vParseNamespaceStatic(Stz* szSawTerm);      // -> other
    void vParseNamespaceStruct(Stz* szSawTerm);
    void vParseNamespaceTemplate(Stz* szSawTerm);
    void vParseNamespaceTypedef(Stz* szSawTerm);
    void vParseNamespaceUnion(Stz* szSawTerm);
  
    void vParseClassBase(Stz* szSawTerm);
    void vParseClassBlock(Stz* szSawTerm);
    void vParseClassOther(Stz* szSawTerm);
    void vParseClassTemplate(Stz* szSawTerm);

    void vParseBlockFriend(Stz* szSawTerm);
    bool bParseBlockLabel(Stz* szSawTerm);
    void vParseBlockMember(Stz* szSawTerm);
    void vParseBlockOther(Stz* szSawTerm);
    void vParseBlockStruct(Stz* szSawTerm);
    void vParseBlockUnion(Stz* szSawTerm);

    void vParseConst(Stz* szSawTerm);
  
    // - + - + - + - + - + - + - + - + - + - + - + - +
    // term collection
  
    void vCollectBalanced(Stz* szOpen, Stz* szClose);
    void vCollectBalancedAngles(Stz* szSawTerm);
    void vCollectBalancedBraces(Stz* szSawTerm);
    void vCollectBalancedParenthesis(Stz* szSawTerm);
  
    void vCollectOtherTerms(Stz* szSawTerm);
  
    bool bCollectMemberTerms(Stz* szSawTerm);
    bool bCollectFunctionTerms(Stz* szSawTerm, Token& roFunNm, eFN& reFun);
    bool bCollectTemplateArgs(Stz* szSawTerm);

    void vDiscardBalanced(Stz* szOpen, Stz* szClose);
    void vDiscardBalancedAngles(Stz* szSawTerm);
    void vDiscardBalancedBraces(Stz* szSawTerm);
    void vDiscardBalancedParenthesis(Stz* szSawTerm);
  
    void vDiscardOtherTokens(Stz* szSawTerm);
    void vDiscardTill(Stz* szTerm, Stz* szTill);
  
    void vSkipBalancedBraces(Stz* szSawTerm); // to be removed
 
    // - + - + - + - + - + - + - + - + - + - + - + - +
    // terms
  
    Stz* szTerm(i8 i_Term);
    void vTermAdd(Stz* szTerm);
    bool bTermsAreFunction(void);
    bool bTermsAreVariable(void);
    i8   nTerms(void);
    void vTermsReset(void);
  
   private:
    Footer* aFooter(void);
  };

  Vortex* _pVortex(void* aAddr);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
