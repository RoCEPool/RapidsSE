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
  m_pInput = nullptr;
    
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
/* process a source file (.h or .hpp)                                         */
/*----------------------------------------------------------------------------*/
void Vortex::vProcessSource(Stz* szSourcePath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  m_pInput = new Input;
  m_pInput->vPushFile(szSourcePath);
  
  Token m_xTokenNm;
  Token m_xTokenXyz;
  while (m_pInput->bGetToken(&m_xTokenXyz)) {
   
    if (m_xTokenXyz.bSame("friend"_Stz)) {
      
      while (m_pInput->bGetToken(&m_xTokenXyz)) {
        if (m_xTokenXyz.bSame(";"_Stz))
         break;
      }
      continue;
    }

    if (m_xTokenXyz.bSame("class"_Stz)) {
      
      if (! m_pInput->bGetToken(&m_xTokenNm))
        break;
      
      if (! m_pInput->bGetToken(&m_xTokenXyz))
        break;

      if (m_xTokenXyz.bSame(":"_Stz) || m_xTokenXyz.bSame("{"_Stz)) {
        //vTrace("  %s\n"_Stz,m_xTokenNm.azChar());
        m_pVortexFile->vPrint("%s\n"_Stz,m_xTokenNm.azChar());
      }
    }
  }

  delete m_pInput;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
