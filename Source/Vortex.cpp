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
  
  m_pVortexFile = nullptr;
  m_sSourcePath = ""_Stz;
  m_pInput      = nullptr;
  // m_xToken_ClassNm
  // xTkn
  
  m_bTraceTokens = false;
  m_nClasses     = 0;
  m_nMembers     = 0;

  m_nTerms       = 0;
  
  m_pMetaGraph     = new MetaGraph;
  m_pConsSet       = m_pMetaGraph->pConstSet();
  m_pMetaClass_cur = nullptr;
  m_pMemberSet_cur = nullptr;
  m_pMemberVar_cur = nullptr;
  
  _checkInitialised_(CN_VORTEX);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Vortex::~Vortex(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  delete m_pVortexFile;
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
  
  if (m_pVortexFile && m_pVortexFile->bIsOpen())
    m_pVortexFile->vClose();
  
  delete m_pVortexFile;
  m_pVortexFile = nullptr;
}

/*----------------------------------------------------------------------------*/
/* open the vortex                                                            */
/*----------------------------------------------------------------------------*/
bool Vortex::bOpen(Stz* szVortexPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
    
  RASSERT(! m_pVortexFile);
  
  m_pVortexFile = new File(nullptr);
  
  bool b = m_pVortexFile->bOpenWrite(szVortexPath);
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
  m_nTerms++;
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
/* process all classes in the current source file (.h or .hpp)                */
/*----------------------------------------------------------------------------*/
void Vortex::vParseAll(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;

  while (bGetToken(&xTkn)) {                                    // 🌀

    if (xTkn.bSame("static"_Stz)) {
      
     Token xTkn;
     if (! bGetToken(&xTkn))
        break;
     if (! xTkn.bSame("const"_Stz))
        break;
     if (! bGetToken(&xTkn))
        break;
     if (! xTkn.bSame("i8"_Stz))
        break;
       
      vParseConst();
      continue;
    }
    
    if (xTkn.bSame("friend"_Stz)) {
      
      while (bGetToken(&xTkn)) {
        if (xTkn.bSame(";"_Stz))
         break;
      }
      continue;
    }

    if (! xTkn.bSame("class"_Stz))
      continue;
      
    if (! bGetToken(&m_xToken_ClassNm))
      break;
    
    if (! bGetToken(&xTkn))
      break;

    // -- -- -- -- -- --
    
    if (xTkn.bSame(":"_Stz)) {
      m_pMetaClass_cur = m_pMetaGraph->pAddMetaClass(m_xToken_ClassNm.szTerm());
      m_pMemberSet_cur = nullptr;
      m_pMemberVar_cur = nullptr;
      vParseClassBase();
      continue;
    }
    
    if (xTkn.bSame("{"_Stz)) {
      m_pMetaClass_cur = m_pMetaGraph->pAddMetaClass(m_xToken_ClassNm.szTerm());
      m_pMemberSet_cur = nullptr;
      m_pMemberVar_cur = nullptr;
      vParseClassBlock();
      continue;
    }
    
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
  
  m_pVortexFile->vPrint("%s\n"_Stz,m_xToken_ClassNm.azTerm());
  m_nClasses++;
  
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
  
  m_nMembers = 0;
  
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
    ConstDef* pConstDef = m_pConsSet->pAddConstDef(xConstNm.szTerm());
    pConstDef->vTerms(m_oTerms);
  }
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
    
    if (xTkn.bSame("private"_Stz)) {
      RASSERT(bGetToken(&xTkn));
      RASSERT(xTkn.bSame(":"_Stz));
      continue;
    }
    if (xTkn.bSame("protected"_Stz)) {
      RASSERT(bGetToken(&xTkn));
      RASSERT(xTkn.bSame(":"_Stz));
      continue;
    }
    if (xTkn.bSame("public"_Stz)) {
      RASSERT(bGetToken(&xTkn));
      RASSERT(xTkn.bSame(":"_Stz));
      continue;
    }
    
    if (xTkn.bSame("struct"_Stz)) {
      m_pMemberSet_cur = m_pMetaClass_cur->pAddMemberSet("struct"_Stz,m_pMemberSet_cur);
      vParseStructMember();
      m_pMemberSet_cur = nullptr;
      continue;
    }
    
    if (xTkn.bSame("union"_Stz)) {
      m_pMemberSet_cur = m_pMetaClass_cur->pAddMemberSet("union"_Stz,m_pMemberSet_cur);
      vParseUnionMember();
      m_pMemberSet_cur = nullptr;
      continue;
    }

    if (xTkn.bSame("}"_Stz)) {
      RASSERT(bGetToken(&xTkn));
      vTrace("\n"_Stz);
      return false;
    }

    if (! m_pMemberSet_cur)
      m_pMemberSet_cur = m_pMetaClass_cur->pAddMemberSet("linear"_Stz,m_pMemberSet_cur);
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
  
  vTrace("%lld.%lld) %s : "_Stz,
         m_nClasses,
         m_nMembers++,
         m_xToken_ClassNm.azTerm());
  
  //vTrace("%s "_Stz,xTkn.azChar());
  
  m_nTerms = 0;
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
    
  if (! MemberVar::bIsMemberVar(m_xToken_ClassNm.szTerm(),&m_oTerms)) {
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
  
  m_pMemberVar_cur = m_pMemberSet_cur->pAddMemberVar(szVarNm);
  
  m_pMemberVar_cur->vTypeNm(szTypeNm);
  m_pMemberVar_cur->vPointers(nPointers);
  m_pMemberVar_cur->vFunPtr(bFunPtr);
  m_pMemberVar_cur->vElements(nElements);
  m_pMemberVar_cur->vBits(nBits);

  vDebugPoint();
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
