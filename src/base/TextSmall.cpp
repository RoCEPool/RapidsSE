/*============================================================================*/
/* TextSmall.cpp -- Small Text                                                  */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TextSmall* BASE::_pTextSmall(void* aAddr)
{
  TextSmall* pText = reinterpret_cast<TextSmall*>(aAddr);
  
  RASSERT_TYPE(pText,CN_TEXTSMALL);
  
  return pText;
}

/*----------------------------------------------------------------------------*/
/* new a Text from the heap                                                   */
/*----------------------------------------------------------------------------*/
void* TextSmall::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CN_TEXTSMALL);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void TextSmall::operator delete(void* pTextAddress)
{
  TextSmall* pTextSmall = reinterpret_cast<TextSmall*>(pTextAddress);
  
  Heaps::vFree(pTextSmall);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TextSmall::TextSmall(void)
: TextBuffer(CN_TEXTSMALL, C_SIZE_SMALL_TEXT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXTSMALL);
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caText,C_SIZE_SMALL_TEXT,CN_TEXTSMALL);
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
Stz* TextSmall::aGetText(void)
{
  return (Stz*)&m_caText[0];
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* TextSmall::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXT);

  return &m_xFooter;
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
