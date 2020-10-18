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
  u1 uHigh;
  u1 uLow;
  i8 nStrBytes;
  i8 i_Tail_rel;
  i8 i_Offset;
  byte* aByte;
  
  i8 i_BaseOffsetMeta = RowCoder::iBaseOffsetMeta();


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
    //aHeadLine->vMetaClue(i_Col,aColEnc->m_uMetaClue);
    
    i_Offset = i_BaseOffsetMeta + i_Col;
    aByte = ((byte*)aHeadLine) + i_Offset;
    *aByte = aColEnc->m_uMetaClue;

    
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
    if (DM_VCH == aColEnc->m_pCol->eDatum())
      goto L_PackVch;
    
    //vPutData_Bytes_v2(aHeadLine,pEncoding->xaColEnc[i_Col]);
    
    //      -- HeadLine / OverLine --
    // +--------------------------------+
    // | ...{D}                         |
    // +--------------------------------+
    
    // put {D}
    aHeadLine->vPutDataLogical(aColEnc->m_i_Offset,
                               aColEnc->m_wDataCode.m_iWrd,
                               aColEnc->m_nDataSize);

    goto L_RowPacked;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
  L_PackVch:
    // vch packing
    
    METACLUE_SPLIT(aColEnc->m_uMetaClue,uHigh,uLow);
    
    if (MN_STRING_TAIL != uHigh)
      goto L_PackVch_Head;
    
    // calculate [TL]
    i_Tail_rel = aHeadLine->iGetTailRel(aColEnc->m_aTailLine);
    
    //      -- HeadLine / OverLine --                -- TailLine --
    // +--------------------------------+   **********************************
    // | ...[TL]                        |---* <U><L><Stl> ////////////////// *
    // +--------------------------------+   **********************************
    //
    
    // put [TL]
    aHeadLine->vPutDataLogical(aColEnc->m_i_Offset,
                               i_Tail_rel,
                               C_2B);
    goto L_RowPacked;

    
  L_PackVch_Head:
    if (MN_STRING_x1  == uHigh)
      nStrBytes = STRx1(uLow);
    else
    if (MN_STRING_x2  == uHigh)
      nStrBytes = STRx2(uLow);
    else
    if (MN_STRING_x4  == uHigh)
      nStrBytes = STRx4(uLow);
    else
    if (MN_STRING_x8  == uHigh)
      nStrBytes = STRx8(uLow);
    else {
      RASSERT(0);
    }
    
    //      -- HeadLine / OverLine --
    // +--------------------------------+
    // | ...<StringZ>                   |
    // +--------------------------------+
    
    // put <StringZ> (with null terminator)
    aHeadLine->vPutDataLogical(aColEnc->m_i_Offset,
                               aColEnc->m_wDataCode.m_szVch,
                               nStrBytes);

  L_RowPacked:
    {;}
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
