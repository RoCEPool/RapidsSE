/*============================================================================*/
/* TextTiny.cpp -- Tiny Text                                                  */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,TextTiny,CN_TEXTTINY)
__aNewFunction(TextTiny,CN_TEXTTINY)
__vDeleteFunction(TextTiny,CN_TEXTTINY)
__aFooterFunction(TextTiny)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TextTiny::TextTiny(void)
: TextAbstract(CN_TEXTTINY, C_TEXT_TINY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXTTINY);
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caText,C_TEXT_TINY,CN_TEXTTINY);
#else
  m_caText[0] = '\0';
#endif

  _checkInitialised_(CN_TEXTTINY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TEXTTINY);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TextTiny::~TextTiny(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXTTINY);
}

/*----------------------------------------------------------------------------*/
/* get text to given value                                                    */
/*----------------------------------------------------------------------------*/
Stz* TextTiny::szArea(void)
{
  return (Stz*)&m_caText[0];
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
