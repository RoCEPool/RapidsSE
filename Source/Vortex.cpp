/*============================================================================*/
/* Vortex.cpp - Vortex                                                        */
/*============================================================================*/

#include "Utilities.h"

#define TRACE(fmt, ...) if (m_bTrace) vTrace(fmt, ##__VA_ARGS__)

#define CHECKED(fun)    if (! fun) vThrowProblem(__LINE__);


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
  m_pInput   = nullptr;
  m_oCurTkn.vReset();
  m_bEOF         = false;
  
  // tracing and debugging

  m_bPrint       = true;
  m_bTrace       = true;
  m_bContMember  = false;
  m_bContPart    = true;
  m_bContType    = false;

  // output
  
  m_pOutputFile  = nullptr;
  m_pMetaGraph   = new MetaGraph;
  
  m_p_ConstSet   = m_pMetaGraph->pConstSet();
  m_p_MetaType   = nullptr;
  m_p_MetaMember = nullptr;

  // working
  
  for (i8 i_Saw = 0; i_Saw < VP_ENUMS; i_Saw++)
    m_baPart[i_Saw] = false;

  m_sConstNm     = C_NO_NAME;
  m_oConstTerms.vReset();
  
  m_bInNamespace = false;
  m_sNamespaceNm = C_NO_NAME;
  
  m_bInTemplate  = false;
  m_oTplSpec.vReset();
  
  m_sTplSpec     = C_NO_NAME;
  m_bInClass     = false;
  m_sClassNm     = C_NO_NAME;
  
  m_iNextStruct  = 0;
  m_iNextUnion   = 0;

  m_sTypeNm      = C_NO_NAME;

  m_oTypeSpec.vReset();
  m_nAsterisks = 0;
  m_oFunVarNm.vReset();
  m_bIsFunPtr  = false;
  m_nFieldBits = 0;
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
    
  m_pInput->vPushCharser();
  bool b = m_pInput->bUseFile(szSourcePath);

  m_pInput->vUseBegin();

  b = bGetNextToken();
  
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
/* recognise a const                                                          */
/*----------------------------------------------------------------------------*/
void Vortex::vRecogniseConst(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  ConstDef* pConstDef = m_p_ConstSet->pAddConstDef(m_sConstNm);
  pConstDef->vTerms(m_oConstTerms);
  
  vTraceConstant();
}

/*----------------------------------------------------------------------------*/
/* recognise a function                                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vRecogniseFunction(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  if (m_bInClass) {
    vTraceFunction();
    // not recording functions
  }
  else {
    vTraceFunction();
  // not recording functions
  }
}


/*----------------------------------------------------------------------------*/
/* recognise a function                                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vRecogniseStructure(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vTraceStructure();
}

/*----------------------------------------------------------------------------*/
/* recognise a variable                                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vRecogniseVariable(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (m_bInClass) {
    vTraceVariable();
  }
  else {
    vTraceVariable();
  }
  
  m_oTypeSpec.vReset();
  m_oFunVarNm.vReset();
  

}


/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                            top-down parsing                                */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* parse all parts in the current source file (.h or .hpp)                    */
/*----------------------------------------------------------------------------*/
bool Vortex::bParseAllParts(void)
{
  vTraceEnter((char*)__func__);
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  RASSERT(! m_bInNamespace);
  
  // -- parse till EOF --
  while (! m_bEOF) {
    
    try {
      vKeywordDescend();
      
      vTraceEnter((char*)__func__);
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
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  if (m_oCurTkn.bSame("class"_Stz))     vParseClass();      else
  if (m_oCurTkn.bSame("const"_Stz))     vParseConst();      else
  if (m_oCurTkn.bSame("enum"_Stz))      vParseEnum();       else
  if (m_oCurTkn.bSame("extern"_Stz))    vParseExtern();     else
  if (m_oCurTkn.bSame("friend"_Stz))    vParseFriend();     else
  if (m_oCurTkn.bSame("#"_Stz))         vParseHash();       else
  if (m_oCurTkn.bSame("inline"_Stz))    vParseInline();     else
  if (m_oCurTkn.bSame("namespace"_Stz)) vParseNamespace();  else
  if (m_oCurTkn.bSame("[["_Stz))        vParseOpenSquare(); else
  if (m_oCurTkn.bSame("public"_Stz))    vParsePublic();     else
  if (m_oCurTkn.bSame(";"_Stz))         vParseSemicolon();  else
  if (m_oCurTkn.bSame("static"_Stz))    vParseStatic();     else
  if (m_oCurTkn.bSame("struct"_Stz))    vParseStruct();     else
  if (m_oCurTkn.bSame("template"_Stz))  vParseTemplate();   else
  if (m_oCurTkn.bSame("typedef"_Stz))   vParseTypedef();    else
  if (m_oCurTkn.bSame("union"_Stz))     vParseUnion();      else
  if (m_oCurTkn.bSame("unsigned"_Stz))  vParseUnsigned();   else
  if (m_oCurTkn.bSame("using"_Stz))     vParseUsing();
  else                                  vParseVarFun();
}

/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                             keyword parsing                                */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* parse: CLASS _^_ <name> ; | : ... | { ...                    leaf | branch */
/*----------------------------------------------------------------------------*/
void Vortex::vParseClass(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_oCurTkn.bSame("class"_Stz));
  RASSERT(! m_p_MetaType);
      
  // get <name>
  vGetNextToken();
  s32 sClassNm = m_oCurTkn.szChars();

  // get `;` or `:` or `{`
  vGetNextToken();

  // forward declaration of a class?
  if (m_oCurTkn.bSame(";"_Stz)) {
    // leaf
    vCppPartSaw(VP_FORWARD_CLASS);
    vCppPartClear(VP_FORWARD_CLASS);
    m_oClassArgs.vReset();
    return;
  }

  // actual class
  vTypePush(sClassNm,MT_LINEAR,CP_CLASS_BEGIN);
  if (m_oTplSpec.nItems()) {
    m_p_MetaType->vTplParams(& m_oTplSpec);
    m_oTplSpec.vReset();
  }

  vClassBegin(sClassNm);

  vCppPartSaw(VP_CLASS);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vRecogniseStructure();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (m_oCurTkn.bSame(":"_Stz)) {
    
    vCppPartSaw(VP_BASE);
    vGetNextToken();
    vBase_List();
    
    RASSERT(bCppPartActive(VP_MEMBERS_END));
  }
  else
  if (m_oCurTkn.bSame("{"_Stz)) {
    
    vCppPartSaw(VP_MEMBERS);
            
    vGetNextToken();
    vMembers_List();
    
    RASSERT(bCppPartActive(VP_MEMBERS_END));
    CHECKED(m_oCurTkn.bSame(";"_Stz));
    
    bool bMaybe = bGetNextToken();
  }
  else {
    vThrowProblem(__LINE__);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vCppPartSaw(VP_CLASS_END);
  
  vCppPartClear(VP_CLASS);
  vCppPartClear(VP_BASE);
  vCppPartClear(VP_MEMBERS);
  vCppPartClear(VP_MEMBERS_END);

  m_oClassArgs.vReset();
  
  vClassEnd();
}

/*----------------------------------------------------------------------------*/
/* parse: CONST _^_ <type> <name> = <terms>                   collecting leaf */
/*----------------------------------------------------------------------------*/
void Vortex::vParseConst(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("const"_Stz));
  //RASSERT(0 == m_oOtherTerms.nItems());

  // get <type>
  vGetNextToken();
  
  for (;;) {
    CHECKED(bGetNextToken());
    if (! m_oCurTkn.bSame("*"_Stz))
      break;
  }
    
  // get <type>
  m_sConstNm = m_oCurTkn.szChars();

  // get =
  vGetExpectedToken("="_Stz);
  
  // collect ... ; into m_oConstTerms
  m_oOtherTerms.vReset();
  vCollectRemainingTerms(m_oConstTerms);
  
  vCppPartSaw(VP_CONST);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vRecogniseConst();

  m_oConstTerms.vReset();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vCppPartClear(VP_CONST);

  bool bMayBe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: ENUM _^_ ... ;                                      discarding leaf */
/*----------------------------------------------------------------------------*/
void Vortex::vParseEnum(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("enum"_Stz));

  vDiscardTill(m_oCurTkn.szChars(),";"_Stz);
  
  vCppPartSaw(VP_ENUM);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // discarded
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vCppPartClear(VP_ENUM);

  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: EXTERN _^_ ... ;                                    discarding leaf */
/*----------------------------------------------------------------------------*/
void Vortex::vParseExtern(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("extern"_Stz));

  vDiscardTill(m_oCurTkn.szChars(),";"_Stz);
  
  vCppPartSaw(VP_EXTERN);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // discarded
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vCppPartClear(VP_EXTERN);

  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: FRIEND _^_ ... ;                                    discarding leaf */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFriend(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("friend"_Stz));
  RASSERT(m_bInClass);

  vDiscardTill(m_oCurTkn.szChars(),";"_Stz);
  
  vCppPartSaw(VP_FRIEND_CLASS);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // discarded
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vCppPartClear(VP_FRIEND_CLASS);

  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: # _^_ DEFINE ... | IF ... | INCLUDE ... | PRAGMA ...         branch */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHash(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("#"_Stz));
    
  vCppPartSaw(VP_HASH);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  CHECKED(bGetNextToken());
  
  if (m_oCurTkn.bSame("define"_Stz))  vHash_Define();    else
  if (m_oCurTkn.bSame("if"_Stz))      vHash_If();        else
  if (m_oCurTkn.bSame("ifdef"_Stz))   vHash_Ifdef();     else
  if (m_oCurTkn.bSame("include"_Stz)) vHash_Include();   else
  if (m_oCurTkn.bSame("pragma"_Stz))  vHash_Pragma();    else
  {
    vThrowProblem(__LINE__);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vCppPartClear(VP_HASH);
}

/*----------------------------------------------------------------------------*/
/* parse: INLINE ...                                                  descend */
/*----------------------------------------------------------------------------*/
void Vortex::vParseInline(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("inline"_Stz));
 
  vCppPartSaw(VP_INLINE);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vGetNextToken();
  vKeywordDescend();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vCppPartClear(VP_INLINE);
}

/*----------------------------------------------------------------------------*/
/* parse: NAMESPACE _^_ <name> { <keyword>...                repeated descend */
/*----------------------------------------------------------------------------*/
void Vortex::vParseNamespace(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_oCurTkn.bSame("namespace"_Stz));
  
  // get <name> of namespace
  vGetNextToken();
  
  // get {
  vGetExpectedToken("{"_Stz);
  
  vNamepaceBegin(m_oCurTkn.szChars());
  vCppPartSaw(VP_NAMESPACE);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vGetNextToken();

  vTraceEnter((char*)__func__);

  // parse ... }
  for (;;) {
  
    // test for } being end-of-namespace
    if (m_oCurTkn.bSame("}"_Stz))
      break;
    
    // -- -- -- -- -- --
        
    vKeywordDescend();
    
    vTraceEnter((char*)__func__);
  }
  
  vCppPartSaw(VP_NAMESPACE_END);
  vNamepaceEnd();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vCppPartClear(VP_NAMESPACE);
  vCppPartClear(VP_NAMESPACE_END);

  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: [ _^_ [ NORETURN ] ] ...                                    descend */
/*----------------------------------------------------------------------------*/
void Vortex::vParseOpenSquare(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_oCurTkn.bSame("[["_Stz));
    
  vGetExpectedToken("noreturn"_Stz);
  vGetExpectedToken("]]"_Stz);
  
  vCppPartSaw(VP_NORETURN);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vGetNextToken();

  vKeywordDescend();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vCppPartClear(VP_NORETURN);
}

/*----------------------------------------------------------------------------*/
/* parse: PUBLIC _^_ :                                                   leaf */
/*----------------------------------------------------------------------------*/
void Vortex::vParsePublic(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("public"_Stz));
  
  vGetExpectedToken(":"_Stz);

  vCppPartSaw(VP_LABEL);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // leaf
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vCppPartClear(VP_PUBLIC);
  
  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: ; _^_                                                          leaf */
/*----------------------------------------------------------------------------*/
void Vortex::vParseSemicolon(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame(";"_Stz));
  
  vCppPartSaw(VP_EMPTY);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // leaf
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vCppPartClear(VP_PUBLIC);
  
  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: STATIC _^_ ...                                              descend */
/*----------------------------------------------------------------------------*/
void Vortex::vParseStatic(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_oCurTkn.bSame("static"_Stz));
    
  vCppPartSaw(VP_STATIC);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vGetNextToken();
  
  vKeywordDescend();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vCppPartClear(VP_STATIC);
}

/*----------------------------------------------------------------------------*/
/* parse: STRUCT _^_ { ...                                     leaf | members */
/*----------------------------------------------------------------------------*/
void Vortex::vParseStruct(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_oCurTkn.bSame("struct"_Stz));

  
  /*
    
   1) struct { ... } ;
      
   2) struct { ... } <memberNm> ;
   
   3) struct { ... } <memberNm> [ <constNm> ] ;

   4) struct <structNm> ;

   5) struct <structNm> { ... } ;

   6) struct <structNm> <memberNm> ;
   
   */
  
  s32 sStructNm;

  vGetNextToken();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // scenarios 1), 2) and 3) anonymous structures
  
  if (m_oCurTkn.bSame("{"_Stz)) {
    
    sStructNm.vSprint("_STRUCT_%lld"_Stz,++m_iNextStruct);

    vTypePush(sStructNm,MT_LINEAR,CP_STRUCT);
    vRecogniseStructure();

    vCppPartSaw(VP_STRUCT);
    
    // -- -- -- -- -- --
    
    vGetNextToken();
    vMembers_List();
    
    // -- -- -- -- -- --
    

    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // 1) struct { ... } ;
    
    if (m_oCurTkn.bSame(";"_Stz)) {
      vTypePop();
      goto L_Clear;
    }
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // 2) struct { ... } <memberNm> ;
    
    vGetNextToken();
    if (m_oCurTkn.bSame(";"_Stz)) {
      vTypePop();
      goto L_Clear;
    }
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // 3) struct { ... } <memberNm> [ <constNm> ] ;

    CHECKED(m_oCurTkn.bSame("["_Stz));
    vGetNextToken();
    vGetExpectedToken("]"_Stz);
    vGetExpectedToken(";"_Stz);
    vTypePop();
    goto L_Clear;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // scenarios 4), 5) and 6) named structures
  
  sStructNm = m_oCurTkn.szChars();
  
  // get ; or {
  vGetNextToken();
  
  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
  // 4) struct <blockNm> ;
  
  if (m_oCurTkn.bSame(";"_Stz))
    goto L_Clear;

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
  // 5) struct <blockNm> { ... } ;
  
  if (m_oCurTkn.bSame("{"_Stz)) {
    
    vTypePush(sStructNm,MT_LINEAR,CP_STRUCT);
    vRecogniseStructure();
    
    // -- -- -- -- -- --
    vGetNextToken();
    vMembers_List();
    CHECKED(m_oCurTkn.bSame(";"_Stz));
    // -- -- -- -- -- --

    vTypePop();
    goto L_Clear;
  }
  
  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
  // 6) struct <structNm> <memberNm> ;
  
  vGetExpectedToken(";"_Stz);

  // <#> handle member name
  
  vCppPartSaw(VP_STRUCT);
  
  goto L_Clear;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  L_Clear:
  
  vCppPartClear(VP_STRUCT);
  
  vDebugPoint();
  
  
  bool bMayBe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: TEMPLATE _^_ < params > ...                                 descend */
/*----------------------------------------------------------------------------*/
void Vortex::vParseTemplate(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_oCurTkn.bSame("template"_Stz));
        
  vGetExpectedToken("<"_Stz);
  
  m_oTplSpec.vReset();
  vCollectTemplateSpec(m_oTplSpec);

  vCppPartSaw(VP_TEMPLATE);

  vTemplateBegin();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vGetNextToken();

  vKeywordDescend();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vTemplateEnd();
  
  m_oTplSpec.vReset();

  vCppPartClear(VP_TEMPLATE);
}

/*----------------------------------------------------------------------------*/
/* parse: TYPEDEF _^_ ... ;                                   discarding leaf */
/*----------------------------------------------------------------------------*/
void Vortex::vParseTypedef(void)                           // MORE WORK REQUIRED
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("typedef"_Stz));
      
  vDiscardTill(m_oCurTkn.szChars(),";"_Stz);
  
  vCppPartSaw(VP_TYPEDEF);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // discarded
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vCppPartClear(VP_TYPEDEF);
  
  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: UNION _^_ { ...                                             members */
/*----------------------------------------------------------------------------*/
void Vortex::vParseUnion(void)
{
  vTraceEnter((char*)__func__);
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
    sUnionNm = m_oCurTkn.szChars();
    vGetExpectedToken("{"_Stz);
  }

  vTypePush(sUnionNm,MT_UNION,CP_UNION);
  
  vCppPartSaw(VP_UNION);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  vGetNextToken();

  vMembers_List();
  CHECKED(m_oCurTkn.bSame(";"_Stz));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vTypePop();

  vCppPartClear(VP_UNION);

  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: UNSIGNED _^_ ...                                            descend */
/*----------------------------------------------------------------------------*/
void Vortex::vParseUnsigned(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("unsigned"_Stz));
    
  vCppPartSaw(VP_UNSIGNED);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vGetNextToken();
  
  vKeywordDescend();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vCppPartClear(VP_UNSIGNED);
}

/*----------------------------------------------------------------------------*/
/* parse: USING _^_ NAMESPACE <name> ;                                   leaf */
/*----------------------------------------------------------------------------*/
void Vortex::vParseUsing(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_oCurTkn.bSame("using"_Stz));
    
  // get NAMESPACE
  vGetExpectedToken("namespace"_Stz);

  // get <name>
  vGetNextToken();

  // get ;
  vGetExpectedToken(";"_Stz);
  
  vCppPartSaw(VP_USING);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // leaf
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vCppPartClear(VP_USING);
  
  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: <other> _^_ <term> ...                                     sequence */
/*----------------------------------------------------------------------------*/
void Vortex::vParseVarFun(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
    
  vVarFun_SequenceBegin();

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


/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                          second-level parsing                              */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* branch: CLASS <classNm> : _^_ ...                                  members */
/*----------------------------------------------------------------------------*/
void Vortex::vBase_List(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_bInClass);

  while (bGetNextToken()) {
     
    Stz* szTerm = m_oCurTkn.szChars();
     
    // ignore optional visibility
    if (szTerm->bOneOf("private"_Stz,"protected"_Stz,"public"_Stz,nullptr))
      vGetNextToken();

    // record the name of the inherited class
    Stz* szInheritedClassNm = m_oCurTkn.szChars();

    // add inherited type to current type
    MetaInherit* pInherit = m_p_MetaType->pAddInherit(szInheritedClassNm);
    vCppPartSaw(VP_INHERITED_CLASS);
    
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
      vCppPartSaw(VP_BASE_END);
            vGetNextToken();
      break;
    }

    // base list is continued by a comma
    CHECKED(szTerm->bSame(","_Stz));
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vMembers_List();
  CHECKED(m_oCurTkn.bSame(";"_Stz));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: # DEFINE _^_ ... ENDDEFINE                                     leaf */
/*----------------------------------------------------------------------------*/
void Vortex::vHash_Define(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("define"_Stz));
      
  vGetNextToken();
  while (! m_oCurTkn.bSame("enddefine"_Stz))
    vGetNextToken();
      
  vCppPartSaw(VP_HASH_DEFINE);
   
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vCppPartClear(VP_HASH_DEFINE);

  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: # IF _^_ ... #ENDIF                                            leaf */
/*----------------------------------------------------------------------------*/
void Vortex::vHash_If(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("if"_Stz));
    
  vGetNextToken();
  while (! m_oCurTkn.bSame("endif"_Stz))
    vGetNextToken();
      
  vCppPartSaw(VP_HASH_IF);
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vCppPartClear(VP_HASH_IF);

  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: # IF _^_ ... #ENDIF                                            leaf */
/*----------------------------------------------------------------------------*/
void Vortex::vHash_Ifdef(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("ifdef"_Stz));
    
  vGetNextToken();
  while (! m_oCurTkn.bSame("endif"_Stz))
    vGetNextToken();
      
  vCppPartSaw(VP_HASH_IF);
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vCppPartClear(VP_HASH_IF);

  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: # INCLUDE _^_ <filePath>                                       leaf */
/*----------------------------------------------------------------------------*/
void Vortex::vHash_Include(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("include"_Stz));
  
  vGetNextToken();
  
  if (m_oCurTkn.bSame("<"_Stz))
    vDiscardBalancedAngles();
    
  vCppPartSaw(VP_HASH_INCLUDE);
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vCppPartClear(VP_HASH_INCLUDE);

  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* parse: # PRAGMA _^_ ONCE                                              leaf */
/*----------------------------------------------------------------------------*/
void Vortex::vHash_Pragma(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("pragma"_Stz));
  
  vGetNextToken();
  
  if (m_oCurTkn.bSame("once"_Stz)) {
    vCppPartSaw(VP_HASH_PRAGMA_ONCE);
    vCppPartClear(VP_HASH_PRAGMA_ONCE);
  }
  else
  if (m_oCurTkn.bSame("pack"_Stz)) {
    vGetExpectedToken("("_Stz);
    vDiscardBalancedParenthesis();
    vCppPartSaw(VP_HASH_PRAGMA_PACK);
    vCppPartClear(VP_HASH_PRAGMA_PACK);
  }
  else {
    vThrowProblem(__LINE__);
  }
  
  bool bMaybe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* skip a label i.e. private|protected|public :                               */
/*----------------------------------------------------------------------------*/
bool Vortex::bLabel_Opt(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
    
  if (m_oCurTkn.bSame("private"_Stz)) {
    CHECKED(bGetNextToken());
    CHECKED(m_oCurTkn.bSame(":"_Stz));
    vCppPartSaw(VP_LABEL);
    vCppPartClear(VP_LABEL);
    vGetNextToken();
    return true;
  }
  else
  if (m_oCurTkn.bSame("protected"_Stz)) {
    CHECKED(bGetNextToken());
    CHECKED(m_oCurTkn.bSame(":"_Stz));
    vCppPartSaw(VP_LABEL);
    vCppPartClear(VP_LABEL);
    vGetNextToken();
    return true;
  }
  else
  if (m_oCurTkn.bSame("public"_Stz)) {
    CHECKED(bGetNextToken());
    CHECKED(m_oCurTkn.bSame(":"_Stz));
    vCppPartSaw(VP_LABEL);
    vCppPartClear(VP_LABEL);
    vGetNextToken();
    return true;
  }
  else {
    return false;
  }
}

/*----------------------------------------------------------------------------*/
/* parse:  ... CLASS | STRUCT | UNION  ... { _^_ ... } ...            descend */
/*----------------------------------------------------------------------------*/
void Vortex::vMembers_List(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
    
  for (;;) {

    // block is terminated by } ;
    if (m_oCurTkn.bSame("}"_Stz))
      break;
    
    if (bLabel_Opt()) {
      vTraceEnter((char*)__func__);
      continue;
    }
    
    vKeywordDescend();
    
    vTraceEnter((char*)__func__);
  }
  
  vCppPartSaw(VP_MEMBERS_END);
  
  bool bMaybe = bGetNextToken();
}


/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                      variable / function sequence                          */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* beginning of var/fun sequence                                sequence leaf */
/*----------------------------------------------------------------------------*/
void Vortex::vVarFun_SequenceBegin(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_oTypeArgs.vReset();
  
  m_nAsterisks = 0;

  vVarFun_Type();
}


/*----------------------------------------------------------------------------*/
/* collect type spec for variable or function -> vVarFun_Name()      sequence */
/*----------------------------------------------------------------------------*/
void Vortex::vVarFun_Type(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  m_sTypeNm = m_oCurTkn.szChars();

  vGetNextToken();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // possible < classArgs >
  if (m_oCurTkn.bSame("<"_Stz)) {
    vCollectTypeArgs(m_oTypeArgs);
    vGetNextToken();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // zero or more * asterisks
  for (;;) {
    
    // collected when we have no more asterisks
    if (! m_oCurTkn.bSame("*"_Stz))
      break;
    
    // we have an asterisk
    m_oTypeSpec.vInsert(m_oCurTkn.szChars());
    m_nAsterisks++;
  
    vGetNextToken();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // a possible & reference
  if (m_oCurTkn.bSame("&"_Stz))
    vGetNextToken();

  vVarFun_TypeScope();
}

/*----------------------------------------------------------------------------*/
/* scope test                                                                 */
/*----------------------------------------------------------------------------*/
void Vortex::vVarFun_TypeScope(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  // do we see the scope resolution opeator (::)
  //   indicating templated function in a _t.hpp file?
  if (m_oCurTkn.bSame("::"_Stz)) {
    
    vGetNextToken();

    if (m_oCurTkn.bSame(m_oTypeSpec.szName(0))) {
      vVarFun_Name();
      goto L_Return;
    }
    
    if (m_oCurTkn.bSame("~"_Stz)) {
      vVarFun_Name();
      goto L_Return;
    }
    
    if (m_oCurTkn.bSame("operator"_Stz)) {
      vGetNextToken();
      vVarFun_Operator();
      goto L_Return;
    }
    
    //NEVER_REACH
  }
  
  vVarFun_Special();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  L_Return:

  return;
}

/*----------------------------------------------------------------------------*/
/* collect type spec for variable or function -> vVarFun_Name()      sequence */
/*----------------------------------------------------------------------------*/
void Vortex::vVarFun_Special(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // typeless constructor / destructor / auto-conversion operator
  if (m_bInClass) {
    
    // -- -- -- -- -- -- -- -- -- -- -- --
    // 1) typeless constructor
    if (m_sTypeNm.bSame(m_sClassNm)) {
      
      // 1a) do we have a constructor?
      //        e.g. myClass <i8> (void);
      if (m_oCurTkn.bSame("("_Stz)) {
        
        // constructor (no specifier)
        TRACE("  (typeless) constructor\n"_Stz);
        vVarFun_Constructor();
        goto L_Return;
      }
            
      // 1b) at this point we have type which is the class
      //       e.g.  myClass<i8>* pGetParent(void);
      vVarFun_Name();
      goto L_Return;
    }
    
    // -- -- -- -- -- -- -- -- -- -- -- --
    // 2) typeless destructor
    if (m_sTypeNm.bSame("~"_Stz)) {
      TRACE("  (typeless) destructor\n"_Stz);
      vVarFun_Destructor();
      goto L_Return;
    }
    
    // -- -- -- -- -- -- -- -- -- -- -- --
    // 3) typeless operator
    if (m_sTypeNm.bSame("operator"_Stz)) {
      TRACE("  (typeless) operator\n"_Stz);
      vVarFun_Operator();
      goto L_Return;
    }
    
    // no typeless constructor/destructor/operator found
  }

  // no specifier (constructor / destructor) after  <classNm> <typeAgs> ::
  
  vVarFun_Name();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  L_Return:
  
  return;
}

/*----------------------------------------------------------------------------*/
/* collect name of a variable or function -> vVarFun_AryFunVar()     sequence */
/*----------------------------------------------------------------------------*/
void Vortex::vVarFun_Name(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  m_bIsFunPtr = false;

  m_oFunVarNm.vReset();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // do we have ( * <name> ) ( ... ) ?
  
  if (m_oCurTkn.bSame("("_Stz)) {
    
    m_bIsFunPtr = true;
    
    vGetExpectedToken("*"_Stz);
    vGetNextToken();
    m_oFunVarNm.vInsert(m_oCurTkn.szChars());
    vGetExpectedToken(")"_Stz);
    
    vGetExpectedToken("("_Stz);
    vCollectBalancedParenthesis(m_oOtherTerms);

    vCppPartSaw(VP_FUNCTION_POINTER);
        
    vGetNextToken();
    
    vVarFun_AryFunVar();
    goto L_Return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  L_Parse_Function_Name:
  
  // do we have operator <name>
  if (m_oCurTkn.bSame("operator"_Stz)) {
    vGetNextToken();
    vVarFun_Operator();
    goto L_Return;
  }
  else if (m_oCurTkn.bSame("~"_Stz)) {
    vGetNextToken();
    vVarFun_Destructor();
    goto L_Return;
  }
  
  // we have a <name>
  m_oFunVarNm.vInsert(m_oCurTkn.szChars());
  vTraceFunVarNm();
  vCppPartSaw(VP_FUNVAR_NAME);
  
  vGetNextToken();
  
  // do we have type qualifier args?
  if (! m_oCurTkn.bSame("<"_Stz)) {
    vVarFun_AryFunVar();
    goto L_Return;
  }
  
  vCollectTypeArgs(m_oTypeArgs);
  vCppPartSaw(VP_TYPE_ARGS);
    
  vGetNextToken();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (! m_oCurTkn.bSame("::"_Stz)) {
    vVarFun_AryFunVar();
    goto L_Return;
  }
    
  vShiftClassNm();
  
  vGetNextToken();
  
  vTraceEnter((char*)__func__);

  goto L_Parse_Function_Name;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  L_Return:
  return;
}


/*----------------------------------------------------------------------------*/
/* detect next is array, function or variable                        sequence */
/*----------------------------------------------------------------------------*/
void Vortex::vVarFun_AryFunVar(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (m_oCurTkn.bSame("["_Stz)) {
    vVarFun_Array();
    return;
  }
  
  if (m_oCurTkn.bSame(";"_Stz)) {
    vRecogniseVariable();
    vGetNextToken();
    return;
  }
  
  if (m_oCurTkn.bSame("("_Stz)) {
    vVarFun_FunBody();
    return;
  }
  
  if (m_oCurTkn.bSame(":"_Stz)) {
    
    vGetNextToken();
    
    // detected a field
    
    vVarFun_Field();
    return;
  }

  if (m_oCurTkn.bSame("::"_Stz)) {
    vGetNextToken();
    vVarFun_Name();
    return;
  }
    

  if (m_oCurTkn.bSame("="_Stz)) {
    RASSERT(! m_bInClass);
    vCollectRemainingTerms(m_oOtherTerms);
    vRecogniseVariable();
    m_oOtherTerms.vReset();
    bool bMayBe = bGetNextToken();
    return;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* collect array information                                    sequence leaf */
/*----------------------------------------------------------------------------*/
void Vortex::vVarFun_Array(void)
{
  vTraceEnter((char*)__func__);
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
     
    m_oArraySpec.vInsert(m_oCurTkn.szChars());
  }

  vGetNextToken();
  if (m_oCurTkn.bSame("["_Stz))
    goto L_Dimension;
 
  if (m_oCurTkn.bSame("="_Stz))
    vGetNextToken();

  if (m_oCurTkn.bSame("{"_Stz)) {
    vDiscardBalancedBraces();
    vGetNextToken();
  }
  
  CHECKED(m_oCurTkn.bSame(";"_Stz));
  
  vCppPartSaw(VP_ARRAY_SPEC);
  
  vCppPartSaw(VP_MEMBER_VARIABLE);
    
  m_oTypeSpec.vReset();
  m_oFunVarNm.vReset();
  
  bool bMayBe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* collect information about a field                            sequence leaf */
/*----------------------------------------------------------------------------*/
void Vortex::vVarFun_Field(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  // field bits
  
  m_nFieldBits = m_oCurTkn.iToI8();
  
  vGetExpectedToken(";"_Stz);
  
  vCppPartSaw(VP_FIELD_BITS);
  
  vCppPartClear(VP_FIELD_BITS);

  vGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* collect function body                                        sequence leaf */
/*----------------------------------------------------------------------------*/
void Vortex::vVarFun_FunBody(void)
{
  vTraceEnter((char*)__func__);
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
      
      m_oFunSpec.vInsert(m_oCurTkn.szChars());
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
  
  if (m_oCurTkn.bSame("{"_Stz)) {
    vVarFun_FunCode();
    return;
  }
  else {
    vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (m_bInClass) {
    vCppPartSaw(VP_MEMBER_FUNCTION);
  }
  else {
    vCppPartSaw(VP_FUNCTION);
  }
  
  bool bMayBe = bGetNextToken();


  m_oTypeSpec.vReset();
  m_oFunVarNm.vReset();
}

/*----------------------------------------------------------------------------*/
/* collect function code                                        sequence leaf */
/*----------------------------------------------------------------------------*/
void Vortex::vVarFun_FunCode(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_oCurTkn.bSame("{"_Stz));

  m_bIsFun = true;
  m_oFunSpec.vReset();

  m_bIsFun = true;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vDiscardBalancedBraces();
  
  vCppPartSaw(VP_FUNCTION_BODY);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vVarFun_SequenceEnd();
}

/*----------------------------------------------------------------------------*/
/* collect information about a constructor -> vVarFun_FunBody        sequence */
/*----------------------------------------------------------------------------*/
void Vortex::vVarFun_Constructor(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  RASSERT(m_oCurTkn.bSame("("_Stz));

  m_bIsFunPtr = false;

  vGetNextToken();

  m_oFunVarNm.vInsert(m_sClassNm);

  vVarFun_FunBody();
}

/*----------------------------------------------------------------------------*/
/* collect information about a destructor -> vVarFun_FunBody         sequence */
/*----------------------------------------------------------------------------*/
void Vortex::vVarFun_Destructor(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  RASSERT(! m_oCurTkn.bSame("~"_Stz));
  
  m_bIsFunPtr = false;
  
  vGetNextToken();

  // possible < classArgs >
  if (m_oCurTkn.bSame("<"_Stz)) {
    vCollectTypeArgs(m_oTypeArgs);
    vGetNextToken();
  }

  m_oFunVarNm.vInsert(m_sClassNm);

  vVarFun_FunBody();
}

/*----------------------------------------------------------------------------*/
/* collect information about operator function -> vVarFun_FunBody()  sequence */
/*----------------------------------------------------------------------------*/
void Vortex::vVarFun_Operator(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  for (;;) {
    vGetNextToken();
    
    if (m_oCurTkn.bSame("("_Stz))
      break;
  }
  
  vCppPartSaw(VP_OPERATOR);

  vVarFun_FunBody();
  
  vCppPartClear(VP_OPERATOR);
}


/*----------------------------------------------------------------------------*/
/* end of var/fun sequence                                      sequence leaf */
/*----------------------------------------------------------------------------*/
void Vortex::vVarFun_SequenceEnd(void)
{
  vTraceEnter((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (m_bInClass) {
    vCppPartSaw(VP_MEMBER_FUNCTION);
  }
  else {
    vCppPartSaw(VP_FUNCTION);
  }
  
  vRecogniseFunction();

  m_oTypeSpec.vReset();
  m_oFunVarNm.vReset();

  bool bMayBe = bGetNextToken();
}

/*----------------------------------------------------------------------------*/
/* shift funvar name etc to be class                                          */
/*----------------------------------------------------------------------------*/
void Vortex::vShiftClassNm(void)
{
  vTraceEnter_short((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_sClassNm = m_oFunVarNm.szName(0);
  
  m_oClassArgs.vReset();
  m_oClassArgs.vCopy(&m_oTypeArgs);
  
  m_oFunVarNm.vReset();
  m_oTypeArgs.vReset();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get expected token                                                         */
/*----------------------------------------------------------------------------*/
void Vortex::vGetExpectedToken(Stz* szExpect)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  CHECKED(bGetNextToken());
  
  if (! m_oCurTkn.bSame(szExpect)) {
    TRACE("Expecting token: `%s` got: `%s` @%lld.%lld\n"_Stz,
          szExpect->azChar(),
          m_oCurTkn.szChars(),
          m_oCurTkn.iLineNo(),
          m_oCurTkn.iColNo());
    
    vThrowProblem(__LINE__);
  }
  
}

/*----------------------------------------------------------------------------*/
/* get next token                                                             */
/*----------------------------------------------------------------------------*/
bool Vortex::bGetNextToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_pInput->vReadToken();
  m_bEOF = ! m_pInput->bSeenEOF();
  
  if (m_bTrace) {
    if (m_bEOF)
      vTrace("  got: EOF\n"_Stz);
    else
      vTrace("  got: `%s` @%lld,%lld\n"_Stz,
             m_oCurTkn.szChars(),
             m_oCurTkn.iLineNo(),
             m_oCurTkn.iColNo());
  }
    
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

/*----------------------------------------------------------------------------*/
/* collect delimited tokens (potentially nested)                              */
/*----------------------------------------------------------------------------*/
void Vortex::vCollectBalanced(TagSet& roTerms, Stz* szOpen, Stz* szClose)
{
  vTraceEnter_short((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
         
  for (;;) {
    
    vGetNextToken();
    roTerms.vInsert(m_oCurTkn.szChars());
    
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
void Vortex::vCollectBalancedAngles(TagSet& roTerms)
{
  vTraceEnter_short((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vCollectBalanced(roTerms,"<"_Stz,">"_Stz);
}

/*----------------------------------------------------------------------------*/
/* collect balanced curly braces pair: { ... }                                */
/*----------------------------------------------------------------------------*/
void Vortex::vCollectBalancedBraces(TagSet& roTerms)
{
  vTraceEnter_short((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vCollectBalanced(roTerms,"{"_Stz,"}"_Stz);
}

/*----------------------------------------------------------------------------*/
/* collect balanced parenthesis pair: ( ... )                                 */
/*----------------------------------------------------------------------------*/
void Vortex::vCollectBalancedParenthesis(TagSet& roTerms)
{
  vTraceEnter_short((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vCollectBalanced(roTerms,"("_Stz,")"_Stz);
}

/*----------------------------------------------------------------------------*/
/* collect other terms: <other> ... | ; | { ... }                             */
/*----------------------------------------------------------------------------*/
void Vortex::vCollectRemainingTerms(TagSet& roTerms)
{
  vTraceEnter_short((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  //RASSERT(0 == m_oOtherTerms.nItems());
  
  roTerms.vReset();
  
  for (;;) {
    
    vGetNextToken();
    roTerms.vInsert(m_oCurTkn.szChars());

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
void Vortex::vCollectTemplateSpec(TagSet& roTplSepc)
{
  vTraceEnter_short((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_oCurTkn.bSame("<"_Stz));

  roTplSepc.vReset();
      
  // collect ... >
  while (bGetNextToken()) {
    if (m_oCurTkn.bSame(">"_Stz))
      break;
    roTplSepc.vInsert(m_oCurTkn.szChars());
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
void Vortex::vCollectTypeArgs(TagSet& roTypeArgs)
{
  vTraceEnter_short((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_oCurTkn.bSame("<"_Stz));

  roTypeArgs.vReset();
      
  // collect ... >
  while (bGetNextToken()) {
    if (m_oCurTkn.bSame(">"_Stz))
      break;
    roTypeArgs.vInsert(m_oCurTkn.szChars());
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
  vTraceEnter_short((char*)__func__);
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
  vTraceEnter_short((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vDiscardBalanced("<"_Stz,">"_Stz);
}

/*----------------------------------------------------------------------------*/
/* discard balanced curly braces pair: { ... }                                */
/*----------------------------------------------------------------------------*/
void Vortex::vDiscardBalancedBraces(void)
{
  vTraceEnter_short((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vDiscardBalanced("{"_Stz,"}"_Stz);
}

/*----------------------------------------------------------------------------*/
/* discard balanced parenthesis pair: ( ... )                                 */
/*----------------------------------------------------------------------------*/
void Vortex::vDiscardBalancedParenthesis(void)
{
  vTraceEnter_short((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vDiscardBalanced("("_Stz,")"_Stz);
}

/*----------------------------------------------------------------------------*/
/* discard inline tokens: ... INLINE { ... }                                  */
/*----------------------------------------------------------------------------*/
void Vortex::vDiscardInlineTokens(void)
{
  vTraceEnter_short((char*)__func__);
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
  vTraceEnter_short((char*)__func__);
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
  vTraceEnter_short((char*)__func__);
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
    
  TRACE("Skipping: %s "_Stz,szTerm->azChar());
  if (szTerm->bSame(szTill)) {
    TRACE("\n"_Stz,szTerm->azChar());
    return;
  }
        
  while (bGetNextToken()) {
    TRACE("%s "_Stz,m_oCurTkn.szChars());
    if (m_oCurTkn.bSame(szTill)) {
      TRACE("\n"_Stz,m_oCurTkn.szChars());
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
void Vortex::vTypeNewClass(Stz* szClassNm)
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
void Vortex::vTypeNewStruct(Stz* szStructNm)
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
void Vortex::vTypeNewUnion(Stz* szUnionNm)
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


/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                                 machinary                                  */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* is syntax element active?                                                  */
/*----------------------------------------------------------------------------*/
bool Vortex::bCppPartActive(eVP eCppPart)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  bool bActive = m_baPart[eCppPart];

  return bActive;
}

/*----------------------------------------------------------------------------*/
/* clear given syntax element                                                  */
/*----------------------------------------------------------------------------*/
void Vortex::vCppPartClear(eVP eCppPart)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  m_baPart[eCppPart] = false;
}

/*----------------------------------------------------------------------------*/
/* saw given syntax element                                                   */
/*----------------------------------------------------------------------------*/
void Vortex::vCppPartSaw(eVP eCppPart)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  m_baPart[eCppPart] = true;

  if (m_bTrace)
    TRACE("  Saw: %s\n\n"_Stz,aVP[eCppPart]);
  
  if (m_bContPart)
    m_bContPart = bTraceAgain();
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
/* get keystroke to continue again                                            */
/*----------------------------------------------------------------------------*/
bool Vortex::bTraceAgain(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vTraceParts();
  
  vInfo("[ vortex >> "_Stz);
  //fflush(stdin);
  
  char c = getc(stdin);
  
  if ('.' == c)
    return false;
  else
    return true;
}

/*----------------------------------------------------------------------------*/
/* trace constant                                                             */
/*----------------------------------------------------------------------------*/
void Vortex::vTraceConstant(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (! m_bTrace)
    return;
  
  s48K sTrace;
  
  TRACE("  Constant: %s\n"_Stz,sTrace.azChar());
}

/*----------------------------------------------------------------------------*/
/* trace function entry                                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vTraceEnter(char* azFunc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  if (! m_bTrace)
    return;
  
  vTrace("%lld/%s/%16s/  `%s` @%lld,%lld %s%s%s%s%s%s%s%s%s\n"_Stz,
    
    m_nSources,
    m_sSourcePath.szStz(),
    azFunc,
  
    m_oCurTkn.szChars(),
    m_oCurTkn.iLineNo(),
    m_oCurTkn.iColNo(),

    m_bInNamespace ? " --NAMESPACE `" : "",
    m_bInNamespace ? m_sNamespaceNm.azChar() : "",
    m_bInNamespace ? "`" : "",
    
    m_bInClass     ? " --CLASS `" : "",
    m_bInClass     ? m_sClassNm.azChar() : "",
    m_bInClass     ? "`" : "",
    
    m_bInTemplate  ? " --TPLSPEC `" : "",
    m_bInTemplate  ? m_sTplSpec.azChar() : "",
    m_bInTemplate  ? "`" : "",
  
    C_VARGS_END
  );
}

/*----------------------------------------------------------------------------*/
/* trace function entry                                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vTraceEnter_short(char* azFunc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  if (! m_bTrace)
    return;
  
  vTrace("%lld/%s/%16s/  `%s` @%lld,%lld\n"_Stz,
    
    m_nSources,
    m_sSourcePath.szStz(),
    azFunc,
    
    m_oCurTkn.szChars(),
    m_oCurTkn.iLineNo(),
    m_oCurTkn.iColNo(),

    C_VARGS_END
  );
}

/*----------------------------------------------------------------------------*/
/* trace function                                                             */
/*----------------------------------------------------------------------------*/
void Vortex::vTraceFunction(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (! m_bTrace)
    return;
  
  s48K sTrace;
  
  TRACE("  Function: %s\n"_Stz,sTrace.azChar());
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
/* trace cpp parts seen                                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vTraceParts(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (! m_bTrace)
    return;
  
  s48K sTrace;
  
  for (i8 i_Saw = 0; i_Saw < VP_ENUMS; i_Saw++) {
    
    if (! m_baPart[i_Saw])
      continue;
    
    if (i_Saw)
      sTrace.vAppend(' ');

    sTrace.vConcat(aVP[i_Saw]);
  }

  TRACE("  Parts: %s\n"_Stz,sTrace.azChar());
}

/*----------------------------------------------------------------------------*/
/* trace structure                                                            */
/*----------------------------------------------------------------------------*/
void Vortex::vTraceStructure(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (! m_bTrace)
    return;
  
  s48K sTrace;
  
  TRACE("  Structure: %s\n"_Stz,sTrace.azChar());
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

/*----------------------------------------------------------------------------*/
/* trace type specification terms                                             */
/*----------------------------------------------------------------------------*/
void Vortex::vTraceVariable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (! m_bTrace)
    return;
  
  s48K sTerms;
  
  TRACE("  Variable: %s\n"_Stz,sTerms.azChar());
}

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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
