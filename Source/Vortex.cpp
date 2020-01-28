/*============================================================================*/
/* Vortex.cpp - Vortex                                                        */
/*============================================================================*/

#include "Utilities.h"

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
  
  m_pOutputFile  = nullptr;
  m_sSourcePath  = ""_Stz;
  m_pInput       = nullptr;
  
  m_pMetaGraph   = new MetaGraph;
  m_p_ConsSet    = m_pMetaGraph->pConstSet();
  m_p_MetaClass  = nullptr;
  m_p_MemberSet  = nullptr;
  m_p_MemberVar  = nullptr;
  
  //m_oTerms
  m_bTraceTokens = false;
  
  m_bInClass     = false;
  m_sClassNm     = C_NO_NAME;
  
  m_bInNamespace = false;
  m_sNamespaceNm = C_NO_NAME;

  _checkInitialised_(CN_VORTEX);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Vortex::~Vortex(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  delete m_pOutputFile;
  delete m_pInput;
  
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
    
  m_pMetaGraph->vRecPrint();
  
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

  vTrace("Parsing file %s\n"_Stz,szSourcePath->azChar());
  
  m_sSourcePath = szSourcePath;
  m_pInput = new Input;
  m_pInput->vPushFile(szSourcePath);
  
  //vProcAll();
  bool b = bParseFile();
  if (! b) {
    vTrace("\n\n"_Stz);
    vTrace("** Unable to fully parse %s\n"_Stz,szSourcePath->azChar());
    vTrace("** Last Part: %s\n"_Stz,m_sPart.azChar());
    vDebugPoint();
  }
  
  delete m_pInput;
  m_pInput = nullptr;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add term                                                                   */
/*----------------------------------------------------------------------------*/
void Vortex::vAddTerm(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_oTerms.vAdd(szTerm);
}

/*----------------------------------------------------------------------------*/
/* begin recognise                                                            */
/*----------------------------------------------------------------------------*/
void Vortex::vBeginRecognise(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* end recognise                                                              */
/*----------------------------------------------------------------------------*/
void Vortex::vEndRecognise(bool bCorrectly)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* add term                                                                   */
/*----------------------------------------------------------------------------*/
bool Vortex::bCollectTerms(Stz* szEnd, bool bAddEnd)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;
  for (;;) {
    
    if (! bGetToken(xTkn))
      return false;
    
    if (xTkn.bSame(szEnd)) {
      
      if (bAddEnd)
        m_oTerms.vAdd(xTkn.szTerm());
      
      break;
    }
    
    m_oTerms.vAdd(xTkn.szTerm());
  }

  s48K sNamesString;
  m_oTerms.vNamesString(sNamesString);
  
  vTrace("Terms: %s\n"_Stz,sNamesString.azChar());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get next token                                                             */
/*----------------------------------------------------------------------------*/
bool Vortex::bGetToken(Token& roToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  bool b = m_pInput->bGetToken(&roToken);
  
  if (b) {
    if (m_sPart.nLength())
      m_sPart.vAppend(' ');
    m_sPart.vConcat(roToken.szTerm());
  }
  
  if (b && m_bTraceTokens)
    vTrace("Part: %s\n"_Stz,m_sPart.azChar());
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* start new c++ part                                                         */
/*----------------------------------------------------------------------------*/
void Vortex::vNewPart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_sPart = ""_Stz;
}

/*----------------------------------------------------------------------------*/
/* recognised c++ part                                                        */
/*----------------------------------------------------------------------------*/
void Vortex::vRecognised(eCP ePart)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vTrace("Recognised %s\n"_Stz,aCP[ePart]);
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* are terms a function ?                                                     */
/*----------------------------------------------------------------------------*/
bool Vortex::bTermsAreFunction(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  Stz* szTerm;
 
  i8 nTerms = m_oTerms.nItems();
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // term 0
  
  i8 i_Term = 0;
  szTerm = m_oTerms.szName(i_Term++);
  
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
  szTerm = m_oTerms.szName(i_Term++);
  while (szTerm->bSame("*"_Stz))
    szTerm = m_oTerms.szName(i_Term++);
  
  // -- -- -- -- -- --
  // test for member which is a member which is a function pointer
  if (m_bInClass && szTerm->bSame("("_Stz))
    return false;

  // -- -- -- -- -- --
  // attempt to detect a function
  szTerm = m_oTerms.szName(i_Term++);
  while (i_Term < nTerms) {
    if (szTerm->bSame("("_Stz))
      return true;
    szTerm = m_oTerms.szName(i_Term++);
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
  
   i8 nTerms = m_oTerms.nItems();
     
   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   // term 0
   
   i8 i_Term = 0;
   szTerm = m_oTerms.szName(i_Term++);
  
  // -- -- -- -- -- --
  // exclude constructor function
  if (m_bInClass && szTerm->bSame(m_sClassNm.szStz()) &&
      m_oTerms.szName(i_Term)->bSame("("_Stz)) {
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
  szTerm = m_oTerms.szName(i_Term++);
  while (szTerm->bSame("*"_Stz))
    szTerm = m_oTerms.szName(i_Term++);
  
  // -- -- -- -- -- --
  // test for variable which is a function pointer
  if (szTerm->bSame("("_Stz))
    return true;

  // -- -- -- -- -- --
  // attempt to detect a function
  szTerm = m_oTerms.szName(i_Term++);
  while (i_Term < nTerms) {
    if (szTerm->bSame("("_Stz))
      return false;
    szTerm = m_oTerms.szName(i_Term++);
  }
  
  // -- -- -- -- -- --
  // must be a variable
  return true;
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
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* process all parts in the current source file (.h or .hpp)                  */
/*----------------------------------------------------------------------------*/
bool Vortex::bParseFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
    
  m_bTraceTokens = true;
  
  Token xTkn;
  while (bGetToken(xTkn)) {
  
    Stz* szTerm = xTkn.szTerm();
    
    try {
      if (szTerm->bSame("class"_Stz)) {
        //vParseClass(szTerm);
        vThrowProblem(__LINE__);
      }
      else
      if (szTerm->bSame("enum"_Stz)) {
        vParseEnum(szTerm);
      }
      else
      if (szTerm->bSame("friend"_Stz)) {
        vParseFriend(szTerm);
      }
      else
      if (szTerm->bSame("#"_Stz)) {
        vParseHash(szTerm);
      }
      else
      if (szTerm->bSame("namespace"_Stz)) {
        vParseNamespace(szTerm);
      }
      else
      if (szTerm->bSame("static"_Stz)) {
        vParseStatic(szTerm);
      }
      else
      if (szTerm->bSame("using"_Stz)) {
        vParseUsing(szTerm);
      }
      else {
        vParse_other(szTerm);
      }
      vNewPart();
    }
    catch (i8 i_Problem) {
      return false;
    }
    
  } // while more tokens
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse: namespace <namespaceNm> { ... class _^_ ...                         */
/*----------------------------------------------------------------------------*/
void Vortex::vParseClass(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(szSawTerm->bSame("class"_Stz));
  
  vDebugPoint();
  
  Token xTkn;
  
  // SE semantics: we should be in a namespace
  CHECKED(m_bInNamespace);

  CHECKED(bGetToken(xTkn));
  m_sClassNm = xTkn.szTerm();
  m_bInClass = true;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  CHECKED(bGetToken(xTkn));

  // forward declaration of a class?
  if (xTkn.bSame(";"_Stz)) {
    vRecognised(CP_FORWARD_CLASS);
    m_bInClass = false;
    m_sClassNm = ""_Stz;
    return;
  }

  m_p_MetaClass = m_pMetaGraph->pAddMetaClass(m_sClassNm.szStz());
  
  // detect class base clause ?
  if (xTkn.bSame(":"_Stz)) {
    m_p_MemberSet = nullptr;
    m_p_MemberVar = nullptr;
    vParseClassBase(xTkn.szTerm());
    return;
  }
  
  // detect class block ?
  if (xTkn.bSame("{"_Stz)) {
    m_p_MemberSet = nullptr;
    m_p_MemberVar = nullptr;
    vParseClassBlock(xTkn.szTerm());
    return;
  }

  vThrowProblem(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* parse within:                                                              */
/*----------------------------------------------------------------------------*/
void Vortex::vParseClassBase(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame(":"_Stz));
  
  Token xTkn;

  vDebugPoint();
  
  while (bGetToken(xTkn)) {

    Stz* szTerm = xTkn.szTerm();
    
    // class base is terminated by {
    if (szTerm->bSame("{"_Stz)) {
      vParseClassBlock(szTerm);
      return;
    }

    // skip token
    
  }

  vThrowProblem(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* parse: namespace <namespaceNm> { ... class _^_ ...                         */
/*----------------------------------------------------------------------------*/
void Vortex::vParseClassBlock(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(szSawTerm->bSame("{"_Stz));
  
  vDebugPoint();
  
  Token xTkn;

  vThrowProblem(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* parse enum _^_ ...                                                         */
/*----------------------------------------------------------------------------*/
void Vortex::vParseEnum(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("enum"_Stz));
  
  vDebugPoint();
  
  vSkipTillSemicolon();
  
  vRecognised(CP_ENUM);
}

/*----------------------------------------------------------------------------*/
/* parse friend _^_ ...                                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFriend(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("friend"_Stz));
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* parse hash _^_ ...                                                         */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHash(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("#"_Stz));
  
  vDebugPoint();
  
  Token xTkn;
  
  RASSERT(bGetToken(xTkn));
  
  if (xTkn.bSame("define"_Stz)) {
    RASSERT(bGetToken(xTkn));
    vThrowProblem(__LINE__);
    return;
  }

  if (xTkn.bSame("include"_Stz)) {
    RASSERT(bGetToken(xTkn));
    vRecognised(CP_FILE_INCLUDE);
    return;
  }

  if (xTkn.bSame("pragma"_Stz)) {
    RASSERT(bGetToken(xTkn));
    RASSERT(xTkn.bSame("once"_Stz));
    vRecognised(CP_PRAGMA_ONCE);
    return;
  }

  vThrowProblem(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* parse single class member (no label)                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vParseMember(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
 
  //RASSERT(szSawTerm->bSame("???"_Stz));
           
  if (bRecogniseStruct(szTerm))
    return;
    
  if (bRecogniseUnion(szTerm))
    return;

  if (! m_p_MemberSet)
    m_p_MemberSet = m_p_MetaClass->pAddMemberSet("linear"_Stz,m_p_MemberSet);

  vProcMemberProper(szTerm);
}

/*----------------------------------------------------------------------------*/
/* parse within: namespace _^_ <name> {                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vParseNamespace(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("namespace"_Stz));
  
  Token xTkn;

  vDebugPoint();
  
  CHECKED(bGetToken(xTkn));
  m_sNamespaceNm = xTkn.szTerm();
  m_bInNamespace = true;
  
  CHECKED(bGetToken(xTkn));
  CHECKED(xTkn.bSame("{"_Stz));

  while (bGetToken(xTkn)) {
  
    Stz* szTerm = xTkn.szTerm();
    
    if (xTkn.bSame("}"_Stz))  {
      vRecognised(CP_NAMESPACE);
      m_bInNamespace = false;
      m_sNamespaceNm = ""_Stz;
      return;
    }
    else if (xTkn.bSame("class"_Stz)) {
      vParseClass(szTerm);
    }
    else if (xTkn.bSame("enum "_Stz)) {
      vParseEnum(szTerm);
    }
    else if (xTkn.bSame("friend"_Stz)) {
      vParseFriend(szTerm);
    }
    else if (xTkn.bSame("#"_Stz)) {
      vParseHash(szTerm);
    }
    else if (xTkn.bSame("static"_Stz)) {
      vParseStatic(szTerm);
    }
    else if (xTkn.bSame("template"_Stz)) {
      vParseTemplate(szTerm);
    }
    else {
      vParseNamespaceFunction(szTerm);
    }
  } // while more tokens
  
  vThrowProblem(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* parse namespace _^_ ...                                                    */
/*----------------------------------------------------------------------------*/
void Vortex::vParseNamespaceFunction(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  //RASSERT(szSawTerm->bSame("???"_Stz));
  
  m_oTerms.vReset();

  m_oTerms.vAdd(szTerm);
  
  CHECKED(bCollectTerms(";"_Stz,true));
  
  vDebugPoint();
  
  CHECKED(bTermsAreFunction());
  
  vRecognised(CP_NAMESPACE_FUN);
}

/*----------------------------------------------------------------------------*/
/* parse static _^_ ...                                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vParseStatic(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("static"_Stz));
  
  vDebugPoint();
  
  if (! m_bInClass) {
    vSkipTillSemicolon();
    vRecognised(CP_STATIC);
    return;
  }
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* parse template _^_ ...                                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vParseTemplate(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("template"_Stz));
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* parse using _^_ ...                                                        */
/*----------------------------------------------------------------------------*/
void Vortex::vParseUsing(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("using"_Stz));
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* parse <term> _^_                                                           */
/*----------------------------------------------------------------------------*/
void Vortex::vParse_other(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  //RASSERT(szSawTerm->bSame("???"_Stz));
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* skip a label i.e. private|protected|public :                               */
/*----------------------------------------------------------------------------*/
bool Vortex::bSkipLabel(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;
  
  if (szTerm->bSame("private"_Stz)) {
    RASSERT(bGetToken(xTkn));
    RASSERT(xTkn.bSame(":"_Stz));
    return true;
  }
  
  if (szTerm->bSame("protected"_Stz)) {
    RASSERT(bGetToken(xTkn));
    RASSERT(xTkn.bSame(":"_Stz));
    return true;
  }
  
  if (szTerm->bSame("public"_Stz)) {
    RASSERT(bGetToken(xTkn));
    RASSERT(xTkn.bSame(":"_Stz));
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* skip till ;                                                                */
/*----------------------------------------------------------------------------*/
void Vortex::vSkipTillSemicolon(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vTrace("Skipping: "_Stz);

  Token xTkn;
  while (bGetToken(xTkn)) {
    vTrace("%s "_Stz,xTkn.azTerm());
    if (xTkn.bSame(";"_Stz)) {
      vTrace("\n"_Stz,xTkn.azTerm());
      return;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* skip till ;                                                                */
/*----------------------------------------------------------------------------*/
void Vortex::vSkipTillSemicolon(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;
  
  vTrace("Skipping: %s "_Stz,szTerm->azChar());
  if (szTerm->bSame(";"_Stz)) {
    vTrace("\n"_Stz,szTerm->azChar());
    return;
  }
        
  while (bGetToken(xTkn)) {
    vTrace("%s "_Stz,xTkn.azTerm());
    if (xTkn.bSame(";"_Stz)) {
      vTrace("\n"_Stz,xTkn.azTerm());
      return;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* skip till ;                                                                */
/*----------------------------------------------------------------------------*/
void Vortex::vSkipUnrecognised(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;
  
  vTrace("Unrecognised: %s "_Stz,szTerm->azChar());
  if (szTerm->bSame(";"_Stz)) {
    vTrace("\n"_Stz,szTerm->azChar());
    return;
  }
        
  while (bGetToken(xTkn)) {
    vTrace("%s "_Stz,xTkn.azTerm());
    if (xTkn.bSame(";"_Stz)) {
      vTrace("\n"_Stz,xTkn.azTerm());
      return;
    }
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/




/*============================================================================*/
/*                                  REMOVE                                    */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/* process all classes in the current source file (.h or .hpp)                */
/*----------------------------------------------------------------------------*/
void Vortex::vProcAll(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Token xFirstTkn;
  while (bGetToken(xFirstTkn)) {                                          // 🌀

    if (bRecogniseClass(xFirstTkn.szTerm()))
      continue;

    if (bRecogniseConst(xFirstTkn.szTerm()))
      continue;
    
    if (bRecogniseFriend(xFirstTkn.szTerm()))
      continue;

    if (bRecogniseHash(xFirstTkn.szTerm()))
      continue;
    
    if (bRecogniseNamespace(xFirstTkn.szTerm()))
      continue;
    
    if (bRecogniseUsing(xFirstTkn.szTerm()))
      continue;
    
    vSkipTillSemicolon(xFirstTkn.szTerm());
    
  } // while

}

/*----------------------------------------------------------------------------*/
/* parse base (inheritence) clause of a class                                 */
/*----------------------------------------------------------------------------*/
void Vortex::vProcClassBase(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  // just saw: class <classNm :
  
  Token xTkn;
  while (bGetToken(xTkn)) {
    
    if (xTkn.bSame("{"_Stz)) {
      vProcClassBlock();
      break;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* parse class block, i.e. the { ... } sequence                               */
/*----------------------------------------------------------------------------*/
void Vortex::vProcClassBlock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  // just saw {
  
  vProcClassMembers();
}

/*----------------------------------------------------------------------------*/
/* parse class function                                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vProcClassFunction(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  // just saw {
  
  Token xTkn;
  while (bGetToken(xTkn)) {
    
    if (xTkn.bSame("}"_Stz))
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* parse all members of a class                                               */
/*----------------------------------------------------------------------------*/
void Vortex::vProcClassMembers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(0);
  

}

/*----------------------------------------------------------------------------*/
/* parse: static const i8 <constNm> ^_ = ... ;                                */
/*----------------------------------------------------------------------------*/
void Vortex::vProcConst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  // seen: static const i8
  
  m_oTerms.vReset();
  
  Token xConstNm;
  if (! bGetToken(xConstNm))
     return;

  Token xTkn;
  do {
    if (! bGetToken(xTkn))
      return;
    
  } while (! xTkn.bSame(";"_Stz));

  vTrace("Const %s\n"_Stz,xConstNm.azTerm());
  
  if (bCollectTerms(";"_Stz)) {
    ConstDef* pConstDef = m_p_ConsSet->pAddConstDef(xConstNm.szTerm());
    pConstDef->vTerms(m_oTerms);
  }
}

/*----------------------------------------------------------------------------*/
/* parse:                                                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vProcEnum(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);

}

/*----------------------------------------------------------------------------*/
/* parse:                                                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vProcHashDefine(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Token xTkn;

  RASSERT(bGetToken(xTkn));
}

/*----------------------------------------------------------------------------*/
/* parse:                                                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vProcHashInclude(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Token xTkn;

  RASSERT(bGetToken(xTkn));
}

/*----------------------------------------------------------------------------*/
/* parse:                                                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vProcHashPragma(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Token xTkn;

  RASSERT(bGetToken(xTkn));
}

/*----------------------------------------------------------------------------*/
/* parse single (optionally labelled) member                                  */
/*----------------------------------------------------------------------------*/
void Vortex::vProcLabelledMember(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  RASSERT(m_bInClass);
  
  Token xTkn;
  CHECKED(bGetToken(xTkn));

  if (bRecogniseStruct(xTkn.szTerm()))
    return;
    
  if (bRecogniseUnion(xTkn.szTerm()))
    return;

  if (! m_p_MemberSet)
    m_p_MemberSet = m_p_MetaClass->pAddMemberSet("linear"_Stz,m_p_MemberSet);
  
  vProcMemberProper(xTkn.szTerm());
  
}

/*----------------------------------------------------------------------------*/
/* parse member                                                               */
/*----------------------------------------------------------------------------*/
void Vortex::vProcMemberProper(Stz* szFirstTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Stz* szClassNm = m_p_MetaClass->szClassNm();
  
  vTrace("%lld.%lld) %s : "_Stz,
         m_pMetaGraph->nItems(),
         m_p_MemberSet->nItems(),
         szClassNm->azChar());
  
  //vTrace("%s "_Stz,xTkn.azChar());
  
  m_oTerms.vReset();

  vAddTerm(szFirstTerm);
  
  Token xTkn;
  while (bGetToken(xTkn)) {
    
    if (xTkn.bSame(";"_Stz)) {
      break;
    }
    
    if (xTkn.bSame("{"_Stz)) {
      vProcClassFunction();
      break ;
    }

    vAddTerm(xTkn.szTerm());
  }
    
  if (! MemberVar::bIsMemberVar(szClassNm,&m_oTerms)) {
    vTrace("  not a member variable\n"_Stz);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8   nPointers;
  bool bFunPtr;
  i8   nElements;
  i8   nBits;

  
  Stz* szVarNm;
  Stz* szTypeNm;
  szVarNm = MemberVar::szVarNm(&m_oTerms,szTypeNm,
                                nPointers,bFunPtr,nElements,nBits);
  
  m_p_MemberVar = m_p_MemberSet->pAddMemberVar(szVarNm);
  
  m_p_MemberVar->vTypeNm(szTypeNm);
  m_p_MemberVar->vPointers(nPointers);
  m_p_MemberVar->vFunPtr(bFunPtr);
  m_p_MemberVar->vElements(nElements);
  m_p_MemberVar->vBits(nBits);

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* process all classes in the current source file (.h or .hpp)                */
/*----------------------------------------------------------------------------*/
bool Vortex::bProcNamespace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Token xTkn;
  
  RASSERT(bGetToken(xTkn));
  RASSERT(xTkn.bSame("{"_Stz));
  
  while (bGetToken(xTkn)) {

    if (xTkn.bSame("}"_Stz))
      return true;
    
    if (bRecogniseClass(xTkn.szTerm()))
      continue;

    if (bRecogniseConst(xTkn.szTerm()))
      continue;
    
    if (bRecogniseFriend(xTkn.szTerm()))
      continue;

    if (bRecogniseHash(xTkn.szTerm()))
      continue;
    
    if (bRecogniseTemplate(xTkn.szTerm()))
      continue;
    
    // -- last --
    if (bRecogniseFunction(xTkn.szTerm()))
      continue;
    
    vSkipUnrecognised(xTkn.szTerm());
    
  } // while

  return false;
}

/*----------------------------------------------------------------------------*/
/* parse struct array                                                         */
/*----------------------------------------------------------------------------*/
void Vortex::vProcStructArray(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Token xTkn;
  RASSERT(bGetToken(xTkn));
  vAddTerm(xTkn.szTerm());
  
  RASSERT(bGetToken(xTkn));
  RASSERT(xTkn.bSame("]"_Stz));
  vAddTerm(xTkn.szTerm());

  RASSERT(bGetToken(xTkn));
  RASSERT(xTkn.bSame(";"_Stz));
  vAddTerm(xTkn.szTerm());
};

/*----------------------------------------------------------------------------*/
/* parse struct member                                                        */
/*----------------------------------------------------------------------------*/
void Vortex::vProcStructMember(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  // just saw: struct
  
  m_bTraceTokens = true;
  
  // may see name
  Token xTkn;
  RASSERT(bGetToken(xTkn));
  if (! xTkn.bSame("{"_Stz)) {
    RASSERT(bGetToken(xTkn));
    if (! xTkn.bSame("{"_Stz)) {
      RASSERT(bGetToken(xTkn));
      RASSERT(xTkn.bSame(";"_Stz));
      
      m_bTraceTokens = false;
      return;
    }
  }

  while (bGetToken(xTkn)) {

    if (xTkn.bSame("}"_Stz)) {
      RASSERT(bGetToken(xTkn));
      if (! xTkn.bSame(";"_Stz)) {
        RASSERT(bGetToken(xTkn));
        if (! xTkn.bSame(";"_Stz)) {
          vAddTerm(xTkn.szTerm());
          vProcStructArray();
        }
      }
      break;
    }
    
    vProcMemberProper(xTkn.szTerm());
  }
  
  m_bTraceTokens = false;
}

/*----------------------------------------------------------------------------*/
/* parse union member                                                         */
/*----------------------------------------------------------------------------*/
void Vortex::vProcUnionMember(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  // just saw: union
  
  //m_bTraceTokens = true;
  
  Token xTkn;
  RASSERT(bGetToken(xTkn));
  RASSERT(xTkn.bSame("{"_Stz));

  while (bGetToken(xTkn)) {

    if (xTkn.bSame("}"_Stz)) {
      RASSERT(bGetToken(xTkn));
      RASSERT(xTkn.bSame(";"_Stz));
      break;
    }
    
    vProcMemberProper(xTkn.szTerm());
  }
  
  //m_bTraceTokens = false;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: class ... ; ?                                            */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseClass(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(0);
  
  Token xTkn;
  Token xClassNm;


  if (! xTkn.bSame("class"_Stz))
    return false;
    
  if (! bGetToken(xClassNm))
    return false;

  if (! bGetToken(xTkn))
    return false;

  // -- -- -- -- -- --
  
  m_pOutputFile->vPrint("%s\n"_Stz,xClassNm.azTerm());
  
  m_p_MetaClass = m_pMetaGraph->pAddMetaClass(xClassNm.szTerm());
  if (xTkn.bSame(":"_Stz)) {
    m_p_MemberSet = nullptr;
    m_p_MemberVar = nullptr;
    vProcClassBase();
    return true;
  }
  
  if (xTkn.bSame("{"_Stz)) {
    m_p_MemberSet = nullptr;
    m_p_MemberVar = nullptr;
    vProcClassBlock();
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: static const i8 ... ; ?                                  */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseConst(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(0);
  
  Token xTkn;
  
  if (! szTerm->bSame("static"_Stz))
    return false;
      
  if (! bGetToken(xTkn))
    return false;
  if (! xTkn.bSame("const"_Stz))
    return false;

  if (! bGetToken(xTkn))
    return false;
  if (! xTkn.bSame("i8"_Stz))
    return false;

  vProcConst();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: enum ... ;                                               */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseEnum(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: friend ... ;                                             */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseFriend(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Token xTkn;
  
  if (! szTerm->bSame("friend"_Stz))
    return false;
        
  while (bGetToken(xTkn)) {
    if (xTkn.bSame(";"_Stz))
      break;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: function                                                 */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseFunction(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
    
  RASSERT(0);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: # ...                                                    */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseHash(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Token xTkn;
  
  if (! szTerm->bSame("#"_Stz))
    return false;
  
  RASSERT(bGetToken(xTkn));
  
  if (xTkn.bSame("define"_Stz)) {
    RASSERT(bGetToken(xTkn));
    return true;
  }

  if (xTkn.bSame("include"_Stz)) {
    RASSERT(bGetToken(xTkn));
    return true;
  }

  if (xTkn.bSame("pragma"_Stz)) {
    RASSERT(bGetToken(xTkn));
    RASSERT(xTkn.bSame("once"_Stz));
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: private|protected|public :                               */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseLabel(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Token xTkn;
  
  if (szTerm->bSame("private"_Stz)) {
    RASSERT(bGetToken(xTkn));
    RASSERT(xTkn.bSame(":"_Stz));
    return true;
  }
  
  if (szTerm->bSame("protected"_Stz)) {
    RASSERT(bGetToken(xTkn));
    RASSERT(xTkn.bSame(":"_Stz));
    return true;
  }
  
  if (szTerm->bSame("public"_Stz)) {
    RASSERT(bGetToken(xTkn));
    RASSERT(xTkn.bSame(":"_Stz));
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: namespace ...                                            */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseNamespace(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  if (! szTerm->bSame("namespace"_Stz))
    return false;

  Token xTkn;
  
  RASSERT(bGetToken(xTkn));
  
  m_sNamespaceNm = xTkn.szTerm();
  
  bool b = bProcNamespace();

  return b;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: struct {                                                 */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseStruct(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Token xTkn;
  
  if (! szTerm->bSame("struct"_Stz))
    return false;
  
  m_p_MemberSet = m_p_MetaClass->pAddMemberSet("struct"_Stz,m_p_MemberSet);
  vProcStructMember();
  m_p_MemberSet = nullptr;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: template ...                                             */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseTemplate(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  if (! szTerm->bSame("template"_Stz))
    return false;

  
  
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: union  {                                                 */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseUnion(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Token xTkn;
  
  if (! szTerm->bSame("union"_Stz))
    return false;
  
  m_p_MemberSet = m_p_MetaClass->pAddMemberSet("union"_Stz,m_p_MemberSet);
  vProcUnionMember();
  m_p_MemberSet = nullptr;

  return true;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: using namespace ...                                      */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseUsing(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Token xTkn;
  
  if (! szTerm->bSame("using"_Stz))
    return false;
  
  RASSERT(bGetToken(xTkn));
  RASSERT(xTkn.bSame("namespace"_Stz));

  RASSERT(bGetToken(xTkn));
  
  RASSERT(bGetToken(xTkn));
  RASSERT(xTkn.bSame(";"_Stz));

  return true;
}


