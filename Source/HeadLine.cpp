/*============================================================================*/
/* HeadLine.hpp -- a head line                                                */
/*============================================================================*/

#include "CompactBuffers.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
HeadLine::HeadLine(void)
{
  _nonProper_
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
HeadLine::~HeadLine(void) noexcept(false)
{
  _nonProper_
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of a byte from logicalRow : head[+over]                        */
/*----------------------------------------------------------------------------*/
byte* HeadLine::ayAdrLogical(i8 i_Offset)
{
  _nonProper_
  
  // scenario 1: simple case (no overflow), getting address from head
  if (! bHasOverflow()) {
    byte* ayAdr = (byte*)this + i_Offset;
    return ayAdr;
  }
  
  // scenario 2: there is an overflow, but getting address from head
  i8 nHeadBytes = nBytes();
  if (i_Offset < nHeadBytes - 2) {
    byte* ayAdr = (byte*)this + i_Offset;
    return ayAdr;
  }
  
  // scenario 3: there is an overflow, and getting address from overflow
  OverLine* aOverLine = aGetOver();
  i8 i_Offset_over = i_Offset - nHeadBytes;
  byte* ayAdr = (byte*)aOverLine + i_Offset_over;
  return ayAdr;
}

/*----------------------------------------------------------------------------*/
/* return size of head in bytes                                               */
/*----------------------------------------------------------------------------*/
i8 HeadLine::nBytes(void)
{
  _nonProper_
  
  i8 nSizeBytes = nWords() * C_BYTES_PER_WORD;
  
  return nSizeBytes;
}

/*----------------------------------------------------------------------------*/
/* mark headline as deleted                                                   */
/*----------------------------------------------------------------------------*/
void HeadLine::vDelete(void)
{
  _nonProper_
  
  vW(C_HEADLINE_DELETED);
}

/*----------------------------------------------------------------------------*/
/* is headline deleted ?                                                      */
/*----------------------------------------------------------------------------*/
bool HeadLine::bIsDeleted(void)
{
  _nonProper_
  
  i8 nSizeWords = nW();
  
  return nSizeWords == C_HEADLINE_DELETED ? true : false;
}

/*----------------------------------------------------------------------------*/
/* dump head line (and possibly over line)                                    */
/*----------------------------------------------------------------------------*/
void HeadLine::vDump(void)
{
  _nonProper_
  
  i8 nSizeBytes = nBytes();
  MemLib::vWordDump(this,nSizeBytes);
  
  if (! bHasOverflow())
    return;
  
  OverLine* aOverLine = aGetOver();
  
  aOverLine->vDump();
}

/*----------------------------------------------------------------------------*/
/* calculate free space of a head line                                        */
/*----------------------------------------------------------------------------*/
i8 HeadLine::nFreeSpace(StreamDef* pStreamDef)
{
  _nonProper_
  
  //MemLib::vWordDump(aHeadLine,m_nHeadBytes);
  
  Column* pLastCol = pStreamDef->pGetColumn(pStreamDef->nColumns(C_ALL) - 1);
  
 // ASSERT(pLastCol->bIsLastColumn());
  
  i8 i_ColCrs_ = 0;
  i8 i_OfsCrs_ = C_UNKNOWN;

  eMC eLastEncoding = eGetEncMetaCode(pLastCol,i_ColCrs_,i_OfsCrs_,pStreamDef);
  
  i8 nDataBytes = RowCoder::nDataSizeOfCode(eLastEncoding);
  
  i8 nFreeBytes = nBytes() - i_OfsCrs_ - nDataBytes;
  
//  if (nFreeBytes < 0)
//    vDebugPoint();
  
  return nFreeBytes;
}

/*----------------------------------------------------------------------------*/
/* get data code from logical row : head[+over]                               */
/*----------------------------------------------------------------------------*/
w8 HeadLine::wGetLogical(i8 i_Offset, i8 nDataSize)
{
  _nonProper_
  
  w8 wDataCode = 0;
  
  // scenario 1: simple case (no overflow), getting bytes from head
  if (! bHasOverflow()) {
    wDataCode = wGet(i_Offset,nDataSize);
    return wDataCode;
  }
  
  // scenario 2: there is an overflow, but getting bytes from head
  i8 nHeadBytes = nBytes();
  if (i_Offset < nHeadBytes - 2) {
    wDataCode = wGet(i_Offset,nDataSize);
    return wDataCode;
  }
  
  // scenario 3: there is an overflow, and getting bytes from overflow
  OverLine* aOverLine = aGetOver();
  i8 i_Offset_over = i_Offset - nHeadBytes;
  wDataCode = aOverLine->wGet(i_Offset_over,nDataSize);
  return wDataCode;
}

/*----------------------------------------------------------------------------*/
/* get the encoding (metaclue and offset) of a column datum                   */
/*----------------------------------------------------------------------------*/
u1 HeadLine::uGetEncMetaClue(Column* pForCol, i8& i_ColCrs_, i8& i_OfsCrs_,
                           StreamDef* pStreamDef)
{
  _nonProper_
  
  i8 i_ForCol = pForCol->iColumnNo();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // move cursor forward to column required                            NOT RIGHT
  
  if (C_UNKNOWN == i_OfsCrs_ || i_ForCol < i_ColCrs_) {
    
    i_ColCrs_ = 0;
    i_OfsCrs_ = RowCoder::iBaseOffsetData(pStreamDef);
  }
  
  for (i8 i_Col = i_ColCrs_; i_Col < i_ForCol; i_Col++) {
    
    eMC eEncoding = eMetaCode(i_Col);
    
    if (MC_OVERFLOW_2T == eEncoding) {
      i_OfsCrs_ = nBytes() + C_U_1B;
      eEncoding = (eMC)wGetLogical(i_OfsCrs_++,C_1B);
      i_OfsCrs_ += RowCoder::nDataSizeOfCode(eEncoding);
    }
    else {
      i_OfsCrs_ += RowCoder::nDataSizeOfCode(eEncoding);
    }
    
    if (MC_STR_FIT_1L == eEncoding) {
      i_OfsCrs_ += wGetLogical((i_OfsCrs_ - 1),C_1B);
    }
    else
    if (MC_STR_FIT_2L == eEncoding) {
      i_OfsCrs_ += wGetLogical((i_OfsCrs_ - 1),C_2B);
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the encoding for the column
  
  eMC eEncoding = eMetaCode(i_ForCol);
  if (MC_OVERFLOW_2T == eEncoding) {
    i_OfsCrs_ = nBytes() + C_U_1B + C_M_1B;
    OverLine* aOverLine = aGetOver();
    eEncoding = (eMC)aOverLine->w8_1B(1);
  }
  
  i_ColCrs_ = i_ForCol;
  
  return eEncoding;
}

/*----------------------------------------------------------------------------*/
/* get the encoding (metacode and offset) of a column datum                   */
/*----------------------------------------------------------------------------*/
eMC HeadLine::eGetEncMetaCode(Column* pForCol, i8& i_ColCrs_, i8& i_OfsCrs_,
                           StreamDef* pStreamDef)
{
  _nonProper_
  
  i8 i_ForCol = pForCol->iColumnNo();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // move cursor forward to column required
  
  if (C_UNKNOWN == i_OfsCrs_ || i_ForCol < i_ColCrs_) {
    
    i_ColCrs_ = 0;
    i_OfsCrs_ = RowCoder::iBaseOffsetData(pStreamDef);
  }
  
  for (i8 i_Col = i_ColCrs_; i_Col < i_ForCol; i_Col++) {
    
    eMC eEncoding = eMetaCode(i_Col);
    
    if (MC_OVERFLOW_2T == eEncoding) {
      i_OfsCrs_ = nBytes() + C_U_1B;
      eEncoding = (eMC)wGetLogical(i_OfsCrs_++,C_1B);
      i_OfsCrs_ += RowCoder::nDataSizeOfCode(eEncoding);
    }
    else {
      i_OfsCrs_ += RowCoder::nDataSizeOfCode(eEncoding);
    }
    
    if (MC_STR_FIT_1L == eEncoding) {
      i_OfsCrs_ += wGetLogical((i_OfsCrs_ - 1),C_1B);
    }
    else
    if (MC_STR_FIT_2L == eEncoding) {
      i_OfsCrs_ += wGetLogical((i_OfsCrs_ - 1),C_2B);
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the encoding for the column
  
  eMC eEncoding = eMetaCode(i_ForCol);
  if (MC_OVERFLOW_2T == eEncoding) {
    i_OfsCrs_ = nBytes() + C_U_1B + C_M_1B;
    OverLine* aOverLine = aGetOver();
    eEncoding = (eMC)aOverLine->w8_1B(1);
  }
  
  i_ColCrs_ = i_ForCol;
  
  return eEncoding;
}

/*----------------------------------------------------------------------------*/
/* get overflow line using relative index [OT]                                */
/*----------------------------------------------------------------------------*/
OverLine* HeadLine::aGetOver(void)
{
  _nonProper_
  
  i8 i_Over_rel = iOverflow2B();
  OverLine* aOver = RowNub::aGetOver_rel(this,i_Over_rel);
  
  return aOver;
}

/*----------------------------------------------------------------------------*/
/* get tail line using relative index [FT]                                    */
/*----------------------------------------------------------------------------*/
i8 HeadLine::iGetTailRel(TailLine* TailLine)
{
  _nonProper_
  
  i8 i_Tail_rel = RowNub::iUnitDiff(this,TailLine);
  
  return i_Tail_rel;
}

/*----------------------------------------------------------------------------*/
/* get tail line using relative index [FT]                                    */
/*----------------------------------------------------------------------------*/
TailLine* HeadLine::aGetTailRel(i8 i_Tail_rel)
{
  _nonProper_
  
  TailLine* aTail = RowNub::aGetTail_rel(this,i_Tail_rel);
  
  return aTail;
}

/*----------------------------------------------------------------------------*/
/* does this head have an overflow?                                           */
/*----------------------------------------------------------------------------*/
bool HeadLine::bHasOverflow(void)
{
  _nonProper_
  
  i8 nSizeWords = nW();
  
  if (nSizeWords >= 128)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* mask this head to have an overflow                                         */
/*----------------------------------------------------------------------------*/
void HeadLine::vHasOverflow(void)
{
  _nonProper_
  
  i8 nSizeWords = nW();
  
  if (nSizeWords < 128) {
    nSizeWords += 128;
    vW(nSizeWords);
  }
}

/*----------------------------------------------------------------------------*/
/* store meta clue                                                            */
/*----------------------------------------------------------------------------*/
void HeadLine::vMetaClue(i8 i_Col, u1 uMetaClue)
{
  _nonProper_
  
  i8 i_BaseOffsetMeta = RowCoder::iBaseOffsetMeta();
  
  i8 i_Offset = i_BaseOffsetMeta + i_Col;
  
  v1B(i_Offset,uMetaClue);
}

/*----------------------------------------------------------------------------*/
/* store meta code                                                            */
/*----------------------------------------------------------------------------*/
void HeadLine::vMetaCode(i8 i_Col, eMC eMetaCode)
{
  _nonProper_
  
  i8 i_BaseOffsetMeta = RowCoder::iBaseOffsetMeta();
  
  i8 i_Offset = i_BaseOffsetMeta + i_Col;
  
  v1B(i_Offset,eMetaCode);
}

/*----------------------------------------------------------------------------*/
/* get the relative distance to the overflow line [OT] which is 2B            */
/*----------------------------------------------------------------------------*/
i8 HeadLine::iOverflow2B(void)
{
  _nonProper_
  
  i8 nHeadBytes = nBytes();
  
  i8 iOver2B = w8_2B(nHeadBytes - C_2B);

  return iOver2B;
}

/*----------------------------------------------------------------------------*/
/* set the relative distance to the overflow line [OT] which is 2B            */
/*----------------------------------------------------------------------------*/
void HeadLine::vOverflow2B(i8 iOver2B)
{
  _nonProper_
  
  i8 nHeadBytes = nBytes();
  
  v2B(nHeadBytes - C_2B,iOver2B);
}

/*----------------------------------------------------------------------------*/
/* put data code into logical compact row : head[+over]                       */
/*----------------------------------------------------------------------------*/
void HeadLine::vPutDataLogical(i8 i_Offset, w8 wDataCode, i8 nDataSize)
{
  _nonProper_
  
  // scenario 1: simple case (no overflow), putting bytes into head
  if (! bHasOverflow()) {
    vPut(i_Offset,wDataCode,nDataSize);
    return;
  }
  
  // scenario 2: there is an overflow, but putting bytes into head
  i8 nHeadBytes = nBytes();
  if (i_Offset < nHeadBytes - 2) {
    vPut(i_Offset,wDataCode,nDataSize);
    return;
  }
  
  // scenario 3: there is an overflow, and putting bytes into overflow
  
  OverLine* aOverLine = aGetOver();
  i8 i_Offset_over = i_Offset - nHeadBytes;
  
  ASSERT(i_Offset_over < aOverLine->nBytes());
  
  aOverLine->vPut(i_Offset_over,wDataCode,nDataSize);
}

/*----------------------------------------------------------------------------*/
/* get perspective                                                            */
/*----------------------------------------------------------------------------*/
t8 HeadLine::tPerspective(void)
{
  _nonProper_
  
  t8 tPerspective = w8_8B(C_HEADLINE_OFS_PERSPECTIVE);

  return tPerspective;
}

/*----------------------------------------------------------------------------*/
/* set perspective                                                            */
/*----------------------------------------------------------------------------*/
void HeadLine::vPerspective(t8 tPerspective)
{
  _nonProper_
  
  v8B(C_HEADLINE_OFS_PERSPECTIVE,tPerspective);
}

/*----------------------------------------------------------------------------*/
/* put string into logical compact row : head[+over]                          */
/*----------------------------------------------------------------------------*/
void HeadLine::vPutDataLogical(i8 i_Offset, Stz* szString, i8 nLength)
{
  _nonProper_
  
  byte* ayAppend = ayAdrLogical(i_Offset);
  
  MemLib::vCopy(ayAppend,szString,nLength,CN_ROWCOMPACTOR);
}

/*----------------------------------------------------------------------------*/
/* set overflow line as relative index [OT]                                   */
/*----------------------------------------------------------------------------*/
void HeadLine::vSetOver(OverLine* aOverLine)
{
  _nonProper_
  
  i8 i_Over_rel = RowNub::iUnitDiff(this,(TailLine*)aOverLine);
  
  vOverflow2B(i_Over_rel);
 
  vHasOverflow();
}

/*----------------------------------------------------------------------------*/
/* return size of head in units                                               */
/*----------------------------------------------------------------------------*/
i8 HeadLine::nUnits(void)
{
  _nonProper_
  
  i8 nSizeUnits = w8_1B(C_HEADLINE_OFS_WORDS);
    
  if (nSizeUnits >= 128)
    nSizeUnits -= 128;
  
  return nSizeUnits;
}

/*----------------------------------------------------------------------------*/
/* get the size <W> of this headline in words                                 */
/*----------------------------------------------------------------------------*/
i8 HeadLine::nW(void)
{
  _nonProper_
  
  i8 nWords = w8_1B(C_HEADLINE_OFS_WORDS);
  
  return nWords;
}

/*----------------------------------------------------------------------------*/
/* set the size <W> of this headline in words                                 */
/*----------------------------------------------------------------------------*/
void HeadLine::vW(i8 nWords)
{
  _nonProper_
  
  v1B(C_HEADLINE_OFS_WORDS,nWords);
}

/*----------------------------------------------------------------------------*/
/* return size of head in words                                               */
/*----------------------------------------------------------------------------*/
i8 HeadLine::nWords(void)
{
  _nonProper_
  
  i8 nSizeWords = nUnits();
  
  return nSizeWords;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
