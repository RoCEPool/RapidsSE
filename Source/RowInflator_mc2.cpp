/*============================================================================*/
/* RowInflator_mc2.cpp -- Row Inflator (for Meta Code Version 2)              */
/*============================================================================*/

#include "CompactBuffers.h"

/*----------------------------------------------------------------------------*/
/* inflate set onto Dtsk                                                      */
/*----------------------------------------------------------------------------*/
void RowInflator::vInflateSet_v2(i8** aaiDstk, HeadLine* aHeadLine,
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

  i8* aiDstk_lcl = *aaiDstk;
  
  i8 i_WhichCol = 0;
  i_ColToRetrieve = iWhichCol[i_WhichCol];
    
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
        *++aiDstk_lcl = C_DATUM_NULL;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_1B:
      case MC_FLT_1B:
      case MC_INT_1B:
      case MC_IVL_1B:
      case MC_TME_1B:
        *++aiDstk_lcl = *(u1*)(ayDataByte);
        ayDataByte += C_1B;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_2B:
      case MC_FLT_2B:
      case MC_INT_2B:
      case MC_IVL_2B:
      case MC_TME_2B:
        *++aiDstk_lcl = *(u2*)(ayDataByte);
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
        *++aiDstk_lcl = wWord.m_iI8;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_4B:
      case MC_FLT_4B:
      case MC_INT_4B:
      case MC_IVL_4B:
      case MC_TME_4B:
        *++aiDstk_lcl = *(u4*)(ayDataByte);
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
        *++aiDstk_lcl = wWord.m_iI8;
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
        *++aiDstk_lcl = wWord.m_iI8;
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
        *++aiDstk_lcl = wWord.m_iI8;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_8B:
      case MC_FLT_8B:
      case MC_INT_8B:
      case MC_IVL_8B:
      case MC_TME_8B:
      case MC_TSP_8B:
        *++aiDstk_lcl = *(u8*)(ayDataByte);
        ayDataByte += C_8B;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_M_ONE:
      case MC_INT_M_ONE:
        *++aiDstk_lcl = -1;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_ZERO:
      case MC_INT_ZERO:
        *++aiDstk_lcl =  0;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_ONE:
      case MC_INT_ONE:
        *++aiDstk_lcl =  1;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_TWO:
      case MC_INT_TWO:
        *++aiDstk_lcl =  2;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_THREE:
      case MC_INT_THREE:
        *++aiDstk_lcl =  3;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_FOUR:
      case MC_INT_FOUR:
        *++aiDstk_lcl =  4;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_FIVE:
      case MC_INT_FIVE:
        *++aiDstk_lcl =  5;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_SIX:
      case MC_INT_SIX:
        *++aiDstk_lcl =  6;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_SEVEN:
      case MC_INT_SEVEN:
        *++aiDstk_lcl =  7;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_EIGHT:
      case MC_INT_EIGHT:
        *++aiDstk_lcl =  8;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_NINE:
      case MC_INT_NINE:
        *++aiDstk_lcl =  9;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BCD_TEN:
      case MC_INT_TEN:
        *++aiDstk_lcl = 10;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BOL_FALSE:
        *++aiDstk_lcl =  0;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BOL_TRUE:
        *++aiDstk_lcl =  1;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_DTE_E0_2B:
        *++aiDstk_lcl = *(u2*)ayDataByte;
        ayDataByte += C_2B;
        vDteFromEdt(aiDstk_lcl,C_ERA_0);
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_DTE_E1_2B:
        *++aiDstk_lcl = *(u2*)ayDataByte;
        ayDataByte += C_2B;
        vDteFromEdt(aiDstk_lcl,C_ERA_1);
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_DTE_E2_2B:
        *++aiDstk_lcl = *(u2*)ayDataByte;
        ayDataByte += C_2B;
        vDteFromEdt(aiDstk_lcl,C_ERA_2);
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_STR_EMPTY: // not implemented yet
        RASSERT(0);
        //*(char**)(aiColVal++) = acString;
        //*acString++ = '\0';
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
        
        acString = (char*)++aiDstk_lcl;
        
        MemLib::vCopy((byte*)acString,
                      (byte*)ayDataByte,
                      nLength,
                      CN_ROWCOMPACTOR);
        
        acString   += nLength;
        *acString++ = '\0';
        
        aiDstk_lcl += (nLength+1) / 8 + 1;
        *++aiDstk_lcl = nLength+1;

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
        
        acString = (char*)++aiDstk_lcl;
        
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

    // move to next column to retrieve
    if (++i_WhichCol >= nWhichCols) {
      *aaiDstk = aiDstk_lcl;
      return;
    }
    
    i_ColToRetrieve = iWhichCol[i_WhichCol];
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
