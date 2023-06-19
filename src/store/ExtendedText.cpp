/*============================================================================*/
/* ExtendedText.cpp --  Extended (multi-nuffer) Text                          */
/*============================================================================*/

#include "ExtendedText.hpp"

/*----------------------------------------------------------------------------*/
/*-- Lifecycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, ExtendedText, CN_EXTENDEDTEXT)
__aNewFunction(ExtendedText, CN_EXTENDEDTEXT)
__vDeleteFunction(ExtendedText, CN_EXTENDEDTEXT)
__aFooterFunction(ExtendedText)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ExtendedText::ExtendedText(void)
: Object(CN_EXTENDEDTEXT)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  m_nTextBlocks = 0;
  m_pExtTextBuffer = nullptr;

  _postcondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ExtendedText::~ExtendedText(void) noexcept(false)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  delete m_pExtTextBuffer;

  _postcondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add sub-string to text                                                     */
/*----------------------------------------------------------------------------*/
void ExtendedText::vConcat(Stz* szSubStr)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  if (! m_pExtTextBuffer) {
    m_pExtTextBuffer = new ExtendedTextBuffer;
    m_nTextBlocks++;
  }

  bool b = m_pExtTextBuffer->bConcat(szSubStr);
  RASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* copy given extended text argument                                          */
/*----------------------------------------------------------------------------*/
void ExtendedText::vCopy(ExtendedText* pExtText_arg)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  vEmpty();

  if (pExtText_arg->nTextBlocks()) {
    vConcat(pExtText_arg->szTextBlock(0));
  }
}

/*----------------------------------------------------------------------------*/
/* copy given extended text argument                                          */
/*----------------------------------------------------------------------------*/
void ExtendedText::vCopy(Stz* szStr)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  vEmpty();

  vConcat(szStr);
}


/*----------------------------------------------------------------------------*/
/* empty this extended text                                                   */
/*----------------------------------------------------------------------------*/
void ExtendedText::vEmpty(void)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  if (m_pExtTextBuffer)
    delete m_pExtTextBuffer;

  m_pExtTextBuffer = nullptr;
  m_nTextBlocks = 0;
}

/*----------------------------------------------------------------------------*/
/* get extended text buffer                                                   */
/*----------------------------------------------------------------------------*/
ExtendedTextBuffer* ExtendedText::pExtTextBuffer(i8 i_TextBuffer)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  return m_pExtTextBuffer;
}

/*----------------------------------------------------------------------------*/
/* set extended text buffer                                                   */
/*----------------------------------------------------------------------------*/
void ExtendedText::vExtTextBuffer(i8 i_TextBuffer, ExtendedTextBuffer* pExtTextBuffer)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

   m_pExtTextBuffer = pExtTextBuffer;
}

/*----------------------------------------------------------------------------*/
/* retrieve text as a single string (false if it doesn't fit)                 */
/*----------------------------------------------------------------------------*/
bool ExtendedText::bRetrieve(String* pString)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  pString->vCopy(szTextBlock(0));

  return true;
}

/*----------------------------------------------------------------------------*/
/* add sub-string to text                                                     */
/*----------------------------------------------------------------------------*/
i8 ExtendedText::nTextBlocks(void)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  return m_nTextBlocks;
}

/*----------------------------------------------------------------------------*/
/* get a text block                                                           */
/*----------------------------------------------------------------------------*/
Stz* ExtendedText::szTextBlock(i8 i_TextBlock)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  return m_pExtTextBuffer->szTextBlock();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/