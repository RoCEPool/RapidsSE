/*============================================================================*/
/* RowInflator.cpp -- Row Inflator Mark V                                     */
/*============================================================================*/

#include "CompactBuffers.h"

/*----------------------------------------------------------------------------*/
/* inflate a signle column onto Dstk                                          */
/*----------------------------------------------------------------------------*/
void RowInflator::vInflateColI_stk(i8** aaiDstk, HeadLine* aHeadLine,
                                   StreamDef* pStreamDef)
{
  eMC  eMetaCode;
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

  i8 nWhichCols = 0;
  i8 iWhichCol[C_MAX_COLUMNS];
  
  i8 i_ColToRetrieve = *((*aaiDstk)--);
  RASSERT(i_ColToRetrieve >= 0 && i_ColToRetrieve < nCols);

  iWhichCol[nWhichCols++] = i_ColToRetrieve;
  
  //i8 iEndMarker = **aaiDstk; (*aaiDstk)--;
  i8 iEndMarker = *((*aaiDstk)--);
  RASSERT(-1 == iEndMarker);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  
  i8 i_WhichCol = 0;
  i_ColToRetrieve = iWhichCol[i_WhichCol];
  
  i8* aiColVal = ++*aaiDstk;
  
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
        
    eMetaCode = *aeMetaCode++;
    
    if (i_Col != i_ColToRetrieve) {
            
    L_SkipCodeSwitch:
      
      switch(eMetaCode) {
          
        case MC_STR_FIT_1L:
          nLength = *(u1*)ayDataByte++;
          ayDataByte += nLength;
          break;
          
        case MC_STR_FIT_2L:
          wWord.m_iI8 = 0;
          wWord.m_yaByte[0] = *ayDataByte++;
          wWord.m_yaByte[1] = *ayDataByte++;
          nLength = wWord.m_iI8;
          ayDataByte += nLength;
          break;
          
        case MC_OVERFLOW_2T: {
          OverLine* aOverLine = aHeadLine->aGetOver();
          eMetaCode = * aAdd<eMC*>(aOverLine,C_U_1B);
          ayDataByte = aAdd<byte*>(aOverLine,C_U_1B + C_M_1B);
          goto L_SkipCodeSwitch;
          break;
        }
          
        default:
          ayDataByte += nDataSize(eMetaCode);
          break;
      }
      
      continue;
    }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
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
        /*
        nLength = *(u1*)ayDataByte++;
        MemLib::vCopy((byte*)acString,
                      (byte*)ayDataByte,
                      nLength,
                      CN_ROWCOMPACTOR);
        *(char**)(aiColVal++) = acString;
        ayDataByte += nLength;
        acString   += nLength;
        *acString++ = '\0';
        */
        
        nLength = *(u1*)ayDataByte++;
        
        acString = (char*)*aaiDstk;
        
        MemLib::vCopy((byte*)acString,
                      (byte*)ayDataByte,
                      nLength,
                      CN_ROWCOMPACTOR);
        
        acString   += nLength;
        *acString++ = '\0';
        
        *aaiDstk += (nLength+1) / 8 + 1;
        *(++*aaiDstk) = nLength+1;

        ayDataByte += nLength;
        
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_STR_FIT_2L:
        /*
        RASSERT(0); // temp
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
        */

        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        nLength = wWord.m_iI8;
        
        MemLib::vCopy((byte*)acString,
                     (byte*)ayDataByte,
                     nLength,
                     CN_ROWCOMPACTOR);

        acString   += nLength;
        *acString++ = '\0';

        *aaiDstk += (nLength+1) / 8 + 1;
        *(++*aaiDstk) = nLength+1;

        ayDataByte += nLength;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_STR_TAIL_2T:
        /*
        RASSERT(0); // temp
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
        */
        
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        i_Tail_rel = wWord.m_iI8;
        aTail = aHeadLine->aGetTailRel(i_Tail_rel);
        nLength = aTail->w8_1B(+ C_U_1B);
        
        acString = (char*)*aaiDstk;

        MemLib::vCopy((byte*)acString,
                      (byte*)aTail+C_U_1B+C_L_1B,
                      nLength,
                      CN_ROWCOMPACTOR);
        
        acString   += nLength;
        *acString++ = '\0';

        *aaiDstk += (nLength+1) / 8 + 1;
        *(++*aaiDstk) = nLength+1;

        ayDataByte += nLength;

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
    
    return;
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* inflate headline into row                                                  */
/*----------------------------------------------------------------------------*/
void RowInflator::vInflateColumn(RowSingle* pRow, HeadLine* aHeadLine, i8 i_ColToRetrieve)
{
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
        default: ayDataByte += nDataSize(eMetaCode); break;
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
void RowInflator::vInflateColumns_stk(i8** aaiDstk, HeadLine* aHeadLine,
                                      StreamPix* pStreamPix)
{
  eMC  eMetaCode;
  SingleWord wWord;
  
  i8   i_Tail_rel;
  i8   nLength;
  char* aString;
  char* acString = nullptr;
  
  TailLine* aTail;
  
#ifdef WITH_INFLATE_TRACE
  static i8 nTotalRows = 0;
  vTrace("vInflate() of row %lld\n"_Stz,nTotalRows++);
  vDumpHead("aHeadLine"_Stz,aHeadLine,pRow->m_pColVec);
#endif
  
  eMC* aeMetaCode = aHeadLine->aeMetaArea();
  i8 nCols = pStreamPix->m_nCols - pStreamPix->m_nExtraCols;
  
  i8 i_BaseOffsetData = C_W_1B   // (ControlDate) 1 byte for the headline size (in words)
                        + C_T_8B // (ControlDate) 8 bytes for the perspective
                        + nCols; // metaSize
  
  byte* ayDataByte = aAdd<byte*>(aHeadLine,i_BaseOffsetData);
  
  i8* aiColVal = ++*aaiDstk;
  
  i8 i_Ofs;
  char* azStz;
 
  //i8* iaRowSlabCells = (i8*)pRowSlab->amCellArray();
  
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
#ifdef WITH_INFLATE_TRACE
    Column* pCol = pRow->m_pColVec->pGet(i_Col);
#endif
    
    eMetaCode = *aeMetaCode++;
    
    // column to be projected ?
    if ( ! pStreamPix->m_bProject[i_Col] ) {
      
      
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
        default: ayDataByte += nDataSize(eMetaCode); break;
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
        acString = (char*)(aiColVal++);
        *acString++ = '\0';
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_STR_FIT_1L:
        nLength = *(u1*)ayDataByte++;
        
        acString = (char*)*aiColVal;
        
        MemLib::vCopy((byte*)acString,
                      (byte*)ayDataByte,
                      nLength,
                      CN_ROWCOMPACTOR);
        
        acString   += nLength;
        *acString++ = '\0';
        
        *aiColVal += (nLength+1) / 8 + 1;
        *((i8*)++*aiColVal) = nLength+1;
        
        ayDataByte += nLength;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_STR_FIT_2L:
        nLength = *(u1*)ayDataByte++;
        
        acString = (char*)*aiColVal;
        
        MemLib::vCopy((byte*)acString,
                      (byte*)ayDataByte,
                      nLength,
                      CN_ROWCOMPACTOR);
        
        acString   += nLength;
        *acString++ = '\0';
        
        *aiColVal += (nLength+1) / 8 + 1;
        *((i8*)++*aiColVal) = nLength+1;
        
        ayDataByte += nLength;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_STR_TAIL_2T:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        i_Tail_rel = wWord.m_iI8;
        aTail = aHeadLine->aGetTailRel(i_Tail_rel);
        nLength = aTail->w8_1B(+ C_U_1B);
        
        acString = (char*)*aiColVal;
        
        MemLib::vCopy((byte*)acString,
                      (byte*)aTail+C_U_1B+C_L_1B,
                      nLength,
                      CN_ROWCOMPACTOR);
        
        acString   += nLength;
        *acString++ = '\0';
        
        *aaiDstk += (nLength+1) / 8 + 1;
        *((i8*)++*aiColVal) = nLength+1;
        
        ayDataByte += nLength;
        
        break;
        
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
  
  }
}

/*----------------------------------------------------------------------------*/
/* inflate headline into row                                                  */
/*----------------------------------------------------------------------------*/
void RowInflator::vInflateColumns(RowSlab* pRowSlab, HeadLine* aHeadLine)
{
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
  StreamPix* pStreamPix = pRowSlab->aStreamPix();
  
  i8 nCols = pRowSlab->aStreamPix()->m_nCols - pStreamPix->m_nExtraCols;
  
  eMC* aeMetaCode = aHeadLine->aeMetaArea();
  
  i8 i_BaseOffsetData = RowCoder::iBaseOffsetData(pRowSlab->aStreamPix());
  
  byte* ayDataByte = aAdd<byte*>(aHeadLine,i_BaseOffsetData);
  
  //i8* aiColVal = & pRow->m_waColVal[0].m_iWrd;
  i8 i_Ofs;
  char* azStz;
  
  //i8* iaRowSlabCells = (i8*)pRowSlab->amCellArray();
  
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
#ifdef WITH_INFLATE_TRACE
    Column* pCol = pRow->m_pColVec->pGet(i_Col);
#endif
    
    eMetaCode = *aeMetaCode++;
    
    // column to be projected ?
    if ( ! pStreamPix->m_bProject[i_Col] ) {
      
      //pRowSlab->vDeposit(C_UNKNOWN);
      
      //iaRowSlabCells[pRowSlab->i_CellAdv()] = C_DATUM_NULL;
      //i8 iIgnore = pRowSlab->i_CellAdv();
      pRowSlab->vAdvanceCursor();
      
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
        default: ayDataByte += nDataSize(eMetaCode); break;
      }
      
      continue;
    }
    
  L_MetaCodeSwitch:
    switch (eMetaCode) {
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_NULL:
        //*aiColVal++ = C_DATUM_NULL;
        pRowSlab->vDeposit(C_DATUM_NULL);
        //iaRowSlabCells[pRowSlab->i_CellAdv()] = C_DATUM_NULL;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_1B:
      case MC_FLT_1B:
      case MC_INT_1B:
      case MC_IVL_1B:
      case MC_TME_1B:
        //*aiColVal++ = *(u1*)(ayDataByte);
        pRowSlab->vDeposit((i8)(*(u1*)(ayDataByte)));
        //iaRowSlabCells[pRowSlab->i_CellAdv()] = (i8)(*(u1*)(ayDataByte));
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
        //iaRowSlabCells[pRowSlab->i_CellAdv()] = (i8)(*(u2*)(ayDataByte));
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
        //iaRowSlabCells[pRowSlab->i_CellAdv()] = wWord.m_iI8;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_4B:
      case MC_FLT_4B:
      case MC_INT_4B:
      case MC_IVL_4B:
      case MC_TME_4B:
        //*aiColVal++ = *(u4*)(ayDataByte);
        pRowSlab->vDeposit((i8)(*(u4*)(ayDataByte)));
        //iaRowSlabCells[pRowSlab->i_CellAdv()] = (i8)(*(u4*)(ayDataByte));
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
        //iaRowSlabCells[pRowSlab->i_CellAdv()] = wWord.m_iI8;
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
        //iaRowSlabCells[pRowSlab->i_CellAdv()] = wWord.m_iI8;
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
        //iaRowSlabCells[pRowSlab->i_CellAdv()] = wWord.m_iI8;
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
        //iaRowSlabCells[pRowSlab->i_CellAdv()] = (i8)(*(u8*)(ayDataByte));
        ayDataByte += C_8B;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_M_ONE:
      case MC_INT_M_ONE:
        //*aiColVal++ = -1;
        pRowSlab->vDeposit((i8)-1);
        //iaRowSlabCells[pRowSlab->i_CellAdv()] = (i8)-1;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_ZERO:
      case MC_INT_ZERO:
        //*aiColVal++ =  0;
        pRowSlab->vDeposit((i8)0);
        //iaRowSlabCells[pRowSlab->i_CellAdv()] = 0;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_ONE:
      case MC_INT_ONE:
        //*aiColVal++ =  1;
        pRowSlab->vDeposit((i8)1);
        //iaRowSlabCells[pRowSlab->i_CellAdv()] = 1;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_TWO:
      case MC_INT_TWO:
        //*aiColVal++ =  2;
        pRowSlab->vDeposit((i8)2);
        //iaRowSlabCells[pRowSlab->i_CellAdv()] = 2;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_THREE:
      case MC_INT_THREE:
        //*aiColVal++ =  3;
        pRowSlab->vDeposit((i8)3);
        //iaRowSlabCells[pRowSlab->i_CellAdv()] = 3;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_FOUR:
      case MC_INT_FOUR:
        //*aiColVal++ =  4;
        pRowSlab->vDeposit((i8)4);
        //iaRowSlabCells[pRowSlab->i_CellAdv()] = 4;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_FIVE:
      case MC_INT_FIVE:
        //*aiColVal++ =  5;
        pRowSlab->vDeposit((i8)5);
        //iaRowSlabCells[pRowSlab->i_CellAdv()] = 5;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_SIX:
      case MC_INT_SIX:
        //*aiColVal++ =  6;
        pRowSlab->vDeposit((i8)6);
        //iaRowSlabCells[pRowSlab->i_CellAdv()] = 6;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_SEVEN:
      case MC_INT_SEVEN:
        //*aiColVal++ =  7;
        pRowSlab->vDeposit((i8)7);
        //iaRowSlabCells[pRowSlab->i_CellAdv()] = 7;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_EIGHT:
      case MC_INT_EIGHT:
        //*aiColVal++ =  8;
        pRowSlab->vDeposit((i8)8);
        //iaRowSlabCells[pRowSlab->i_CellAdv()] = 8;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_NINE:
      case MC_INT_NINE:
        //*aiColVal++ =  9;
        pRowSlab->vDeposit((i8)9);
        //iaRowSlabCells[pRowSlab->i_CellAdv()] = 9;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_TEN:
      case MC_INT_TEN:
        //*aiColVal++ = 10;
        pRowSlab->vDeposit((i8)10);
        //iaRowSlabCells[pRowSlab->i_CellAdv()] = 10;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BOL_FALSE:
        //*aiColVal++ =  0;
        pRowSlab->vDeposit((i8)0);
        //iaRowSlabCells[pRowSlab->i_CellAdv()] = 0;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BOL_TRUE:
        //*aiColVal++ =  1;
        pRowSlab->vDeposit((i8)1);
        //iaRowSlabCells[pRowSlab->i_CellAdv()] = 1;
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_DTE_E0_2B:
        //*aiColVal++ = *(u2*)ayDataByte;
        //ayDataByte += C_2B;
        //vDteFromEdt((aiColVal-1),C_ERA_0);
        //pRowSlab->vDeposit((i8)(*(u2*)(ayDataByte)));
        //pRowSlab->vDepositCursor(&aCell);
        //vDteFromEdt((i8*)aCell,C_ERA_0);
        
        //        iaRowSlabCells[pRowSlab->i_CellAdv()] = (i8)(*(u2*)(ayDataByte));
        //        ayDataByte += C_2B;
        //        vDteFromEdt((i8*)&iaRowSlabCells[pRowSlab->i_CellCrs()],C_ERA_0);
        
        pRowSlab->vDeposit(iDteFromEdt((i8)(*(u2*)(ayDataByte)),C_ERA_0));
        ayDataByte += C_2B;
        
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_DTE_E1_2B:
        //*aiColVal++ = *(u2*)ayDataByte;
        //ayDataByte += C_2B;
        //vDteFromEdt((aiColVal-1),C_ERA_1);
        //pRowSlab->vDeposit((i8)(*(u2*)(ayDataByte)));
        //pRowSlab->vDepositCursor(&aCell);
        //vDteFromEdt((i8*)aCell,C_ERA_1);
        
        //        iaRowSlabCells[pRowSlab->i_CellAdv()] = (i8)(*(u2*)(ayDataByte));
        //        ayDataByte += C_2B;
        //        vDteFromEdt((i8*)&iaRowSlabCells[pRowSlab->i_CellCrs()],C_ERA_1);
        
        pRowSlab->vDeposit(iDteFromEdt((i8)(*(u2*)(ayDataByte)),C_ERA_1));
        ayDataByte += C_2B;
        
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_DTE_E2_2B:
        //*aiColVal++ = *(u2*)ayDataByte;
        //ayDataByte += C_2B;
        //vDteFromEdt((aiColVal-1),C_ERA_2);
        //pRowSlab->vDeposit((i8)(*(u2*)(ayDataByte)));
        //pRowSlab->vDepositCursor(&aCell);
        //vDteFromEdt((i8*)aCell,C_ERA_2);
        
        //        iaRowSlabCells[pRowSlab->i_CellAdv()] = (i8)(*(u2*)(ayDataByte));
        //        ayDataByte += C_2B;
        //        vDteFromEdt((i8*)&iaRowSlabCells[pRowSlab->i_CellCrs()],C_ERA_2);
        
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
        
        //        azStz = (char*)pRowSlab->m_xWsp.ayLatterHead();
        //        pRowSlab->i_CellAdv();
        //        i_Ofs = - (nDifference(azStz, & iaRowSlabCells[pRowSlab->i_CellCrs()]));
        //        iaRowSlabCells[pRowSlab->i_CellCrs()] = i_Ofs;
        //        nLength = *(u1*)ayDataByte++;
        //        *azStz = '\0';
        //        pRowSlab->m_xWsp.vTakeHead(nLength);
        
        break;
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_STR_FIT_1L:
        //        nLength = *(u1*)ayDataByte++;
        //        MemLib::vCopy((byte*)acString,
        //                      (byte*)ayDataByte,
        //                      nLength,
        //                      CN_ROWCOMPACTOR);
        //        *(char**)(aiColVal++) = acString;
        //        ayDataByte += nLength;
        //        acString   += nLength;
        //        *acString++ = '\0';
        
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
        
        //        azStz = (char*)pRowSlab->m_xWsp.ayLatterHead();
        //        pRowSlab->i_CellAdv();
        //        i_Ofs = - (nDifference(azStz, & iaRowSlabCells[pRowSlab->i_CellCrs()]));
        //        iaRowSlabCells[pRowSlab->i_CellCrs()] = i_Ofs;
        //        nLength = *(u1*)ayDataByte++;
        //
        //        MemLib::vCopy((byte*)azStz,
        //         (byte*)ayDataByte,
        //         nLength,
        //         CN_ROWCOMPACTOR);
        //         ayDataByte += nLength;
        //
        //        azStz    += nLength;
        //        *azStz = '\0';
        //        pRowSlab->m_xWsp.vTakeHead(nLength);
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
        
        //        azStz = (char*)pRowSlab->m_xWsp.ayLatterHead();
        //        pRowSlab->i_CellAdv();
        //        i_Ofs = - (nDifference(azStz, & iaRowSlabCells[pRowSlab->i_CellCrs()]));
        //        iaRowSlabCells[pRowSlab->i_CellCrs()] = i_Ofs;
        //        wWord.m_iI8 = 0;
        //        wWord.m_yaByte[0] = *ayDataByte++;
        //        wWord.m_yaByte[1] = *ayDataByte++;
        //        nLength = wWord.m_iI8;
        //
        //        MemLib::vCopy((byte*)azStz,
        //                      (byte*)ayDataByte,
        //                      nLength,
        //                      CN_ROWCOMPACTOR);
        //        ayDataByte += nLength;
        //        azStz    += nLength;
        //        *azStz = '\0';
        //        pRowSlab->m_xWsp.vTakeHead(nLength);
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
        
        //        wWord.m_iI8 = 0;
        //        wWord.m_yaByte[0] = *ayDataByte++;
        //        wWord.m_yaByte[1] = *ayDataByte++;
        //        i_Tail_rel = wWord.m_iI8;
        //        aTail = aHeadLine->aGetTailRel(i_Tail_rel);
        //        nLength = aTail->w8_1B(+ C_U_1B);
        //
        //        azStz = (char*)pRowSlab->m_xWsp.ayLatterHead();
        //        pRowSlab->i_CellAdv();
        //        i_Ofs = - (nDifference(azStz, & iaRowSlabCells[pRowSlab->i_CellCrs()]));
        //        iaRowSlabCells[pRowSlab->i_CellCrs()] = i_Ofs;
        //
        //        MemLib::vCopy((byte*)azStz,
        //                      (byte*)aTail+C_U_1B+C_L_1B,
        //                      nLength,
        //                      CN_ROWCOMPACTOR);
        //        azStz    += nLength;
        //        *azStz = '\0';
        //        pRowSlab->m_xWsp.vTakeHead(nLength);
        
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
    
  }
}
/*----------------------------------------------------------------------------*/
/* inflate headline into row                                                  */
/*----------------------------------------------------------------------------*/
void RowInflator::vInflateRow(RowSingle* pRow, HeadLine* aHeadLine)
{
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
void RowInflator::vInflateRow(RowSlab* pRowSlab, HeadLine* aHeadLine)
{
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
