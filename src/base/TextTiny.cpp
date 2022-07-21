/*============================================================================*/
/* TextTiny.cpp -- Tiny Text                                                  */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TextTiny* BASE::_pTextTiny(void* aAddr)
{
  TextTiny* pText = reinterpret_cast<TextTiny*>(aAddr);
  
  RASSERT_TYPE(pText,CN_TEXTTINY);
  
  return pText;
}

/*----------------------------------------------------------------------------*/
/* new a Text from the heap                                                   */
/*----------------------------------------------------------------------------*/
void* TextTiny::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CN_TEXTTINY);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TextTiny::operator delete(void* pTextAddress)
{
  TextTiny* pTextTiny = reinterpret_cast<TextTiny*>(pTextAddress);
  
  Heaps::vFree(pTextTiny);
}

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

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* TextTiny::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXT);

  return &m_xFooter;
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
