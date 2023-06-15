/*============================================================================*/
/* ExtendedBufferBuffer.cpp -- Extended Text Buffer                           */
/*============================================================================*/

#include "ExtendedTextBuffer.hpp"

/*----------------------------------------------------------------------------*/
/*-- Lifecycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, ExtendedTextBuffer, CN_EXTENDEDTEXTBUFFER)
__aNewFunction(ExtendedTextBuffer, CN_EXTENDEDTEXTBUFFER)
__vDeleteFunction(ExtendedTextBuffer, CN_EXTENDEDTEXTBUFFER)
__aFooterFunction(ExtendedTextBuffer)



/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ExtendedTextBuffer::ExtendedTextBuffer(void)
: Object(CN_EXTENDEDTEXTBUFFER)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXTBUFFER, _FOOTER_);

  MemLib::vZero(m_acTextBlock,sizeof(m_acTextBlock),CN_EXTENDEDTEXTBUFFER);

  _postcondition_(_MAGIC_, CN_EXTENDEDTEXTBUFFER, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ExtendedTextBuffer::~ExtendedTextBuffer(void) noexcept(false)

{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXTBUFFER, _FOOTER_);

  _postcondition_(_MAGIC_, CN_EXTENDEDTEXTBUFFER, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* concat sub string to text                                                  */
/*----------------------------------------------------------------------------*/
bool ExtendedTextBuffer::bConcat(Stz* szSubStr)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXTBUFFER, _FOOTER_);

  StrLib::vConcat(m_acTextBlock,szSubStr->azChar());

  _postcondition_(_MAGIC_, CN_EXTENDEDTEXTBUFFER, _FOOTER_);

  return true;
}

/*----------------------------------------------------------------------------*/
/* get text block                                                             */
/*----------------------------------------------------------------------------*/
Stz* ExtendedTextBuffer::szTextBlock(void)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXTBUFFER, _FOOTER_);

  return (Stz*)m_acTextBlock;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/