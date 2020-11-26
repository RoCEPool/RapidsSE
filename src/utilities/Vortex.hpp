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
  
    s256         m_sSourcePath;  // .h/hpp source path
    i8           m_nSources;     // sources so far
    Input*       m_pInput;       // input token stream of source file
    bool         m_bEOF;         // end-of-file reached?
    Token        m_oCurTkn;      // current token

    // tracing and debugging
  
    bool        m_bPrint;        // print activity
    bool        m_bTrace;        // trace activity
    bool        m_bContMember;   // get keystroke for new member
    bool        m_bContPart;     // get keystroke for part recognition
    bool        m_bContType;     // get keystroke for new type

    // output section
  
    File*       m_pOutputFile;   // output file
    MetaGraph*  m_pMetaGraph;    // the metagraph that is being created

    ConstSet*   m_p_ConstSet;    // set of const i8's etc
    MetaType*   m_p_MetaType;    // meta type
    MetaMember* m_p_MetaMember;  // meta member
 
    // working section
  
    bool        m_baPart[VP_ENUMS]; // which cpp syntax parts are active?
  
    s32         m_sConstNm;      // const name
    TagSet    m_oConstTerms;   // collected terms of const

    bool        m_bInNamespace;  // currently in a namespace?
    s32         m_sNamespaceNm;  // current namespace name
  
    bool        m_bInClass;      // currently in a class?
    s32         m_sClassNm;      // current class name
  
    bool        m_bInTemplate;   // currently in a namespace?
    TagSet    m_oTplSpec;      // working set of collected template terms
    s4K         m_sTplSpec;      // current template specification
  
    i8          m_iNextStruct;   // next anonymous c++ struct
    i8          m_iNextUnion;    // next anonymous c++ union

    s32         m_sTypeNm;       // current type name
    TagSet    m_oTypeSpec;     // working set of collected specifier terms
    i8          m_nAsterisks;
    TagSet    m_oFunVarNm;     // working set of collected funvar name terms
    bool        m_bIsFunPtr;     //
    i8          m_nFieldBits;   
    bool        m_bIsArray;      //
    TagSet    m_oArraySpec;    // working set of collected array spec terms
    bool        m_bIsFun;        //
    TagSet    m_oFunSpec;      // working set of collected function spec terms
    TagSet    m_oTypeArgs;     // working set of collected template args terms
    TagSet    m_oClassArgs;    // working set of collected template args terms

    TagSet    m_oOtherTerms;   // working set of collected other terms
 
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
  
    void vRecogniseConst(void);
    void vRecogniseFunction(void);
    void vRecogniseStructure(void);
    void vRecogniseVariable(void);
  
    // - + - + - + - + - + - + - + - + - + - + - + - +
    // keyword parsing
  
    bool bParseAllParts(void);
    void vKeywordDescend(void);
  
    void vParseClass(void);
    void vParseConst(void);
    void vParseEnum(void);
    void vParseExtern(void);
    void vParseFriend(void);
    void vParseHash(void);
    void vParseInline(void);
    void vParseNamespace(void);
    void vParseOpenSquare(void);
    void vParsePublic(void);
    void vParseSemicolon(void);
    void vParseStatic(void);
    void vParseStruct(void);
    void vParseTemplate(void);
    void vParseTypedef(void);
    void vParseUnion(void);
    void vParseUnsigned(void);
    void vParseUsing(void);
    void vParseVarFun(void);

    // - + - + - + - + - + - + - + - + - + - + - + - +
    // second-level parsing

    void vBase_List(void);
  
    void vHash_Define(void);
    void vHash_If(void);
    void vHash_Ifdef(void);
    void vHash_Include(void);
    void vHash_Pragma(void);
  
    bool bLabel_Opt(void);

    void vMembers_List(void);
  
    // - + - + - + - + - + - + - + - + - + - + - + - +
    // variable / function sequence

    void vVarFun_SequenceBegin(void);
  
    void vVarFun_Type(void);
    void vVarFun_TypeScope(void);
    void vVarFun_Special(void);
    void vVarFun_Name(void);

    void vVarFun_AryFunVar(void);
    void vVarFun_Array(void);
    void vVarFun_Field(void);
    void vVarFun_FunBody(void);
  
    void vVarFun_FunCode(void);

    void vVarFun_Constructor(void);
    void vVarFun_Destructor(void);
    void vVarFun_Operator(void);

    void vVarFun_SequenceEnd(void);

    // - + - + - + - + - + - + - + - + - + - + - + - +
    // begin and end

    void vClassBegin(Stz* szClassNm);
    void vClassEnd(void);
    void vNamepaceBegin(Stz* szNamespaceNm);
    void vNamepaceEnd(void);
    void vTemplateBegin(void);
    void vTemplateEnd(void);

    // - + - + - + - + - + - + - + - + - + - + - + - +
    // meta members and type
    
    void vMemberNew(void);
    
    void vTypeNewClass(Stz* szClassNm);
    void vTypeNewStruct(Stz* szStructNm);
    void vTypeNewUnion(Stz* szUnionNm);
    
    void vTypePop(void);
    void vTypePush(Stz* szTypeNm, eMT eStructure, eCP eCppPart);

    // - + - + - + - + - + - + - + - + - + - + - + - +
    // term collection

    void vCollectBalanced(TagSet& roTerms, Stz* szOpen, Stz* szClose);
    void vCollectBalancedAngles(TagSet& roTerms);
    void vCollectBalancedBraces(TagSet& roTerms);
    void vCollectBalancedParenthesis(TagSet& roTerms);
    void vCollectRemainingTerms(TagSet& roTerms);
    void vCollectTemplateSpec(TagSet& roTplSepc);
    void vCollectTypeArgs(TagSet& roTypeArgs);

    void vDiscardBalanced(Stz* szOpen, Stz* szClose);
    void vDiscardBalancedAngles(void);
    void vDiscardBalancedBraces(void);
    void vDiscardBalancedParenthesis(void);
    void vDiscardInlineTokens(void);
    void vDiscardStaticTokens(void);
    void vDiscardTill(Stz* szTerm, Stz* szTill);
 
    void vGetExpectedToken(Stz* szExpect);
    bool bGetNextToken(void);
    void vGetNextToken(void);

    void vShiftClassNm(void);

    // - + - + - + - + - + - + - + - + - + - + - + - +
    // machinary

    bool bCppPartActive(eVP eCppPart);
    void vCppPartClear(eVP eCppPart);
    void vCppPartSaw(eVP eCppPart);
  
    void vThrowProblem(i8 i_Problem);
  
    bool bTraceAgain(void);
    void vTraceConstant(void);
    void vTraceEnter(char* azFunc);
    void vTraceEnter_short(char* azFunc);
    void vTraceFunction(void);
    void vTraceFunVarNm(void);
    void vTraceNamespaceNm(void);
    void vTraceParts(void);
    void vTraceStructure(void);
    void vTraceTypeArgs(void);
    void vTraceTypeSpec(void);
    void vTraceVariable(void);

   private:
    Footer* aFooter(void);
  };

  Vortex* _pVortex(void* aAddr);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
