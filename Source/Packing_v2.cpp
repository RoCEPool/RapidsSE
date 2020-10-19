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
  
  // note: an OverLine will have been alloted if needed
  
  _ColEnc* aColEnc;
  i8 nHeadSize;
  u1 uMetaCode;
  u1 uHigh;
  u1 uLow;
  i8 nStrBytes;
  i8 i_Tail_rel;
  i8 i_Offset;
  i8 i_Offset_over;
  byte* aByte;
  
  i8 i_BaseOffsetMeta = RowCoder::iBaseOffsetMeta();

  i8 nHeadBytes = aHeadLine->nBytes();
  OverLine* aOverLine = nullptr;

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
    //aHeadLine->vMetaCode(i_Col,aColEnc->m_uMetaCode);
    
    i_Offset = i_BaseOffsetMeta + i_Col;
    aByte = ((byte*)aHeadLine) + i_Offset;
    *aByte = aColEnc->m_uMetaCode;

    
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
    uMetaCode = METACODE_FORM(MC_OVERFLOW,2);
//    aHeadLine->vMetaCode(i_Col,uMetaCode);
    i_Offset = i_BaseOffsetMeta + i_Col;
    aByte = ((byte*)aHeadLine) + i_Offset;
    *aByte = uMetaCode;


    aOverLine = aHeadLine->aGetOver();
    
    // +================================+
    // | <W><M>...     <MC_OVERFLOW_2T> |
    // +================================+
    // | {D}...                         |
    // +--------------------------------+   +--------------------------------+
    // |                           [OL] |---| <U><M> /////////////////////// |
    // +--------------------------------+   +--------------------------------+
    
    // put <M> into over line
    aByte = ((byte*)aOverLine) + C_U_1B;
    *aByte = aColEnc->m_uMetaCode;

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
    if (aOverLine) {
      i_Offset_over = aColEnc->m_i_Offset - nHeadBytes;

      aOverLine->vPut(i_Offset_over,
                      aColEnc->m_wDataCode.m_iWrd,
                      aColEnc->m_nDataSize);
    }
    else {
      aHeadLine->vPut(aColEnc->m_i_Offset,
                      aColEnc->m_wDataCode.m_iWrd,
                      aColEnc->m_nDataSize);
    }
    goto L_RowPacked;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
  L_PackVch:
    // vch packing
    
    METACODE_SPLIT(aColEnc->m_uMetaCode,uHigh,uLow);
    
    if (MC_STRING_TAIL != uHigh)
      goto L_PackVch_Head;
    
    // calculate [TL]
    i_Tail_rel = aHeadLine->iGetTailRel(aColEnc->m_aTailLine);
    
    //      -- HeadLine / OverLine --                -- TailLine --
    // +--------------------------------+   **********************************
    // | ...[TL]                        |---* <U><L><Stl> ////////////////// *
    // +--------------------------------+   **********************************
    //
    
    // put [TL]
    if (aOverLine) {
      i_Offset_over = aColEnc->m_i_Offset - nHeadBytes;
      aOverLine->vPut(i_Offset_over,i_Tail_rel,C_2B);
    }
    else {
      aHeadLine->vPut(aColEnc->m_i_Offset,i_Tail_rel,C_2B);
    }
    goto L_RowPacked;

    
  L_PackVch_Head:
    if (MC_STRING_x1  == uHigh)
      nStrBytes = STRx1(uLow);
    else
    if (MC_STRING_x2  == uHigh)
      nStrBytes = STRx2(uLow);
    else
    if (MC_STRING_x4  == uHigh)
      nStrBytes = STRx4(uLow);
    else
    if (MC_STRING_x8  == uHigh)
      nStrBytes = STRx8(uLow);
    else {
      RASSERT(0);
    }
    
    //      -- HeadLine / OverLine --
    // +--------------------------------+
    // | ...<StringZ>                   |
    // +--------------------------------+
    
    // put <StringZ> (with null terminator)
    if (aOverLine) {
      i_Offset_over = aColEnc->m_i_Offset - nHeadBytes;
      aByte = ((byte*)aOverLine) + i_Offset_over;
      MemLib::vCopy(aByte,aColEnc->m_wDataCode.m_szVch,nStrBytes,CN_ROWCOMPACTOR);

//      aHeadLine->vPutDataLogical(aColEnc->m_i_Offset,
//                                 aColEnc->m_wDataCode.m_szVch,
//                                 nStrBytes);
    }
    else {
      aByte = ((byte*)aHeadLine) + aColEnc->m_i_Offset;
      MemLib::vCopy(aByte,aColEnc->m_wDataCode.m_szVch,nStrBytes,CN_ROWCOMPACTOR);

//      aHeadLine->vPutDataLogical(aColEnc->m_i_Offset,
//                                 aColEnc->m_wDataCode.m_szVch,
//                                 nStrBytes);
    }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
  L_RowPacked:
    {;}
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
