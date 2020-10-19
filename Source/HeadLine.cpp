/*============================================================================*/
/* HeadLine.hpp -- a head line                                                */
/*============================================================================*/

#include "CompactBuffers.h"

extern EnvFlags gxEnvFlags;

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
  
  i8 nDataBytes;
  
  //MemLib::vWordDump(aHeadLine,m_nHeadBytes);
  
  Column* pLastCol = pStreamDef->pGetColumn(pStreamDef->nColumns(C_ALL) - 1);
  
 // ASSERT(pLastCol->bIsLastColumn());
  
  i8 i_ColCrs_ = 0;
  i8 i_OfsCrs_ = C_UNKNOWN;

  
  u1 uLastEncoding = uGetEncMetaCode(pLastCol,i_ColCrs_,i_OfsCrs_,pStreamDef);
  
  nDataBytes = RowCoder::nDataSizeOfCode(uLastEncoding);
  
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
/* get the encoding (metacode and offset) of a column datum                   */
/*----------------------------------------------------------------------------*/
u1 HeadLine::uGetEncMetaCode(Column* pForCol, i8& i_ColCrs_, i8& i_OfsCrs_,
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
    
    u1 uEncoding = uMetaCode(i_Col);
    
    u1 uHigh = METACODE_HIGH(uEncoding);
    u1 uLow  = METACODE_LOW(uEncoding);

    if (MC_OVERFLOW == uHigh) {
      i_OfsCrs_ = nBytes() + C_U_1B;
      uEncoding = (u1)wGetLogical(i_OfsCrs_++,C_1B);
      i_OfsCrs_ += RowCoder::nDataSizeOfCode(uEncoding);
    }
    else {
      i_OfsCrs_ += RowCoder::nDataSizeOfCode(uEncoding);
    }
    
    if (MC_STRING_x1 == uHigh) {
      i_OfsCrs_ += STRx1(uLow);
    }
    else
    if (MC_STRING_x2 == uHigh) {
      i_OfsCrs_ += STRx2(uLow);
    }
    else
    if (MC_STRING_x4 == uHigh) {
      i_OfsCrs_ += STRx4(uLow);
    }
    else
    if (MC_STRING_x8 == uHigh) {
      i_OfsCrs_ += STRx8(uLow);
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the encoding for the column
  
  u1 uEncoding = uMetaCode(i_ForCol);
  u1 uHigh = METACODE_HIGH(uEncoding);
  u1 uLow  = METACODE_LOW(uEncoding);
  if (MC_OVERFLOW == uHigh) {
    i_OfsCrs_ = nBytes() + C_U_1B + C_M_1B;
    OverLine* aOverLine = aGetOver();
    uEncoding = (u1)aOverLine->w8_1B(1);
  }
  
  i_ColCrs_ = i_ForCol;
  
  return uEncoding;
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
/* store meta code                                                            */
/*----------------------------------------------------------------------------*/
void HeadLine::vMetaCode(i8 i_Col, u1 uMetaCode)
{
  _nonProper_
  
  i8 i_BaseOffsetMeta = RowCoder::iBaseOffsetMeta();
  
  i8 i_Offset = i_BaseOffsetMeta + i_Col;
  
  v1B(i_Offset,uMetaCode);
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
