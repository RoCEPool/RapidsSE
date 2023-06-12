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
  m_p_VlTextBuffer = nullptr;

  _postcondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ExtendedText::~ExtendedText(void) noexcept(false)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  delete m_p_VlTextBuffer;

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
void ExtendedText::vAddSubStr(Stz* szSubStr)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  if (! m_p_VlTextBuffer) {
    m_p_VlTextBuffer = new ExtendedTextBuffer;
    m_nTextBlocks++;
  }

  bool b = m_p_VlTextBuffer->bAddSubStr(szSubStr);
  RASSERT(b);
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

  return m_p_VlTextBuffer->szTextBlock();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/