/*============================================================================*/
/* Vortex.cpp - buffer pool analyzer                                             */
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
    
  if (! m_pVortexFile)
    return;
  
  m_pVortexFile->vClose();
  delete m_pVortexFile;
  
  m_pVortexFile = nullptr;
}

/*----------------------------------------------------------------------------*/
/* get next token                                                             */
/*----------------------------------------------------------------------------*/
bool Vortex::bGetToken(Token* pToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  bool b = m_pInput->bGetToken(pToken);
  
  if (b && m_bTraceTokens)
    vTrace("Tkn: %s\n"_Stz,pToken->azChar());
  
  return b;
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
/* parse base clause                                                          */
/*----------------------------------------------------------------------------*/
void Vortex::vParseBase(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  // just saw :
  while (bGetToken(&m_xToken_Other)) {
    
    if (m_xToken_Other.bSame("{"_Stz)) {
      vParseClass();
      break;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* parse class                                                                */
/*----------------------------------------------------------------------------*/
void Vortex::vParseClass(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  // just saw {
  
  m_pVortexFile->vPrint("%s\n"_Stz,m_xToken_ClassNm.azChar());
  
  vParseMembers();
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
/* parse single member                                                        */
/*----------------------------------------------------------------------------*/
bool Vortex::bParseMember(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
 
  // just saw {

  for (i8 i_Token = 0; bGetToken(&m_xToken_Other); i_Token++) {

    if (0 == i_Token && m_xToken_Other.bSame(";"_Stz)) {
      break;
    }
    
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
    
    if (m_xToken_Other.bSame("union"_Stz)) {
      vParseMemberUnion();
    }

    
    if (m_xToken_Other.bSame("}"_Stz)) {
      RASSERT(bGetToken(&m_xToken_Other));
      vTrace("\n"_Stz);
      return false;
    }

    vParseMemberProper();
  }
    
  return false;
}

/*----------------------------------------------------------------------------*/
/* parse main portion of a member                                             */
/*----------------------------------------------------------------------------*/
void Vortex::vParseMemberProper(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vTrace("%s + "_Stz,m_xToken_ClassNm.azChar());
  vTrace("%s "_Stz,m_xToken_Other.azChar());

  i8 i_Token = 0;
  for (; bGetToken(&m_xToken_Other); i_Token++) {
    
    if (m_xToken_Other.bSame(";"_Stz)) {
      break;
    }
    
    if (m_xToken_Other.bSame("{"_Stz)) {
      vParseClassFunction();
      return ;
    }


    vTrace("%s "_Stz,m_xToken_Other.azChar());
  }
  
  if (i_Token)
    vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* parse main portion of a member                                             */
/*----------------------------------------------------------------------------*/
void Vortex::vParseMemberUnion(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  // just saw: union
  
  m_bTraceTokens = true;
  
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
  
  m_bTraceTokens = false;
}

/*----------------------------------------------------------------------------*/
/* parse all members                                                          */
/*----------------------------------------------------------------------------*/
void Vortex::vParseMembers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  // just saw {
  
  for (;;) {

    if (! bParseMember())
      break;
    
  }
  
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
  
  while (bGetToken(&m_xToken_Other)) {
   
    if (m_xToken_Other.bSame("friend"_Stz)) {
      
      while (bGetToken(&m_xToken_Other)) {
        if (m_xToken_Other.bSame(";"_Stz))
         break;
      }
      continue;
    }

    if (m_xToken_Other.bSame("class"_Stz)) {
      
      if (! bGetToken(&m_xToken_ClassNm))
        break;
      
      if (! bGetToken(&m_xToken_Other))
        break;
      
      if (m_xToken_Other.bSame("{"_Stz)) {
        vParseClass();
        continue;
      }
      
      if (m_xToken_Other.bSame(":"_Stz)) {
        vParseBase();
        continue;
      }
    }
  }

  delete m_pInput;
  m_pInput = nullptr;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
