/*============================================================================*/
/* Vortex.cpp - Vortex                                                        */
/*============================================================================*/

#include "Utilities.h"

#define TRACE(fmt, ...) if (m_bTrace) vTrace(fmt, ##__VA_ARGS__)

#define FNENTRY \
  TRACE("%lld/%s/%16s/  `%s`  %s%s%s%s%s%s%s%s%s\n"_Stz \
    \
    , m_nSources \
    \
    , m_sSourcePath.szStz() \
    \
    , __func__ \
    \
    , m_oCurTkn.szTerm() \
    \
    , m_bInNamespace ? " --NAMESPACE `" : "" \
    , m_bInNamespace ? m_sNamespaceNm.azChar() : "" \
    , m_bInNamespace ? "`" : "" \
    \
    , m_bInClass     ? " --CLASS `" : "" \
    , m_bInClass     ? m_sClassNm.azChar() : "" \
    , m_bInClass     ? "`" : "" \
    \
    , m_bInTemplate  ? " --TPLSPEC `" : "" \
    , m_bInTemplate  ? m_sTplSpec.azChar() : "" \
    , m_bInTemplate  ? "`" : "" \
    \
    );

#define GATHER \
  TRACE("%lld/%s/%16s/  `%s`  %s%s%s%s%s%s%s%s%s\n"_Stz \
    \
    , m_nSources \
    \
    , m_sSourcePath.szStz() \
    \
    , __func__ \
    \
    , m_oCurTkn.szTerm() \
    \
    , m_bInNamespace ? " --NAMESPACE `" : "" \
    , m_bInNamespace ? m_sNamespaceNm.azChar() : "" \
    , m_bInNamespace ? "`" : "" \
    \
    , m_bInClass     ? " --CLASS `" : "" \
    , m_bInClass     ? m_sClassNm.azChar() : "" \
    , m_bInClass     ? "`" : "" \
    \
    , m_bInTemplate  ? " --TPLSPEC `" : "" \
    , m_bInTemplate  ? m_sTplSpec.azChar() : "" \
    , m_bInTemplate  ? "`" : "" \
    \
    );

#define FN \
  TRACE("%lld/%s/%16s/  `%s`\n"_Stz \
    \
    , m_nSources \
    \
    , m_sSourcePath.szStz() \
    \
    , __func__ \
    \
    , m_oCurTkn.szTerm() \
    \
    );

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
  m_oCurTkn.vReset();
  m_bEOF         = false;
  
  // tracing and debugging

  m_bPrint       = true;
  m_bTrace       = true;
  m_bContMember  = false;
  m_bContPart    = false;
  m_bContType    = false;

  // output
  
  m_pOutputFile  = nullptr;
  m_pMetaGraph   = new MetaGraph;
  
  // working
  
  m_bInClass     = false;
  m_sClassNm     = C_NO_NAME;
  m_bInNamespace = false;
  m_sNamespaceNm = C_NO_NAME;
  m_bInTemplate  = false;
  m_oTplSpec.vReset();
  m_sTplSpec     = C_NO_NAME;

  
  m_iNextStruct  = 0;
  m_iNextUnion   = 0;

  m_p_MetaType   = nullptr;
  m_p_MetaMember = nullptr;
  m_p_ConstSet   = m_pMetaGraph->pConstSet();
  
  m_oTypeSpec.vReset();
  m_nAsterisks = 0;
  m_oFunVarNm.vReset();
  m_bIsFunPtr  = false;
  m_bIsArray   = false;
  m_oArraySpec.vReset();
  m_bIsFun     = false;
  m_oFunSpec.vReset();
  m_oTypeArgs.vReset();

  //m_oOtherTerms
  //m_oTplParamTerms

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
  
  bool b = bGetNextToken();
  
  if (b) {
    
    b = bParseAllParts();
    
    if (b) {
      TRACE("Parsed file %lld) %s\n"_Stz,m_nSources,szSourcePath->azChar());
      TRACE("=============================================================\n"_Stz);
    }
    else {
      TRACE("\n\n"_Stz);
      TRACE("** Unable to fully process %s\n"_Stz,szSourcePath->azChar());
      TRACE("\n\n"_Stz);
      NEVER_REACH;
    }
    
  }
  else {
    TRACE("Parsed empty file %lld) %s\n"_Stz,m_nSources,szSourcePath->azChar());
    TRACE("=============================================================\n"_Stz);
  }

  m_nSources++;
  
  delete m_pInput;
  m_pInput = nullptr;
  
  RASSERT(! m_bInClass);
  RASSERT(! m_bInNamespace);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get expected token                                                         */
/*----------------------------------------------------------------------------*/
void Vortex::vGetExpectedToken(Stz* szExpect)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  CHECKED(m_pInput->bGetToken(&m_oCurTkn));
  
  if (! m_oCurTkn.bSame(szExpect)) {
    TRACE("Expecting token: %s got: %s\n"_Stz,
           szExpect->azChar(), m_oCurTkn.azTerm());
    
    vThrowProblem(__LINE__);
  }
  
}

/*----------------------------------------------------------------------------*/
/* get next token                                                             */
/*----------------------------------------------------------------------------*/
bool Vortex::bGetNextToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_bEOF = ! m_pInput->bGetToken(&m_oCurTkn);
    
  return ! m_bEOF;
}

/*----------------------------------------------------------------------------*/
/* get next token                                                             */
/*----------------------------------------------------------------------------*/
void Vortex::vGetNextToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  bool b = bGetNextToken();
  
  if (! b)
    vThrowProblem(__LINE__);
}


/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                            top-down parsing                                */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* parse all parts in the current source file (.h or .hpp)                    */
/*----------------------------------------------------------------------------*/
bool Vortex::bParseAllParts(void)
{
  FNENTRY
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  RASSERT(! m_bInNamespace);
  
  // -- parse till EOF --
  while (! m_bEOF) {
    
    try {
      vKeywordDescend();
      
      FNENTRY
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
void Vortex::vKeywordDescend(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  if (m_oCurTkn.bSame("class"_Stz))     vParseClass();     else
  if (m_oCurTkn.bSame("const"_Stz))     vParseConst();     else
  if (m_oCurTkn.bSame("enum"_Stz))      vParseEnum();      else
  if (m_oCurTkn.bSame("extern"_Stz))    vParseExtern();    else
  if (m_oCurTkn.bSame("friend"_Stz))    vParseFriend();    else
  if (m_oCurTkn.bSame("#"_Stz))         vParseHash();      else
  if (m_oCurTkn.bSame("namespace"_Stz)) vParseNamespace(); else
  if (m_oCurTkn.bSame("public"_Stz))    vParsePublic();    else
  if (m_oCurTkn.bSame(";"_Stz))         vParseSemicolon(); else
  if (m_oCurTkn.bSame("static"_Stz))    vParseStatic();    else
  if (m_oCurTkn.bSame("struct"_Stz))    vParseStruct();    else
  if (m_oCurTkn.bSame("template"_Stz))  vParseTemplate();  else
  if (m_oCurTkn.bSame("typedef"_Stz))   vParseTypedef();   else
  if (m_oCurTkn.bSame("union"_Stz))     vParseUnion();     else
  if (m_oCurTkn.bSame("using"_Stz))     vParseUsing();
  else                                  vParseFunVar(false);
}

/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                      class, block, member parsing                          */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* parse: CLASS <classNm> : _^_ ...                                           */
/*----------------------------------------------------------------------------*/
void Vortex::vParseBaseList(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_bInClass);
  
  vDebugPoint();
   
  while (bGetNextToken()) {
     
    Stz* szTerm = m_oCurTkn.szTerm();
     
    // ignore optional visibility
    if (szTerm->bOneOf("private"_Stz,"protected"_Stz,"public"_Stz,nullptr))
      vGetNextToken();

    // record the name of the inherited class
    Stz* szInheritedClassNm = m_oCurTkn.szTerm();

    // add inherited type to current type
    MetaInherit* pInherit = m_p_MetaType->pAddInherit(szInheritedClassNm);
    vPartRecognised(CP_INHERITED_CLASS);

    vGetNextToken();

    // collect optional template arguments <...>
    if (szTerm->bSame("<"_Stz)) {
     
      vCollectTypeArgs(m_oTypeArgs);
      pInherit->vTplArgs(&m_oTypeArgs);
      m_oTypeArgs.vReset();
     
      vGetNextToken();
    }

    // base list is terminated by open brace, being the beginning of members
    if (szTerm->bSame("{"_Stz)) {
      vPartRecognised(CP_BASE_END);
      vGetNextToken();
      break;
    }

    // base list is continued by a comma
    CHECKED(szTerm->bSame(","_Stz));
  }

  // parse class members
  vParseMembers(true);
}

/*----------------------------------------------------------------------------*/
/* parse: CLASS _^_ <name> : ...  | { ...                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vParseClass(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_oCurTkn.bSame("class"_Stz));
  RASSERT(! m_p_MetaType);
      
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get <name>
  
  vGetNextToken();
  s32 sClassNm = m_oCurTkn.szTerm();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get ; or : or {
  
  vGetNextToken();
    
  // forward declaration of a class?
  if (m_oCurTkn.bSame(";"_Stz)) {
    vPartRecognised(CP_FORWARD);
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vTypePush(sClassNm,MT_LINEAR,CP_CLASS_BEGIN);
  vClassBegin(sClassNm);
  
  if (m_oTplSpec.nItems()) {
    m_p_MetaType->vTplParams(& m_oTplSpec);
    m_oTplSpec.vReset();
  }

  if (m_oCurTkn.bSame(":"_Stz)) {
    vPartRecognised(CP_CLASS_BEGIN);
    vPartRecognised(CP_BASE_BEGIN);
    vGetNextToken();
    vParseBaseList();
  }
  else
  if (m_oCurTkn.bSame("{"_Stz)) {
    vPartRecognised(CP_CLASS_BEGIN);
    vPartRecognised(CP_MEMBERS_BEGIN);
    vGetNextToken();
    vParseMembers(true);
  }
  else {
    vThrowProblem(__LINE__);
  }

  vPartRecognised(CP_CLASS_END);
  vClassEnd();
}

/*----------------------------------------------------------------------------*/
/* parse: [STATIC] CONST _^_ ... ;                                            */
/*----------------------------------------------------------------------------*/
void Vortex::vParseConst(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("const"_Stz));
  //RASSERT(0 == m_oOtherTerms.nItems());

  m_oOtherTerms.vReset();

  // get <type>
  vGetNextToken();
  
  for (;;) {
    CHECKED(bGetNextToken());
    if (! m_oCurTkn.bSame("*"_Stz))
      break;
  }
    
  // get <type>
  m_sConstNm = m_oCurTkn.szTerm();

  // get =
  vGetExpectedToken("="_Stz);
  
  // get ... ;
  vCollectRemainingTerms(m_oOtherTerms);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  ConstDef* pConstDef = m_p_ConstSet->pAddConstDef(m_sConstNm);
  pConstDef->vTerms(m_oOtherTerms);

  vPartRecognised(CP_CONST);
  
  m_oOtherTerms.vReset();
  
  bool bMayBe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: ENUM _^_ ... ;                                                      */
/*----------------------------------------------------------------------------*/
void Vortex::vParseEnum(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("enum"_Stz));
    
  vDiscardTill(m_oCurTkn.szTerm(),";"_Stz);
  
  vPartRecognised(CP_ENUM);

  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: EXTERN _^_ ... ;                                                    */
/*----------------------------------------------------------------------------*/
void Vortex::vParseExtern(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("extern"_Stz));
    
  vDiscardTill(m_oCurTkn.szTerm(),";"_Stz);
  
  vPartRecognised(CP_ENUM);
  
  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: FRIEND _^_ ... ;                                                    */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFriend(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("friend"_Stz));
  RASSERT(m_bInClass);
  
  vDiscardTill(m_oCurTkn.szTerm(),";"_Stz);
  
  vPartRecognised(CP_FRIEND);
  
  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: # _^_ DEFINE ... | IF ... | INCLUDE ... | PRAGMA ...                */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHash(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("#"_Stz));
    
  CHECKED(bGetNextToken());
  
  if (m_oCurTkn.bSame("define"_Stz)) {
    vParseHashDefine();
    return;
  }

  if (m_oCurTkn.bSame("if"_Stz)) {
    vParseHashIf();
    return;
  }

  if (m_oCurTkn.bSame("ifdef"_Stz)) {
    vParseHashIfdef();
    return;
  }

  if (m_oCurTkn.bSame("include"_Stz)) {
    vParseHashInclude();
    return;
  }

  if (m_oCurTkn.bSame("pragma"_Stz)) {
    vParseHashPragma();
    return;
  }

  vThrowProblem(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* parse: # DEFINE _^_ ... ENDDEFINE                                          */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHashDefine(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("define"_Stz));
      
  vGetNextToken();
  while (! m_oCurTkn.bSame("enddefine"_Stz)) {
    vGetNextToken();
  }
      
  vPartRecognised(CP_HASH_DEFINE);
 
  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: # IF _^_ ... #ENDIF                                                 */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHashIf(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("if"_Stz));
    
  vGetNextToken();
  while (! m_oCurTkn.bSame("endif"_Stz)) {
    vGetNextToken();
  }
      
  vPartRecognised(CP_HASH_IF);
  
  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: # IF _^_ ... #ENDIF                                                 */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHashIfdef(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("ifdef"_Stz));
    
  vGetNextToken();
  while (! m_oCurTkn.bSame("endif"_Stz)) {
    vGetNextToken();
  }
      
  vPartRecognised(CP_HASH_IF);
  
  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: # INCLUDE _^_ <filePath>                                            */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHashInclude(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("include"_Stz));
  
  vGetNextToken();
  
  if (m_oCurTkn.bSame("<"_Stz))
    vDiscardBalancedAngles();
    
  vPartRecognised(CP_HASH_INCLUDE);
  
  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: # PRAGMA _^_ ONCE                                                   */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHashPragma(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("pragma"_Stz));
  
  vGetNextToken();
  
  if (m_oCurTkn.bSame("once"_Stz)) {
    vPartRecognised(CP_HASH_PRAGMA_ONCE);
    
    bool bMaybe = bGetNextToken();
    return;
  }

  if (m_oCurTkn.bSame("pack"_Stz)) {
    vGetExpectedToken("("_Stz);
    vDiscardBalancedParenthesis();
    vPartRecognised(CP_HASH_PRAGMA_PACK);
    
    bool bMaybe = bGetNextToken();
    return;
  }

  vThrowProblem(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* parse: NAMESPACE _^_ <name> { <parts> }                                    */
/*----------------------------------------------------------------------------*/
void Vortex::vParseNamespace(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_oCurTkn.bSame("namespace"_Stz));
  
  // get <name> of namespace
  vGetNextToken();
  
  vNamepaceBegin(m_oCurTkn.szTerm());
  
  // get {
  vGetExpectedToken("{"_Stz);


  vTraceNamespaceNm();
  vPartRecognised(CP_NAMESPACE_BEGIN);
  
  vGetNextToken();

  FNENTRY

  // parse ... }
  for (;;) {
  
    // test for } being end-of-namespace
    if (m_oCurTkn.bSame("}"_Stz))  {
      vTraceNamespaceNm();
      vPartRecognised(CP_NAMESPACE_END);
      vNamepaceEnd();
      
      bool bMaybe = bGetNextToken();
      return;
    }
    
    // -- -- -- -- -- --
    
    Stz* szKeyWord = m_oCurTkn.szTerm();
    
    vKeywordDescend();
    
    FNENTRY
    
  } // while more tokens
  
}

/*----------------------------------------------------------------------------*/
/* parse: ; _^_                                                               */
/*----------------------------------------------------------------------------*/
void Vortex::vParsePublic(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("public"_Stz));

  vGetExpectedToken(":"_Stz);

  vPartRecognised(CP_LABEL);
  
  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: ; _^_                                                               */
/*----------------------------------------------------------------------------*/
void Vortex::vParseSemicolon(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame(";"_Stz));
  
  vPartRecognised(CP_EMPTY);
  
  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: STATIC _^_ CONST ... | ...                                          */
/*----------------------------------------------------------------------------*/
void Vortex::vParseStatic(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_oCurTkn.bSame("static"_Stz));
    
  vGetNextToken();

  if (m_oCurTkn.bSame("const"_Stz)) {
    vParseConst();
    return;
  }
  
  if (m_oCurTkn.bSame("inline"_Stz)) {
    vDiscardInlineTokens();
    vPartRecognised(CP_STATIC);
    bool bMaybe = bGetNextToken();
    return;
  }
  
  vDiscardStaticTokens();
  vPartRecognised(CP_STATIC);
  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: STRUCT _^_ { ...                                                    */
/*----------------------------------------------------------------------------*/
void Vortex::vParseStruct(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_oCurTkn.bSame("struct"_Stz));

  /*
    
   1) struct { ... } ;
      
   2) struct { ... } <memberNm> ;
   
   3) struct { ... } <memberNm> [ <constNm> ] ;

   4) struct <structNm> ;

   5) struct <structNm> { ... } ;

   */
  
  s32 sStructNm;

  vGetNextToken();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // scenarios 1), 2) and 3)
  
  if (m_oCurTkn.bSame("{"_Stz)) {
    
    sStructNm.vSprint("_STRUCT_%lld"_Stz,++m_iNextStruct);

    vTypePush(sStructNm,MT_LINEAR,CP_STRUCT);
    vGetNextToken();
    vParseMembers(false);
    vTypePop();

    // 1) struct { ... } ;
    //vGetNextToken();
    if (m_oCurTkn.bSame(";"_Stz))
      return;
    
    // <#> handle member name in xTkn
    
    // 2) struct { ... } <memberNm> ;
    vGetNextToken();
    if (m_oCurTkn.bSame(";"_Stz))
      return;

    // 3) struct { ... } <memberNm> [ <constNm> ] ;
    CHECKED(m_oCurTkn.bSame("["_Stz));
    vGetNextToken();
    vGetExpectedToken("]"_Stz);
    vGetExpectedToken(";"_Stz);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // scenarios 4) and 5)
  
  // <#> handle member name in xTkn

  sStructNm = m_oCurTkn.szTerm();
  
  // get ; or {
  vGetNextToken();
  
  // 4) struct <blockNm> ;
  if (m_oCurTkn.bSame(";"_Stz))
    return;

  // 5) struct <blockNm> { ... } ;
  if (m_oCurTkn.bSame("{"_Stz)) {
    
    vTypePush(sStructNm,MT_LINEAR,CP_STRUCT);
    vGetNextToken();
    vParseMembers(true);
    vTypePop();
    
    return;
  }
  
  // 6) struct <blockNm> <memberNm> ;
  vGetExpectedToken(";"_Stz);

  // <#> handle member name
  
  vPartRecognised(CP_STRUCT);
}

/*----------------------------------------------------------------------------*/
/* parse: TEMPLATE _^_ < ... > CLASS ...  | <other> ...                       */
/*----------------------------------------------------------------------------*/
void Vortex::vParseTemplate(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_oCurTkn.bSame("template"_Stz));
        
  vGetExpectedToken("<"_Stz);
  
  vCollectTemplateSpec(m_oTplSpec);
  vTemplateBegin();
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // class, friend or <funVar<
  
  vGetNextToken();
  
  if (m_oCurTkn.bSame("class"_Stz)) {
    vParseClass();
    vTemplateEnd();
    return;
  }
  
  if (m_oCurTkn.bSame("friend"_Stz)) {
    vParseFriend();
    vTemplateEnd();
    return;
  }
  
  vParseFunVar(true);
  vTemplateEnd();
  
  m_oClassArgs.vReset();
}

/*----------------------------------------------------------------------------*/
/* parse: TYPEDEF _^_ ... ;                                                   */
/*----------------------------------------------------------------------------*/
void Vortex::vParseTypedef(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("typedef"_Stz));
  
  vDebugPoint();
  
  vDiscardTill(m_oCurTkn.szTerm(),";"_Stz);
  
  vPartRecognised(CP_TYPEDEF);
  
  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: UNION _^_ { ...                                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vParseUnion(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_oCurTkn.bSame("union"_Stz));
  
  /*
    
   1) union { ... } ;
      
   2) union <unionNm> { ... } ;

   */

  s32 sUnionNm;
  
  vGetNextToken();
  if (m_oCurTkn.bSame("{"_Stz)) {
    // 1) union { ... } ;
    sUnionNm.vSprint("_UNION_%lld"_Stz,++m_iNextUnion);
  }
  else {
    // 2) union <unionNm> { ... } ;
    sUnionNm = m_oCurTkn.szTerm();
    vGetExpectedToken("{"_Stz);
  }

  vGetNextToken();

  vTypePush(sUnionNm,MT_UNION,CP_UNION);
  vParseMembers(true);
  vTypePop();
}

/*----------------------------------------------------------------------------*/
/* parse: USING _^_ NAMESPACE <name> ;                                        */
/*----------------------------------------------------------------------------*/
void Vortex::vParseUsing(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_oCurTkn.bSame("using"_Stz));
    
  // get NAMESPACE
  vGetExpectedToken("namespace"_Stz);

  // get <name>
  vGetNextToken();

  // get ;
  vGetExpectedToken(";"_Stz);
  
  vPartRecognised(CP_USING);
  
  bool bMaybe = bGetNextToken();
}


/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                                begin and end                               */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* descend into parsing a class                                               */
/*----------------------------------------------------------------------------*/
void Vortex::vClassBegin(Stz* szClassNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(! m_bInClass);
  
  m_sClassNm = szClassNm;
  m_bInClass = true;
  
  TRACE("  ClassNm: %s\n"_Stz,m_sClassNm.szStz());
}

/*----------------------------------------------------------------------------*/
/* ascend from parsing a class                                                */
/*----------------------------------------------------------------------------*/
void Vortex::vClassEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_bInClass);

  vTypePop();

  m_bInClass = false;
  m_sClassNm = ""_Stz;

  m_p_MetaType  = nullptr;
  
}

/*----------------------------------------------------------------------------*/
/* descend into parsing a namespace                                           */
/*----------------------------------------------------------------------------*/
void Vortex::vNamepaceBegin(Stz* szNamespaceNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(! m_bInNamespace);
  RASSERT(! m_bInClass);

  m_sNamespaceNm = szNamespaceNm;
  m_bInNamespace = true;
}

/*----------------------------------------------------------------------------*/
/* ascend from parsing a namespace                                            */
/*----------------------------------------------------------------------------*/
void Vortex::vNamepaceEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_bInNamespace);

  m_bInNamespace = false;
  m_sNamespaceNm = ""_Stz;
}

/*----------------------------------------------------------------------------*/
/* descend into template                                                      */
/*----------------------------------------------------------------------------*/
void Vortex::vTemplateBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(! m_bInTemplate);
  
  m_bInTemplate = true;
}

/*----------------------------------------------------------------------------*/
/* ascend from a template                                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vTemplateEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_bInTemplate);

  m_bInTemplate = false;
  m_sTplSpec    = ""_Stz;
}


/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                         function / variable gathering                      */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* parse: <other> _^_ <term> ...                                              */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFunVar(bool bTemplated)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
    
  vParseFV_Type();

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
  
  m_p_MetaMember = m_p_MetaType->pAddMember(szVarNm);
  
  m_p_MetaMember->vTypeNm(szTypeNm);
  m_p_MetaMember->vPointers(nPointers);
  m_p_MetaMember->vFunPtr(bFunPtr);
  m_p_MetaMember->vElements(nElements);
  m_p_MetaMember->vBits(nBits);

#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
}


/*----------------------------------------------------------------------------*/
/* parse information about type spec for a function or variable               */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFV_Type(void)
{
  GATHER
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_oTypeArgs.vReset();
  
  m_nAsterisks = 0;

  for (;;) {
    
    if (m_oCurTkn.bSame("inline"_Stz)) {
      vGetNextToken();
      continue;
    }
    
    if (m_oCurTkn.bSame("static"_Stz)) {
      vGetNextToken();
      continue;
    }
    
    if (m_oCurTkn.bSame("unsigned"_Stz)) {
      vGetNextToken();
      continue;
    }

    if (m_oCurTkn.bSame("["_Stz)) {
      vGetExpectedToken("["_Stz);
      vGetNextToken();
      vGetExpectedToken("]"_Stz);
      vGetExpectedToken("]"_Stz);
      vGetNextToken();
      continue;
    }

    break;
  }
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // detect the case of no specifier (constructor / destructor)
  if (m_bInClass) {
    
    // -- -- -- -- -- -- -- -- -- -- -- --
    // 1)
    if (m_oCurTkn.bSame(m_sClassNm)) {
      
      vGetNextToken();
      
      
      // do we have template arguments?
      if (m_oCurTkn.bSame("<"_Stz)) {
        vCollectTypeArgs(m_oTypeArgs);
        vGetNextToken();
      }
      

      if (m_oCurTkn.bSame("("_Stz)) {
        
        // 1a) constructor (no specifier)
        TRACE("  (typeless) constructor\n"_Stz);
        vGetNextToken();
        vParseFV_Constructor();
        return;
      }
            
      // 1b)
      goto L_Template_Args;
    }
    
    // -- -- -- -- -- -- -- -- -- -- -- --
    // 2) destructor (no specifier)
    if (m_oCurTkn.bSame("~"_Stz)) {
      TRACE("  (typeless) destructor\n"_Stz);
      vGetNextToken();
      vParseFV_Name();
      return;
    }
    
    // -- -- -- -- -- -- -- -- -- -- -- --
    // 3) typeless operator (no specifier)
    if (m_oCurTkn.bSame("operator"_Stz)) {
      vGetNextToken();
      vParseFV_Operator();
      return;
    }
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // not in class
  
  // auto conversion operator
  if (m_oCurTkn.bSame("operator"_Stz)) {
    vGetNextToken();
    vParseFV_Operator();
    return;
  }

  
  // we have a <name>
  m_oTypeSpec.vAdd(m_oCurTkn.szTerm());
  vGetNextToken();
  
  L_Template_Args:
  
  // do we have template arguments?
  if (m_oCurTkn.bSame("<"_Stz)) {
    vCollectTypeArgs(m_oTypeArgs);
    vGetNextToken();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // no specifier (constructor / destructor) after  <classNm> <typeAgs> ::
  
  if (m_oCurTkn.bSame(":"_Stz)) {
    vGetExpectedToken(":"_Stz);
    vGetNextToken();

    if (m_oCurTkn.bSame(m_oTypeSpec.szName(0))) {
      vParseFV_Name();
      return;
    }
    
    if (m_oCurTkn.bSame("~"_Stz)) {
      vParseFV_Name();
      return;
    }
    
    if (m_oCurTkn.bSame("operator"_Stz)) {
      vGetNextToken();
      vParseFV_Operator();
      return;
    }
    
    NEVER_REACH
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // zero or more asterisks
  for (;;) {
    
    // collected when we have no more asterisks
    if (! m_oCurTkn.bSame("*"_Stz))
      break;
    
    // we have an asterisk
    m_oTypeSpec.vAdd(m_oCurTkn.szTerm());
    m_nAsterisks++;
  

    vGetNextToken();
  };

  // a possible &
  if (m_oCurTkn.bSame("&"_Stz))
    vGetNextToken();

  vTraceTypeSpec();
  vPartRecognised(CP_TYPE_SPECIFICATION);
  vParseFV_Name();
}

/*----------------------------------------------------------------------------*/
/* parse:  ... CLASS | STRUCT | UNION  ... { _^_ ... } [ ; ]                  */
/*----------------------------------------------------------------------------*/
void Vortex::vParseMembers(bool bSemiTerminated)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
    
  for (;;) {

    // block is terminated by } ;
    if (m_oCurTkn.bSame("}"_Stz)) {
      if (bSemiTerminated)
        vGetExpectedToken(";"_Stz);
      bool bMaybe = bGetNextToken();
      return;
    }
    
    if (bIgnoreOptLabel(m_oCurTkn.szTerm())) {
      FNENTRY
      continue;
    }
    
    vKeywordDescend();
    FNENTRY
  }
}


/*----------------------------------------------------------------------------*/
/* gather information about a function or variable name                       */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFV_Name(void)
{
  GATHER
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  m_bIsFunPtr = false;

  m_oFunVarNm.vReset();

  // do we have ( * <name> ) ( ...) ?
  if (m_oCurTkn.bSame("("_Stz)) {
    
    m_bIsFunPtr = true;
    
    vGetExpectedToken("*"_Stz);
    vGetNextToken();
    m_oFunVarNm.vAdd(m_oCurTkn.szTerm());
    vGetExpectedToken(")"_Stz);
    
    vGetExpectedToken("("_Stz);
    vCollectBalancedParenthesis(m_oOtherTerms);

    vPartRecognised(CP_FUNCTION_POINTER);
    
    vGetNextToken();
    
    vParseFV_AryFunVar();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  L_Parse_Function_Name:
  
  // do we have operator <name>
  if (m_oCurTkn.bSame("operator"_Stz)) {
    vGetNextToken();
    vParseFV_Operator();
    return;
  }
  else if (m_oCurTkn.bSame("~"_Stz)) {
    vGetNextToken();
    m_oFunVarNm.vAdd(m_oCurTkn.szTerm());
    vPartRecognised(CP_DESTRUCTOR);
    vGetNextToken();
    
    vParseFV_AryFunVar();
    return;
  }
  
  // we have a <name>
  m_oFunVarNm.vAdd(m_oCurTkn.szTerm());
  vTraceFunVarNm();
  vPartRecognised(CP_FUNVAR_NAME);

  vGetNextToken();
  
  // do we have type qualifier args?
  if (! m_oCurTkn.bSame("<"_Stz)) {
    vParseFV_AryFunVar();
    return;
  }
  
  vCollectTypeArgs(m_oTypeArgs);
  vPartRecognised(CP_TYPE_ARGS);
  
  vGetNextToken();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (! m_oCurTkn.bSame(":"_Stz)) {
    vParseFV_AryFunVar();
    return;
  }
  
  vGetExpectedToken(":"_Stz);
  
  vShiftClassNm();
  
  vGetNextToken();
  
  GATHER

  goto L_Parse_Function_Name;
}


/*----------------------------------------------------------------------------*/
/* detect parse next for array, function or variable                          */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFV_AryFunVar(void)
{
  GATHER
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (m_oCurTkn.bSame("["_Stz)) {
    vParseFV_Array();
    return;
  }
  
  if (m_oCurTkn.bSame(";"_Stz)) {
    vParseFV_Variable();
    return;
  }
  
  if (m_oCurTkn.bSame("("_Stz)) {
    vParseFV_FunBody();
    return;
  }
  
  if (m_oCurTkn.bSame(":"_Stz)) {
    
    vGetNextToken();
    if (m_oCurTkn.bSame(":"_Stz)) {
      vGetNextToken();
      vParseFV_Name();
      return;
    }
    
    // detected a field
    
    vParseFV_Field();
    return;
  }

  
  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* parse array                                                                */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFV_Array(void)
{
  GATHER
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_bIsArray = false;
  
  if (! m_oCurTkn.bSame("["_Stz))
    return;
  
  m_bIsArray = true;
  
  
  L_Dimension:
  
  for (;;) {
     
    vGetNextToken();
     
     if (m_oCurTkn.bSame("]"_Stz))
       break;
     
    m_oArraySpec.vAdd(m_oCurTkn.szTerm());
  }

  vGetNextToken();
  if (m_oCurTkn.bSame("["_Stz))
    goto L_Dimension;
  
  CHECKED(m_oCurTkn.bSame(";"_Stz));
  
  vPartRecognised(CP_ARRAY_SPEC);

  vPartRecognised(CP_MEMBER_VARIABLE);
  
  m_oTypeSpec.vReset();
  m_oFunVarNm.vReset();
  
  vGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* gather information about a function or variable name                       */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFV_Constructor(void)
{
  GATHER
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  m_bIsFunPtr = false;

  m_oFunVarNm.vAdd(m_sClassNm);

  vParseFV_FunBody();
}

/*----------------------------------------------------------------------------*/
/* gather information about a function or variable name                       */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFV_Field(void)
{
  GATHER
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  vGetExpectedToken(";"_Stz);
  
  vPartRecognised(CP_MEMBER_VARIABLE);

  vGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse function body                                                        */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFV_FunBody(void)
{
  GATHER
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_bIsFun = true;
  m_oFunSpec.vReset();

  m_bIsFun = true;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (! m_oCurTkn.bSame(")"_Stz)) {
    i8 nParenthesis = 1;
    for (;;) {
      
      vGetNextToken();
      
      // test for close
      if (m_oCurTkn.bSame(")"_Stz) && 0 == --nParenthesis)
        break;
      
      if (m_oCurTkn.bSame("("_Stz))
        nParenthesis++;
      
      m_oFunSpec.vAdd(m_oCurTkn.szTerm());
    }
  }
  
  vGetNextToken();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (m_oCurTkn.bSame("noexcept"_Stz)) {

   vGetExpectedToken("("_Stz);
    
   vGetNextToken();

   vGetExpectedToken(")"_Stz);
   
   vGetNextToken();

  }
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (m_oCurTkn.bSame(":"_Stz)) {

    for (;;) {
       
      vGetNextToken();
       
       // test for close
       if (m_oCurTkn.bSame("{"_Stz))
         break;
       
      // ignore term
    }

  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  //vParseFV_FunBody();
  
  if (m_oCurTkn.bSame("{"_Stz)) {

    vDiscardBalancedBraces();
  
    vPartRecognised(CP_FUNCTION_BODY);
  }
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vPartRecognised(CP__CRUDELY__);
  
  bool bMayBe = bGetNextToken();


  m_oTypeSpec.vReset();
  m_oFunVarNm.vReset();
}

/*----------------------------------------------------------------------------*/
/* parse function operator                                                    */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFV_Operator(void)
{
  GATHER
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  for (;;) {
    vGetNextToken();
    
    if (m_oCurTkn.bSame("("_Stz))
      break;
  }
  
  vParseFV_FunBody();
}

/*----------------------------------------------------------------------------*/
/* parse variable                                                             */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFV_Variable(void)
{
  GATHER
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (m_bInClass) {
    vTraceTypeSpec();
    vTraceFunVarNm();
    vPartRecognised(CP_MEMBER_VARIABLE);
  }
  else
    vPartRecognised(CP__CRUDELY__);

  m_oTypeSpec.vReset();
  m_oFunVarNm.vReset();
  
  vGetNextToken();
}


/*----------------------------------------------------------------------------*/
/* shift funvar name etc to be class                                          */
/*----------------------------------------------------------------------------*/
void Vortex::vShiftClassNm(void)
{
  FN
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_sClassNm = m_oFunVarNm.szName(0);
  
  m_oClassArgs.vCopy(&m_oTypeArgs);
  
  m_oFunVarNm.vReset();
  m_oTypeArgs.vReset();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* collect delimited tokens (potentially nested)                              */
/*----------------------------------------------------------------------------*/
void Vortex::vCollectBalanced(EnameSet& roTerms, Stz* szOpen, Stz* szClose)
{
  FN
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
         
  for (;;) {
    
    vGetNextToken();
    roTerms.vAdd(m_oCurTkn.szTerm());
    
    // test for close
    if (m_oCurTkn.bSame(szClose))
      return;
    
    // test for nested open
    if (m_oCurTkn.bSame(szOpen))
      vCollectBalanced(roTerms,szOpen,szClose);
      
    // ignore any other token
  }
  
}

/*----------------------------------------------------------------------------*/
/* collect balanced angle brackets pair: < ... >                              */
/*----------------------------------------------------------------------------*/
void Vortex::vCollectBalancedAngles(EnameSet& roTerms)
{
  FN
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vCollectBalanced(roTerms,"<"_Stz,">"_Stz);
}

/*----------------------------------------------------------------------------*/
/* collect balanced curly braces pair: { ... }                                */
/*----------------------------------------------------------------------------*/
void Vortex::vCollectBalancedBraces(EnameSet& roTerms)
{
  FN
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vCollectBalanced(roTerms,"{"_Stz,"}"_Stz);
}

/*----------------------------------------------------------------------------*/
/* collect balanced parenthesis pair: ( ... )                                 */
/*----------------------------------------------------------------------------*/
void Vortex::vCollectBalancedParenthesis(EnameSet& roTerms)
{
  FN
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vCollectBalanced(roTerms,"("_Stz,")"_Stz);
}

/*----------------------------------------------------------------------------*/
/* collect other terms: <other> ... | ; | { ... }                             */
/*----------------------------------------------------------------------------*/
void Vortex::vCollectRemainingTerms(EnameSet& roTerms)
{
  FN
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  //RASSERT(0 == m_oOtherTerms.nItems());
  
  roTerms.vReset();
  
  for (;;) {
    
    vGetNextToken();
    roTerms.vAdd(m_oCurTkn.szTerm());

    // test for terminating ;
    if (m_oCurTkn.bSame(";"_Stz))
      return;
    
    // test for terminating brace pair
    if (m_oCurTkn.bSame("{"_Stz)) {
      vCollectBalancedBraces(m_oOtherTerms);
      return;
    }
    
    // ignore any other token
  }

}

/*----------------------------------------------------------------------------*/
/* collect all template parameters : TEMPLATE  < _^_ ... >                    */
/*----------------------------------------------------------------------------*/
void Vortex::vCollectTemplateSpec(EnameSet& roTplSepc)
{
  FN
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_oCurTkn.bSame("<"_Stz));

  roTplSepc.vReset();
      
  // collect ... >
  while (bGetNextToken()) {
    if (m_oCurTkn.bSame(">"_Stz))
      break;
    roTplSepc.vAdd(m_oCurTkn.szTerm());
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s48K sTerms;
  roTplSepc.vNamesString(sTerms);
  
  m_sTplSpec = "<"_Stz;
  m_sTplSpec.vConcat(sTerms);
  m_sTplSpec.vConcat("> "_Stz);
}

/*----------------------------------------------------------------------------*/
/* collect all template arguments : < _^_ ... >                               */
/*----------------------------------------------------------------------------*/
void Vortex::vCollectTypeArgs(EnameSet& roTypeArgs)
{
  FN
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_oCurTkn.bSame("<"_Stz));

  roTypeArgs.vReset();
      
  // collect ... >
  while (bGetNextToken()) {
    if (m_oCurTkn.bSame(">"_Stz))
      break;
    roTypeArgs.vAdd(m_oCurTkn.szTerm());
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s48K sTerms;
  roTypeArgs.vNamesString(sTerms);
  
  TRACE("  TplArgs: %s\n"_Stz,sTerms.azChar());
}

/*----------------------------------------------------------------------------*/
/* discard delimited tokens (potentially nested)                              */
/*----------------------------------------------------------------------------*/
void Vortex::vDiscardBalanced(Stz* szOpen, Stz* szClose)
{
  FN
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
         
  for (;;) {
    
    vGetNextToken();
    
    // test for close
    if (m_oCurTkn.bSame(szClose))
      return;
    
    // test for nested open
    if (m_oCurTkn.bSame(szOpen))
      vDiscardBalanced(szOpen,szClose);
      
    // ignore any other token
  }
  
}

/*----------------------------------------------------------------------------*/
/* discard balanced angle brackets pair: < ... >                              */
/*----------------------------------------------------------------------------*/
void Vortex::vDiscardBalancedAngles(void)
{
  FN
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vDiscardBalanced("<"_Stz,">"_Stz);
}

/*----------------------------------------------------------------------------*/
/* discard balanced curly braces pair: { ... }                                */
/*----------------------------------------------------------------------------*/
void Vortex::vDiscardBalancedBraces(void)
{
  FN
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vDiscardBalanced("{"_Stz,"}"_Stz);
}

/*----------------------------------------------------------------------------*/
/* discard balanced parenthesis pair: ( ... )                                 */
/*----------------------------------------------------------------------------*/
void Vortex::vDiscardBalancedParenthesis(void)
{
  FN
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vDiscardBalanced("("_Stz,")"_Stz);
}

/*----------------------------------------------------------------------------*/
/* discard inline tokens: ... INLINE { ... }                                  */
/*----------------------------------------------------------------------------*/
void Vortex::vDiscardInlineTokens(void)
{
  FN
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  for (;;) {
    
   vGetNextToken();
    
    // test for terminating ;
    if (m_oCurTkn.bSame(";"_Stz))
      return;
    
    // test for terminating brace pair
    if (m_oCurTkn.bSame("{"_Stz)) {
      vDiscardBalancedBraces();
      return;
    }
    
    // ignore any other token
  }

}

/*----------------------------------------------------------------------------*/
/* discard static tokens: ... STATIC ... | ; | { ... } ;                      */
/*----------------------------------------------------------------------------*/
void Vortex::vDiscardStaticTokens(void)
{
  FN
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  for (;;) {
    
   vGetNextToken();
    
    // test for terminating ;
    if (m_oCurTkn.bSame(";"_Stz))
      return;
    
    // test for terminating brace pair
    if (m_oCurTkn.bSame("{"_Stz)) {
      vDiscardBalancedBraces();
      vGetExpectedToken(";"_Stz);
      return;
    }
    
    // ignore any other token
  }

}

/*----------------------------------------------------------------------------*/
/* discard terms till given term                                              */
/*----------------------------------------------------------------------------*/
void Vortex::vDiscardTill(Stz* szTerm, Stz* szTill)
{
  FN
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
    
  TRACE("Skipping: %s "_Stz,szTerm->azChar());
  if (szTerm->bSame(szTill)) {
    TRACE("\n"_Stz,szTerm->azChar());
    return;
  }
        
  while (bGetNextToken()) {
    TRACE("%s "_Stz,m_oCurTkn.azTerm());
    if (m_oCurTkn.bSame(szTill)) {
      TRACE("\n"_Stz,m_oCurTkn.azTerm());
      return;
    }
  }
}


/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                                meta type                                   */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */


/*----------------------------------------------------------------------------*/
/* create new member of a c++ class                                           */
/*----------------------------------------------------------------------------*/
void Vortex::vMemberNew(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* create new meta type for a c++ class                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vTypeNew_class(Stz* szClassNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (m_oTplSpec.nItems()) {
    m_p_MetaType = new MetaType(szClassNm,
                                MT_TEMPLATE,
                                CP_TEMPLATE_CLASS,
                                m_p_MetaType);
    
    m_p_MetaType->vTplParams(&m_oTplSpec);
  }
  else {
    m_p_MetaType = new MetaType(szClassNm,
                                MT_LINEAR,
                                CP_CLASS_BEGIN,
                                m_p_MetaType);
  }
  
  //if (m_bContType)
  //  getc(stdin);
}

/*----------------------------------------------------------------------------*/
/* create new meta type for a c++ struct                                      */
/*----------------------------------------------------------------------------*/
void Vortex::vTypeNew_struct(Stz* szStructNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_p_MetaType = new MetaType(szStructNm,
                              MT_LINEAR,
                              CP_STRUCT,
                              m_p_MetaType);
  
//if (m_bContType)
//  getc(stdin);
}

/*----------------------------------------------------------------------------*/
/* create new meta type for a c++ union                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vTypeNew_union(Stz* szUnionNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_p_MetaType = new MetaType(szUnionNm,
                              MT_UNION,
                              CP_UNION,
                              m_p_MetaType);
//if (m_bContType)
//  getc(stdin);
}

/*----------------------------------------------------------------------------*/
/* pop to outer type                                                          */
/*----------------------------------------------------------------------------*/
void Vortex::vTypePop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_p_MetaType);
  
  m_p_MetaType = m_p_MetaType->p_OuterType();
}

/*----------------------------------------------------------------------------*/
/* create and push new type                                                   */
/*----------------------------------------------------------------------------*/
void Vortex::vTypePush(Stz* szTypeNm, eMT eStructure, eCP eCppPart)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_pMetaGraph);
  
  if (m_p_MetaType) {
    // creating a new nested type (created inside current type)
    m_p_MetaType = new MetaType(szTypeNm,
                                eStructure,
                                eCppPart,
                                m_p_MetaType);
  }
  else {
    // creating a new top-level type (not nested)
    m_p_MetaType = new MetaType(szTypeNm,
                                eStructure,
                                eCppPart,
                                nullptr);
    
    m_pMetaGraph->vAddMetaType(m_p_MetaType);
  }
}

/*----------------------------------------------------------------------------*/
/* skip a label i.e. private|protected|public :                               */
/*----------------------------------------------------------------------------*/
bool Vortex::bIgnoreOptLabel(Stz* szSawTerm)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
    
  if (szSawTerm->bSame("private"_Stz)) {
    CHECKED(bGetNextToken());
    CHECKED(m_oCurTkn.bSame(":"_Stz));
    vPartRecognised(CP_LABEL);
    vGetNextToken();
    return true;
  }
  
  if (szSawTerm->bSame("protected"_Stz)) {
    CHECKED(bGetNextToken());
    CHECKED(m_oCurTkn.bSame(":"_Stz));
    vPartRecognised(CP_LABEL);
    vGetNextToken();
    return true;
  }
  
  if (szSawTerm->bSame("public"_Stz)) {
    CHECKED(bGetNextToken());
    CHECKED(m_oCurTkn.bSame(":"_Stz));
    vPartRecognised(CP_LABEL);
    vGetNextToken();
    return true;
  }
  
  return false;
}




/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                                 machinary                                  */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* get keystroke to continue again                                            */
/*----------------------------------------------------------------------------*/
bool Vortex::bContinueAgain(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  char c = getc(stdin);
  
  if ('.' == c)
    return false;
  else
    return true;
}

/*----------------------------------------------------------------------------*/
/* recognised c++ part                                                        */
/*----------------------------------------------------------------------------*/
void Vortex::vPartRecognised(eCP eCppPart)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (m_bTrace)
    TRACE("Recognised %s\n\n"_Stz,aCP[eCppPart]);
  
  if (m_bContPart)
    m_bContPart = bContinueAgain();
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
/* trace function/variable name terms                                         */
/*----------------------------------------------------------------------------*/
void Vortex::vTraceFunVarNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (!m_bTrace)
    return;
  
  s48K sTerms;
  m_oFunVarNm.vNamesString(sTerms);
  
  TRACE("  FunVarNm: %s\n"_Stz,sTerms.azChar());
}

/*----------------------------------------------------------------------------*/
/* trace namespace name                                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vTraceNamespaceNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (!m_bTrace)
    return;
  
  TRACE("  NamespaceNm: %s\n"_Stz,m_sNamespaceNm.szStz());
}

/*----------------------------------------------------------------------------*/
/* trace type args                                                            */
/*----------------------------------------------------------------------------*/
void Vortex::vTraceTypeArgs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (!m_bTrace)
    return;
  
  s48K sTerms;
  m_oTypeArgs.vNamesString(sTerms);
  
  TRACE("  TypeArgs: %s\n"_Stz,sTerms.azChar());
}

/*----------------------------------------------------------------------------*/
/* trace type specification terms                                             */
/*----------------------------------------------------------------------------*/
void Vortex::vTraceTypeSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (!m_bTrace)
    return;
  
  s48K sTerms;
  m_oTypeSpec.vNamesString(sTerms);
  
  TRACE("  TypeSpec: %s\n"_Stz,sTerms.azChar());
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
