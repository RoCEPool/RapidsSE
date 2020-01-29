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
  
    MetaGraph*  m_pMetaGraph;   // the metagraph that is created
    MetaBlock*  m_p_MetaBlock;  // current working meta block
    MetaMember* m_p_MetaMember; // current working meta member
    ConstSet*   m_p_ConstSet;   // set of static const i8's

    EnameSet    m_oFunVarTerms; // working set of collected terms
    bool        m_bTraceTokens; // trace tokens
   
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
    bool bParseFile(void);
  
    void vParseFileEnum(Stz* szSawTerm);
    void vParseFileFunction(Stz* szSawTerm);
    void vParseFileHash(Stz* szSawTerm);
    void vParseFileNamespace(Stz* szSawTerm);
    void vParseFileStatic(Stz* szSawTerm);
    void vParseFileUsing(Stz* szSawTerm);

    void vParseHashDefine(Stz* szSawTerm);
    void vParseHashInclude(Stz* szSawTerm);
    void vParseHashPragma(Stz* szSawTerm);

    void vParseNamespaceClass(Stz* szSawTerm);
    void vParseNamespaceFunction(Stz* szSawTerm);
    void vParseNamespaceStatic(Stz* szSawTerm);
    void vParseNamespaceStruct(Stz* szSawTerm);
    void vParseNamespaceTemplate(Stz* szSawTerm);
    void vParseNamespaceTypedef(Stz* szSawTerm);
    void vParseNamespaceUnion(Stz* szSawTerm);
  
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
  
    void vAddTopBlock(Stz* szBlockNm, eMB m_eBlock);
    void vAscendFromClass(void);
    void vAscendFromNamepace(void);
    void vCollectFunVarTerm(Stz* szTerm);
    bool bCollectFunVarTerms(Stz* szEnd, bool bAddEnd=false);
    void vDescendIntoClass(Stz* szClassNm);
    void vDescendIntoNamepace(Stz* szNamespaceNm);
    bool bGetNextToken(Token& roToken);
    void vPopNestedBlock(void);
    void vPushNestedBlock(Stz* szBlockNm, eMB eBlock);
    void vRecognised(eCP ePart);
    void vResetFunVarTerms(void);
    void vResetPart(void);
    //void vSkipTillSemicolon(void);
    void vSkipTill(Stz* szTerm, Stz* szTill);
    void vSkipUnrecognised(Stz* szTerm);
    bool bTermsAreFunction(void);
    bool bTermsAreVariable(void);
    void vThrowProblem(i8 i_Problem);



    // - - - - - - - - - - - - - - - - - -
    // remove
  
//    void vProcAll(void);
//    void vProcClassBase(void);
//    void vProcClassBlock(void);
//    void vProcClassFunction(void);
//    void vProcClassMembers(void);
//    void vProcConst(void);
//    void vProcEnum(void);
//    void vProcHashDefine(void);
//    void vProcHashInclude(void);
//    void vProcHashPragma(void);
//    void vProcLabelledMember(void);
//    void vProcMemberProper(Stz* szFirstTerm);
//    bool bProcNamespace(void);
//    void vProcStructArray(void);
//    void vProcStructMember(void);
//    void vProcUnionMember(void);
//  
//    bool bRecogniseClass(Stz* szTerm);
//    bool bRecogniseConst(Stz* szTerm);
//    bool bRecogniseEnum(Stz* szTerm);
//    bool bRecogniseFriend(Stz* szTerm);
//    bool bRecogniseFunction(Stz* szTerm);
//    bool bRecogniseHash(Stz* szTerm);
//    bool bRecogniseLabel(Stz* szTerm);
//    bool bRecogniseMbrOrFun(Stz* szTerm);
//    bool bRecogniseNamespace(Stz* szTerm);
//    bool bRecogniseStruct(Stz* szTerm);
//    bool bRecogniseTemplate(Stz* szTerm);
//    bool bRecogniseUnion(Stz* szTerm);
//    bool bRecogniseUsing(Stz* szTerm);
 
  private:
  
    Footer* aFooter(void);
  };

  Vortex* _pVortex(void* aAddr);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
