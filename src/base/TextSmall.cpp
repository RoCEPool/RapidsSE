/*============================================================================*/
/* TextSmall.cpp -- Small Text                                                */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,TextSmall,CN_TEXTSMALL)
__aNewFunction(TextSmall,CN_TEXTSMALL)
__vDeleteFunction(TextSmall,CN_TEXTSMALL)
__aFooterFunction(TextSmall)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TextSmall::TextSmall(void)
: TextAbstract(CN_TEXTSMALL, C_TEXT_SMALL)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXTSMALL);
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caText,C_TEXT_SMALL,CN_TEXTSMALL);
#else
  m_caText[0] = '\0';
#endif

  _checkInitialised_(CN_TEXTSMALL);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TEXTSMALL);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TextSmall::~TextSmall(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXTSMALL);
}

/*----------------------------------------------------------------------------*/
/* get text to given value                                                    */
/*----------------------------------------------------------------------------*/
Stz* TextSmall::szArea(void)
{
  return (Stz*)&m_caText[0];
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
