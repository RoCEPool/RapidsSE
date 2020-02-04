/*============================================================================*/
/* Vortex.cpp - Vortex                                                        */
/*============================================================================*/

#include "Utilities.h"

#define TRACE(fmt, ...) if (m_bTrace) vTrace(fmt, ##__VA_ARGS__)
/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Utilities,Vortex,CN_VORTEX)
__aNewFunction(Vortex,CN_VORTEX)
__vDeleteFunction(Vortex,CN_VORTEX)
__aFooterFunction(Vortex)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Vortex::Vortex(void)
: Object(CN_VORTEX)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  // input
  
  m_sSourcePath  = ""_Stz;
  m_nSources     = 0;
  m_pInput       = nullptr;
  m_bPrint       = true;
  m_bTrace       = true;

  // output
  
  m_pOutputFile  = nullptr;
  m_pMetaGraph   = new MetaGraph;
  
  // working
  
  m_sFragment    = ""_Stz;
  
  m_bInClass     = false;
  m_sClassNm     = C_NO_NAME;
  m_bInNamespace = false;
  m_sNamespaceNm = C_NO_NAME;
  
  m_iNextStruct  = 0;
  m_iNextUnion   = 0;

  m_p_MetaStructure  = nullptr;
  m_p_MetaStructure  = nullptr;
  m_p_MetaMember = nullptr;
  m_p_ConstSet   = m_pMetaGraph->pConstSet();

  //m_oOtherTerms
  
  _checkInitialised_(CN_VORTEX);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Vortex::~Vortex(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  delete m_pInput;
  delete m_pOutputFile;
  delete m_pMetaGraph;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

#define CHECKED(fun) if (! fun) vThrowProblem(__LINE__);

/*----------------------------------------------------------------------------*/
/* close the vortex                                                           */
/*----------------------------------------------------------------------------*/
void Vortex::vClose(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
    
  if (m_bPrint)
    m_pMetaGraph->vRecursivePrint();
  
  if (m_pOutputFile && m_pOutputFile->bIsOpen())
    m_pOutputFile->vClose();
  
  delete m_pOutputFile;
  m_pOutputFile = nullptr;
}

/*----------------------------------------------------------------------------*/
/* open the vortex                                                            */
/*----------------------------------------------------------------------------*/
bool Vortex::bOpen(Stz* szVortexPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
    
  RASSERT(! m_pOutputFile);
  
  m_pOutputFile = new File(nullptr);
  
  bool b = m_pOutputFile->bOpenWrite(szVortexPath);
  RASSERT(b);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* process a source file (.h or .hpp)                                         */
/*----------------------------------------------------------------------------*/
void Vortex::vProcessSource(Stz* szSourcePath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  TRACE("\n\n\n"_Stz);
  TRACE("-------------------------------------------------------------\n"_Stz);
  TRACE("Parsing file %lld) %s\n"_Stz,m_nSources,szSourcePath->azChar());
  
  m_sSourcePath = szSourcePath;
  m_pInput = new Input;
  m_pInput->vPushFile(szSourcePath);
  
  bool b = bParseCppParts();
  
  if (b) {
    TRACE("Parsed file %lld) %s\n"_Stz,m_nSources,szSourcePath->azChar());
    TRACE("=============================================================\n"_Stz);
  }
  else {
    TRACE("\n\n"_Stz);
    TRACE("** Unable to fully process %s\n"_Stz,szSourcePath->azChar());
    TRACE("** Last Fragment: %s\n"_Stz,m_sFragment.azChar());
    TRACE("\n\n"_Stz);
    NEVER_REACH;
  }
  
  m_nSources++;
  
  delete m_pInput;
  m_pInput = nullptr;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                            top-down parsing                                */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* parse all parts in the current source file (.h or .hpp)                    */
/*----------------------------------------------------------------------------*/
bool Vortex::bParseCppParts(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  RASSERT(! m_bInNamespace);
  
  // -- parse till EOF --
  Token xTkn;
  while (bGetNextToken(xTkn)) {
  
    TRACE("/%s/ >%s KeyWord: %s\n"_Stz,
           m_sSourcePath.szStz(),__func__,xTkn.azTerm());
    
    try {
      vKeywordDescend(xTkn.szTerm());
    }
    catch (i8 i_Problem) {
      return false;
    }
    
  } // while more tokens
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* decode keyword and descend into parse                                      */
/*----------------------------------------------------------------------------*/
void Vortex::vKeywordDescend(Stz* szKeyWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  if (szKeyWord->bSame("class"_Stz))     vParseClass(szKeyWord);     else
  if (szKeyWord->bSame("const"_Stz))     vParseConst(szKeyWord);     else
  if (szKeyWord->bSame("enum"_Stz))      vParseEnum(szKeyWord);      else
  if (szKeyWord->bSame("friend"_Stz))    vParseFriend(szKeyWord);    else
  if (szKeyWord->bSame("#"_Stz))         vParseHash(szKeyWord);      else
  if (szKeyWord->bSame("namespace"_Stz)) vParseNamespace(szKeyWord); else
  if (szKeyWord->bSame(";"_Stz))         vParseSemicolon(szKeyWord); else
  if (szKeyWord->bSame("static"_Stz))    vParseStatic(szKeyWord);    else
  if (szKeyWord->bSame("struct"_Stz))    vParseStruct(szKeyWord);    else
  if (szKeyWord->bSame("template"_Stz))  vParseTemplate(szKeyWord);  else
  if (szKeyWord->bSame("typedef"_Stz))   vParseTypedef(szKeyWord);   else
  if (szKeyWord->bSame("union"_Stz))     vParseUnion(szKeyWord);     else
  if (szKeyWord->bSame("using"_Stz))     vParseUsing(szKeyWord);
  else                                   vParseOther(szKeyWord);
}

/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                      class, block, member parsing                          */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* parse within:                                                              */
/*----------------------------------------------------------------------------*/
void Vortex::vParseBase(Stz* szSawTerm)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame(":"_Stz));
  RASSERT(m_bInClass);
  
  vParseInherit(szSawTerm);
  
}

/*----------------------------------------------------------------------------*/
/* parse                                                                      */
/*----------------------------------------------------------------------------*/
void Vortex::vParseInherit(Stz* szSawTerm)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;

  vDebugPoint();
  
  while (bGetNextToken(xTkn)) {
    
    Stz* szTerm = xTkn.szTerm();
    
    TRACE("   %s\n"_Stz,szTerm);

    if (szTerm->bOneOf("private"_Stz,"protected"_Stz,"public"_Stz,nullptr)) {
      vGetNextToken(xTkn);
      TRACE("   %s\n"_Stz,szTerm);
    }

    Stz* szBaseClassNm = xTkn.szTerm();
    
    // add
    //MetaInherit* pInherit = m_p_MetaStructure->pAddInherit(szBaseClassNm);
    
    vGetNextToken(xTkn);

    // class base is terminated by {
    if (szTerm->bSame("{"_Stz))
      break;

    CHECKED(szTerm->bSame(","_Stz));
  }

  MetaStructure* p_Structure = pParseBraces("{"_Stz,true);

}

/*----------------------------------------------------------------------------*/
/* within file, parse  CLASS _^_ <name> : ...  | { ...                        */
/*----------------------------------------------------------------------------*/
void Vortex::vParseClass(Stz* szSawTerm)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(szSawTerm->bSame("class"_Stz));
  RASSERT(! m_p_MetaStructure);
    
  Token xTkn;
  Token xClassNm;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get <name>
  
  vGetNextToken(xTkn);
  xClassNm = xTkn;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get ; or : or {
  
  vGetNextToken(xTkn);
    
  // forward declaration of a class?
  if (xTkn.bSame(";"_Stz)) {
    vPartRecognised(CP_FORWARD);
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vStructurePush(xClassNm.szTerm(),MS_LINEAR,CP_CLASS);
  vClassDescend(xClassNm.szTerm());

  if (xTkn.bSame(":"_Stz)) {
    // detect class base clause ?
    vParseBase(xTkn.szTerm());
  }
  else
  if (xTkn.bSame("{"_Stz)) {
    // detect class block ?
    MetaStructure* p_Structure = pParseBraces(xTkn.szTerm(),true);
  }
  else {
    vThrowProblem(__LINE__);
  }

  vStructurePop();
  vClassAscend();
}

/*----------------------------------------------------------------------------*/
/* parse [STATIC] CONST _^_ ... ;                                             */
/*----------------------------------------------------------------------------*/
void Vortex::vParseConst(Stz* szSawTerm)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("const"_Stz));
  RASSERT(0 == m_oOtherTerms.nItems());

  Token xTkn;
  Token xConstType;
  Token xConstNm;

  // get <type>
  vGetNextToken(xConstType);
  
  for (;;) {
    CHECKED(bGetNextToken(xTkn));
    if (! xTkn.bSame("*"_Stz))
      break;
  }
    
  // get <type>
  xConstNm = xTkn;

  // get =
  vGetExpectedToken(xTkn,"="_Stz);
  
  // get ... ;
  vCollectOtherTerms("="_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  ConstDef* pConstDef = m_p_ConstSet->pAddConstDef(xConstNm.szTerm());
  pConstDef->vTerms(m_oOtherTerms);

  vPartRecognised(CP_CONST);
  
  vOtherTermsReset();
}

/*----------------------------------------------------------------------------*/
/* parse ENUM _^_ ... ;                                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vParseEnum(Stz* szSawTerm)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("enum"_Stz));
    
  vDiscardTill(szSawTerm,";"_Stz);
  
  vPartRecognised(CP_ENUM);
}

/*----------------------------------------------------------------------------*/
/* parse FRIEND _^_ ... ;                                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFriend(Stz* szSawTerm)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("friend"_Stz));
  
  vDiscardTill(szSawTerm,";"_Stz);
  
  vPartRecognised(CP_FRIEND);
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* parse # _^_ DEFINE ... | IF ... | INCLUDE ... | PRAGMA ...                 */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHash(Stz* szSawTerm)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("#"_Stz));
    
  Token xTkn;
  CHECKED(bGetNextToken(xTkn));
  
  if (xTkn.bSame("define"_Stz)) {
    vParseHashDefine(xTkn.szTerm());
    return;
  }

  if (xTkn.bSame("if"_Stz)) {
    vParseHashIf(xTkn.szTerm());
    return;
  }

  if (xTkn.bSame("ifdef"_Stz)) {
    vParseHashIfdef(xTkn.szTerm());
    return;
  }

  if (xTkn.bSame("include"_Stz)) {
    vParseHashInclude(xTkn.szTerm());
    return;
  }

  if (xTkn.bSame("pragma"_Stz)) {
    vParseHashPragma(xTkn.szTerm());
    return;
  }

  vThrowProblem(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* within #, parse DEFINE _^_ ... ENDDEFINE                                   */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHashDefine(Stz* szSawTerm)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("define"_Stz));
    
  Token xTkn;
  
  vGetNextToken(xTkn);
  while (! xTkn.bSame("enddefine"_Stz)) {
    vGetNextToken(xTkn);
  }
      
  vPartRecognised(CP_HASH_DEFINE);
}

/*----------------------------------------------------------------------------*/
/* within #, parse IF _^_ ... #ENDIF                                          */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHashIf(Stz* szSawTerm)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("if"_Stz));
  
  Token xTkn;
  
  vGetNextToken(xTkn);
  while (! xTkn.bSame("endif"_Stz)) {
    vGetNextToken(xTkn);
  }
      
  vPartRecognised(CP_HASH_IF);
}

/*----------------------------------------------------------------------------*/
/* within #, parse IF _^_ ... #ENDIF                                          */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHashIfdef(Stz* szSawTerm)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("ifdef"_Stz));
  
  Token xTkn;
  
  vGetNextToken(xTkn);
  while (! xTkn.bSame("endif"_Stz)) {
    vGetNextToken(xTkn);
  }
      
  vPartRecognised(CP_HASH_IF);
}

/*----------------------------------------------------------------------------*/
/* within #, parse INCLUDE _^_ <filePath>                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHashInclude(Stz* szSawTerm)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("include"_Stz));
  
  Token xTkn;
  
  vGetNextToken(xTkn);
  
  if (xTkn.bSame("<"_Stz))
    vDiscardBalancedAngles("<"_Stz);
    
  vPartRecognised(CP_HASH_INCLUDE);
}

/*----------------------------------------------------------------------------*/
/* within #, parse # PRAGMA _^_ ONCE                                          */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHashPragma(Stz* szSawTerm)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("pragma"_Stz));
  
  Token xTkn;
  vGetNextToken(xTkn);
  
  if (xTkn.bSame("once"_Stz)) {
    vPartRecognised(CP_HASH_PRAGMA_ONCE);
    return;
  }

  if (xTkn.bSame("pack"_Stz)) {
    vGetExpectedToken(xTkn,"("_Stz);
    vDiscardBalancedParenthesis("("_Stz);
    vPartRecognised(CP_HASH_PRAGMA_PACK);
    return;
  }

  vThrowProblem(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* within block, skip a label i.e. private|protected|public :                 */
/*----------------------------------------------------------------------------*/
bool Vortex::bParseLabel(Stz* szSawTerm)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;
  
  if (szSawTerm->bSame("private"_Stz)) {
    CHECKED(bGetNextToken(xTkn));
    CHECKED(xTkn.bSame(":"_Stz));
    vPartRecognised(CP_LABEL);
    return true;
  }
  
  if (szSawTerm->bSame("protected"_Stz)) {
    CHECKED(bGetNextToken(xTkn));
    CHECKED(xTkn.bSame(":"_Stz));
    vPartRecognised(CP_LABEL);
    return true;
  }
  
  if (szSawTerm->bSame("public"_Stz)) {
    CHECKED(bGetNextToken(xTkn));
    CHECKED(xTkn.bSame(":"_Stz));
    vPartRecognised(CP_LABEL);
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* parse NAMESPACE _^_ <name> { <parts> }                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vParseNamespace(Stz* szSawTerm)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(szSawTerm->bSame("namespace"_Stz));
  
  // get <name> of namespace
  Token xTkn;
  vGetNextToken(xTkn);
  
  vNamepaceDescend(xTkn.szTerm());
  
  // get {
  vGetExpectedToken(xTkn,"{"_Stz);

  // parse ... }
  while (bGetNextToken(xTkn)) {
  
    // test for } being end-of-namespace
    if (xTkn.bSame("}"_Stz))  {
      vPartRecognised(CP_NAMESPACE);
      vNamepaceAscend();
      return;
    }
    
    // -- -- -- -- -- --
    
    Stz* szKeyWord = xTkn.szTerm();
    
    vKeywordDescend(szKeyWord);
    
  } // while more tokens
  
  vThrowProblem(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* within file, parse <other> _^_ <term> ...                                  */
/*----------------------------------------------------------------------------*/
void Vortex::vParseOther(Stz* szSawTerm)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vCollectOtherTerms(szSawTerm);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
#if 0
  i8   nPointers;
  bool bFunPtr;
  i8   nElements;
  i8   nBits;

  
  Stz* szVarNm;
  Stz* szTypeNm;
  szVarNm = MetaMember::szMemberNm(&m_oOtherTerms,szTypeNm,
                                   nPointers,bFunPtr,nElements,nBits);
  
  m_p_MetaMember = m_p_MetaStructure->pAddMember(szVarNm);
  
  m_p_MetaMember->vTypeNm(szTypeNm);
  m_p_MetaMember->vPointers(nPointers);
  m_p_MetaMember->vFunPtr(bFunPtr);
  m_p_MetaMember->vElements(nElements);
  m_p_MetaMember->vBits(nBits);

#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vPartRecognised(CP_OTHER);
  
  vOtherTermsReset();
}

/*----------------------------------------------------------------------------*/
/* parse ;                                                                    */
/*----------------------------------------------------------------------------*/
void Vortex::vParseSemicolon(Stz* szSawTerm)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame(";"_Stz));
  
  vPartRecognised(CP_EMPTY);
}

/*----------------------------------------------------------------------------*/
/* parse STATIC CONST ... | ...                                               */
/*----------------------------------------------------------------------------*/
void Vortex::vParseStatic(Stz* szSawTerm)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(szSawTerm->bSame("static"_Stz));
    
  Token xTkn;
  vGetNextToken(xTkn);

  if (xTkn.bSame("const"_Stz)) {
    vParseConst("const"_Stz);
    return;
  }
  
  if (xTkn.bSame("inline"_Stz)) {
    vDiscardInlineTokens(xTkn.szTerm());
    return;
  }
  
  vDiscardStaticTokens(xTkn.szTerm());
  vPartRecognised(CP_STATIC);
}

/*----------------------------------------------------------------------------*/
/* parse STRUCT { ...                                                         */
/*----------------------------------------------------------------------------*/
void Vortex::vParseStruct(Stz* szSawTerm)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(szSawTerm->bSame("struct"_Stz));

  /*
    
   1) struct { ... } ;
      
   2) struct { ... } <memberNm> ;
   
   3) struct { ... } <memberNm> [ <constNm> ] ;

   4) struct <structNm> ;

   5) struct <structNm> { ... } ;

   */
  
  Token xTkn;
  s32 sStructNm;

  vGetNextToken(xTkn);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // scenarios 1), 2) and 3)
  
  if (xTkn.bSame("{"_Stz)) {
    
    sStructNm.vSprint("_STRUCT_%lld"_Stz,++m_iNextStruct);

    vStructurePush(sStructNm,MS_LINEAR,CP_STRUCT);
    MetaStructure* p_Structure = pParseBraces("{"_Stz,false);
    vStructurePop();

    // 1) struct { ... } ;
    vGetNextToken(xTkn);
    if (xTkn.bSame(";"_Stz))
      return;
    
    // <#> handle member name in xTkn
    
    // 2) struct { ... } <memberNm> ;
    vGetNextToken(xTkn);
    if (xTkn.bSame(";"_Stz))
      return;

    // 3) struct { ... } <memberNm> [ <constNm> ] ;
    CHECKED(xTkn.bSame("["_Stz));
    vGetNextToken(xTkn);
    vGetExpectedToken(xTkn,"]"_Stz);
    vGetExpectedToken(xTkn,";"_Stz);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // scenarios 4) and 5)
  
  // <#> handle member name in xTkn

  sStructNm = xTkn.szTerm();
  
  // get ; or {
  vGetNextToken(xTkn);
  
  // 4) struct <blockNm> ;
  if (xTkn.bSame(";"_Stz))
    return;

  // 5) struct <blockNm> { ... } ;
  if (xTkn.bSame("{"_Stz)) {
    
    vStructurePush(xTkn.szTerm(),MS_LINEAR,CP_STRUCT);
    MetaStructure* p_Structure = pParseBraces(xTkn.szTerm(),true);
    vStructurePop();
    
    return;
  }
  
  // 6) struct <blockNm> <memberNm> ;
  vGetExpectedToken(xTkn,";"_Stz);

  // <#> handle member name
  
  vPartRecognised(CP_STRUCT);
}

/*----------------------------------------------------------------------------*/
/* parse TEMPLATE _^_ < ... > CLASS ...  | <other> ...                        */
/*----------------------------------------------------------------------------*/
void Vortex::vParseTemplate(Stz* szSawTerm)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(szSawTerm->bSame("template"_Stz));
      
  Token xTkn;
  
  vGetExpectedToken(xTkn,"<"_Stz);
  
  vCollectTplTerms("<"_Stz);
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // class or other
  
  vGetNextToken(xTkn);
  
  if (xTkn.bSame("class"_Stz)) {
    vParseClass(xTkn.szTerm());
    return;
  }
  
  vParseOther(xTkn.szTerm());
}

/*----------------------------------------------------------------------------*/
/* parse TYPEDEF _^_ ... ;                                                    */
/*----------------------------------------------------------------------------*/
void Vortex::vParseTypedef(Stz* szSawTerm)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("typedef"_Stz));
  
  vDebugPoint();
  
  vDiscardTill(szSawTerm,";"_Stz);
  
  vPartRecognised(CP_TYPEDEF);
}

/*----------------------------------------------------------------------------*/
/* parse UNION  { ...                                                         */
/*----------------------------------------------------------------------------*/
void Vortex::vParseUnion(Stz* szSawTerm)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(szSawTerm->bSame("union"_Stz));
  
  /*
    
   1) union { ... } ;
      
   2) union <unionNm> { ... } ;

   */

  Token xTkn;
  s32 sUnionNm;
  
  vGetNextToken(xTkn);
  if (xTkn.bSame("{"_Stz)) {
    // 1) union { ... } ;
    sUnionNm.vSprint("_UNION_%lld"_Stz,++m_iNextUnion);
  }
  else {
    // 2) union <unionNm> { ... } ;
    sUnionNm = xTkn.szTerm();
    vGetExpectedToken(xTkn,"{"_Stz);
  }
  
  vStructurePush(sUnionNm,MS_UNION,CP_UNION);
  MetaStructure* p_Structure = pParseBraces("{"_Stz,true);
  vStructurePop();
}

/*----------------------------------------------------------------------------*/
/* parse USING _^_ NAMESPACE <name> ;                                         */
/*----------------------------------------------------------------------------*/
void Vortex::vParseUsing(Stz* szSawTerm)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(szSawTerm->bSame("using"_Stz));
  
  Token xTkn;
  
  // get NAMESPACE
  vGetExpectedToken(xTkn,"namespace"_Stz);

  // get <name>
  vGetNextToken(xTkn);

  // get ;
  vGetExpectedToken(xTkn,";"_Stz);
  
  vPartRecognised(CP_USING);
}

/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                    parsing of class | struct | union { ... }               */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* parse:  { ... _^_ ... } [ ; ]                                              */
/*----------------------------------------------------------------------------*/
MetaStructure* Vortex::pParseBraces(Stz* szSawTerm, bool bSemiTerminated)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(szSawTerm->bSame("{"_Stz));
  
  Token xTkn;

  while (bGetNextToken(xTkn)) {

    Stz* szTerm = xTkn.szTerm();

    // block is terminated by } ;
    if (szTerm->bSame("}"_Stz)) {
      if (bSemiTerminated)
        vGetExpectedToken(xTkn,";"_Stz);
      vPartRecognised(CP_BLOCK);
      return m_p_MetaStructure;;
    }
    
    if (bParseLabel(xTkn.szTerm()))
      continue;
    
    vKeywordDescend(xTkn.szTerm());
  }

  vThrowProblem(__LINE__);
  
  NEVER_REACH;
}

/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                            term collection                                 */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* collect delimited tokens (potentially nested)                              */
/*----------------------------------------------------------------------------*/
void Vortex::vCollectBalanced(Stz* szOpen, Stz* szClose)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
         
  Token xTkn;
  for (;;) {
    
    vGetOtherTerm(xTkn);
    
    // test for close
    if (xTkn.bSame(szClose))
      return;
    
    // test for nested open
    if (xTkn.bSame(szOpen))
      vCollectBalanced(szOpen,szClose);
      
    // ignore any other token
  }
  
}

/*----------------------------------------------------------------------------*/
/* collect balanced angle brackets pair: < ... >                              */
/*----------------------------------------------------------------------------*/
void Vortex::vCollectBalancedAngles(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vCollectBalanced("<"_Stz,">"_Stz);
}

/*----------------------------------------------------------------------------*/
/* collect balanced curly braces pair: { ... }                                */
/*----------------------------------------------------------------------------*/
void Vortex::vCollectBalancedBraces(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vCollectBalanced("{"_Stz,"}"_Stz);
}

/*----------------------------------------------------------------------------*/
/* collect balanced parenthesis pair: ( ... )                                 */
/*----------------------------------------------------------------------------*/
void Vortex::vCollectBalancedParenthesis(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vCollectBalanced("("_Stz,")"_Stz);
}

/*----------------------------------------------------------------------------*/
/* collect other terms: <other> ... | ; | { ... }                             */
/*----------------------------------------------------------------------------*/
void Vortex::vCollectOtherTerms(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0 == m_oOtherTerms.nItems());
  
  Token xTkn;
  for (;;) {
    
    vGetOtherTerm(xTkn);
    
    // test for terminating ;
    if (xTkn.bSame(";"_Stz))
      return;
    
    // test for terminating brace pair
    if (xTkn.bSame("{"_Stz)) {
      vCollectBalancedBraces("{"_Stz);
      return;
    }
    
    // ignore any other token
  }

}

/*----------------------------------------------------------------------------*/
/* collect all template terms : TEMPLATE  < _^_ ... >                         */
/*----------------------------------------------------------------------------*/
void Vortex::vCollectTplTerms(Stz* szSawTerm)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(szSawTerm->bSame("<"_Stz));

  m_oTplTerms.vReset();
  
  vFragmentMarker("@"_Stz);
  
  Token xTkn;
  
  // collect ... >
  while (bGetNextToken(xTkn)) {
    if (xTkn.bSame(">"_Stz))
      break;
    m_oTplTerms.vAdd(xTkn.szTerm());
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s48K sTerms;
  m_oTplTerms.vNamesString(sTerms);
  
  TRACE("  TplTerms: %s\n"_Stz,sTerms.azChar());
}

/*----------------------------------------------------------------------------*/
/* discard delimited tokens (potentially nested)                              */
/*----------------------------------------------------------------------------*/
void Vortex::vDiscardBalanced(Stz* szOpen, Stz* szClose)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
         
  Token xTkn;
  for (;;) {
    
    vGetNextToken(xTkn);
    
    // test for close
    if (xTkn.bSame(szClose))
      return;
    
    // test for nested open
    if (xTkn.bSame(szOpen))
      vDiscardBalanced(szOpen,szClose);
      
    // ignore any other token
  }
  
}

/*----------------------------------------------------------------------------*/
/* discard balanced angle brackets pair: < ... >                              */
/*----------------------------------------------------------------------------*/
void Vortex::vDiscardBalancedAngles(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vDiscardBalanced("<"_Stz,">"_Stz);
}

/*----------------------------------------------------------------------------*/
/* discard balanced curly braces pair: { ... }                                */
/*----------------------------------------------------------------------------*/
void Vortex::vDiscardBalancedBraces(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vDiscardBalanced("{"_Stz,"}"_Stz);
}

/*----------------------------------------------------------------------------*/
/* discard balanced parenthesis pair: ( ... )                                 */
/*----------------------------------------------------------------------------*/
void Vortex::vDiscardBalancedParenthesis(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vDiscardBalanced("("_Stz,")"_Stz);
}

/*----------------------------------------------------------------------------*/
/* discard inline tokens: ... INLINE { ... }                                  */
/*----------------------------------------------------------------------------*/
void Vortex::vDiscardInlineTokens(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;
  for (;;) {
    
   vGetNextToken(xTkn);
    
    // test for terminating ;
    if (xTkn.bSame(";"_Stz))
      return;
    
    // test for terminating brace pair
    if (xTkn.bSame("{"_Stz)) {
      vDiscardBalancedBraces("{"_Stz);
      return;
    }
    
    // ignore any other token
  }

}

/*----------------------------------------------------------------------------*/
/* discard static tokens: ... STATIC ... | ; | { ... } ;                      */
/*----------------------------------------------------------------------------*/
void Vortex::vDiscardStaticTokens(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;
  for (;;) {
    
   vGetNextToken(xTkn);
    
    // test for terminating ;
    if (xTkn.bSame(";"_Stz))
      return;
    
    // test for terminating brace pair
    if (xTkn.bSame("{"_Stz)) {
      vDiscardBalancedBraces("{"_Stz);
      vGetExpectedToken(xTkn,";"_Stz);
      return;
    }
    
    // ignore any other token
  }

}


/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                              term skipping                                 */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* skip till given term                                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vDiscardTill(Stz* szTerm, Stz* szTill)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;
  
  TRACE("Skipping: %s "_Stz,szTerm->azChar());
  if (szTerm->bSame(szTill)) {
    TRACE("\n"_Stz,szTerm->azChar());
    return;
  }
        
  while (bGetNextToken(xTkn)) {
    TRACE("%s "_Stz,xTkn.azTerm());
    if (xTkn.bSame(szTill)) {
      TRACE("\n"_Stz,xTkn.azTerm());
      return;
    }
  }
}


/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                               meta structure                               */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* create new meta structure for c++ class                                    */
/*----------------------------------------------------------------------------*/
void Vortex::vStructureNew_class(Stz* szClassNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (m_oTplTerms.nItems()) {
    m_p_MetaStructure = new MetaStructure(szClassNm,
                                          MS_TEMPLATE,
                                          CP_CLASS,
                                          m_pMetaGraph,
                                          m_p_MetaStructure);
    
    m_p_MetaStructure->vTplArgs(&m_oTplTerms);
  }
  else {
    m_p_MetaStructure = new MetaStructure(szClassNm,
                                          MS_LINEAR,
                                          CP_CLASS,
                                          m_pMetaGraph,
                                          m_p_MetaStructure);
  }
}

/*----------------------------------------------------------------------------*/
/* create new meta structure for c++ struct                                   */
/*----------------------------------------------------------------------------*/
void Vortex::vStructureNew_struct(Stz* szStructNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_p_MetaStructure = new MetaStructure(szStructNm,
                                        MS_LINEAR,
                                        CP_STRUCT,
                                        m_pMetaGraph,
                                        m_p_MetaStructure);
}

/*----------------------------------------------------------------------------*/
/* create new meta structure for c++ union                                    */
/*----------------------------------------------------------------------------*/
void Vortex::vStructureNew_union(Stz* szUnionNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_p_MetaStructure = new MetaStructure(szUnionNm,
                                        MS_UNION,
                                        CP_UNION,
                                        m_pMetaGraph,
                                        m_p_MetaStructure);
}

/*----------------------------------------------------------------------------*/
/* pop at structure end                                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vStructurePop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_p_MetaStructure);
  
  m_p_MetaStructure = m_p_MetaStructure->p_ParentStructure();
}

/*----------------------------------------------------------------------------*/
/* push new structure                                                         */
/*----------------------------------------------------------------------------*/
void Vortex::vStructurePush(Stz* szBlockNm, eMS eStructure, eCP eCppPart)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_pMetaGraph);
  
  if (m_p_MetaStructure) {
    m_p_MetaStructure = new MetaStructure(szBlockNm,
                                          eStructure,
                                          eCppPart,
                                          m_pMetaGraph,
                                          m_p_MetaStructure);
  }
  else {
    m_p_MetaStructure = new MetaStructure(szBlockNm,
                                          eStructure,
                                          eCppPart,
                                          m_pMetaGraph,
                                          nullptr);
    m_pMetaGraph->vAppendStructure(m_p_MetaStructure);
  }
}


/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                                   terms                                    */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* get term                                                                   */
/*----------------------------------------------------------------------------*/
Stz* Vortex::szTerm(i8 i_Term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Stz* sz = m_oOtherTerms.szName(i_Term);
  
  return sz;
}

/*----------------------------------------------------------------------------*/
/* add term                                                                   */
/*----------------------------------------------------------------------------*/
void Vortex::vTermAdd(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_oOtherTerms.vAdd(szTerm);
}

/*----------------------------------------------------------------------------*/
/* get number of terms                                                        */
/*----------------------------------------------------------------------------*/
i8 Vortex::nTerms(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  i8 nTerms = m_oOtherTerms.nItems();
  
  return nTerms;
}

/*----------------------------------------------------------------------------*/
/* are terms a function ?                                                     */
/*----------------------------------------------------------------------------*/
bool Vortex::bTermsAreFunction(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  Stz* szTerm;
 
  i8 nTerms = m_oOtherTerms.nItems();
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // term 0
  
  i8 i_Term = 0;
  szTerm = m_oOtherTerms.szName(i_Term++);
  
  // -- -- -- -- -- --
  // constructor function
  if (m_bInClass && szTerm->bSame(m_sClassNm.szStz())) {
    return true;
  }
  
  // -- -- -- -- -- --
  // destructor function
  if (m_bInClass && szTerm->bSame("~"_Stz))
    return true;
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //
  
  // nb. szTerm is return type at this point

  
  // -- -- -- -- -- --
  // skip zero or more '*'
  szTerm = m_oOtherTerms.szName(i_Term++);
  while (szTerm->bSame("*"_Stz))
    szTerm = m_oOtherTerms.szName(i_Term++);
  
  // -- -- -- -- -- --
  // test for member which is a member which is a function pointer
  if (m_bInClass && szTerm->bSame("("_Stz))
    return false;

  // -- -- -- -- -- --
  // attempt to detect a function
  szTerm = m_oOtherTerms.szName(i_Term++);
  while (i_Term < nTerms) {
    if (szTerm->bSame("("_Stz))
      return true;
    szTerm = m_oOtherTerms.szName(i_Term++);
  }
  
  // -- -- -- -- -- --
  // must be a variable
  return false;
}

/*----------------------------------------------------------------------------*/
/* are terms a variable ?                                                     */
/*----------------------------------------------------------------------------*/
bool Vortex::bTermsAreVariable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
   Stz* szTerm;
  
   i8 nTerms = m_oOtherTerms.nItems();
     
   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   // term 0
   
   i8 i_Term = 0;
   szTerm = m_oOtherTerms.szName(i_Term++);
  
  // -- -- -- -- -- --
  // exclude constructor function
  if (m_bInClass && szTerm->bSame(m_sClassNm.szStz()) &&
      m_oOtherTerms.szName(i_Term)->bSame("("_Stz)) {
    return false;
  }
  
  // -- -- -- -- -- --
  // exclude destructor function
  if (m_bInClass && szTerm->bSame("~"_Stz))
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //
  
  // nb. szTerm is return type at this point

  
  // -- -- -- -- -- --
  // skip zero or more '*'
  szTerm = m_oOtherTerms.szName(i_Term++);
  while (szTerm->bSame("*"_Stz))
    szTerm = m_oOtherTerms.szName(i_Term++);
  
  // -- -- -- -- -- --
  // test for variable which is a function pointer
  if (szTerm->bSame("("_Stz))
    return true;

  // -- -- -- -- -- --
  // attempt to detect a function
  szTerm = m_oOtherTerms.szName(i_Term++);
  while (i_Term < nTerms) {
    if (szTerm->bSame("("_Stz))
      return false;
    szTerm = m_oOtherTerms.szName(i_Term++);
  }
  
  // -- -- -- -- -- --
  // must be a variable
  return true;
}

/*----------------------------------------------------------------------------*/
/* reset terms                                                                */
/*----------------------------------------------------------------------------*/
void Vortex::vTermsReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_oOtherTerms.vReset();
}

/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                                 machinary                                  */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* ascend from parsing a class                                                */
/*----------------------------------------------------------------------------*/
void Vortex::vClassAscend(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_bInClass);

  m_bInClass = false;
  m_sClassNm = ""_Stz;

  m_p_MetaStructure  = nullptr;
}

/*----------------------------------------------------------------------------*/
/* descend into parsing a class                                               */
/*----------------------------------------------------------------------------*/
void Vortex::vClassDescend(Stz* szClassNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(! m_bInClass);
  
  m_sClassNm = szClassNm;
  m_bInClass = true;
}

/*----------------------------------------------------------------------------*/
/* get next token                                                             */
/*----------------------------------------------------------------------------*/
void Vortex::vDiscardNextToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;
  
  CHECKED(m_pInput->bGetToken(&xTkn))
  
  if (m_bTrace)
    TRACE("Discarding: %s\n"_Stz,xTkn.azTerm());
}

/*----------------------------------------------------------------------------*/
/* get expected token                                                         */
/*----------------------------------------------------------------------------*/
void Vortex::vGetExpectedToken(Token& roToken, Stz* szExpect)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  CHECKED(m_pInput->bGetToken(&roToken));
  
  if (! roToken.bSame(szExpect)) {
    TRACE("Expecting token: %s got: %s\n"_Stz,
           szExpect->azChar(), roToken.azTerm());
    
    vThrowProblem(__LINE__);
  }
  
  vFragmentAppend(roToken.szTerm());
}

/*----------------------------------------------------------------------------*/
/* get next token                                                             */
/*----------------------------------------------------------------------------*/
bool Vortex::bGetNextToken(Token& roToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  bool b = m_pInput->bGetToken(&roToken);
  
  if (b)
    vFragmentAppend(roToken.szTerm());
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get next token                                                             */
/*----------------------------------------------------------------------------*/
void Vortex::vGetNextToken(Token& roToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  bool b = bGetNextToken(roToken);
  
  if (! b)
    vThrowProblem(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* get next token as per normal but also and put it into m_oOtherTerms        */
/*----------------------------------------------------------------------------*/
void Vortex::vGetOtherTerm(Token& roTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  CHECKED(bGetNextToken(roTerm))
  
  m_oOtherTerms.vAdd(roTerm.szTerm());
}

/*----------------------------------------------------------------------------*/
/* get next token as per normal but also and put it into m_oTplArgTerms       */
/*----------------------------------------------------------------------------*/
void Vortex::vGetTplArg(Token& roTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  CHECKED(bGetNextToken(roTerm))
  
  m_oTplTerms.vAdd(roTerm.szTerm());
}

/*----------------------------------------------------------------------------*/
/* append sub-string to fragment string                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vFragmentAppend(Stz* szString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (m_sFragment.nLength())
    m_sFragment.vAppend(' ');
  
  m_sFragment.vConcat(szString);
  
  if (m_bTrace)
    TRACE("Fragment: %s\n"_Stz,m_sFragment.azChar());
}

/*----------------------------------------------------------------------------*/
/* append sub-string to fragment string with marker                           */
/*----------------------------------------------------------------------------*/
void Vortex::vFragmentMarker(Stz* szString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (m_sFragment.nLength())
    m_sFragment.vAppend(' ');

  m_sFragment.vConcat("❗️"_Stz);
  m_sFragment.vConcat(szString);

  if (m_bTrace)
    TRACE("Fragment: %s\n"_Stz,m_sFragment.azChar());
}

/*----------------------------------------------------------------------------*/
/* reset fragment string                                                      */
/*----------------------------------------------------------------------------*/
void Vortex::vFragmentReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_sFragment = ""_Stz;
}

/*----------------------------------------------------------------------------*/
/* ascend from parsing a namespace                                            */
/*----------------------------------------------------------------------------*/
void Vortex::vNamepaceAscend(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_bInNamespace);

  m_bInNamespace = false;
  m_sNamespaceNm = ""_Stz;
}

/*----------------------------------------------------------------------------*/
/* descend into parsing a namespace                                           */
/*----------------------------------------------------------------------------*/
void Vortex::vNamepaceDescend(Stz* szNamespaceNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(! m_bInNamespace);
  RASSERT(! m_bInClass);

  m_sNamespaceNm = szNamespaceNm;
  m_bInNamespace = true;
}

/*----------------------------------------------------------------------------*/
/* reset other terms                                                          */
/*----------------------------------------------------------------------------*/
void Vortex::vOtherTermsReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_oOtherTerms.vReset();
}

/*----------------------------------------------------------------------------*/
/* recognised c++ part                                                        */
/*----------------------------------------------------------------------------*/
void Vortex::vPartRecognised(eCP eCppPart)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (m_bTrace)
    TRACE("Recognised: %s\n\n"_Stz,aCP[eCppPart]);

  vFragmentReset();
}

/*----------------------------------------------------------------------------*/
/* throw problem                                                              */
/*----------------------------------------------------------------------------*/
void Vortex::vThrowProblem(i8 i_Problem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  throw i_Problem;
}

/*----------------------------------------------------------------------------*/
/* reset template argument terms                                              */
/*----------------------------------------------------------------------------*/
void Vortex::vTplTermsReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_oTplTerms.vReset();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

 1)
    void vCheckAssumptions ( void ) ;
 
 2)
    Forth * _pForth  ( void * aAddr ) ;
 
 3)    template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
    OVER * Nub_t <CN,HEAD,OVER,TAIL,DESC> :: aAllotOver (i8 nOverSize)
 
 
 4)    template <eCN CN, class HEAD, class OVER, class TAIL, class DESC, i8 BYTES>
    TrayNL_t < CN,HEAD,OVER,TAIL,DESC,BYTES> * _pNubNL (void* aAddr);

 5)
       template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
    Nub_t < CN,HEAD,OVER,TAIL,DESC> :: Nub_t (void)
    : Object(CN)
    { .. }

 6)
       template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
    Nub_t < CN,HEAD,OVER,TAIL,DESC> :: ~ Nub_t (void) noexcept(false)
    { ... }
 
 7)    template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
    i8 Nub_t  <CN,HEAD,OVER,TAIL,DESC> :: nBytes (i8 nUnits) { .. }


maybe
  ?)    template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
    Some_t <CN>* Nub_t <CN,HEAD,OVER,TAIL,DESC> :: aFun (i8 nOverSize)

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#if 0
/*----------------------------------------------------------------------------*/
/* collect terms for function intro:                                          */
/*----------------------------------------------------------------------------*/
bool Vortex::bCollectFunctionTerms(Stz* szSawTerm, Token& roFunNm, eFN& reFun)
{
  TRACE("/%s/ >%s SAW: %s\n"_Stz,m_sSourcePath.szStz(),__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  
   1)
      void vCheckAssumptions ( void ) ;
   
   2)
      Forth * _pForth  ( void * aAddr ) ;
   
   3)    template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
      OVER * Nub_t <CN,HEAD,OVER,TAIL,DESC> :: aAllotOver (i8 nOverSize)
   
   
   4)    template <eCN CN, class HEAD, class OVER, class TAIL, class DESC, i8 BYTES>
      TrayNL_t < CN,HEAD,OVER,TAIL,DESC,BYTES> * _pNubNL (void* aAddr);

   5)
         template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
      Nub_t < CN,HEAD,OVER,TAIL,DESC> :: Nub_t (void)
      : Object(CN)
      { .. }

   6)
         template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
      Nub_t < CN,HEAD,OVER,TAIL,DESC> :: ~ Nub_t (void) noexcept(false)
      { ... }
   
   7)    template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
      i8 Nub_t  <CN,HEAD,OVER,TAIL,DESC> :: nBytes (i8 nUnits) { .. }


  maybe
    ?)    template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
      Some_t <CN>* Nub_t <CN,HEAD,OVER,TAIL,DESC> :: aFun (i8 nOverSize)

   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  
  vFragmentMarker("@"_Stz);

  Token xTkn;

  m_oFunctionTerms.vReset();
  
  s32 sPrior = szSawTerm;
  
  xTkn.vCopy(sPrior);
  xTkn.vToken(TN_NAME);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // skip over optional inline

  if (xTkn.bSame("inline"_Stz)) {
    m_oFunctionTerms.vAdd(szSawTerm);
    CHECKED(bGetNextToken(xTkn));
  }
  m_oFunctionTerms.vAdd(xTkn.szTerm());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  while (! xTkn.bSame("("_Stz)) {
    m_oFunctionTerms.vAdd(xTkn.szTerm());
    sPrior = xTkn.szTerm();
    CHECKED(bGetNextToken(xTkn));
  }
  m_oFunctionTerms.vAdd(xTkn.szTerm());

  roFunNm.vCopy(sPrior);
  roFunNm.vToken(TN_NAME);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  while (! xTkn.bSame(")"_Stz)) {
    m_oFunctionTerms.vAdd(xTkn.szTerm());
    CHECKED(bGetNextToken(xTkn));
  }
  m_oFunctionTerms.vAdd(xTkn.szTerm());

  CHECKED(bGetNextToken(xTkn));

  if (xTkn.bSame(":"_Stz)) {
    while (! xTkn.bSame("{"_Stz)) {
      CHECKED(bGetNextToken(xTkn));
    }
  }
  
  if (xTkn.bSame("noexcept"_Stz)) {
    CHECKED(bGetNextToken(xTkn));
    CHECKED(xTkn.bSame("("_Stz));
    CHECKED(bGetNextToken(xTkn));
    CHECKED(xTkn.bSame("false"_Stz));
    CHECKED(bGetNextToken(xTkn));
    CHECKED(xTkn.bSame(")"_Stz));
    CHECKED(bGetNextToken(xTkn));
  }

  if (xTkn.bSame("{"_Stz)) {
    vSkipBalancedBraces("{"_Stz);
  }
  else {
    CHECKED(xTkn.bSame(";"_Stz));
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s48K sTermsString;
  m_oFunctionTerms.vNamesString(sTermsString);
  
  TRACE("  %s FunctionTerms: %s ReturnType: %s\n"_Stz,
         __func__,
         roFunNm.szTerm(),sTermsString.azChar());
  
  return true;
}

#endif

