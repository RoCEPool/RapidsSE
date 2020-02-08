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
    bool        m_bEOF;          // end-of-file reached?
    Token       m_oCurTkn;       // current token

    // tracing and debugging
  
    bool        m_bPrint;        // print activity
    bool        m_bTrace;        // trace activity
    bool        m_bContMember;   // get keystroke for new member
    bool        m_bContPart;     // get keystroke for part recognition
    bool        m_bContType;     // get keystroke for new type

    // output section
  
    File*       m_pOutputFile;   // output file
    MetaGraph*  m_pMetaGraph;    // the metagraph that is being created
  
    // working section
  
    bool        m_bInClass;      // currently in a class?
    s32         m_sClassNm;      // current class name
    bool        m_bInNamespace;  // currently in a namespace?
    s32         m_sNamespaceNm;  // current namespace name
    bool        m_bInTemplate;   // currently in a namespace?
    EnameSet    m_oTplSpec;      // working set of collected template terms
    s4K         m_sTplSpec;      // current template specification
  
    s32         m_sConstNm;      // current class name

    i8          m_iNextStruct;   // next anonymous c++ struct
    i8          m_iNextUnion;    // next anonymous c++ union

    MetaType*   m_p_MetaType;    // current working meta type
    MetaMember* m_p_MetaMember;  // current working meta member
    ConstSet*   m_p_ConstSet;    // set of static const i8's etc

    EnameSet    m_oTypeSpec;     // working set of collected specifier terms
    i8          m_nAsterisks;
    EnameSet    m_oFunVarNm;     // working set of collected funvar name terms
    bool        m_bIsFunPtr;     //
    bool        m_bIsArray;      //
    EnameSet    m_oArraySpec;    // working set of collected array spec terms
    bool        m_bIsFun;        //
    EnameSet    m_oFunSpec;      // working set of collected function spec terms
    EnameSet    m_oTypeArgs;     // working set of collected template args terms
    EnameSet    m_oClassArgs;    // working set of collected template args terms

    EnameSet    m_oOtherTerms;   // working set of collected other terms
 
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
  
    void vGetExpectedToken(Stz* szExpect);
    bool bGetNextToken(void);
    void vGetNextToken(void);

    // - + - + - + - + - + - + - + - + - + - + - + - +
    // top-down parsing of C++ parts
  
    bool bParseAllParts(void);
    void vKeywordDescend(void);
  
    void vParseBaseList(void);
    void vParseClass(void);
    void vParseConst(void);
    void vParseEnum(void);
    void vParseExtern(void);
    void vParseFriend(void);
    void vParseHash(void);
    void vParseHashDefine(void);
    void vParseHashIf(void);
    void vParseHashIfdef(void);
    void vParseHashInclude(void);
    void vParseHashPragma(void);
    void vParseNamespace(void);
    void vParsePublic(void);
    void vParseSemicolon(void);
    void vParseStatic(void);
    void vParseStruct(void);
    void vParseTemplate(void);
    void vParseTypedef(void);
    void vParseUnion(void);
    void vParseUsing(void);

    // - + - + - + - + - + - + - + - + - + - + - + - +
    // begin and end
  
    void vClassBegin(Stz* szClassNm);
    void vClassEnd(void);
    void vNamepaceBegin(Stz* szNamespaceNm);
    void vNamepaceEnd(void);
    void vTemplateBegin(void);
    void vTemplateEnd(void);

    // - + - + - + - + - + - + - + - + - + - + - + - +
    // function variable gather

    void vParseMembers(bool bSemiTerminated);
  
    void vParseFunVar(bool bTemplated);

    void vParseFV_Type(void);
    void vParseFV_Name(void);

    void vParseFV_AryFunVar(void);
    void vParseFV_Array(void);
    void vParseFV_Constructor(void);
    void vParseFV_Field(void);
    void vParseFV_FunBody(void);
    void vParseFV_Operator(void);
    void vParseFV_Variable(void);

    bool bParseLabel_Opt(void);

    void vShiftClassNm(void);
  
    // - + - + - + - + - + - + - + - + - + - + - + - +
    // term collection

    void vCollectBalanced(EnameSet& roTerms, Stz* szOpen, Stz* szClose);
    void vCollectBalancedAngles(EnameSet& roTerms);
    void vCollectBalancedBraces(EnameSet& roTerms);
    void vCollectBalancedParenthesis(EnameSet& roTerms);
    void vCollectRemainingTerms(EnameSet& roTerms);
    void vCollectTemplateSpec(EnameSet& roTplSepc);
    void vCollectTypeArgs(EnameSet& roTypeArgs);

    void vDiscardBalanced(Stz* szOpen, Stz* szClose);
    void vDiscardBalancedAngles(void);
    void vDiscardBalancedBraces(void);
    void vDiscardBalancedParenthesis(void);
    void vDiscardInlineTokens(void);
    void vDiscardStaticTokens(void);
    void vDiscardTill(Stz* szTerm, Stz* szTill);
 
    // - + - + - + - + - + - + - + - + - + - + - + - +
    // meta members and type
    
    void vMemberNew(void);
    
    void vTypeNew_class(Stz* szClassNm);
    void vTypeNew_struct(Stz* szStructNm);
    void vTypeNew_union(Stz* szUnionNm);
  
    void vTypePop(void);
    void vTypePush(Stz* szTypeNm, eMT eStructure, eCP eCppPart);

    
     // - + - + - + - + - + - + - + - + - + - + - + - +
     // machinary
   
     bool bContinueAgain(void);
     void vPartRecognised(eCP eCppPart);
     void vThrowProblem(i8 i_Problem);
     void vTraceFunVarNm(void);
     void vTraceNamespaceNm(void);
     void vTraceTypeArgs(void);
     void vTraceTypeSpec(void);

   private:
    Footer* aFooter(void);
  };

  Vortex* _pVortex(void* aAddr);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
