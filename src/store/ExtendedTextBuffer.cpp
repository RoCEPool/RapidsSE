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

  _checkInitialised_(CN_EXTENDEDTEXTBUFFER);

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

  ASSERT(szSubStr->nSize() <= m_sTextBlock.nSpace());

  bool b = m_sTextBlock.bConcat(szSubStr);

  _postcondition_(_MAGIC_, CN_EXTENDEDTEXTBUFFER, _FOOTER_);

  return b;
}

/*----------------------------------------------------------------------------*/
/* get text block                                                             */
/*----------------------------------------------------------------------------*/
Stz* ExtendedTextBuffer::szTextBlock(void)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXTBUFFER, _FOOTER_);

  return m_sTextBlock.szStz();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/