/*============================================================================*/
/* Text.cpp -- text (can be of varying sizes - tiny, small, medium, large)    */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,Text,CN_TEXT)
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
  
  vNewBuffer(256);

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

  vDeleteBuffer();
}

/*----------------------------------------------------------------------------*/
/* repair the text                                                            */
/*----------------------------------------------------------------------------*/
void Text::vRepair(Object* pObject)
{
  _static_

  Text* pText = _pText(pObject);

  delete pText;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* concat | operator                                                          */
/*----------------------------------------------------------------------------*/
Text& Text::operator |(Stz* szArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXT);
  
  s4K sRevised = szGet();

  sRevised.vConcat(szArg);
  
  vSet(sRevised);
  
  return *(Text*)this;
}

/*----------------------------------------------------------------------------*/
/* concat | operator                                                          */
/*----------------------------------------------------------------------------*/
Text& Text::operator |(Text* pArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXT);
  
  s4K sRevised = szGet();

  sRevised.vConcat(pArg->szGet());
  
  vSet(sRevised);
  
  return *(Text*)this;
}


/*----------------------------------------------------------------------------*/
/* set text to given value                                                    */
/*----------------------------------------------------------------------------*/
Stz* Text::szGet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXT);

  return m_pTextBuffer->szArea();
}

/*----------------------------------------------------------------------------*/
/* get the length of the text string                                          */
/*----------------------------------------------------------------------------*/
i8 Text::nLength(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXT);

  return m_nSizeText;
}

/*----------------------------------------------------------------------------*/
/* get the size of the underlying buffer                                      */
/*----------------------------------------------------------------------------*/
i8 Text::nBufferSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXT);

  return m_pTextBuffer->nBufferSize();
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
    
    // does the string fit the current buffer
    if ( nStringSize > m_pTextBuffer->nBufferSize())
      // - no
      vDeleteBuffer();
  }
  
  if ( ! m_pTextBuffer ) {
    // allocate a new buffer
         if (nStringSize < C_TEXT_TINY)   m_pTextBuffer = new TextTiny();
    else if (nStringSize < C_TEXT_SMALL)  m_pTextBuffer = new TextSmall();
    else if (nStringSize < C_TEXT_MEDIUM) m_pTextBuffer = new TextMedium();
    else if (nStringSize < C_TEXT_LARGE)  m_pTextBuffer = new TextLarge();
    else
      RASSERT(0);
  }

  // copy
  m_nSizeText = nStringSize;
  MemLib::vCopy(szGet(),szArg,nStringSize,CN_TEXT);
}

/*----------------------------------------------------------------------------*/
/* Revert a larger buffer back to the smallest one                            */
/*----------------------------------------------------------------------------*/
void Text::vRevertToTiny(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXT);

  if (!m_pTextBuffer) {
    return;
  }

  eCN eClass = m_pTextBuffer->eGetClassNo();
  switch (eClass) {
    case CN_TEXTTINY  :  return;
    case CN_TEXTSMALL :  break;
    case CN_TEXTMEDIUM:  break;
    case CN_TEXTLARGE :  break;
    default:
      NEVER_REACH;
  }

  vDeleteBuffer();
  vNewBuffer(0);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete current text buffer                                                 */
/*----------------------------------------------------------------------------*/
void Text::vDeleteBuffer(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TEXT);

  if (m_pTextBuffer) {

    eCN eClass = m_pTextBuffer->eGetClassNo();
    switch (eClass) {
      case CN_TEXTTINY  :  delete _pTextTiny(m_pTextBuffer);   break;
      case CN_TEXTSMALL :  delete _pTextSmall(m_pTextBuffer);  break;
      case CN_TEXTMEDIUM:  delete _pTextMedium(m_pTextBuffer); break;
      case CN_TEXTLARGE :  delete _pTextLarge(m_pTextBuffer);  break;
      default:
        NEVER_REACH;
    }
  }

  m_pTextBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/* allocate a suitably sized text buffer                                      */
/*----------------------------------------------------------------------------*/
void Text::vNewBuffer(i8 nStringSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXT);
  
  RASSERT(! m_pTextBuffer);
  
  // now allocate a new buffer
  if (nStringSize < C_TEXT_TINY)
    m_pTextBuffer = new TextTiny();
  
  else if (nStringSize < C_TEXT_SMALL)
    m_pTextBuffer = new TextSmall();

  else if (nStringSize < C_TEXT_MEDIUM)
    m_pTextBuffer = new TextMedium();

  else if (nStringSize < C_TEXT_LARGE)
    m_pTextBuffer = new TextLarge();
  
  else
    RASSERT(0);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
