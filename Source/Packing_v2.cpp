/*============================================================================*/
/* Packing_v2.cpp -- Compact Row Packing_v2                                   */
/*============================================================================*/

#include "CompactBuffers.h"

/*----------------------------------------------------------------------------*/
/* pack the encoded inflated row into head[+over]{+tail}                      */
/*----------------------------------------------------------------------------*/
void Packing_v2::vPackCrw_v2(HeadLine* aHeadLine, Encoding_v2* pEncoding)
{
  _static_
  
  _ColEnc* aColEnc;
  i8 nHeadSize;
  u1 uMetaClue;
  
  i8 nCols = pEncoding->m_nColumns;
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
        
    aColEnc = &pEncoding->xaColEnc[i_Col];
    nHeadSize = pEncoding->m_nHeadSize;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -//
    //                            meta code packing                           //
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -//

    if (aColEnc->m_bSplit)
      goto L_MetaSplit;

    /*
              -- HeadLine --                      -- OverLine --
     +================================+
     | <W><M>...                      |
     +================================+
     | {D}...                         |
     +--------------------------------+   +--------------------------------+
     |                           [OL] |---| <U> ////////////////////////// |
     +--------------------------------+   +--------------------------------+
     
     where:
     <W>  is one byte word length of head line
     <M>  is a metacode for a column value
     {D}  is a set of data bytes for a column value
     [OL] is 2B relative index of the over line
     <U>  is one byte space unit length of over line
     */
    
    // > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > >
    
    // put meta code <M> into meta tape of head
    
    //            -- HeadLine --
    // +================================+
    // | <W><M>...<M>                   |
    // +================================+
        
    // set <M> for this column
    aHeadLine->vMetaClue(i_Col,aColEnc->m_uMetaClue);
    
    goto L_DataPack;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
  L_MetaSplit:
   
    /*
     -- Head-Line --                      -- Over-Line --
     +================================+
     | <W><M>...                      |
     +================================+
     | {D}...                         |
     +--------------------------------+   +--------------------------------+
     |                           [OL] |---| <U> ////////////////////////// |
     +--------------------------------+   +--------------------------------+
     
     where:
     [OL] is 2B relative index of the over line
     <W>  is one byte word length of head line
     <M>  is a metacode for a column value
     {D}  is a set of data bytes for a column value
     <U>  is one byte space unit length of over line
     */
    
    // > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > >

    // +================================+
    // | <W><M>...     <MC_OVERFLOW_2T> |
    // +================================+
    // | {D}...                         |
    // +--------------------------------+   +--------------------------------+
    // |                           [OL] |---| <U> ////////////////////////// |
    // +--------------------------------+   +--------------------------------+
    
    
    // set <MC_OVERFLOW_2T> for this column
    uMetaClue = METACLUE_FORM(MN_OVERFLOW,2);
    aHeadLine->vMetaClue(i_Col,uMetaClue);

    // +================================+
    // | <W><M>...     <MC_OVERFLOW_2T> |
    // +================================+
    // | {D}...                         |
    // +--------------------------------+   +--------------------------------+
    // |                           [OL] |---| <U><M> /////////////////////// |
    // +--------------------------------+   +--------------------------------+
    
    // put <M> into over line
    aHeadLine->vPutDataLogical(nHeadSize + C_U_1B,
                              (i8)aColEnc->m_uMetaClue,
                              C_1B);
    goto L_DataPack;
    

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -//
    //                            data code packing                           //
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -//

  L_DataPack:
    //vPutDataCode_v2(aHeadLine,pEncoding->xaColEnc[i_Col]);
    
    // no data code {D} case
    if (! aColEnc->m_nDataSize)
      goto L_RowPacked;
    
    // non-vch packing as {1..8B} (max of 1 word)
    if (DM_VCH != aColEnc->m_pCol->eDatum()) {
      vPutData_Bytes_v2(aHeadLine,pEncoding->xaColEnc[i_Col]);
      return;
    }
    
    // vch packing
    
    u1 uHigh;
    u1 uLow;
    
    METACLUE_SPLIT(aColEnc->m_uMetaClue,uHigh,uLow);

    if (MN_STRING_x1  == uHigh)
      vPutData_STRINGx1(aHeadLine,pEncoding->xaColEnc[i_Col],STRx1(uLow));
    else
    if (MN_STRING_x2  == uHigh)
      vPutData_STRINGx1(aHeadLine,pEncoding->xaColEnc[i_Col],STRx2(uLow));
    else
    if (MN_STRING_x4  == uHigh)
      vPutData_STRINGx1(aHeadLine,pEncoding->xaColEnc[i_Col],STRx4(uLow));
    else
    if (MN_STRING_x8  == uHigh)
      vPutData_STRINGx1(aHeadLine,pEncoding->xaColEnc[i_Col],STRx8(uLow));
    else
      vPutData_STR_TAIL_2T_v2(aHeadLine,pEncoding->xaColEnc[i_Col]);

  L_RowPacked:
    {;}
  }
}


/*----------------------------------------------------------------------------*/
/*                            meta code packing                               */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/*                            data code packing                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* put data code {D} for encoded column value into crw                        */
/*----------------------------------------------------------------------------*/
void Packing_v2::vPutDataCode_v2(HeadLine* aHeadLine, _ColEnc& xColEnc)
{
  // no data code {D} case
  if (! xColEnc.m_nDataSize)
    return;
  
  // non-vch packing as {1..8B} (max of 1 word)
  if (DM_VCH != xColEnc.m_pCol->eDatum()) {
    vPutData_Bytes_v2(aHeadLine,xColEnc);
    return;
  }
  
  // vch packing
  
  u1 uHigh;
  u1 uLow;
  
  METACLUE_SPLIT(xColEnc.m_uMetaClue,uHigh,uLow);

  if (MN_STRING_x1  == uHigh)
    vPutData_STRINGx1(aHeadLine,xColEnc,STRx1(uLow));
  else
  if (MN_STRING_x2  == uHigh)
    vPutData_STRINGx1(aHeadLine,xColEnc,STRx2(uLow));
  else
  if (MN_STRING_x4  == uHigh)
    vPutData_STRINGx1(aHeadLine,xColEnc,STRx4(uLow));
  else
  if (MN_STRING_x8  == uHigh)
    vPutData_STRINGx1(aHeadLine,xColEnc,STRx8(uLow));
  else
    vPutData_STR_TAIL_2T_v2(aHeadLine,xColEnc);
}

/*----------------------------------------------------------------------------*/
/* put data bytes for unary MC_xyz_1..8B (e.g. for MC_DTE_E1_2B, ...)         */
/*----------------------------------------------------------------------------*/
void Packing_v2::vPutData_Bytes_v2(HeadLine* aHeadLine, _ColEnc& xColEnc)
{
  //      -- HeadLine / OverLine --
  // +--------------------------------+
  // | ...{D}                         |
  // +--------------------------------+
  
  // put {D}
  aHeadLine->vPutDataLogical(xColEnc.m_i_Offset,
                           xColEnc.m_wDataCode.m_iWrd,
                           xColEnc.m_nDataSize);
}

/*----------------------------------------------------------------------------*/
/* put data bytes for MC_STR_FIT_1L : <L><ShortN>                             */
/*----------------------------------------------------------------------------*/
void Packing_v2::vPutData_STRINGx1(HeadLine* aHeadLine, _ColEnc& xColEnc, i8 nBytes)
{
  // get <ShortN>
  char* azStringZ = (char*)xColEnc.m_wDataCode.m_szVch;
    
  i8 nS = xColEnc.m_nDataSize;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  //      -- HeadLine / OverLine --
  // +--------------------------------+
  // | ...<StringZ>                   |
  // +--------------------------------+
  
  // put <StringZ> (with null terminator)
  aHeadLine->vPutDataLogical(xColEnc.m_i_Offset,
                             (Stz*)azStringZ,
                             nBytes);
}





/*----------------------------------------------------------------------------*/
/* put data bytes for MC_STR_FIT_1L : <L><ShortN>                             */
/*----------------------------------------------------------------------------*/
void Packing_v2::vPutData_STR_FIT_1L_v2(HeadLine* aHeadLine, _ColEnc& xColEnc)
{
  // get <ShortN>
  char* azShortN = (char*)xColEnc.m_wDataCode.m_szVch;
  
  // calc <L> (which includes the null term character)
  i8 nL = xColEnc.m_nDataSize - C_L_1B;
  RASSERT(nL == StrLib::nLength(azShortN) + C_NULL_1B);
  RASSERT(nL < (i8)C_1_BYTE);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //      -- HeadLine / OverLine --
  // +--------------------------------+
  // | ...<L>                         |
  // +--------------------------------+
  
  // put <L> (which includes the null term character)
  aHeadLine->vPutDataLogical(xColEnc.m_i_Offset,
                           nL,
                           C_1B);
  
  //      -- HeadLine / OverLine --
  // +--------------------------------+
  // | ...<L><ShortN>                 |
  // +--------------------------------+
  
  // put <ShortN> (with null terminator)
  aHeadLine->vPutDataLogical(xColEnc.m_i_Offset + C_L_1B,
                           (Stz*)azShortN,
                           nL);
}

/*----------------------------------------------------------------------------*/
/* put data bytes for MC_STR_FIT_2L : <LL><LongN>                             */
/*----------------------------------------------------------------------------*/
void Packing_v2::vPutData_STR_FIT_2L_v2(HeadLine* aHeadLine, _ColEnc& xColEnc)
{
  // get <LongN>
  char* azLongN = (char*)xColEnc.m_wDataCode.m_szVch;
  
  // calc <LL>
  i8 nLL = xColEnc.m_nDataSize - C_L_2B;
  RASSERT(nLL == StrLib::nLength(azLongN) + C_NULL_1B);
  RASSERT(nLL < (i8)C_2_BYTES);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //      -- HeadLine / OverLine --
  // +--------------------------------+
  // | ...<LL>                        |
  // +--------------------------------+
  
  // put <LL>
  aHeadLine->vPutDataLogical(xColEnc.m_i_Offset,
                           nLL,
                           C_2B);
  
  //      -- HeadLine / OverLine --
  // +--------------------------------+
  // | ...<LL><LongN>                 |
  // +--------------------------------+
  
  // put <LongN> (has null terminator)
  aHeadLine->vPutDataLogical(xColEnc.m_i_Offset + C_L_2B,
                           (Stz*)azLongN,
                           nLL);
}

/*----------------------------------------------------------------------------*/
/* put data bytes for MC_STR_TAIL_2T : [TL]                                   */
/*----------------------------------------------------------------------------*/
void Packing_v2::vPutData_STR_TAIL_2T_v2(HeadLine* aHeadLine, _ColEnc& xColEnc)
{
  // calculate [TL]
  i8 i_Tail_rel = aHeadLine->iGetTailRel(xColEnc.m_aTailLine);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //      -- HeadLine / OverLine --                -- TailLine --
  // +--------------------------------+   **********************************
  // | ...[TL]                        |---* <U><L><Stl> ////////////////// *
  // +--------------------------------+   **********************************
  //
  
  // put [TL]
  aHeadLine->vPutDataLogical(xColEnc.m_i_Offset,
                           i_Tail_rel,
                           C_2B);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
