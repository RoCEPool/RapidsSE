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
  
  m_pOutputFile = nullptr;
  m_sSourcePath = ""_Stz;
  m_pInput      = nullptr;
  
  m_pMetaGraph  = new MetaGraph;
  m_p_ConsSet   = m_pMetaGraph->pConstSet();
  m_p_MetaClass = nullptr;
  m_p_MemberSet = nullptr;
  m_p_MemberVar = nullptr;
  
  //m_oTerms
  m_bTraceTokens = false;
  
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

  m_sSourcePath = szSourcePath;
  m_pInput = new Input;
  m_pInput->vPushFile(szSourcePath);
  
  vParseAll();

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
  
  m_oTerms.vReset();
  
  Token xTkn;
  for (;;) {
    
    if (! bGetToken(&xTkn))
      return false;
    
    if (xTkn.bSame(szEnd)) {
      
      if (bAddEnd)
        m_oTerms.vAdd(xTkn.szTerm());
      
      return true;
    }
    
    m_oTerms.vAdd(xTkn.szTerm());
  }

}

/*----------------------------------------------------------------------------*/
/* get next token                                                             */
/*----------------------------------------------------------------------------*/
bool Vortex::bGetToken(Token* pToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  bool b = m_pInput->bGetToken(pToken);
  
  if (b && m_bTraceTokens)
    vTrace("Tkn: %s\n"_Stz,pToken->azTerm());
  
  return b;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* process all classes in the current source file (.h or .hpp)                */
/*----------------------------------------------------------------------------*/
void Vortex::vDetectAll(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;
  
  if (xTkn.bSame("class"_Stz))     {  }
  else
  if (xTkn.bSame("enum "_Stz))     {  }
  else
  if (xTkn.bSame("friend"_Stz))    {  }
  else
  if (xTkn.bSame("#"_Stz))         {  }
  else
  if (xTkn.bSame("namespace"_Stz)) {  }
  else
  if (xTkn.bSame("static"_Stz))    {  }
  else
  if (xTkn.bSame("using"_Stz))     {  }
  else {
    // otherwise
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* process all classes in the current source file (.h or .hpp)                */
/*----------------------------------------------------------------------------*/
void Vortex::vParseAll(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xFirstTkn;
  while (bGetToken(&xFirstTkn)) {                                          // 🌀

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
void Vortex::vParseClassBase(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  // just saw: class <classNm :
  
  Token xTkn;
  while (bGetToken(&xTkn)) {
    
    if (xTkn.bSame("{"_Stz)) {
      vParseClassBlock();
      break;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* parse class block, i.e. the { ... } sequence                               */
/*----------------------------------------------------------------------------*/
void Vortex::vParseClassBlock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  // just saw {
  
  vParseClassMembers();
}

/*----------------------------------------------------------------------------*/
/* parse class function                                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vParseClassFunction(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  // just saw {
  
  Token xTkn;
  while (bGetToken(&xTkn)) {
    
    if (xTkn.bSame("}"_Stz))
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* parse all members of a class                                               */
/*----------------------------------------------------------------------------*/
void Vortex::vParseClassMembers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  // just saw {
  
  vTrace("File %s\n"_Stz,m_sSourcePath.azChar());
  
  for (;;) {

    if (! bParseLabelledMember())
      break;
  }
  
}

/*----------------------------------------------------------------------------*/
/* parse: static const i8 <constNm> ^_ = ... ;                                */
/*----------------------------------------------------------------------------*/
void Vortex::vParseConst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  // seen: static const i8
  
  m_oTerms.vReset();
  
  Token xConstNm;
  if (! bGetToken(&xConstNm))
     return;

  Token xTkn;
  do {
    if (! bGetToken(&xTkn))
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
void Vortex::vParseEnum(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* parse:                                                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHashDefine(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;

  RASSERT(bGetToken(&xTkn));
}

/*----------------------------------------------------------------------------*/
/* parse:                                                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHashInclude(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;

  RASSERT(bGetToken(&xTkn));
}

/*----------------------------------------------------------------------------*/
/* parse:                                                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHashPragma(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;

  RASSERT(bGetToken(&xTkn));
}

/*----------------------------------------------------------------------------*/
/* parse single (optionally labelled) member                                  */
/*----------------------------------------------------------------------------*/
bool Vortex::bParseLabelledMember(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
 
  // just saw {
  
  Token xTkn;
  for (i8 i_Token = 0; bGetToken(&xTkn); i_Token++) {

    if (0 == i_Token && xTkn.bSame(";"_Stz))
      break;
    
    if (xTkn.bSame("}"_Stz)) {
      RASSERT(bGetToken(&xTkn));
      vTrace("\n"_Stz);
      return false;
    }
    
    if (bRecogniseLabel(xTkn.szTerm()))
      continue;
        
    if (bRecogniseStruct(xTkn.szTerm()))
      continue;
    
    if (bRecogniseUnion(xTkn.szTerm()))
      continue;

    if (! m_p_MemberSet)
      m_p_MemberSet = m_p_MetaClass->pAddMemberSet("linear"_Stz,m_p_MemberSet);
    vParseMemberProper(xTkn.szTerm());
  }
    
  return false;
}

/*----------------------------------------------------------------------------*/
/* parse member                                                               */
/*----------------------------------------------------------------------------*/
void Vortex::vParseMemberProper(Stz* szFirstTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Stz* szClassNm = m_p_MetaClass->szClassNm();
  
  vTrace("%lld.%lld) %s : "_Stz,
         m_pMetaGraph->nItems(),
         m_p_MemberSet->nItems(),
         szClassNm->azChar());
  
  //vTrace("%s "_Stz,xTkn.azChar());
  
  m_oTerms.vReset();

  vAddTerm(szFirstTerm);
  
  Token xTkn;
  while (bGetToken(&xTkn)) {
    
    if (xTkn.bSame(";"_Stz)) {
      break;
    }
    
    if (xTkn.bSame("{"_Stz)) {
      vParseClassFunction();
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
bool Vortex::bParseNamespace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;
  
  RASSERT(bGetToken(&xTkn));
  RASSERT(xTkn.bSame("{"_Stz));
  
  while (bGetToken(&xTkn)) {

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
void Vortex::vParseStructArray(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;
  RASSERT(bGetToken(&xTkn));
  vAddTerm(xTkn.szTerm());
  
  RASSERT(bGetToken(&xTkn));
  RASSERT(xTkn.bSame("]"_Stz));
  vAddTerm(xTkn.szTerm());

  RASSERT(bGetToken(&xTkn));
  RASSERT(xTkn.bSame(";"_Stz));
  vAddTerm(xTkn.szTerm());
};

/*----------------------------------------------------------------------------*/
/* parse struct member                                                        */
/*----------------------------------------------------------------------------*/
void Vortex::vParseStructMember(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  // just saw: struct
  
  m_bTraceTokens = true;
  
  // may see name
  Token xTkn;
  RASSERT(bGetToken(&xTkn));
  if (! xTkn.bSame("{"_Stz)) {
    RASSERT(bGetToken(&xTkn));
    if (! xTkn.bSame("{"_Stz)) {
      RASSERT(bGetToken(&xTkn));
      RASSERT(xTkn.bSame(";"_Stz));
      
      m_bTraceTokens = false;
      return;
    }
  }

  while (bGetToken(&xTkn)) {

    if (xTkn.bSame("}"_Stz)) {
      RASSERT(bGetToken(&xTkn));
      if (! xTkn.bSame(";"_Stz)) {
        RASSERT(bGetToken(&xTkn));
        if (! xTkn.bSame(";"_Stz)) {
          vAddTerm(xTkn.szTerm());
          vParseStructArray();
        }
      }
      break;
    }
    
    vParseMemberProper(xTkn.szTerm());
  }
  
  m_bTraceTokens = false;
}

/*----------------------------------------------------------------------------*/
/* parse union member                                                         */
/*----------------------------------------------------------------------------*/
void Vortex::vParseUnionMember(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  // just saw: union
  
  //m_bTraceTokens = true;
  
  Token xTkn;
  RASSERT(bGetToken(&xTkn));
  RASSERT(xTkn.bSame("{"_Stz));

  while (bGetToken(&xTkn)) {

    if (xTkn.bSame("}"_Stz)) {
      RASSERT(bGetToken(&xTkn));
      RASSERT(xTkn.bSame(";"_Stz));
      break;
    }
    
    vParseMemberProper(xTkn.szTerm());
  }
  
  //m_bTraceTokens = false;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: class ... ; ?                                            */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseClass(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  Token xTkn;
  Token xClassNm;


  if (! xTkn.bSame("class"_Stz))
    return false;
    
  if (! bGetToken(&xClassNm))
    return false;

  if (! bGetToken(&xTkn))
    return false;

  // -- -- -- -- -- --
  
  m_pOutputFile->vPrint("%s\n"_Stz,xClassNm.azTerm());
  
  m_p_MetaClass = m_pMetaGraph->pAddMetaClass(xClassNm.szTerm());
  if (xTkn.bSame(":"_Stz)) {
    m_p_MemberSet = nullptr;
    m_p_MemberVar = nullptr;
    vParseClassBase();
    return true;
  }
  
  if (xTkn.bSame("{"_Stz)) {
    m_p_MemberSet = nullptr;
    m_p_MemberVar = nullptr;
    vParseClassBlock();
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

  Token xTkn;
  
  if (! szTerm->bSame("static"_Stz))
    return false;
      
  if (! bGetToken(&xTkn))
    return false;
  if (! xTkn.bSame("const"_Stz))
    return false;

  if (! bGetToken(&xTkn))
    return false;
  if (! xTkn.bSame("i8"_Stz))
    return false;

  vParseConst();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: enum ... ;                                               */ 
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseEnum(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: friend ... ;                                             */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseFriend(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;
  
  if (! szTerm->bSame("friend"_Stz))
    return false;
        
  while (bGetToken(&xTkn)) {
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
    
  return true;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: # ...                                                    */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseHash(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;
  
  if (! szTerm->bSame("#"_Stz))
    return false;
  
  RASSERT(bGetToken(&xTkn));
  
  if (xTkn.bSame("define"_Stz)) {
    RASSERT(bGetToken(&xTkn));
    return true;
  }

  if (xTkn.bSame("include"_Stz)) {
    RASSERT(bGetToken(&xTkn));
    return true;
  }

  if (xTkn.bSame("pragma"_Stz)) {
    RASSERT(bGetToken(&xTkn));
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
  
  Token xTkn;
  
  if (szTerm->bSame("private"_Stz)) {
    RASSERT(bGetToken(&xTkn));
    RASSERT(xTkn.bSame(":"_Stz));
    return true;
  }
  
  if (szTerm->bSame("protected"_Stz)) {
    RASSERT(bGetToken(&xTkn));
    RASSERT(xTkn.bSame(":"_Stz));
    return true;
  }
  
  if (szTerm->bSame("public"_Stz)) {
    RASSERT(bGetToken(&xTkn));
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
  
  if (! szTerm->bSame("namespace"_Stz))
    return false;

  Token xTkn;
  
  RASSERT(bGetToken(&xTkn));
  
  m_sNamespaceNm = xTkn.szTerm();
  
  bool b = bParseNamespace();

  return b;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: struct {                                                 */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseStruct(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;
  
  if (! szTerm->bSame("struct"_Stz))
    return false;
  
  m_p_MemberSet = m_p_MetaClass->pAddMemberSet("struct"_Stz,m_p_MemberSet);
  vParseStructMember();
  m_p_MemberSet = nullptr;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: template ...                                             */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseTemplate(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
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
  
  Token xTkn;
  
  if (! szTerm->bSame("union"_Stz))
    return false;
  
  m_p_MemberSet = m_p_MetaClass->pAddMemberSet("union"_Stz,m_p_MemberSet);
  vParseUnionMember();
  m_p_MemberSet = nullptr;

  return true;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: using namespace ...                                      */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseUsing(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;
  
  if (! szTerm->bSame("using"_Stz))
    return false;
  
  RASSERT(bGetToken(&xTkn));
  RASSERT(xTkn.bSame("namespace"_Stz));

  RASSERT(bGetToken(&xTkn));
  
  RASSERT(bGetToken(&xTkn));
  RASSERT(xTkn.bSame(";"_Stz));

  return true;
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
        
  while (bGetToken(&xTkn)) {
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
        
  while (bGetToken(&xTkn)) {
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
