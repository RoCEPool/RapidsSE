/*============================================================================*/
/* spVCH.cpp - Vch sprint helper                                              */
/*============================================================================*/

#include "Transform.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
spVCH::spVCH(void)
: Sprint(CN_SPVCH)
{
  _precondition_(_MAGIC_,CN_SPVCH,_FOOTER_);
  
  // set default enclosing char
  vFormat(F_VCH_enclose);

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caVch,sizeof(m_caVch),CN_SPVCH);
#else
  m_caVch[0] = '\0';
#endif
  
  _checkInitialised_(CN_SPVCH);
  
  _postcondition_(_MAGIC_,CN_SPVCH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
spVCH::~spVCH(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPVCH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* spVCH::sz(char* azVch)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  char* aDstChar = &m_caVch[0];
  char* aSrcChar = azVch;
  
  char  cEnclosing = m_caFormat[0];
  
  // put leading enclosing quote/char
  *aDstChar++ = cEnclosing;
  
  // copy from src to dst
  while (*aSrcChar) {
    
    if (*aSrcChar == cEnclosing) {
      // escape a quote/char
      *aDstChar++ = cEnclosing;
      *aDstChar++ = *aSrcChar++;
    }
    else {
      // copy an ordinary char
      *aDstChar++ = *aSrcChar++;
    }
  }
  
  // put trailing enclosing quote/char and \0 terminate
  *aDstChar++ = cEnclosing;
  *aDstChar   = '\0';

  return (Stz*) &m_caVch[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* spVCH::sz(Stz* szVch)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return sz((char*)szVch);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* spVCH::aFooter(void)
{
  _precondition_(_MAGIC_,CN_SPVCH);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
