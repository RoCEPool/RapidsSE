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
    i8          m_nSources;      // sources so far
    Input*      m_pInput;        // input token stream of source file
    bool        m_bTrace;        // trace activity

    // output section
  
    File*       m_pOutputFile;   // output file
    MetaGraph*  m_pMetaGraph;    // the metagraph that is created
  
    // working section
  
    s48K        m_sFragment;     // top-down fragment of tokens

    bool        m_bInClass;      // currently in a class?
    s32         m_sClassNm;      // current class name
    bool        m_bInNamespace;  // currently in a namespace?
    s32         m_sNamespaceNm;  // current namespace name
  
    i8          m_iNextStruct;   // next anonymous struct
    i8          m_iNextUnion;    // next anonymous union

    MetaStructure* m_p_MetaStructure;  // current working meta block
  
    MetaBlock*  m_p_MetaBlock;   // current working meta block
    MetaMember* m_p_MetaMember;  // current working meta member
    ConstSet*   m_p_ConstSet;    // set of static const i8's

    EnameSet    m_oOtherTerms;   // working set of collected terms
    EnameSet    m_oTplArgs;      // working set of collected template args
    
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
  
    void vDiscardNextToken(void);
    void vGetExpectedToken(Token& roToken, Stz* szExpect);
    bool bGetNextToken(Token& roToken);
    void vGetNextToken(Token& roToken);
    void vGetOtherTerm(Token& roTerm);
    void vGetTplArg(Token& roTerm);
    void vFragmentAppend(Stz* szTerm);
    void vFragmentMarker(Stz* szTerm);
    void vFragmentReset(void);
    void vOtherTermsReset(void);
    void vPartRecognised(eCP ePart);
    void vThrowProblem(i8 i_Problem);
    void vTplArgsReset(void);

    // - + - + - + - + - + - + - + - + - + - + - + - +
    // block management
  
    void vBlockNewTop(Stz* szBlockNm, eMS m_eBlock);
    void vBlockPopNested(void);
    void vBlockPushNested(Stz* szBlockNm, eMS eBlock);
  
    void vClassAscend(void);
    void vClassDescend(Stz* szClassNm);
    void vNamepaceAscend(void);
    void vNamepaceDescend(Stz* szNamespaceNm);

    // - + - + - + - + - + - + - + - + - + - + - + - +
    // top-down parsing
  
    bool bParseParts(void);
    void vKeywordDescend(Stz* szKeyWord);

    void vParseBase(Stz* szSawTerm);
    MetaBlock* pParseBlock(Stz* szSawTerm, Stz* szBlockNm, eMS eBlock,
                           bool bSemiTerminated);
    void vParseClass(Stz* szSawTerm);
    void vParseConst(Stz* szSawTerm);
    void vParseEnum(Stz* szSawTerm);
    void vParseFriend(Stz* szSawTerm);
    void vParseHash(Stz* szSawTerm);
    void vParseHashDefine(Stz* szSawTerm);
    void vParseHashIf(Stz* szSawTerm);
    void vParseHashIfdef(Stz* szSawTerm);
    void vParseHashInclude(Stz* szSawTerm);
    void vParseHashPragma(Stz* szSawTerm);
    bool bParseLabel(Stz* szSawTerm);
    void vParseNamespace(Stz* szSawTerm);
    void vParseOther(Stz* szSawTerm);
    void vParseSemicolon(Stz* szSawTerm);
    void vParseStatic(Stz* szSawTerm);
    void vParseStruct(Stz* szSawTerm);
    void vParseTemplate(Stz* szSawTerm);
    void vParseTypedef(Stz* szSawTerm);
    void vParseUnion(Stz* szSawTerm);
    void vParseUsing(Stz* szSawTerm);

    // - + - + - + - + - + - + - + - + - + - + - + - +
    // term collection
  
    void vCollectBalanced(Stz* szOpen, Stz* szClose);
    void vCollectBalancedAngles(Stz* szSawTerm);
    void vCollectBalancedBraces(Stz* szSawTerm);
    void vCollectBalancedParenthesis(Stz* szSawTerm);
    void vCollectOtherTerms(Stz* szSawTerm);
    void vCollectTplArgs(Stz* szSawTerm);

    void vDiscardBalanced(Stz* szOpen, Stz* szClose);
    void vDiscardBalancedAngles(Stz* szSawTerm);
    void vDiscardBalancedBraces(Stz* szSawTerm);
    void vDiscardBalancedParenthesis(Stz* szSawTerm);
    void vDiscardInlineTokens(Stz* szSawTerm);
    void vDiscardStaticTokens(Stz* szSawTerm);
    void vDiscardTill(Stz* szTerm, Stz* szTill);
  
    // - + - + - + - + - + - + - + - + - + - + - + - +
    // meta structure

    void vNewStructure_class(Stz* szClassNm);
    void vNewStructure_struct(Stz* szStructNm);
    void vNewStructure_union(Stz* szUnionNm);

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
