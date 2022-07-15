/*============================================================================*/
/* TextMedium.cpp -- Medium Text                                              */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TextMedium* BASE::_pTextMedium(void* aAddr)
{
  TextMedium* pText = reinterpret_cast<TextMedium*>(aAddr);
  
  RASSERT_TYPE(pText,CN_TEXTMEDIUM);
  
  return pText;
}

/*----------------------------------------------------------------------------*/
/* new a Text from the heap                                                   */
/*----------------------------------------------------------------------------*/
void* TextMedium::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CN_TEXTMEDIUM);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TextMedium::operator delete(void* pTextAddress)
{
  TextMedium* pTextMedium = reinterpret_cast<TextMedium*>(pTextAddress);
  
  Heaps::vFree(pTextMedium);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TextMedium::TextMedium(void)
: TextAbstract(CN_TEXTMEDIUM, C_TEXT_MEDIUM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXTMEDIUM);
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caText,C_SIZE_MEDIUM_TEXT,CN_TEXTMEDIUM);
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
  MemLib::vZero(m_caText,C_SIZE_MEDIUM_TEXT,CN_TEXTMEDIUM);
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
/* repair the text                                                            */
/*----------------------------------------------------------------------------*/
void TextMedium::vRepair(Object* pObject)
{
  _static_

  TextMedium* pTextMedium = _pTextMedium(pObject);

  delete pTextMedium;
}


/*----------------------------------------------------------------------------*/
/* get text to given value                                                    */
/*----------------------------------------------------------------------------*/
Stz* TextMedium::aGetText(void)
{
  return (Stz*)&m_caText[0];
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* TextMedium::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXT);

  return &m_xFooter;
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
