/*============================================================================*/
/* TextLarge.cpp -- Tiny Text                                                  */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,TextLarge,CN_TEXTLARGE)
__aNewFunction(TextLarge,CN_TEXTLARGE)
__vDeleteFunction(TextLarge,CN_TEXTLARGE)
__aFooterFunction(TextLarge)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TextLarge::TextLarge(void)
: TextAbstract(CN_TEXTLARGE, C_TEXT_LARGE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXTLARGE);
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caText,C_TEXT_LARGE,CN_TEXTLARGE);
#else
  m_caText[0] = '\0';
#endif

  _checkInitialised_(C_TEXT_LARGE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TEXTLARGE);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TextLarge::~TextLarge(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXTLARGE);
}

/*----------------------------------------------------------------------------*/
/* get text to given value                                                    */
/*----------------------------------------------------------------------------*/
Stz* TextLarge::szArea(void)
{
  return (Stz*)&m_caText[0];
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
