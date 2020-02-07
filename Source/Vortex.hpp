/*============================================================================*/
/* Vortex.hpp -- Vortex                                                       */
/*============================================================================*/
# pragma once

/*
Refer: http://www.cplusplus.com/doc/tutorial/variables/
 
C++ uses a number of keywords to identify operations and data descriptions; therefore, identifiers created by a programmer cannot match these keywords. The standard reserved keywords that cannot be used for programmer created identifiers are:

alignas, alignof, and, and_eq, asm, auto, bitand, bitor, bool, break, case, catch, char, char16_t, char32_t, class, compl, const, constexpr, const_cast, continue, decltype, default, delete, do, double, dynamic_cast, else, enum, explicit, export, extern, false, float, for, friend, goto, if, inline, int, long, mutable, namespace, new, noexcept, not, not_eq, nullptr, operator, or, or_eq, private, protected, public, register, reinterpret_cast, return, short, signed, sizeof, static, static_assert, static_cast, struct, switch, template, this, thread_local, throw, true, try, typedef, typeid, typename, union, unsigned, using, virtual, void, volatile, wchar_t, while, xor, xor_eq
 
 
 
 Fundamental data types
 The values of variables are stored somewhere in an unspecified location in the computer memory as zeros and ones. Our program does not need to know the exact location where a variable is stored; it can simply refer to it by its name. What the program needs to be aware of is the kind of data stored in the variable. It's not the same to store a simple integer as it is to store a letter or a large floating-point number; even though they are all represented using zeros and ones, they are not interpreted in the same way, and in many cases, they don't occupy the same amount of memory.

 Fundamental data types are basic types implemented directly by the language that represent the basic storage units supported natively by most systems. They can mainly be classified into:
 Character types: They can represent a single character, such as 'A' or '$'. The most basic type is char, which is a one-byte character. Other types are also provided for wider characters.
 Numerical integer types: They can store a whole number value, such as 7 or 1024. They exist in a variety of sizes, and can either be signed or unsigned, depending on whether they support negative values or not.
 Floating-point types: They can represent real values, such as 3.14 or 0.01, with different levels of precision, depending on which of the three floating-point types is used.
 Boolean type: The boolean type, known in C++ as bool, can only represent one of two states, true or false.
 
 */

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
  
    s48K        m_sFragment;     // top-down fragment of source tokens

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
  
    // - + - + - + - + - + - + - + - + - + - + - + - +
    // top-down parsing of C++ parts
  
    bool bParseAllParts(void);
    void vKeywordDescend(void);
    void vParseBaseList(void);
    void vParseClass(void);
    void vParseConst(void);
    void vParseEnum(void);
    void vParseFriend(void);
    void vParseHash(void);
    void vParseHashDefine(void);
    void vParseHashIf(void);
    void vParseHashIfdef(void);
    void vParseHashInclude(void);
    void vParseHashPragma(void);
    void vParseMembers(bool bSemiTerminated);
    void vParseNamespace(void);
    void vParseSemicolon(void);
    void vParseStatic(void);
    void vParseStruct(void);
    void vParseTemplate(void);
    void vParseTypedef(void);
    void vParseUnion(void);
    void vParseUsing(void);

    // - + - + - + - + - + - + - + - + - + - + - + - +
    // function variable gather

    void vParseFunVar(bool bTemplated);

    void vParseFV_Type(void);
    void vParseFV_Name(void);

    void vParseFV_AryFunVar(void);
    void vParseFV_Array(void);
    void vParseFV_Constructor(void);
    void vParseFV_FunBody(void);
    void vParseFV_Operator(void);
    void vParseFV_Variable(void);

    void vClassAscend(void);
    void vClassDescend(Stz* szClassNm);

    void vShiftClassNm(void);
  
    // - + - + - + - + - + - + - + - + - + - + - + - +
    // term collection

    void vCollectBalanced(Stz* szOpen, Stz* szClose);
    void vCollectBalancedAngles(Stz* szSawTerm);
    void vCollectBalancedBraces(Stz* szSawTerm);
    void vCollectBalancedParenthesis(Stz* szSawTerm);
    void vCollectOtherTerms(Stz* szSawTerm);
    void vCollectTplArgs(Stz* szSawTerm);
    void vCollectTplSpec(Stz* szSawTerm);

    void vDiscardBalanced(Stz* szOpen, Stz* szClose);
    void vDiscardBalancedAngles(Stz* szSawTerm);
    void vDiscardBalancedBraces(Stz* szSawTerm);
    void vDiscardBalancedParenthesis(Stz* szSawTerm);
    void vDiscardInlineTokens(Stz* szSawTerm);
    void vDiscardStaticTokens(Stz* szSawTerm);
    void vDiscardTill(Stz* szTerm, Stz* szTill);
  
    // - + - + - + - + - + - + - + - + - + - + - + - +
    // meta members and type
    
    void vMemberNew(void);
    
    void vTypeNew_class(Stz* szClassNm);
    void vTypeNew_struct(Stz* szStructNm);
    void vTypeNew_union(Stz* szUnionNm);
  
    void vTypePop(void);
    void vTypePush(Stz* szTypeNm, eMT eStructure, eCP eCppPart);

    bool bIgnoreOptLabel(Stz* szSawTerm);
  
    // - + - + - + - + - + - + - + - + - + - + - + - +
    // terms
  
    Stz* szTerm(i8 i_Term);
    void vTermAdd(Stz* szTerm);
    i8   nTerms(void);
    void vTermsReset(void);
  
     // - + - + - + - + - + - + - + - + - + - + - + - +
     // machinary
   
     bool bContinueAgain(void);
     void vDiscardNextToken(void);
     void vGetExpectedToken(Token& roToken, Stz* szExpect);
     bool bGetNextToken(Token& roToken);
     void vGetNextToken(Token& roToken);
     void vGetOtherTerm(Token& roTerm);
     void vFragmentAppend(Stz* szTerm);
     void vFragmentMarker(Stz* szTerm);
     void vFragmentReset(void);
     void vNamepaceAscend(void);
     void vNamepaceDescend(Stz* szNamespaceNm);
     void vOtherTermsReset(void);
     void vPartRecognised(eCP eCppPart);
     void vTemplateAscend(void);
     void vTemplateDescend(void);
     void vThrowProblem(i8 i_Problem);
     void vTraceClassNm(void);
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
