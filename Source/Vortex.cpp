/*============================================================================*/
/* Vortex.cpp - Vortex                                                        */
/*============================================================================*/

#include "Utilities.h"

#define TRACE(fmt, ...) if (m_bTrace) vTrace(fmt, ##__VA_ARGS__)

#define FNENTRY \
  TRACE("/%s/%16s: TERM `%s` [[ %s%s%s %s%s%s %s%s%s]]\n"_Stz \
    \
    ,  m_sSourcePath.szStz() \
    \
    , __func__ \
    \
    , m_oCurTkn.szTerm() \
    \
    , m_bInNamespace ? "NAMESPACE `" : "" \
    , m_bInNamespace ? m_sNamespaceNm.azChar() : "" \
    , m_bInNamespace ? "` " : "" \
    \
    , m_bInClass     ? "CLASS `" : "" \
    , m_bInClass     ? m_sClassNm.azChar() : "" \
    , m_bInClass     ? "` " : "" \
    \
    , m_bInTemplate  ? "TPLSPEC `" : "" \
    , m_bInTemplate  ? m_sTplSpec.azChar() : "" \
    , m_bInTemplate  ? "` " : "" \
    \
    );

#define GATHER \
  TRACE("/%s/%16s: TERM `%s` [[ %s%s%s %s%s%s %s%s%s]]\n"_Stz \
    \
    ,  m_sSourcePath.szStz() \
    \
    , __func__ \
    \
    , m_oCurTkn.szTerm() \
    \
    , m_bInNamespace ? "NAMESPACE `" : "" \
    , m_bInNamespace ? m_sNamespaceNm.azChar() : "" \
    , m_bInNamespace ? "` " : "" \
    \
    , m_bInClass     ? "CLASS `" : "" \
    , m_bInClass     ? m_sClassNm.azChar() : "" \
    , m_bInClass     ? "` " : "" \
    \
    , m_bInTemplate  ? "TPLSPEC `" : "" \
    , m_bInTemplate  ? m_sTplSpec.azChar() : "" \
    , m_bInTemplate  ? "` " : "" \
    \
    );

#define FN \
  TRACE("/%s/%16s\n"_Stz \
    \
    ,  m_sSourcePath.szStz() \
    \
    , __func__ \
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
  m_bPrint       = true;
  m_bTrace       = true;
  m_bContMember  = true;
  m_bContPart    = true;
  m_bContType    = true;

  // output
  
  m_pOutputFile  = nullptr;
  m_pMetaGraph   = new MetaGraph;
  
  // working
  
  m_oCurTkn.vReset();
  
  m_sFragment    = ""_Stz;
  
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
  
  bool b = bParseAllParts();
  
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
bool Vortex::bParseAllParts(void)
{
  TRACE("/%s/%16s: [[%s%s%s %s%s%s %s%s%s]]\n"_Stz
        ,  m_sSourcePath.szStz()
        , __func__ \
        
        , m_bInNamespace ? "NAMESPACE`" : ""
        , m_bInNamespace ? m_sNamespaceNm.azChar() : ""
        , m_bInNamespace ? "` " : ""
        
        , m_bInClass     ? "CLASS`" : ""
        , m_bInClass     ? m_sClassNm.azChar() : ""
        , m_bInClass     ? "` " : ""
        
        , m_bInTemplate  ? "TPLSPEC`" : ""
        , m_bInTemplate  ? m_sTplSpec.azChar() : ""
        , m_bInTemplate  ? "` " : ""
        );
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  RASSERT(! m_bInNamespace);
  
  // -- parse till EOF --
  while (bGetNextToken(m_oCurTkn)) {
    
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  if (m_oCurTkn.bSame("class"_Stz))     vParseClass();     else
  if (m_oCurTkn.bSame("const"_Stz))     vParseConst();     else
  if (m_oCurTkn.bSame("enum"_Stz))      vParseEnum();      else
  if (m_oCurTkn.bSame("friend"_Stz))    vParseFriend();    else
  if (m_oCurTkn.bSame("#"_Stz))         vParseHash();      else
  if (m_oCurTkn.bSame("namespace"_Stz)) vParseNamespace(); else
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

  RASSERT(m_oCurTkn.bSame(":"_Stz));
  RASSERT(m_bInClass);
  
  vDebugPoint();
   
  while (bGetNextToken(m_oCurTkn)) {
     
    Stz* szTerm = m_oCurTkn.szTerm();
     
    // ignore optional visibility
    if (szTerm->bOneOf("private"_Stz,"protected"_Stz,"public"_Stz,nullptr))
      vGetNextToken(m_oCurTkn);

    // record the name of the inherited class
    Stz* szInheritedClassNm = m_oCurTkn.szTerm();

    // add inherited type to current type
    MetaInherit* pInherit = m_p_MetaType->pAddInherit(szInheritedClassNm);
    vPartRecognised(CP_INHERITED_CLASS);

    vGetNextToken(m_oCurTkn);

    // collect optional template arguments <...>
    if (szTerm->bSame("<"_Stz)) {
     
      vCollectTplArgs("<"_Stz);
      pInherit->vTplArgs(&m_oTypeArgs);
      m_oTypeArgs.vReset();
     
      vGetNextToken(m_oCurTkn);
    }

    // base list is terminated by open brace, being the beginning of members
    if (szTerm->bSame("{"_Stz)) {
      vPartRecognised(CP_BASE_END);
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
  
  vGetNextToken(m_oCurTkn);
  s32 sClassNm = m_oCurTkn.szTerm();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get ; or : or {
  
  vGetNextToken(m_oCurTkn);
    
  // forward declaration of a class?
  if (m_oCurTkn.bSame(";"_Stz)) {
    vPartRecognised(CP_FORWARD);
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vTypePush(sClassNm,MT_LINEAR,CP_CLASS_BEGIN);
  vClassDescend(sClassNm);
  
  if (m_oTplSpec.nItems()) {
    m_p_MetaType->vTplParams(& m_oTplSpec);
    m_oTplSpec.vReset();
  }

  if (m_oCurTkn.bSame(":"_Stz)) {
    vTraceClassNm();
    vPartRecognised(CP_CLASS_BEGIN);
    vPartRecognised(CP_BASE_BEGIN);
    vParseBaseList();
  }
  else
  if (m_oCurTkn.bSame("{"_Stz)) {
    vTraceClassNm();
    vPartRecognised(CP_CLASS_BEGIN);
    vPartRecognised(CP_MEMBERS_BEGIN);
    vParseMembers(true);
  }
  else {
    vThrowProblem(__LINE__);
  }

  vTraceClassNm();
  vPartRecognised(CP_CLASS_END);
  
  vTypePop();
  vClassAscend();
}

/*----------------------------------------------------------------------------*/
/* parse: [STATIC] CONST _^_ ... ;                                            */
/*----------------------------------------------------------------------------*/
void Vortex::vParseConst(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("const"_Stz));
  RASSERT(0 == m_oOtherTerms.nItems());


  // get <type>
  vGetNextToken(m_oCurTkn);
  
  for (;;) {
    CHECKED(bGetNextToken(m_oCurTkn));
    if (! m_oCurTkn.bSame("*"_Stz))
      break;
  }
    
  // get <type>
  m_sConstNm = m_oCurTkn.szTerm();

  // get =
  vGetExpectedToken(m_oCurTkn,"="_Stz);
  
  // get ... ;
  vCollectOtherTerms("="_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  ConstDef* pConstDef = m_p_ConstSet->pAddConstDef(m_sConstNm);
  pConstDef->vTerms(m_oOtherTerms);

  vPartRecognised(CP_CONST);
  
  vOtherTermsReset();
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
}

/*----------------------------------------------------------------------------*/
/* parse: FRIEND _^_ ... ;                                                    */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFriend(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("friend"_Stz));
  
  vDiscardTill(m_oCurTkn.szTerm(),";"_Stz);
  
  vPartRecognised(CP_FRIEND);
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* parse: <other> _^_ <term> ...                                              */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFunVar(bool bTemplated)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
    
  vDescendFunVar();
  
}

/*----------------------------------------------------------------------------*/
/* parse: # _^_ DEFINE ... | IF ... | INCLUDE ... | PRAGMA ...                */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHash(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("#"_Stz));
    
  CHECKED(bGetNextToken(m_oCurTkn));
  
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
      
  vGetNextToken(m_oCurTkn);
  while (! m_oCurTkn.bSame("enddefine"_Stz)) {
    vGetNextToken(m_oCurTkn);
  }
      
  vPartRecognised(CP_HASH_DEFINE);
}

/*----------------------------------------------------------------------------*/
/* parse: # IF _^_ ... #ENDIF                                                 */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHashIf(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("if"_Stz));
    
  vGetNextToken(m_oCurTkn);
  while (! m_oCurTkn.bSame("endif"_Stz)) {
    vGetNextToken(m_oCurTkn);
  }
      
  vPartRecognised(CP_HASH_IF);
}

/*----------------------------------------------------------------------------*/
/* parse: # IF _^_ ... #ENDIF                                                 */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHashIfdef(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("ifdef"_Stz));
    
  vGetNextToken(m_oCurTkn);
  while (! m_oCurTkn.bSame("endif"_Stz)) {
    vGetNextToken(m_oCurTkn);
  }
      
  vPartRecognised(CP_HASH_IF);
}

/*----------------------------------------------------------------------------*/
/* parse: # INCLUDE _^_ <filePath>                                            */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHashInclude(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("include"_Stz));
  
  vGetNextToken(m_oCurTkn);
  
  if (m_oCurTkn.bSame("<"_Stz))
    vDiscardBalancedAngles("<"_Stz);
    
  vPartRecognised(CP_HASH_INCLUDE);
}

/*----------------------------------------------------------------------------*/
/* parse: # PRAGMA _^_ ONCE                                                   */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHashPragma(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("pragma"_Stz));
  
  vGetNextToken(m_oCurTkn);
  
  if (m_oCurTkn.bSame("once"_Stz)) {
    vPartRecognised(CP_HASH_PRAGMA_ONCE);
    return;
  }

  if (m_oCurTkn.bSame("pack"_Stz)) {
    vGetExpectedToken(m_oCurTkn,"("_Stz);
    vDiscardBalancedParenthesis("("_Stz);
    vPartRecognised(CP_HASH_PRAGMA_PACK);
    return;
  }

  vThrowProblem(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* parse:  ... CLASS ... { _^_ ... } [ ; ]                                    */
/*----------------------------------------------------------------------------*/
void Vortex::vParseMembers(bool bSemiTerminated)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_oCurTkn.bSame("{"_Stz));
  
  while (bGetNextToken(m_oCurTkn)) {

    Stz* szTerm = m_oCurTkn.szTerm();

    // block is terminated by } ;
    if (szTerm->bSame("}"_Stz)) {
      if (bSemiTerminated)
        vGetExpectedToken(m_oCurTkn,";"_Stz);
      vPartRecognised(CP_MEMBERS_END);
      return;
    }
    
    if (bIgnoreOptLabel(m_oCurTkn.szTerm())) {
      FNENTRY
      continue;
    }
    
    vKeywordDescend();
    FNENTRY
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
  vGetNextToken(m_oCurTkn);
  
  vNamepaceDescend(m_oCurTkn.szTerm());
  
  // get {
  vGetExpectedToken(m_oCurTkn,"{"_Stz);

  vTraceNamespaceNm();
  vPartRecognised(CP_NAMESPACE_BEGIN);
  FNENTRY

  // parse ... }
  while (bGetNextToken(m_oCurTkn)) {
  
    // test for } being end-of-namespace
    if (m_oCurTkn.bSame("}"_Stz))  {
      vTraceNamespaceNm();
      vPartRecognised(CP_NAMESPACE_END);
      vNamepaceAscend();
      return;
    }
    
    // -- -- -- -- -- --
    
    Stz* szKeyWord = m_oCurTkn.szTerm();
    
    vKeywordDescend();
    
    FNENTRY
    
  } // while more tokens
  
  vThrowProblem(__LINE__);
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
}

/*----------------------------------------------------------------------------*/
/* parse: STATIC _^_ CONST ... | ...                                          */
/*----------------------------------------------------------------------------*/
void Vortex::vParseStatic(void)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_oCurTkn.bSame("static"_Stz));
    
  vGetNextToken(m_oCurTkn);

  if (m_oCurTkn.bSame("const"_Stz)) {
    vParseConst();
    return;
  }
  
  if (m_oCurTkn.bSame("inline"_Stz)) {
    vDiscardInlineTokens(m_oCurTkn.szTerm());
    return;
  }
  
  vDiscardStaticTokens(m_oCurTkn.szTerm());
  vPartRecognised(CP_STATIC);
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

  vGetNextToken(m_oCurTkn);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // scenarios 1), 2) and 3)
  
  if (m_oCurTkn.bSame("{"_Stz)) {
    
    sStructNm.vSprint("_STRUCT_%lld"_Stz,++m_iNextStruct);

    vTypePush(sStructNm,MT_LINEAR,CP_STRUCT);
    vParseMembers(false);
    vTypePop();

    // 1) struct { ... } ;
    vGetNextToken(m_oCurTkn);
    if (m_oCurTkn.bSame(";"_Stz))
      return;
    
    // <#> handle member name in xTkn
    
    // 2) struct { ... } <memberNm> ;
    vGetNextToken(m_oCurTkn);
    if (m_oCurTkn.bSame(";"_Stz))
      return;

    // 3) struct { ... } <memberNm> [ <constNm> ] ;
    CHECKED(m_oCurTkn.bSame("["_Stz));
    vGetNextToken(m_oCurTkn);
    vGetExpectedToken(m_oCurTkn,"]"_Stz);
    vGetExpectedToken(m_oCurTkn,";"_Stz);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // scenarios 4) and 5)
  
  // <#> handle member name in xTkn

  sStructNm = m_oCurTkn.szTerm();
  
  // get ; or {
  vGetNextToken(m_oCurTkn);
  
  // 4) struct <blockNm> ;
  if (m_oCurTkn.bSame(";"_Stz))
    return;

  // 5) struct <blockNm> { ... } ;
  if (m_oCurTkn.bSame("{"_Stz)) {
    
    vTypePush(m_oCurTkn.szTerm(),MT_LINEAR,CP_STRUCT);
    vParseMembers(true);
    vTypePop();
    
    return;
  }
  
  // 6) struct <blockNm> <memberNm> ;
  vGetExpectedToken(m_oCurTkn,";"_Stz);

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
        
  vGetExpectedToken(m_oCurTkn,"<"_Stz);
  
  vCollectTplSpec("<"_Stz);
  vTemplateDescend();
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // class or other
  
  vGetNextToken(m_oCurTkn);
  
  if (m_oCurTkn.bSame("class"_Stz)) {
    vParseClass();
    vTemplateAscend();
    return;
  }
  
  vParseFunVar(true);
  vTemplateAscend();
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
  
  vGetNextToken(m_oCurTkn);
  if (m_oCurTkn.bSame("{"_Stz)) {
    // 1) union { ... } ;
    sUnionNm.vSprint("_UNION_%lld"_Stz,++m_iNextUnion);
  }
  else {
    // 2) union <unionNm> { ... } ;
    sUnionNm = m_oCurTkn.szTerm();
    vGetExpectedToken(m_oCurTkn,"{"_Stz);
  }
  
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
  vGetExpectedToken(m_oCurTkn,"namespace"_Stz);

  // get <name>
  vGetNextToken(m_oCurTkn);

  // get ;
  vGetExpectedToken(m_oCurTkn,";"_Stz);
  
  vPartRecognised(CP_USING);
}

/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                         function / variable gathering                      */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* descent parsing of function / variable                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vDescendFunVar(void)
{
  GATHER
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vDescendTypeSpec();

}

/*----------------------------------------------------------------------------*/
/* gather information about type spec for a function or variable              */
/*----------------------------------------------------------------------------*/
void Vortex::vDescendTypeSpec(void)
{
  GATHER
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_oTypeArgs.vReset();
  
  m_nAsterisks = 0;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // detect the case of no specifier (constructor / destructor)
  if (m_bInClass) {
    // 1) constructor (no specifier)
    if (m_oCurTkn.bSame(m_sClassNm)) {
      TRACE("  (typeless) constructor\n"_Stz);
      vDescendFunVarNm();
      return;
    }
    // 2) destructor (no specifier)
    if (m_oCurTkn.bSame("~"_Stz)) {
      TRACE("  (typeless) destructor\n"_Stz);
      vDescendFunVarNm();
      return;
    }
    // 3) typeless operator (no specifier)
    if (m_oCurTkn.bSame("operator"_Stz)) {
      TRACE("  (typeless) operator\n"_Stz);
      vDescendFunVarNm();
      return;
    }
  }
  
  // we have a <name>
  m_oTypeSpec.vAdd(m_oCurTkn.szTerm());
  vGetNextToken(m_oCurTkn);
  
  // do we have template arguments?
  if (m_oCurTkn.bSame("<"_Stz)) {
    vCollectTplArgs("<"_Stz);
    vGetNextToken(m_oCurTkn);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // no specifier (constructor / destructor) after  <classNm> <typeAgs> ::
  
  if (m_oCurTkn.bSame(":"_Stz)) {
    vGetExpectedToken(m_oCurTkn,":"_Stz);
    vGetNextToken(m_oCurTkn);

    if (m_oCurTkn.bSame(m_oTypeSpec.szName(0))) {
      vDescendFunVarNm();
      return;
    }
    
    if (m_oCurTkn.bSame("~"_Stz)) {
      vDescendFunVarNm();
      return;
    }
    
    if (m_oCurTkn.bSame("operator"_Stz)) {
      vDescendFunVarNm();
      return;
    }
    
    NEVER_REACH
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // zero or more asterisks
  do {
    
    // collected when we have no more asterisks
    if (! m_oCurTkn.bSame("*"_Stz)) {
      vTraceTypeSpec();
      vPartRecognised(CP_TYPE_SPECIFICATION);
      vDescendRework();
      return;
  }
  
    // we have an asterisk
    m_oTypeSpec.vAdd(m_oCurTkn.szTerm());
    m_nAsterisks++;

  }  while (bGetNextToken(m_oCurTkn));
  
  vThrowProblem(__LINE__);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* gather information about a function or variable name                       */
/*----------------------------------------------------------------------------*/
void Vortex::vDescendFunVarNm(void)
{
  GATHER
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  m_bIsFunPtr = false;

  m_oFunVarNm.vReset();

  // do we have ( * <name> ) ( ...) ?
  if (m_oCurTkn.bSame("("_Stz)) {
    
    m_bIsFunPtr = true;
    
    vGetExpectedToken(m_oCurTkn,"*"_Stz);
    vGetNextToken(m_oCurTkn);
    m_oFunVarNm.vAdd(m_oCurTkn.szTerm());
    vGetExpectedToken(m_oCurTkn,")"_Stz);
    
    vGetExpectedToken(m_oCurTkn,"("_Stz);
    vCollectBalancedParenthesis("("_Stz);

    vPartRecognised(CP_FUNCTION_POINTER);
    
    vGetNextToken(m_oCurTkn);
    
    vDescendRework();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  L_Parse_Function_Name:
  
  // do we have operator <name>
  if (m_oCurTkn.bSame("operator"_Stz)) {
    vGetNextToken(m_oCurTkn);
    m_oFunVarNm.vAdd(m_oCurTkn.szTerm());
    vPartRecognised(CP_OPERATOR);
    vGetNextToken(m_oCurTkn);
    
    vDescendRework();
    return;
  }
  else if (m_oCurTkn.bSame("~"_Stz)) {
    vGetNextToken(m_oCurTkn);
    m_oFunVarNm.vAdd(m_oCurTkn.szTerm());
    vPartRecognised(CP_DESTRUCTOR);
    vGetNextToken(m_oCurTkn);
    
    vDescendRework();
    return;
  }
  
  // we have a <name>
  m_oFunVarNm.vAdd(m_oCurTkn.szTerm());
  vTraceFunVarNm();
  vPartRecognised(CP_FUNVAR_NAME);

  vGetNextToken(m_oCurTkn);
  
  // do we have type qualifier args?
  if (! m_oCurTkn.bSame("<"_Stz)) {
    vDescendRework();
    return;
  }
  
  vCollectTplArgs("<"_Stz);
  vPartRecognised(CP_TYPE_ARGS);
  
  vGetNextToken(m_oCurTkn);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (! m_oCurTkn.bSame(":"_Stz)) {
    vDescendRework();
    return;
  }
  
  vGetExpectedToken(m_oCurTkn,":"_Stz);
  
  vShiftClassNm();
  
  vGetNextToken(m_oCurTkn);
  
  GATHER

  goto L_Parse_Function_Name;
}


/*----------------------------------------------------------------------------*/
/* reworking...                                                               */
/*----------------------------------------------------------------------------*/
void Vortex::vDescendRework(void)
{
  GATHER
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (m_oCurTkn.bSame(";"_Stz)) {
    if (m_bInClass) {
      vTraceTypeSpec();
      vTraceFunVarNm();
      vPartRecognised(CP_MEMBER_VARIABLE);
    }
    else
      vPartRecognised(CP__CRUDELY__);

    m_oTypeSpec.vReset();
    m_oFunVarNm.vReset();

    return;
  }
  
  if (m_oCurTkn.bSame("["_Stz)) {
    vDescendArraySpec();
    vPartRecognised(CP_MEMBER_VARIABLE);
    
    m_oTypeSpec.vReset();
    m_oFunVarNm.vReset();
    return;
  }
  
  vDescendFunSpec();
  vDescendFunBody();

  vPartRecognised(CP__CRUDELY__);
  
  m_oTypeSpec.vReset();
  m_oFunVarNm.vReset();

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
/* collect array specification: ????                                          */
/*----------------------------------------------------------------------------*/
void Vortex::vDescendArraySpec(void)
{
  GATHER
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
    
  m_bIsArray = false;
  
  if (! m_oCurTkn.bSame("["_Stz))
    return;
  
  m_bIsArray = true;
  
  vGetNextToken(m_oCurTkn);

  m_oArraySpec.vAdd(m_oCurTkn.szTerm());
  
  vGetExpectedToken(m_oCurTkn,"]"_Stz);

  vPartRecognised(CP_ARRAY_SPEC);
}

/*----------------------------------------------------------------------------*/
/* collect function body: ????                                                */
/*----------------------------------------------------------------------------*/
void Vortex::vDescendFunBody(void)
{
  GATHER
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
    
  if (! m_oCurTkn.bSame("{"_Stz))
    return;

  vDiscardBalancedBraces("{"_Stz);
  
  vPartRecognised(CP_FUNCTION_BODY);
}

/*----------------------------------------------------------------------------*/
/* collect function spec: ????                                                */
/*----------------------------------------------------------------------------*/
void Vortex::vDescendFunSpec(void)
{
  GATHER
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_bIsFun = true;
  m_oFunSpec.vReset();
  
  if (! m_oCurTkn.bSame("("_Stz))
    return;
  
  m_bIsFun = true;
  
  for (;;) {
     
    vGetNextToken(m_oCurTkn);
     
     // test for close
     if (m_oCurTkn.bSame(")"_Stz))
       break;
     
    m_oFunSpec.vAdd(m_oCurTkn.szTerm());
  }

  
  vGetNextToken(m_oCurTkn);

  vDescendNoExceptSpec();
}

/*----------------------------------------------------------------------------*/
/* collect noexcept terms: ????                                               */
/*----------------------------------------------------------------------------*/
void Vortex::vDescendNoExceptSpec(void)
{
  GATHER
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (! m_oCurTkn.bSame("("_Stz))
    return;

  vGetNextToken(m_oCurTkn);

  vGetExpectedToken(m_oCurTkn,")"_Stz);
  
  vGetNextToken(m_oCurTkn);
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
/* collect all template arguments : < _^_ ... >                               */
/*----------------------------------------------------------------------------*/
void Vortex::vCollectTplArgs(Stz* szSawTerm)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_oCurTkn.bSame("<"_Stz));

  m_oTypeArgs.vReset();
    
  Token xTkn;
  
  // collect ... >
  while (bGetNextToken(xTkn)) {
    if (xTkn.bSame(">"_Stz))
      break;
    m_oTypeArgs.vAdd(xTkn.szTerm());
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s48K sTerms;
  m_oTypeArgs.vNamesString(sTerms);
  
  TRACE("  TplArgs: %s\n"_Stz,sTerms.azChar());
}

/*----------------------------------------------------------------------------*/
/* collect all template parameters : TEMPLATE  < _^_ ... >                    */
/*----------------------------------------------------------------------------*/
void Vortex::vCollectTplSpec(Stz* szSawTerm)
{
  FNENTRY
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_oCurTkn.bSame("<"_Stz));

  m_oTplSpec.vReset();
    
  Token xTkn;
  
  // collect ... >
  while (bGetNextToken(xTkn)) {
    if (xTkn.bSame(">"_Stz))
      break;
    m_oTplSpec.vAdd(xTkn.szTerm());
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s48K sTerms;
  m_oTplSpec.vNamesString(sTerms);
  
  m_sTplSpec = "<"_Stz;
  m_sTplSpec.vConcat(sTerms);
  m_sTplSpec.vConcat("> "_Stz);
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

/*----------------------------------------------------------------------------*/
/* discard terms till given term                                              */
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

  m_p_MetaType  = nullptr;
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
/* append sub-string to fragment string                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vFragmentAppend(Stz* szString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (m_sFragment.nLength())
    m_sFragment.vAppend(' ');
  
  m_sFragment.vConcat(szString);
  
  if (m_bTrace)
    TRACE("  Got `%s`\n"_Stz,m_sFragment.azChar());
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
    TRACE("  Fragment: %s\n"_Stz,m_sFragment.azChar());
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
    TRACE("Recognised %s\n\n"_Stz,aCP[eCppPart]);
  
  if (m_bContPart)
    m_bContPart = bContinueAgain();

  vFragmentReset();
}

/*----------------------------------------------------------------------------*/
/* ascend from a template                                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vTemplateAscend(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_bInTemplate);

  m_bInTemplate = false;
  m_sTplSpec    = ""_Stz;
}

/*----------------------------------------------------------------------------*/
/* descend into template                                                      */
/*----------------------------------------------------------------------------*/
void Vortex::vTemplateDescend(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(! m_bInTemplate);
  
  m_bInTemplate = true;
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
/* trace class name                                                           */
/*----------------------------------------------------------------------------*/
void Vortex::vTraceClassNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (!m_bTrace)
    return;
  
  TRACE("  ClassNm: %s\n"_Stz,m_sClassNm.szStz());
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


#if 0
/*----------------------------------------------------------------------------*/
/* collect terms for function intro:                                          */
/*----------------------------------------------------------------------------*/
bool Vortex::bCollectFunctionTerms(Stz* szSawTerm, Token& roFunNm, eFN& reFun)
{
  FNENTRY
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

