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
  // m_xToken_Other
  
  m_bTraceTokens = false;
  m_nClasses     = 0;
  m_nMembers     = 0;

  m_nTerms       = 0;
  
  m_pMetaGraph     = new MetaGraph;
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
  
  vParseAllClasses();

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
void Vortex::vParseAllClasses(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  while (bGetToken(&m_xToken_Other)) {                                    // 🌀
   
    if (m_xToken_Other.bSame("friend"_Stz)) {
      
      while (bGetToken(&m_xToken_Other)) {
        if (m_xToken_Other.bSame(";"_Stz))
         break;
      }
      continue;
    }

    if (! m_xToken_Other.bSame("class"_Stz))
      continue;
      
    if (! bGetToken(&m_xToken_ClassNm))
      break;
    
    if (! bGetToken(&m_xToken_Other))
      break;

    // -- -- -- -- -- --
    
    if (m_xToken_Other.bSame(":"_Stz)) {
      m_pMetaClass_cur = m_pMetaGraph->pAddMetaClass(m_xToken_ClassNm.szTerm());
      vParseClassBase();
      continue;
    }
    
    if (m_xToken_Other.bSame("{"_Stz)) {
      m_pMetaClass_cur = m_pMetaGraph->pAddMetaClass(m_xToken_ClassNm.szTerm());
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
  
  while (bGetToken(&m_xToken_Other)) {
    
    if (m_xToken_Other.bSame("{"_Stz)) {
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
  
  while (bGetToken(&m_xToken_Other)) {
    
    if (m_xToken_Other.bSame("}"_Stz))
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
/* parse single (optionally labelled) member                                  */
/*----------------------------------------------------------------------------*/
bool Vortex::bParseLabelledMember(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
 
  // just saw {
  
  for (i8 i_Token = 0; bGetToken(&m_xToken_Other); i_Token++) {

    if (0 == i_Token && m_xToken_Other.bSame(";"_Stz))
      break;
    
    if (m_xToken_Other.bSame("private"_Stz)) {
      RASSERT(bGetToken(&m_xToken_Other));
      continue;
    }
    if (m_xToken_Other.bSame("protected"_Stz)) {
      RASSERT(bGetToken(&m_xToken_Other));
      continue;
    }
    if (m_xToken_Other.bSame("public"_Stz)) {
      RASSERT(bGetToken(&m_xToken_Other));
      continue;
    }
    
    if (m_xToken_Other.bSame("struct"_Stz)) {
      m_pMemberSet_cur = m_pMetaClass_cur->pAddMemberSet("struct"_Stz);
      vParseStructMember();
      m_pMemberSet_cur = nullptr;
      continue;
    }
    
    if (m_xToken_Other.bSame("union"_Stz)) {
      m_pMemberSet_cur = m_pMetaClass_cur->pAddMemberSet("union"_Stz);
      vParseUnionMember();
      m_pMemberSet_cur = nullptr;
      continue;
    }

    if (m_xToken_Other.bSame("}"_Stz)) {
      RASSERT(bGetToken(&m_xToken_Other));
      vTrace("\n"_Stz);
      return false;
    }

    if (! m_pMemberSet_cur)
      m_pMemberSet_cur = m_pMetaClass_cur->pAddMemberSet("linear"_Stz);
    vParseMemberProper();
  }
    
  return false;
}

/*----------------------------------------------------------------------------*/
/* parse member                                                               */
/*----------------------------------------------------------------------------*/
void Vortex::vParseMemberProper(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vTrace("%lld.%lld) %s : "_Stz,
         m_nClasses,
         m_nMembers++,
         m_xToken_ClassNm.azTerm());
  
  //vTrace("%s "_Stz,m_xToken_Other.azChar());
  
  m_nTerms = 0;
  m_oTerms.vReset();

  vAddTerm(m_xToken_Other.szTerm());
  
  while (bGetToken(&m_xToken_Other)) {
    
    if (m_xToken_Other.bSame(";"_Stz)) {
      break;
    }
    
    if (m_xToken_Other.bSame("{"_Stz)) {
      vParseClassFunction();
      break ;
    }

    vAddTerm(m_xToken_Other.szTerm());
  }
    
  if (! MemberVar::bIsMemberVar(m_xToken_ClassNm.szTerm(),&m_oTerms)) {
    vTrace("  not a member variable\n"_Stz);
    return;
  }
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* parse struct array                                                         */
/*----------------------------------------------------------------------------*/
void Vortex::vParseStructArray(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(bGetToken(&m_xToken_Other));
  vAddTerm(m_xToken_Other.szTerm());
  
  RASSERT(bGetToken(&m_xToken_Other));
  RASSERT(m_xToken_Other.bSame("]"_Stz));
  vAddTerm(m_xToken_Other.szTerm());

  RASSERT(bGetToken(&m_xToken_Other));
  RASSERT(m_xToken_Other.bSame(";"_Stz));
  vAddTerm(m_xToken_Other.szTerm());
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
  RASSERT(bGetToken(&m_xToken_Other));
  if (! m_xToken_Other.bSame("{"_Stz)) {
    RASSERT(bGetToken(&m_xToken_Other));
    if (! m_xToken_Other.bSame("{"_Stz)) {
      RASSERT(bGetToken(&m_xToken_Other));
      RASSERT(m_xToken_Other.bSame(";"_Stz));
      
      m_bTraceTokens = false;
      return;
    }
  }

  while (bGetToken(&m_xToken_Other)) {

    if (m_xToken_Other.bSame("}"_Stz)) {
      RASSERT(bGetToken(&m_xToken_Other));
      if (! m_xToken_Other.bSame(";"_Stz)) {
        RASSERT(bGetToken(&m_xToken_Other));
        if (! m_xToken_Other.bSame(";"_Stz)) {
          vAddTerm(m_xToken_Other.szTerm());
          vParseStructArray();
        }
      }
      break;
    }
    
    vParseMemberProper();
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
  
  RASSERT(bGetToken(&m_xToken_Other));
  RASSERT(m_xToken_Other.bSame("{"_Stz));

  while (bGetToken(&m_xToken_Other)) {

    if (m_xToken_Other.bSame("}"_Stz)) {
      RASSERT(bGetToken(&m_xToken_Other));
      RASSERT(m_xToken_Other.bSame(";"_Stz));
      break;
    }
    
    vParseMemberProper();
  }
  
  //m_bTraceTokens = false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
