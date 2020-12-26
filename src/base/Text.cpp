/*============================================================================*/
/* Text.cpp -- text (can be of varying sizes - tiny, small, medium, large)    */
/*============================================================================*/

#include "Resources.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Resources,Text,CN_TEXT)
__aNewFunction(Text,CN_TEXT)
__vDeleteFunction(Text,CN_TEXT)
__aFooterFunction(Text)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Text::Text(void)
: Object(CN_TEXT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXT);
  
  m_nSizeText = 0;
  m_pTextBuffer = nullptr;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TEXT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Text::Text(i8 nStringSize)
: Object(CN_TEXT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXT);
  
  m_nSizeText   = 0;
  m_pTextBuffer = nullptr;
  
  vNewBuffer(nStringSize);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TEXT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Text::Text(Stz* szArg)
: Object(CN_TEXT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXT);
  
  m_nSizeText   = 0;
  m_pTextBuffer = nullptr;
  
  vSet(szArg);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TEXT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Text::~Text(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXT);
  
  if (m_pTextBuffer) {
    delete m_pTextBuffer;
  }
  
}

/*----------------------------------------------------------------------------*/
/* set text to given value                                                    */
/*----------------------------------------------------------------------------*/
Stz* Text::szGet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXT);

  return m_pTextBuffer->aGetText();
}

/*----------------------------------------------------------------------------*/
/* update text size                                                           */
/*----------------------------------------------------------------------------*/
void Text::vSizeText(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXT);
  
  Stz* szText = szGet();
  
  m_nSizeText = StrLib::nSize((char*)szText);
}

/*----------------------------------------------------------------------------*/
/* set text to given value                                                    */
/*----------------------------------------------------------------------------*/
void Text::vSet(Stz* szArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXT);
  
  i8 nStringSize = StrLib::nSize((char*)szArg);
  
  // is there a buffer ?
  if ( m_pTextBuffer ) {
    
    // does the string fit the currest buffer
    if ( nStringSize > m_pTextBuffer->nBufferSize()) {
    
      // delete the text buffer
      delete m_pTextBuffer;
      m_pTextBuffer = nullptr;
    }
  }
  
  if ( ! m_pTextBuffer ) {
    
    // now allocate a new buffer
    if (nStringSize < C_SIZE_TINY_TEXT)
      m_pTextBuffer = new TextTiny();
    
    else if (nStringSize < C_SIZE_SMALL_TEXT)
      m_pTextBuffer = new TextSmall();

    else if (nStringSize < C_SIZE_MEDIUM_TEXT)
      m_pTextBuffer = new TextMedium();

    else if (nStringSize < C_SIZE_LARGE_OBJECT)
      m_pTextBuffer = new TextLarge();
    
    else
      RASSERT(0);
  }
  
  m_nSizeText = nStringSize;
  MemLib::vCopy(szGet(),szArg,nStringSize,CN_TEXT);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete current text buffer                                                 */
/*----------------------------------------------------------------------------*/
void Text::vDeleteBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXT);
  
  ASSERT(m_pTextBuffer);

  delete m_pTextBuffer;
  m_pTextBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/* allocate a suitably sized text buffer                                      */
/*----------------------------------------------------------------------------*/
void Text::vNewBuffer(i8 nStringSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXT);
  
  ASSERT(! m_pTextBuffer);
  
  // now allocate a new buffer
  if (nStringSize < C_SIZE_TINY_TEXT)
    m_pTextBuffer = new TextTiny();
  
  else if (nStringSize < C_SIZE_SMALL_TEXT)
    m_pTextBuffer = new TextSmall();

  else if (nStringSize < C_SIZE_MEDIUM_TEXT)
    m_pTextBuffer = new TextMedium();

  else if (nStringSize < C_SIZE_LARGE_OBJECT)
    m_pTextBuffer = new TextLarge();
  
  else
    RASSERT(0);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
