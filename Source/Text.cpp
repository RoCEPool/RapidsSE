/*============================================================================*/
/* String_t.cpp -- fixed-length string variable (template)                    */
/*============================================================================*/

#include "Structures.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Text* Structures::_pText(void* aAddr)
{
  Text* pText = reinterpret_cast<Text*>(aAddr);
  
  RASSERT_TYPE(pText,CN_TEXT);
  
  return pText;
}

/*----------------------------------------------------------------------------*/
/* new a Text from the heap                                                   */
/*----------------------------------------------------------------------------*/
void* Text::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_TEXT);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Text::operator delete(void* pTextAddress)
{
  Text* pText = reinterpret_cast<Text*>(pTextAddress);
  
  HeapMem::vFree(pText);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Text::Text(void)
: Object(CN_TEXT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXT);
  
  m_nSizeText = 0;
  m_aTextBuffer = nullptr;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TEXT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Text::~Text(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXT);
  
  switch (m_aTextBuffer->nBufferSize()) {
      
    case C_SIZE_LARGE_OBJECT:   break;
      
    case C_SIZE_MEDIUM_OBJECT:  break;
      
    case C_SIZE_SMALL_OBJECT:   break;
      
    case C_SIZE_TINY_OBJECT: {
      TextTiny* pTextTiny = _pTextTiny(m_aTextBuffer);
      delete pTextTiny;
      break;
    }
      
    default:
      RASSERT(0);
    
  }

}

/*----------------------------------------------------------------------------*/
/* set text to given value                                                    */
/*----------------------------------------------------------------------------*/
Stz* Text::aGet(void)
{
  return m_aTextBuffer->aGetText();
}

/*----------------------------------------------------------------------------*/
/* set text to given value                                                    */
/*----------------------------------------------------------------------------*/
void Text::vSet(Stz* aString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXT);
  
  i8 nStringSize = StrLib::nSize((char*)aString);
  
  // is there a buffer ?
  if ( m_aTextBuffer ) {
    
    // does the string fit the currest buffer
    if ( nStringSize > m_aTextBuffer->nBufferSize()) {
    
      // delete the text buffer
      delete m_aTextBuffer;
      m_aTextBuffer = nullptr;
    }
  }
  
  if ( ! m_aTextBuffer ) {
    // now allocate a new buffer
    if (nStringSize < C_SIZE_TINY_OBJECT)
      m_aTextBuffer = new TextTiny();
    
    else if (nStringSize < C_SIZE_SMALL_OBJECT)
      m_aTextBuffer = new TextSmall();

    else if (nStringSize < C_SIZE_MEDIUM_OBJECT)
      m_aTextBuffer = new TextMedium();

    else if (nStringSize < C_SIZE_LARGE_OBJECT)
      m_aTextBuffer = new TextLarge();
    
    else
      RASSERT(0);
  }
  m_nSizeText = nStringSize;
  MemLib::vCopy(m_aTextBuffer->aGetText(),aString,nStringSize,CN_TEXT);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Text::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXT);
  
  return &m_xFooter;
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
