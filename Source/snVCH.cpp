/*============================================================================*/
/* snVCH.cpp - Vch scan helper                                                */
/*============================================================================*/

#include "Sprinting.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
snVCH::snVCH(void)
: Scan(CN_SNVCH)
{
  _precondition_(_MAGIC_,CN_SNVCH,_FOOTER_);
  
  // set default enclosing char
  vFormat(F_VCH_enclose);

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caVch,sizeof(m_caVch),CN_SNVCH);
#else
  m_caVch[0] = '\0';
#endif
  
  _checkInitialised_(CN_SNVCH);
  
  _postcondition_(_MAGIC_,CN_SNVCH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
snVCH::~snVCH(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SNVCH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* snVCH::sz(char* azVch)
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
Stz* snVCH::sz(Stz* szVch)
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
Footer* snVCH::aFooter(void)
{
  _precondition_(_MAGIC_,CN_SNVCH);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
