/*============================================================================*/
/* TextMedium.cpp -- Medium Text                                              */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,TextMedium,CN_TEXTMEDIUM)
__aNewFunction(TextMedium,CN_TEXTMEDIUM)
__vDeleteFunction(TextMedium,CN_TEXTMEDIUM)
__aFooterFunction(TextMedium)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TextMedium::TextMedium(void)
: TextAbstract(CN_TEXTMEDIUM, C_TEXT_MEDIUM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXTMEDIUM);
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caText,C_TEXT_MEDIUM,CN_TEXTMEDIUM);
#else
  m_caText[0] = '\0';
#endif

  _checkInitialised_(CN_TEXTMEDIUM);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TEXTMEDIUM);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TextMedium::TextMedium(Stz* szText)
: TextAbstract(CN_TEXTMEDIUM, C_TEXT_MEDIUM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXTMEDIUM);
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caText,C_TEXT_MEDIUM,CN_TEXTMEDIUM);
#else
  m_caText[0] = '\0';
#endif
  
  StrLib::vCopyLen(m_caText,szText->azChar(),C_TEXT_MEDIUM-1);

  _checkInitialised_(CN_TEXTMEDIUM);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TEXTMEDIUM);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TextMedium::~TextMedium(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXTMEDIUM);
}

/*----------------------------------------------------------------------------*/
/* get text to given value                                                    */
/*----------------------------------------------------------------------------*/
Stz* TextMedium::szArea(void)
{
  return (Stz*)&m_caText[0];
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
