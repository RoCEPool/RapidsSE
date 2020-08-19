/*============================================================================*/
/* RowInflator_v2.cpp -- Row Inflator (using Meta Nibble)                     */
/*============================================================================*/

#include "CompactBuffers.h"

#define STRx1(__uLow) (__uLow)            //   0B .. |1 * 16 =  16| ..  15B
#define STRx2(__uLow) ( 16 + 2 * __uLow)  //  16B .. |2 * 16 =  32| ..  47B
#define STRx4(__uLow) ( 48 + 4 * __uLow)  //  48B .. |4 * 16 =  64| .. 111B
#define STRx8(__uLow) (112 + 8 * __uLow)  // 112B .. |8 * 16 = 128| .. 239B

/*----------------------------------------------------------------------------*/
/* inflate set onto Dtsk                                                      */
/*----------------------------------------------------------------------------*/
void RowInflator::vInflateSet_v2(i8** aaiDstk, HeadLine* aHeadLine,
                                   StreamDef* pStreamDef)
{
  eMC  eMetaCode;
  Nibbler  xMetaCode;
  SingleWord wWord;
  
  i8   i_Tail_rel;
  i8   nLength;

  TailLine* aTail;
    
  char* acString; // = nullptr;
  eMC* aeMetaCode = aHeadLine->aeMetaArea();
  
  i8 nCols = pStreamDef->nTotalColumns();
  
  i8 i_BaseOffsetData = C_W_1B   // (ControlDate) 1 byte for the headline size (in words)
                        + C_T_8B // (ControlDate) 8 bytes for the perspective
                        + nCols; // metaSize

  byte* ayDataByte = aAdd<byte*>(aHeadLine,i_BaseOffsetData);
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the set of columns to be retrieved
  
  i8 nWhichCols = 0;
  i8 iWhichCol[C_MAX_COLUMNS];
  
  i8 i_ColToRetrieve;
  for (;;) {
    
    i_ColToRetrieve = *((*aaiDstk)--);
    
    if (-1 == i_ColToRetrieve)
      break;
    
    // must be in ascending order
    ASSERT(0 == nWhichCols ||
           i_ColToRetrieve > iWhichCol[nWhichCols-1]);
    
    iWhichCol[nWhichCols++] = i_ColToRetrieve;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  u1 uHigh;
  u1 uLow;
  i8 nStrLen;
  i8* aiCellValue;

  i8* aiDstk_lcl = *aaiDstk;
  
  byte* ayDstByte = (byte*)aiDstk_lcl;

  i8 i_WhichCol = 0;
  i_ColToRetrieve = iWhichCol[i_WhichCol];
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each column
  
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
        
    eMetaCode = *aeMetaCode++;
    
    uHigh = eMetaCode & 0xF0;
    uLow  = eMetaCode & 0x0F;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ignore column
    
    if (i_Col != i_ColToRetrieve) {

      if (uHigh <=  MN_NUMERIC)  // numeric
        ayDataByte += uLow;
      else
      if (uHigh >=  MN_NEGATIVE) // small value
        {;}
      else
      if (uHigh == MN_STRING_x2) // string x 2
        ayDataByte += STRx2(uLow);
      else
      if (uHigh == MN_STRING_x4) // string x 4
        ayDataByte += STRx4(uLow);
      else
      if (uHigh == MN_STRING_x8) // string x 8
        ayDataByte += STRx8(uLow);
      else {
        RASSERT(0);
      }
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // inflate column
    
    switch (uHigh) {
        
      case MN_NUMERIC: // numeric values 0..8B
        
        switch (uLow) {
            
          case 0x0: // numeric 0B
            *ayDstByte++ = 0; // 0
            *ayDstByte++ = 0; // 1
            *ayDstByte++ = 0; // 2
            *ayDstByte++ = 0; // 3
            *ayDstByte++ = 0; // 4
            *ayDstByte++ = 0; // 5
            *ayDstByte++ = 0; // 6
            *ayDstByte++ = 0; // 7
            break;
            
          case 0x1: // numeric 1B
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = 0; // 1
            *ayDstByte++ = 0; // 2
            *ayDstByte++ = 0; // 3
            *ayDstByte++ = 0; // 4
            *ayDstByte++ = 0; // 5
            *ayDstByte++ = 0; // 6
            *ayDstByte++ = 0; // 7
            break;
            
          case 0x2: // numeric 2B
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = *ayDataByte++; // 1
            *ayDstByte++ = 0; // 2
            *ayDstByte++ = 0; // 3
            *ayDstByte++ = 0; // 4
            *ayDstByte++ = 0; // 5
            *ayDstByte++ = 0; // 6
            *ayDstByte++ = 0; // 7
            break;
            
          case 0x3: // numeric 3B
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = *ayDataByte++; // 1
            *ayDstByte++ = *ayDataByte++; // 2
            *ayDstByte++ = 0; // 3
            *ayDstByte++ = 0; // 4
            *ayDstByte++ = 0; // 5
            *ayDstByte++ = 0; // 6
            *ayDstByte++ = 0; // 7
            break;
            
          case 0x4: // numeric 4B
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = *ayDataByte++; // 1
            *ayDstByte++ = *ayDataByte++; // 2
            *ayDstByte++ = *ayDataByte++; // 3
            *ayDstByte++ = 0; // 4
            *ayDstByte++ = 0; // 5
            *ayDstByte++ = 0; // 6
            *ayDstByte++ = 0; // 7
            break;
            
          case 0x5: // numeric 5B
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = *ayDataByte++; // 1
            *ayDstByte++ = *ayDataByte++; // 2
            *ayDstByte++ = *ayDataByte++; // 3
            *ayDstByte++ = *ayDataByte++; // 4
            *ayDstByte++ = 0; // 5
            *ayDstByte++ = 0; // 6
            *ayDstByte++ = 0; // 7
            break;
            
          case 0x6: // numeric 6B
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = *ayDataByte++; // 1
            *ayDstByte++ = *ayDataByte++; // 2
            *ayDstByte++ = *ayDataByte++; // 3
            *ayDstByte++ = *ayDataByte++; // 4
            *ayDstByte++ = *ayDataByte++; // 5
            *ayDstByte++ = 0; // 6
            *ayDstByte++ = 0; // 7
            break;
            
          case 0x7: // numeric 7B
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = *ayDataByte++; // 1
            *ayDstByte++ = *ayDataByte++; // 2
            *ayDstByte++ = *ayDataByte++; // 3
            *ayDstByte++ = *ayDataByte++; // 4
            *ayDstByte++ = *ayDataByte++; // 5
            *ayDstByte++ = *ayDataByte++; // 6
            *ayDstByte++ = 0; // 7
            break;
            
          case 0x8: // numeric 8B
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = *ayDataByte++; // 1
            *ayDstByte++ = *ayDataByte++; // 2
            *ayDstByte++ = *ayDataByte++; // 3
            *ayDstByte++ = *ayDataByte++; // 4
            *ayDstByte++ = *ayDataByte++; // 5
            *ayDstByte++ = *ayDataByte++; // 6
            *ayDstByte++ = *ayDataByte++; // 7
          break;
            
          case 0x9: // numeric 9B
            RASSERT(0);
            break;
            
          case 0xA: // numeric 10B
            RASSERT(0);
            break;
            
          case 0xB: // numeric 11B
            RASSERT(0);
            break;
            
          case 0xC: // numeric 12B
            RASSERT(0);
            break;
            
          case 0xD: // numeric 13B
            RASSERT(0);
            break;
            
          case 0xE: // numeric 14B
            RASSERT(0);
            break;
            
          case 0xF: // numeric 15B
            RASSERT(0);
            break;
          
        }
        break;
        
      case MN_DATE_E0: // date era-0 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        aiCellValue = (i8*)ayDstByte;
        
        *ayDstByte++ = *ayDataByte++; // 0
        *ayDstByte++ = *ayDataByte++; // 1
        *ayDstByte++ = 0; // 2
        *ayDstByte++ = 0; // 3
        *ayDstByte++ = 0; // 4
        *ayDstByte++ = 0; // 5
        *ayDstByte++ = 0; // 6
        *ayDstByte++ = 0; // 7
        
        vDteFromEdt(aiCellValue,C_ERA_0);
        break;
        
      case MN_DATE_E1: // date era-1 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        aiCellValue = (i8*)ayDstByte;
        
        *ayDstByte++ = *ayDataByte++; // 0
        *ayDstByte++ = *ayDataByte++; // 1
        *ayDstByte++ = 0; // 2
        *ayDstByte++ = 0; // 3
        *ayDstByte++ = 0; // 4
        *ayDstByte++ = 0; // 5
        *ayDstByte++ = 0; // 6
        *ayDstByte++ = 0; // 7
        
        vDteFromEdt(aiCellValue,C_ERA_1);
        break;
        
      case MN_DATE_E2: // date era-2 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        aiCellValue = (i8*)ayDstByte;
        
        *ayDstByte++ = *ayDataByte++; // 0
        *ayDstByte++ = *ayDataByte++; // 1
        *ayDstByte++ = 0; // 2
        *ayDstByte++ = 0; // 3
        *ayDstByte++ = 0; // 4
        *ayDstByte++ = 0; // 5
        *ayDstByte++ = 0; // 6
        *ayDstByte++ = 0; // 7
        
        vDteFromEdt(aiCellValue,C_ERA_2);
        break;
        
      case MN_STRING_x1: // short string 0..15B (no null) - -- -- -- -- -- -- --
        nStrLen = STRx1(uLow);
        memcpy(ayDstByte,ayDataByte,(size_t)nStrLen);
        ayDstByte += nStrLen;
        *ayDstByte++ = '\0';
        break;
        
      case MN_NULL: // null - -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
        *(i8*)ayDstByte = C_DATUM_NULL;
        ayDstByte += C_BYTES_PER_WORD;
        break;
        
      case MN_SPARE0: // spare
        RASSERT(0);
        break;
        
      case MN_OVERFLOW: // overflow 2B
        break;
        
        
      // end where uLow is n-bytes
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // toBytes(uLow) depends on uHigh
        
      case MN_STRING_x2: // string x 2 -- -- -- -- -- -- -- -- -- -- -- -- -- --
        nStrLen = STRx2(uLow);
         memcpy(ayDstByte,ayDataByte,(size_t)nStrLen);
        ayDstByte += nStrLen;
        *ayDstByte++ = '\0';
        break;
        
      case MN_STRING_x4: // string x 4 -- -- -- -- -- -- -- -- -- -- -- -- -- --
        nStrLen = STRx4(uLow);
        memcpy(ayDstByte,ayDataByte,(size_t)nStrLen);
        ayDstByte += nStrLen;
        *ayDstByte++ = '\0';
        break;
        
      case MN_STRING_x8: // string x 8 -- -- -- -- -- -- -- -- -- -- -- -- -- --
        nStrLen = STRx8(uLow);
        memcpy(ayDstByte,ayDataByte,(size_t)nStrLen);
        ayDstByte += nStrLen;
        *ayDstByte++ = '\0';
        break;
        
      case MN_STRING_LEN: // length encoded string -- -- -- -- -- -- -- -- -- --
        RASSERT(0);
        break;
        
      case MN_BLOB_0: // blob type 0 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        RASSERT(0);
        break;
        
      case MN_BLOB_1: // blob type 1 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        RASSERT(0);
        break;
        
      case MN_NEGATIVE: // -ve digit -16..-1 -- -- -- -- -- -- -- -- -- -- -- --
        aiCellValue = (i8*)ayDstByte;
        
        *ayDstByte++ = (i1)uLow + 1; // 0
        *ayDstByte++ = 0; // 1
        *ayDstByte++ = 0; // 2
        *ayDstByte++ = 0; // 3
        *ayDstByte++ = 0; // 4
        *ayDstByte++ = 0; // 5
        *ayDstByte++ = 0; // 6
        *ayDstByte++ = 0; // 7
        
        *aiCellValue = - *aiCellValue;

        break;
        
      case MN_POSITIVE: // +ve digit 1..16 - -- -- -- -- -- -- -- -- -- -- -- --
        *ayDstByte++ = uLow + 1; // 0
        *ayDstByte++ = 0; // 1
        *ayDstByte++ = 0; // 2
        *ayDstByte++ = 0; // 3
        *ayDstByte++ = 0; // 4
        *ayDstByte++ = 0; // 5
        *ayDstByte++ = 0; // 6
        *ayDstByte++ = 0; // 7
        break;
    }
    
    // end inflate column
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    aiDstk_lcl++;
    
    // move to next column to retrieve
    if (++i_WhichCol >= nWhichCols) {
      *aaiDstk = aiDstk_lcl;
      return;
    }
    
    i_ColToRetrieve = iWhichCol[i_WhichCol];
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // end for each column
}

/*----------------------------------------------------------------------------*/
/* inflate headline into row                                                  */
/*----------------------------------------------------------------------------*/
void RowInflator::vInflateColumn_v2(RowSingle* pRow, HeadLine* aHeadLine, i8 i_ColToRetrieve)
{
  NEVER_REACH;
  
  eMC  eMetaCode;
  SingleWord wWord;
  
  i8   i_Tail_rel;
  i8   nCompressed;
  i8   nLength;
  i8   nBytesPartA;
  i8   nBytesPartB;
  char caString[C_4KB];
  
  TailLine* aTail;
  
#ifdef WITH_INFLATE_TRACE
  static i8 nTotalRows = 0;
  vTrace("vInflate() of row %lld\n"_Stz,nTotalRows++);
  vDumpHead("aHeadLine"_Stz,aHeadLine,pRow->m_pColVec);
#endif
  
  char* acString = (char*)pRow->m_xWsp.ayArea();
  
  i8 nCols = pRow->m_nColumns;
  
  eMC* aeMetaCode = aHeadLine->aeMetaArea();
  
  i8 i_BaseOffsetData = RowCoder::iBaseOffsetData(pRow->m_pStreamDef);
  
  byte* ayDataByte = aAdd<byte*>(aHeadLine,i_BaseOffsetData);
  
  i8* aiColVal = & pRow->m_waColVal[0].m_iWrd;
  
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
#ifdef WITH_INFLATE_TRACE
    Column* pCol = pRow->m_pColVec->pGet(i_Col);
#endif
    
    eMetaCode = *aeMetaCode++;
    
    //! temp experiment
    if ( i_Col != i_ColToRetrieve ) {
      
      *aiColVal++ = C_UNKNOWN;
      
    L_SkipCodeSwitch:
      
      switch(eMetaCode) {
          
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        case MC_STR_FIT_1L:
          nLength = *(u1*)ayDataByte++;
          ayDataByte += nLength;
          break;
          
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        case MC_STR_FIT_2L:
          wWord.m_iI8 = 0;
          wWord.m_yaByte[0] = *ayDataByte++;
          wWord.m_yaByte[1] = *ayDataByte++;
          nLength = wWord.m_iI8;
          ayDataByte += nLength;
          break;
          
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        case MC_OVERFLOW_2T: {
          //vDebugPoint();
          OverLine* aOverLine = aHeadLine->aGetOver();
          //aOverLine->vDump();
          
          eMetaCode = * aAdd<eMC*>(aOverLine,C_U_1B);
          ayDataByte = aAdd<byte*>(aOverLine,C_U_1B + C_M_1B);
          goto L_SkipCodeSwitch;
          break;
        }
          
          // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        default: ayDataByte += nDataSizeOfCode(eMetaCode); break;
      }
      
      continue;
    }
    
  L_MetaCodeSwitch:
    switch (eMetaCode) {
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_NULL:
        *aiColVal++ = C_DATUM_NULL;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_1B:
      case MC_FLT_1B:
      case MC_INT_1B:
      case MC_IVL_1B:
      case MC_TME_1B:
        *aiColVal++ = *(u1*)(ayDataByte);
        ayDataByte += C_1B;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_2B:
      case MC_FLT_2B:
      case MC_INT_2B:
      case MC_IVL_2B:
      case MC_TME_2B:
        *aiColVal++ = *(u2*)(ayDataByte);
        ayDataByte += C_2B;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_3B:
      case MC_FLT_3B:
      case MC_INT_3B:
      case MC_IVL_3B:
      case MC_TME_3B:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        *aiColVal++ = wWord.m_iI8;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_4B:
      case MC_FLT_4B:
      case MC_INT_4B:
      case MC_IVL_4B:
      case MC_TME_4B:
        *aiColVal++ = *(u4*)(ayDataByte);
        ayDataByte += C_4B;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_5B:
      case MC_FLT_5B:
      case MC_INT_5B:
      case MC_IVL_5B:
      case MC_TME_5B:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        wWord.m_yaByte[3] = *ayDataByte++;
        wWord.m_yaByte[4] = *ayDataByte++;
        *aiColVal++ = wWord.m_iI8;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_6B:
      case MC_FLT_6B:
      case MC_INT_6B:
      case MC_IVL_6B:
      case MC_TME_6B:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        wWord.m_yaByte[3] = *ayDataByte++;
        wWord.m_yaByte[4] = *ayDataByte++;
        wWord.m_yaByte[5] = *ayDataByte++;
        *aiColVal++ = wWord.m_iI8;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_7B:
      case MC_FLT_7B:
      case MC_INT_7B:
      case MC_IVL_7B:
      case MC_TME_7B:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        wWord.m_yaByte[3] = *ayDataByte++;
        wWord.m_yaByte[4] = *ayDataByte++;
        wWord.m_yaByte[5] = *ayDataByte++;
        wWord.m_yaByte[6] = *ayDataByte++;
        *aiColVal++ = wWord.m_iI8;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_8B:
      case MC_FLT_8B:
      case MC_INT_8B:
      case MC_IVL_8B:
      case MC_TME_8B:
      case MC_TSP_8B:
        *aiColVal++ = *(u8*)(ayDataByte);
        ayDataByte += C_8B;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_M_ONE:
      case MC_INT_M_ONE:
        *aiColVal++ = -1;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_ZERO:
      case MC_INT_ZERO:
        *aiColVal++ =  0;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_ONE:
      case MC_INT_ONE:
        *aiColVal++ =  1;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_TWO:
      case MC_INT_TWO:
        *aiColVal++ =  2;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_THREE:
      case MC_INT_THREE:
        *aiColVal++ =  3;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_FOUR:
      case MC_INT_FOUR:
        *aiColVal++ =  4;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_FIVE:
      case MC_INT_FIVE:
        *aiColVal++ =  5;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_SIX:
      case MC_INT_SIX:
        *aiColVal++ =  6;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_SEVEN:
      case MC_INT_SEVEN:
        *aiColVal++ =  7;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_EIGHT:
      case MC_INT_EIGHT:
        *aiColVal++ =  8;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_NINE:
      case MC_INT_NINE:
        *aiColVal++ =  9;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_TEN:
      case MC_INT_TEN:
        *aiColVal++ = 10;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BOL_FALSE:
        *aiColVal++ =  0;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BOL_TRUE:
        *aiColVal++ =  1;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_DTE_E0_2B:
        *aiColVal++ = *(u2*)ayDataByte;
        ayDataByte += C_2B;
        vDteFromEdt((aiColVal-1),C_ERA_0);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_DTE_E1_2B:
        *aiColVal++ = *(u2*)ayDataByte;
        ayDataByte += C_2B;
        vDteFromEdt((aiColVal-1),C_ERA_1);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_DTE_E2_2B:
        *aiColVal++ = *(u2*)ayDataByte;
        ayDataByte += C_2B;
        vDteFromEdt((aiColVal-1),C_ERA_2);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_STR_EMPTY:
        *(char**)(aiColVal++) = acString;
        *acString++ = '\0';
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_STR_FIT_1L:
        nLength = *(u1*)ayDataByte++;
        MemLib::vCopy((byte*)acString,
                      (byte*)ayDataByte,
                      nLength,
                      CN_ROWCOMPACTOR);
        *(char**)(aiColVal++) = acString;
        ayDataByte += nLength;
        acString   += nLength;
        *acString++ = '\0';
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_STR_FIT_2L:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        nLength = wWord.m_iI8;
        MemLib::vCopy((byte*)acString,
                      (byte*)ayDataByte,
                      nLength,
                      CN_ROWCOMPACTOR);
        *(char**)(aiColVal++) = acString;
        ayDataByte += nLength;
        acString   += nLength;
        *acString++ = '\0';
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_STR_TAIL_2T:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        i_Tail_rel = wWord.m_iI8;
        aTail = aHeadLine->aGetTailRel(i_Tail_rel);
        nLength = aTail->w8_1B(+ C_U_1B);
        MemLib::vCopy(acString,(byte*)aTail+C_U_1B+C_L_1B,nLength,CN_ROWCOMPACTOR);
        *(char**)(aiColVal++) = acString;
        acString += nLength;
        *acString++ = '\0';
        break;
        
         // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_OVERFLOW_2T: {
        //vDebugPoint();
        OverLine* aOverLine = aHeadLine->aGetOver();
        //aOverLine->vDump();

        eMetaCode = * aAdd<eMC*>(aOverLine,C_U_1B);
        ayDataByte = aAdd<byte*>(aOverLine,C_U_1B + C_M_1B);
        goto L_MetaCodeSwitch;
        break;
      }
        
      default:
        RASSERT(0);
    }
    
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef WITH_INFLATE_TRACE
    if (C_DATUM_NULL == *(aiValue-1)) {
      vTrace(" [%lld %15s] null\n"_Stz,i_Col,pCol->szColNm());
      continue; // to next i_Col
    }
    
    switch (pCol->eDatum()) {
      case DM_BOL:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_BCD:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_DTE:
        wWord.m_iDte = *(aiValue-1);
        vTrace(" [%lld %15s] %4lld : %02lld-%02lld-%02lld\n"_Stz,
               i_Col,
               pCol->szColNm(),
               wWord.m_iDte,
               (i8)wWord.xDte.m_uYearDelta + C_YEAR_BASE,
               (i8)wWord.xDte.m_uMonth,
               (i8)wWord.xDte.m_uDay);
        break;
      case DM_FLT:
        vTrace(" [%lld %15s] %lg\n"_Stz,i_Col,pCol->szColNm(),*(d8*)(aiValue-1));
        break;
      case DM_INT:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_IVL:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_TME:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_TSP:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_VCH:
        vTrace(" [%lld %15s] %s\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
        
      default:
        RASSERT(0);
    }
#endif
    
    return;
  }
}


/*----------------------------------------------------------------------------*/
/* inflate headline into row                                                  */
/*----------------------------------------------------------------------------*/
void RowInflator::vInflateRow_v2(RowSingle* pRow, HeadLine* aHeadLine)
{
  NEVER_REACH;
  
  eMC  eMetaCode;
  SingleWord wWord;
  
  i8   i_Tail_rel;
  i8   nCompressed;
  i8   nLength;
  i8   nBytesPartA;
  i8   nBytesPartB;
  char caString[C_4KB];
  
  TailLine* aTail;
  
#ifdef WITH_INFLATE_TRACE
  static i8 nTotalRows = 0;
  vTrace("vInflate() of row %lld\n"_Stz,nTotalRows++);
  vDumpHead("aHeadLine"_Stz,aHeadLine,pRow->m_pColVec);
#endif
  
  char* acString = (char*)pRow->m_xWsp.ayArea();
  
  i8 nCols = pRow->m_nColumns;
  
  eMC*  aeMetaCode = aHeadLine->aeMetaArea();
  
  i8 i_BaseOffsetData = RowCoder::iBaseOffsetData(pRow->m_pStreamDef);

  byte* ayDataByte = aAdd<byte*>(aHeadLine,i_BaseOffsetData);
  
  i8* aiColVal = & pRow->m_waColVal[0].m_iWrd;
  
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
#ifdef WITH_INFLATE_TRACE
    Column* pCol = pRow->m_pColVec->pGet(i_Col);
#endif
    
    eMetaCode = *aeMetaCode++;
    
  L_MetaCodeSwitch:
    switch (eMetaCode) {
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_NULL:
        *aiColVal++ = C_DATUM_NULL;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_1B:
      case MC_FLT_1B:
      case MC_INT_1B:
      case MC_IVL_1B:
      case MC_TME_1B:
        *aiColVal++ = *(u1*)(ayDataByte);
        ayDataByte += C_1B;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_2B:
      case MC_FLT_2B:
      case MC_INT_2B:
      case MC_IVL_2B:
      case MC_TME_2B:
        *aiColVal++ = *(u2*)(ayDataByte);
        ayDataByte += C_2B;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_3B:
      case MC_FLT_3B:
      case MC_INT_3B:
      case MC_IVL_3B:
      case MC_TME_3B:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        *aiColVal++ = wWord.m_iI8;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_4B:
      case MC_FLT_4B:
      case MC_INT_4B:
      case MC_IVL_4B:
      case MC_TME_4B:
        *aiColVal++ = *(u4*)(ayDataByte);
        ayDataByte += C_4B;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_5B:
      case MC_FLT_5B:
      case MC_INT_5B:
      case MC_IVL_5B:
      case MC_TME_5B:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        wWord.m_yaByte[3] = *ayDataByte++;
        wWord.m_yaByte[4] = *ayDataByte++;
        *aiColVal++ = wWord.m_iI8;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_6B:
      case MC_FLT_6B:
      case MC_INT_6B:
      case MC_IVL_6B:
      case MC_TME_6B:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        wWord.m_yaByte[3] = *ayDataByte++;
        wWord.m_yaByte[4] = *ayDataByte++;
        wWord.m_yaByte[5] = *ayDataByte++;
        *aiColVal++ = wWord.m_iI8;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_7B:
      case MC_FLT_7B:
      case MC_INT_7B:
      case MC_IVL_7B:
      case MC_TME_7B:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        wWord.m_yaByte[3] = *ayDataByte++;
        wWord.m_yaByte[4] = *ayDataByte++;
        wWord.m_yaByte[5] = *ayDataByte++;
        wWord.m_yaByte[6] = *ayDataByte++;
        *aiColVal++ = wWord.m_iI8;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_8B:
      case MC_FLT_8B:
      case MC_INT_8B:
      case MC_IVL_8B:
      case MC_TME_8B:
      case MC_TSP_8B:
        *aiColVal++ = *(u8*)(ayDataByte);
        ayDataByte += C_8B;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_M_ONE:
      case MC_INT_M_ONE:
        *aiColVal++ = -1;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_ZERO:
      case MC_INT_ZERO:
        *aiColVal++ =  0;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_ONE:
      case MC_INT_ONE:
        *aiColVal++ =  1;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_TWO:
      case MC_INT_TWO:
        *aiColVal++ =  2;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_THREE:
      case MC_INT_THREE:
        *aiColVal++ =  3;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_FOUR:
      case MC_INT_FOUR:
        *aiColVal++ =  4;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_FIVE:
      case MC_INT_FIVE:
        *aiColVal++ =  5;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_SIX:
      case MC_INT_SIX:
        *aiColVal++ =  6;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_SEVEN:
      case MC_INT_SEVEN:
        *aiColVal++ =  7;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_EIGHT:
      case MC_INT_EIGHT:
        *aiColVal++ =  8;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_NINE:
      case MC_INT_NINE:
        *aiColVal++ =  9;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_TEN:
      case MC_INT_TEN:
        *aiColVal++ = 10;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BOL_FALSE:
        *aiColVal++ =  0;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BOL_TRUE:
        *aiColVal++ =  1;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_DTE_E0_2B:
        *aiColVal++ = *(u2*)ayDataByte;
        ayDataByte += C_2B;
        vDteFromEdt((aiColVal-1),C_ERA_0);
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_DTE_E1_2B:
        *aiColVal++ = *(u2*)ayDataByte;
        ayDataByte += C_2B;
        vDteFromEdt((aiColVal-1),C_ERA_1);
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_DTE_E2_2B:
        *aiColVal++ = *(u2*)ayDataByte;
        ayDataByte += C_2B;
        vDteFromEdt((aiColVal-1),C_ERA_2);
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_STR_EMPTY:
        *(char**)(aiColVal++) = acString;
        *acString++ = '\0';
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_STR_FIT_1L:
        nLength = *(u1*)ayDataByte++;
        MemLib::vCopy((byte*)acString,
                      (byte*)ayDataByte,
                      nLength,
                      CN_ROWCOMPACTOR);
        *(char**)(aiColVal++) = acString;
        ayDataByte += nLength;
        acString   += nLength;
        *acString++ = '\0';
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_STR_FIT_2L:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        nLength = wWord.m_iI8;
        MemLib::vCopy((byte*)acString,
                      (byte*)ayDataByte,
                      nLength,
                      CN_ROWCOMPACTOR);
        *(char**)(aiColVal++) = acString;
        ayDataByte += nLength;
        acString   += nLength;
        *acString++ = '\0';
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_STR_TAIL_2T:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        i_Tail_rel = wWord.m_iI8;
        aTail = aHeadLine->aGetTailRel(i_Tail_rel);
        nLength = aTail->w8_1B(+ C_U_1B);
        MemLib::vCopy(acString,(byte*)aTail+C_U_1B+C_L_1B,nLength,CN_ROWCOMPACTOR);
        *(char**)(aiColVal++) = acString;
        acString += nLength;
        *acString++ = '\0';
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_OVERFLOW_2T: {
        OverLine* aOverLine = aHeadLine->aGetOver();
        eMetaCode = * aAdd<eMC*>(aOverLine,C_U_1B);
        ayDataByte = aAdd<byte*>(aOverLine,C_U_1B + C_M_1B);
        goto L_MetaCodeSwitch;
        break;
      }
        
      default:
        RASSERT(0);
    }
    

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef WITH_INFLATE_TRACE
    if (C_DATUM_NULL == *(aiValue-1)) {
      vTrace(" [%lld %15s] null\n"_Stz,i_Col,pCol->szColNm());
      continue; // to next i_Col
    }
        
    switch (pCol->eDatum()) {
      case DM_BOL:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_BCD:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_DTE:
        wWord.m_iDte = *(aiValue-1);
        vTrace(" [%lld %15s] %4lld : %02lld-%02lld-%02lld\n"_Stz,
               i_Col,
               pCol->szColNm(),
               wWord.m_iDte,
               (i8)wWord.xDte.m_uYearDelta + C_YEAR_BASE,
               (i8)wWord.xDte.m_uMonth,
               (i8)wWord.xDte.m_uDay);
        break;
      case DM_FLT:
        vTrace(" [%lld %15s] %lg\n"_Stz,i_Col,pCol->szColNm(),*(d8*)(aiValue-1));
        break;
      case DM_INT:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_IVL:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_TME:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_TSP:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_VCH:
        vTrace(" [%lld %15s] %s\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;

      default:
        RASSERT(0);
    }
#endif
  }
}

/*----------------------------------------------------------------------------*/
/* inflate headline into row                                                  */
/*----------------------------------------------------------------------------*/
void RowInflator::vInflateRow_v2(RowSlab* pRowSlab, HeadLine* aHeadLine)
{
  NEVER_REACH;
  
  eMC  eMetaCode;
  SingleWord wWord;
  
  i8   i_Tail_rel;
  i8   nLength;
  char* aString;
  
  TailLine* aTail;
  
#ifdef WITH_INFLATE_TRACE
  static i8 nTotalRows = 0;
  vTrace("vInflate() of row %lld\n"_Stz,nTotalRows++);
  vDumpHead("aHeadLine"_Stz,aHeadLine,pRow->m_pColVec);
#endif
  
  //char* acString = (char*)pRow->m_xWsp.ayArea();
  
  i8 nCols = pRowSlab->aStreamPix()->m_nCols - pRowSlab->aStreamPix()->m_nExtraCols;
  
  eMC*  aeMetaCode = aHeadLine->aeMetaArea();
  
  i8 i_BaseOffsetData = RowCoder::iBaseOffsetData(pRowSlab->aStreamPix());
  
  byte* ayDataByte = aAdd<byte*>(aHeadLine,i_BaseOffsetData);
  
  //i8* aiColVal = & pRowSlab->m_waColVal[0].m_iWrd;
  m8* aCell;
  
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
#ifdef WITH_INFLATE_TRACE
    //Column* pCol = pRow->m_pColVec->pGet(i_Col);
#endif
    
    eMetaCode = *aeMetaCode++;
    
  L_MetaCodeSwitch:
    switch (eMetaCode) {
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_NULL:
        pRowSlab->vDeposit(C_DATUM_NULL);
        //*aiColVal++ = C_DATUM_NULL;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_1B:
      case MC_FLT_1B:
      case MC_INT_1B:
      case MC_IVL_1B:
      case MC_TME_1B:
        //*aiColVal++ = *(u1*)(ayDataByte);
        pRowSlab->vDeposit((i8)(*(u1*)(ayDataByte)));
        ayDataByte += C_1B;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_2B:
      case MC_FLT_2B:
      case MC_INT_2B:
      case MC_IVL_2B:
      case MC_TME_2B:
        //*aiColVal++ = *(u2*)(ayDataByte);
        pRowSlab->vDeposit((i8)(*(u2*)(ayDataByte)));
        ayDataByte += C_2B;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_3B:
      case MC_FLT_3B:
      case MC_INT_3B:
      case MC_IVL_3B:
      case MC_TME_3B:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        //*aiColVal++ = wWord.m_iI8;
        pRowSlab->vDeposit((i8)wWord.m_iI8);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_4B:
      case MC_FLT_4B:
      case MC_INT_4B:
      case MC_IVL_4B:
      case MC_TME_4B:
        //*aiColVal++ = *(u4*)(ayDataByte);
        pRowSlab->vDeposit((i8)(*(u4*)(ayDataByte)));
        ayDataByte += C_4B;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_5B:
      case MC_FLT_5B:
      case MC_INT_5B:
      case MC_IVL_5B:
      case MC_TME_5B:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        wWord.m_yaByte[3] = *ayDataByte++;
        wWord.m_yaByte[4] = *ayDataByte++;
        //*aiColVal++ = wWord.m_iI8;
        pRowSlab->vDeposit((i8)wWord.m_iI8);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_6B:
      case MC_FLT_6B:
      case MC_INT_6B:
      case MC_IVL_6B:
      case MC_TME_6B:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        wWord.m_yaByte[3] = *ayDataByte++;
        wWord.m_yaByte[4] = *ayDataByte++;
        wWord.m_yaByte[5] = *ayDataByte++;
        //*aiColVal++ = wWord.m_iI8;
        pRowSlab->vDeposit((i8)wWord.m_iI8);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_7B:
      case MC_FLT_7B:
      case MC_INT_7B:
      case MC_IVL_7B:
      case MC_TME_7B:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        wWord.m_yaByte[3] = *ayDataByte++;
        wWord.m_yaByte[4] = *ayDataByte++;
        wWord.m_yaByte[5] = *ayDataByte++;
        wWord.m_yaByte[6] = *ayDataByte++;
        //*aiColVal++ = wWord.m_iI8;
        pRowSlab->vDeposit((i8)wWord.m_iI8);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_8B:
      case MC_FLT_8B:
      case MC_INT_8B:
      case MC_IVL_8B:
      case MC_TME_8B:
      case MC_TSP_8B:
        //*aiColVal++ = *(u8*)(ayDataByte);
        pRowSlab->vDeposit((i8)(*(u8*)(ayDataByte)));
        ayDataByte += C_8B;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_M_ONE:
      case MC_INT_M_ONE:
        //*aiColVal++ = -1;
        pRowSlab->vDeposit((i8)-1);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_ZERO:
      case MC_INT_ZERO:
        //*aiColVal++ =  0;
        pRowSlab->vDeposit((i8)0);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_ONE:
      case MC_INT_ONE:
        //*aiColVal++ =  1;
        pRowSlab->vDeposit((i8)1);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_TWO:
      case MC_INT_TWO:
        //*aiColVal++ =  2;
        pRowSlab->vDeposit((i8)2);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_THREE:
      case MC_INT_THREE:
        //*aiColVal++ =  3;
        pRowSlab->vDeposit((i8)3);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_FOUR:
      case MC_INT_FOUR:
        //*aiColVal++ =  4;
        pRowSlab->vDeposit((i8)4);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_FIVE:
      case MC_INT_FIVE:
        //*aiColVal++ =  5;
        pRowSlab->vDeposit((i8)5);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_SIX:
      case MC_INT_SIX:
        //*aiColVal++ =  6;
        pRowSlab->vDeposit((i8)6);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_SEVEN:
      case MC_INT_SEVEN:
        //*aiColVal++ =  7;
        pRowSlab->vDeposit((i8)7);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_EIGHT:
      case MC_INT_EIGHT:
        //*aiColVal++ =  8;
        pRowSlab->vDeposit((i8)8);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_NINE:
      case MC_INT_NINE:
        //*aiColVal++ =  9;
        pRowSlab->vDeposit((i8)9);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_TEN:
      case MC_INT_TEN:
        //*aiColVal++ = 10;
        pRowSlab->vDeposit((i8)10);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BOL_FALSE:
        //*aiColVal++ =  0;
        pRowSlab->vDeposit((i8)0);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BOL_TRUE:
        //*aiColVal++ =  1;
        pRowSlab->vDeposit((i8)1);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_DTE_E0_2B:
        //*aiColVal++ = *(u2*)ayDataByte;
        //ayDataByte += C_2B;
        //vDteFromEdt((aiColVal-1),C_ERA_0);
        
//        pRowSlab->vDeposit((i8)(*(u2*)(ayDataByte)));
//        ayDataByte += C_2B;
//        pRowSlab->vDepositCursor(&aCell);
//        vDteFromEdt((i8*)aCell,C_ERA_0);
        
        pRowSlab->vDeposit(iDteFromEdt((i8)(*(u2*)(ayDataByte)),C_ERA_0));
        ayDataByte += C_2B;

        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_DTE_E1_2B:
        //*aiColVal++ = *(u2*)ayDataByte;
        //ayDataByte += C_2B;
        //vDteFromEdt((aiColVal-1),C_ERA_1);
        
//        pRowSlab->vDeposit((i8)(*(u2*)(ayDataByte)));
//        ayDataByte += C_2B;
//        pRowSlab->vDepositCursor(&aCell);
//        vDteFromEdt((i8*)aCell,C_ERA_1);
        
        pRowSlab->vDeposit(iDteFromEdt((i8)(*(u2*)(ayDataByte)),C_ERA_1));
        ayDataByte += C_2B;

        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_DTE_E2_2B:
        //*aiColVal++ = *(u2*)ayDataByte;
        //ayDataByte += C_2B;
        //vDteFromEdt((aiColVal-1),C_ERA_2);
        
//        pRowSlab->vDeposit((i8)(*(u2*)(ayDataByte)));
//        ayDataByte += C_2B;
//        pRowSlab->vDepositCursor(&aCell);
//        vDteFromEdt((i8*)aCell,C_ERA_2);
        
        pRowSlab->vDeposit(iDteFromEdt((i8)(*(u2*)(ayDataByte)),C_ERA_2));
        ayDataByte += C_2B;

        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_STR_EMPTY:
        //*(char**)(aiColVal++) = acString;
        //*acString++ = '\0';
        aString = pRowSlab->azDepositString_begin();
        *aString = '\0';
        pRowSlab->vDepositString_end(1);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_STR_FIT_1L:
        //nLength = *(u1*)ayDataByte++;
        //MemLib::vCopy((byte*)acString,
        //              (byte*)ayDataByte,
        //              nLength,
        //              CN_ROWCOMPACTOR);
        //*(char**)(aiColVal++) = acString;
        //ayDataByte += nLength;
        //acString   += nLength;
        //*acString++ = '\0';
        aString = pRowSlab->azDepositString_begin();
        nLength = *(u1*)ayDataByte++;
        MemLib::vCopy((byte*)aString,
                      (byte*)ayDataByte,
                      nLength,
                      CN_ROWCOMPACTOR);
        ayDataByte += nLength;
        aString    += nLength;
        *aString = '\0';
        pRowSlab->vDepositString_end(nLength);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_STR_FIT_2L:
//        wWord.m_iI8 = 0;
//        wWord.m_yaByte[0] = *ayDataByte++;
//        wWord.m_yaByte[1] = *ayDataByte++;
//        nLength = wWord.m_iI8;
//        MemLib::vCopy((byte*)acString,
//                      (byte*)ayDataByte,
//                      nLength,
//                      CN_ROWCOMPACTOR);
//        *(char**)(aiColVal++) = acString;
//        ayDataByte += nLength;
//        acString   += nLength;
//        *acString++ = '\0';
//
        
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        nLength = wWord.m_iI8;
        aString = pRowSlab->azDepositString_begin();
        MemLib::vCopy((byte*)aString,
                      (byte*)ayDataByte,
                      nLength,
                      CN_ROWCOMPACTOR);
        ayDataByte += nLength;
        aString   += nLength;
        *aString++ = '\0';
        pRowSlab->vDepositString_end(nLength);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_STR_TAIL_2T:
//        wWord.m_iI8 = 0;
//        wWord.m_yaByte[0] = *ayDataByte++;
//        wWord.m_yaByte[1] = *ayDataByte++;
//        i_Tail_rel = wWord.m_iI8;
//        aTail = aHeadLine->aGetTailRel(i_Tail_rel);
//        nLength = aTail->w8_1B(+ C_U_1B);
//        MemLib::vCopy(acString,(byte*)aTail+C_U_1B+C_L_1B,nLength,CN_ROWCOMPACTOR);
//        *(char**)(aiColVal++) = acString;
//        acString += nLength;
//        *acString++ = '\0';
        
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        i_Tail_rel = wWord.m_iI8;
        aTail = aHeadLine->aGetTailRel(i_Tail_rel);
        nLength = aTail->w8_1B(+ C_U_1B);
        aString = pRowSlab->azDepositString_begin();
        MemLib::vCopy(aString,(byte*)aTail+C_U_1B+C_L_1B,nLength,CN_ROWCOMPACTOR);
        aString += nLength;
        *aString++ = '\0';
        pRowSlab->vDepositString_end(nLength);
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_OVERFLOW_2T: {
        OverLine* aOverLine = aHeadLine->aGetOver();
        eMetaCode = * aAdd<eMC*>(aOverLine,C_U_1B);
        ayDataByte = aAdd<byte*>(aOverLine,C_U_1B + C_M_1B);
        goto L_MetaCodeSwitch;
        break;
      }
        
      default:
        RASSERT(0);
    }
    
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef WITH_INFLATE_TRACE
    if (C_DATUM_NULL == *(aiValue-1)) {
      vTrace(" [%lld %15s] null\n"_Stz,i_Col,pCol->szColNm());
      continue; // to next i_Col
    }
    
    switch (pCol->eDatum()) {
      case DM_BOL:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_BCD:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_DTE:
        wWord.m_iDte = *(aiValue-1);
        vTrace(" [%lld %15s] %4lld : %02lld-%02lld-%02lld\n"_Stz,
               i_Col,
               pCol->szColNm(),
               wWord.m_iDte,
               (i8)wWord.xDte.m_uYearDelta + C_YEAR_BASE,
               (i8)wWord.xDte.m_uMonth,
               (i8)wWord.xDte.m_uDay);
        break;
      case DM_FLT:
        vTrace(" [%lld %15s] %lg\n"_Stz,i_Col,pCol->szColNm(),*(d8*)(aiValue-1));
        break;
      case DM_INT:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_IVL:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_TME:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_TSP:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_VCH:
        vTrace(" [%lld %15s] %s\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
        
      default:
        RASSERT(0);
    }
#endif
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
