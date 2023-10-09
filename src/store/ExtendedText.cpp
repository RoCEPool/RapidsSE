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

  m_nBlocks = 0;
  for (i8 i_Block = 0; i_Block < C_EXTENDEDTEXTBUFFER; i_Block++)
    m_paExtTextBuffer[i_Block] = nullptr;

  m_nBytes = 0;
  m_nLines = 0;

  _checkInitialised_(CN_EXTENDEDTEXT);

  _postcondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ExtendedText::~ExtendedText(void) noexcept(false)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  for (i8 i_Block = 0; i_Block < m_nBlocks; i_Block++) {
    delete m_paExtTextBuffer[i_Block];
    m_paExtTextBuffer[i_Block] = nullptr;
  }
  m_nBlocks = 0;

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
void ExtendedText::vConcat(Stz* szStr)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  // anything to concat?
  if (! szStr || ! *szStr->azChar())
    return;

  // at least a first extended text buffer
  vEnsureBuffer();

//  i8 nBytes_allGroups = 0;
  i8 nAvailBytes;
  i8 nBytes_group;
  i8 nLines_group;
  bool b;

  const char* azEnd_group;
  const char* azBegin_group = szStr->azChar();

  for (i8 i_Loop=0; ; i_Loop++) {
    UNUSED(i_Loop);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get tail buffer with usable free space

    ExtendedTextBuffer *pEtxtBuffer = pTailBuffer();

    // determine available bytes in tail buffer
    nAvailBytes = pEtxtBuffer->nFreeBytes() - 1;

    // is the tail buffer fully exhausted?
    if (nAvailBytes < 1) {
      b = bAddBuffer();
      RASSERT(b);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // find end of maximum-sized line group which will fit into tail buffer

    azEnd_group = StrLib::azGroupEnd(azBegin_group, '\n', nAvailBytes,
                                     nBytes_group, nLines_group);

    if (! nBytes_group) {
      // the line group didn't fit into tail buffer
      //   (this would be a single line that is longer than avail bytes)
      b = bAddBuffer();
      RASSERT(b);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // concat line group onto tail buffer

    b = pEtxtBuffer->bConcat((Stz *) azBegin_group, nBytes_group);
    RASSERT(b);

    m_nLines += nLines_group;

//    nBytes_allGroups += nBytes_group;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // end detection
    //                                     ^

    // if last line group ended : "...xyz\0"
    //                                    ^
    //   then there are no more line groups
    if (! *azEnd_group)
      break;

    // advance to next group
    azBegin_group = azEnd_group + 1;

    // if next line group begins : "...\n\0"
    //                                    ^
    //   then there are no more line groups
    if (! *azBegin_group)
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // still more line groups

    // continue processing next line group with another buffer
    b = bAddBuffer();
    RASSERT(b);
  }

  vResum();

  vVerify();
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
    UNUSED(b);
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

  vTrace("~~ExtendedText : %lld blocks %lld lines %lld bytes\n"_Stz,
         m_nBlocks,m_nLines,m_nBytes);

  for (i8 i_Block = 0; i_Block < m_nBlocks; i_Block++) {
    ExtendedTextBuffer* pBuffer = pExtTextBuffer(i_Block);
    vTrace("~~TextBlock[%lld] : %lld bytes \n%s\n"_Stz,
           i_Block,
           StrLib::nSize(pBuffer->szTextBlock()->azChar()),
           pBuffer->szTextBlock());
  }

  vTrace("~~End ExtendedText\n"_Stz);

}

/*----------------------------------------------------------------------------*/
/* empty this extended text                                                   */
/*----------------------------------------------------------------------------*/
void ExtendedText::vEmpty(void)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  for (i8 i_Block = 0; i_Block < C_EXTENDEDTEXTBUFFER; i_Block++) {
    delete m_paExtTextBuffer[i_Block];
    m_paExtTextBuffer[i_Block] = nullptr;
  }

  m_nBlocks = 0;

  m_nBytes = 0;
  m_nLines = 0;
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

  pString->vCopy(""_Stz);
  for (i8 i_Block = 0; i_Block < m_nBlocks; i_Block++)
    pString->vConcat(szTextBlock(i_Block));

  return true;
}

/*----------------------------------------------------------------------------*/
/* add sub-string to text                                                     */
/*----------------------------------------------------------------------------*/
i8 ExtendedText::nTextBlocks(void)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  return m_nBlocks;
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
/* verify                                                                     */
/*----------------------------------------------------------------------------*/
void ExtendedText::vVerify(void)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  i8 nBytes_sum = 0;
  i8 nBytes_block;

  i8 i_Block;
  for (i_Block = 0; i_Block < C_EXTENDEDTEXTBUFFER; i_Block++) {
    if (!m_paExtTextBuffer[i_Block])
      break;

    nBytes_block = szTextBlock(i_Block)->nSize();
    nBytes_sum += nBytes_block;
  }

  RASSERT(i_Block == m_nBlocks);

  RASSERT(nBytes_sum == m_nBytes);
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

  if (m_nBlocks >= C_EXTENDEDTEXTBUFFER)
    return false;

  m_paExtTextBuffer[m_nBlocks++] = new ExtendedTextBuffer;

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
    m_nBlocks++;
  }

}

/*----------------------------------------------------------------------------*/
/* ensure at least buffer 0                                                   */
/*----------------------------------------------------------------------------*/
void ExtendedText::vResum(void)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  m_nBytes = 0;

  for (i8 i_Block = 0; i_Block < m_nBlocks; i_Block++) {
    if (!m_paExtTextBuffer[i_Block])
      break;

    i8 nBytes_block = szTextBlock(i_Block)->nSize();
    m_nBytes += nBytes_block;
  }

}


/*----------------------------------------------------------------------------*/
/* get tail/last buffer                                                       */
/*----------------------------------------------------------------------------*/
ExtendedTextBuffer* ExtendedText::pTailBuffer(void)
{
  _precondition_(_MAGIC_, CN_EXTENDEDTEXT, _FOOTER_);

  ExtendedTextBuffer* pTailBuffer = m_paExtTextBuffer[m_nBlocks - 1];

  return pTailBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/