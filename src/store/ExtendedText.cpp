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
  for (i8 i_Block = 0; i_Block < C_EXTENDEDTEXTBUFFER; i_Block++)
    m_paExtTextBuffer[i_Block] = nullptr;

  _checkInitialised_(CN_EXTENDEDTEXT);

  _postcondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ExtendedText::~ExtendedText(void) noexcept(false)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  for (i8 i_Block = 0; i_Block < m_nTextBlocks; i_Block++) {
    delete m_paExtTextBuffer[i_Block];
    m_paExtTextBuffer[i_Block] = nullptr;
  }
  m_nTextBlocks = 0;

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
void ExtendedText::vConcat2(Stz* szSubStr)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  if (szSubStr->nSize() > C_4K) {
    vTrace("%s : %lld\n"_Stz,__func__ ,szSubStr->nSize());
    vDebugPoint();
  }
  // at least a first extended text buffer
  vEnsureBuffer();

  // get tail buffer
  ExtendedTextBuffer* pExtTextBuffer = pTailBuffer();

  bool b = pExtTextBuffer->bConcat(szSubStr);

  if (!b) {
    b = bAddBuffer();
    if (b) {
      pExtTextBuffer = pTailBuffer();
      b = pExtTextBuffer->bConcat(szSubStr);
    }
  }

  RASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* add sub-string to text                                                     */
/*----------------------------------------------------------------------------*/
void ExtendedText::vConcat(Stz* szSubStr)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  if (szSubStr->nSize() > C_4K) {
    vTrace("%s : %lld\n"_Stz,__func__ ,szSubStr->nSize());
    vDebugPoint();
  }

  // at least a first extended text buffer
  vEnsureBuffer();

  // get tail buffer

  char* azCrs = szSubStr->azChar();
  i8 nGrpLen;
  bool b;

  char* azGrpEnd;
  while (*azCrs) {

    ExtendedTextBuffer* pExtTextBuffer = pTailBuffer();

    vTrace("+++++++\n"_Stz);
    vTrace("%s\n"_Stz,azCrs);
    azGrpEnd = StrLib::azEndOfGroup(azCrs, '\n', pExtTextBuffer->nFreeBytes(), nGrpLen);
    if (! nGrpLen) {
      b = bAddBuffer();
      continue;
    }

    b = pExtTextBuffer->bConcat((Stz*)azCrs, nGrpLen);

    if (*azGrpEnd)
      azGrpEnd++;
    azCrs = azGrpEnd;
    if (*azCrs)
      b = bAddBuffer();
  }

  if (nTextBlocks() > 1) {
    vTrace("-------\n"_Stz);
    vDump();
    vTrace("=======\n"_Stz);
    vDebugPoint();
  }
}

/*----------------------------------------------------------------------------*/
/* add sub-string to text                                                     */
/*----------------------------------------------------------------------------*/
void ExtendedText::vConcat(ExtendedText* pArg)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  i8 nBlocks_arg = pArg->nTextBlocks();
  for (i8 i_Block = 0; i_Block < nBlocks_arg; i_Block++) {

    ExtendedTextBuffer* pBuffer_arg  = pArg->pExtTextBuffer(i_Block);

    Stz* szTextBlock = pBuffer_arg->szTextBlock();

    vConcat(szTextBlock);
  }
}

/*----------------------------------------------------------------------------*/
/* copy given extended text argument                                          */
/*----------------------------------------------------------------------------*/
void ExtendedText::vCopy(ExtendedText* pExtText_arg)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  vEmpty();

  i8 nBlocks_arg = pExtText_arg->nTextBlocks();
  for (i8 i_Block = 0; i_Block < nBlocks_arg; i_Block++) {

    bool b = bAddBuffer();
    ASSERT(b);

    ExtendedTextBuffer* pBuffer_this = pExtTextBuffer(i_Block);
    ExtendedTextBuffer* pBuffer_arg  = pExtText_arg->pExtTextBuffer(i_Block);

    Stz* szTextBlock = pBuffer_arg->szTextBlock();

    b = pBuffer_this->bConcat(szTextBlock);
    ASSERT(b);
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void ExtendedText::vDump(void)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  for (i8 i_Block = 0; i_Block < m_nTextBlocks; i_Block++) {
    ExtendedTextBuffer* pBuffer = pExtTextBuffer(i_Block);
    vTrace("%s\n"_Stz,pBuffer->szTextBlock());
  }
}

/*----------------------------------------------------------------------------*/
/* empty this extended text                                                   */
/*----------------------------------------------------------------------------*/
void ExtendedText::vEmpty(void)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  for (i8 i_Block = 0; i_Block < m_nTextBlocks; i_Block++) {
    delete m_paExtTextBuffer[i_Block];
    m_paExtTextBuffer[i_Block] = nullptr;
  }
  m_nTextBlocks = 0;
}

/*----------------------------------------------------------------------------*/
/* get extended text buffer                                                   */
/*----------------------------------------------------------------------------*/
ExtendedTextBuffer* ExtendedText::pExtTextBuffer(i8 i_TextBuffer)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  return m_paExtTextBuffer[i_TextBuffer];
}

/*----------------------------------------------------------------------------*/
/* set extended text buffer                                                   */
/*----------------------------------------------------------------------------*/
void ExtendedText::vExtTextBuffer(i8 i_TextBuffer, ExtendedTextBuffer* pExtTextBuffer)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  m_paExtTextBuffer[i_TextBuffer] = pExtTextBuffer;
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

  return m_paExtTextBuffer[i_TextBlock]->szTextBlock();
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a buffer                                                               */
/*----------------------------------------------------------------------------*/
bool ExtendedText::bAddBuffer(void)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  if (m_nTextBlocks >= C_EXTENDEDTEXTBUFFER)
    return false;

  m_paExtTextBuffer[m_nTextBlocks++] = new ExtendedTextBuffer;

  return true;
}

/*----------------------------------------------------------------------------*/
/* ensure at least buffer 0                                                   */
/*----------------------------------------------------------------------------*/
void ExtendedText::vEnsureBuffer(void)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  // at least a first extended text buffer
  if (! m_paExtTextBuffer[0]) {
    m_paExtTextBuffer[0] = new ExtendedTextBuffer;
    m_nTextBlocks++;
  }

}

/*----------------------------------------------------------------------------*/
/* get tail/last buffer                                                       */
/*----------------------------------------------------------------------------*/
ExtendedTextBuffer* ExtendedText::pTailBuffer(void)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  ExtendedTextBuffer* pTailBuffer = m_paExtTextBuffer[m_nTextBlocks - 1];

  return pTailBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/