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
  m_pInput      = nullptr;
  // m_xToken_ClassNm
    
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
  Token xTokenXyz;
  while (m_pInput->bGetToken(&xTokenXyz)) {
    
    if (xTokenXyz.bSame("{"_Stz)) {
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
/* parse members                                                              */
/*----------------------------------------------------------------------------*/
void Vortex::vParseMembers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* process a source file (.h or .hpp)                                         */
/*----------------------------------------------------------------------------*/
void Vortex::vProcessSource(Stz* szSourcePath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  m_pInput = new Input;
  m_pInput->vPushFile(szSourcePath);
  
  Token xTokenXyz;
  while (m_pInput->bGetToken(&xTokenXyz)) {
   
    if (xTokenXyz.bSame("friend"_Stz)) {
      
      while (m_pInput->bGetToken(&xTokenXyz)) {
        if (xTokenXyz.bSame(";"_Stz))
         break;
      }
      continue;
    }

    if (xTokenXyz.bSame("class"_Stz)) {
      
      if (! m_pInput->bGetToken(&m_xToken_ClassNm))
        break;
      
      if (! m_pInput->bGetToken(&xTokenXyz))
        break;
      
      if (xTokenXyz.bSame("{"_Stz)) {
        vParseClass();
        continue;
      }
      
      if (xTokenXyz.bSame(":"_Stz)) {
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
