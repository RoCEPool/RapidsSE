/*============================================================================*/
/* TextLarge.cpp -- Tiny Text                                                  */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TextLarge* BASE::_pTextLarge(void* aAddr)
{
  TextLarge* pText = reinterpret_cast<TextLarge*>(aAddr);
  
  RASSERT_TYPE(pText,CN_TEXTLARGE);
  
  return pText;
}

/*----------------------------------------------------------------------------*/
/* new a Text from the heap                                                   */
/*----------------------------------------------------------------------------*/
void* TextLarge::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CN_TEXTLARGE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void TextLarge::operator delete(void* pTextAddress)
{
  TextLarge* pTextLarge = reinterpret_cast<TextLarge*>(pTextAddress);
  
  Heaps::vFree(pTextLarge);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TextLarge::TextLarge(void)
: TextBuffer(CN_TEXTLARGE, C_SIZE_LARGE_TEXT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXTLARGE);
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caText,C_SIZE_LARGE_TEXT,CN_TEXTLARGE);
#else
  m_caText[0] = '\0';
#endif

  _checkInitialised_(C_SIZE_LARGE_TEXT);
  
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
Stz* TextLarge::aGetText(void)
{
  return (Stz*)&m_caText[0];
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* TextLarge::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXTLARGE);

  return &m_xFooter;
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
