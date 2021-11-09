/*============================================================================*/
/* vFLAT_InflateColumn.h -- Column Inflator                                   */
/*============================================================================*/
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) inflate a single column
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

L_InflateAColumn:

L_HighSwitch:
  
  aiCellValue = (i8*)ayDstByte;

  switch (uHigh) {
    case MC_NUMERIC:     goto L_MC_NUMERIC;
    case MC_DATE_E0:     goto L_MC_DATE_E0;
    case MC_DATE_E1:     goto L_MC_DATE_E1;
    case MC_DATE_E2:     goto L_MC_DATE_E2;
    case MC_STRING_x1:   goto L_MC_STRING_x1;
    case MC_NULL:        goto L_MC_NULL;
    case MC_SPARE0:      goto L_MC_SPARE0;
    case MC_OVERFLOW:    goto L_MC_OVERFLOW;
    case MC_STRING_x2:   goto L_MC_STRING_x2;
    case MC_STRING_x4:   goto L_MC_STRING_x4;
    case MC_STRING_x8:   goto L_MC_STRING_x8;
    case MC_STRING_TAIL: goto L_MC_STRING_TAIL;
    case MC_BLOB_0:      goto L_MC_BLOB_0;
    case MC_BLOB_1:      goto L_MC_BLOB_1;
    case MC_NEGATIVE:    goto L_MC_NEGATIVE;
    case MC_POSITIVE:    goto L_MC_POSITIVE;
    default:
      NEVER_REACH;
  }

  NEVER_REACH;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// MC_NUMERIC: numeric values 0..8B
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_MC_NUMERIC:
      
  switch (uLow) {
      
    case 0x0: // MC_NUMERIC 0B
        *aiCellValue = 0;
        ayDstByte += C_8B;
      break;
      
    case 0x1: // MC_NUMERIC 1B
      *aiCellValue = 0;
      *ayDstByte++ = *ayDataByte++; // 0
      ayDstByte += C_7B;
      break;
      
    case 0x2: // MC_NUMERIC 2B
      *aiCellValue = *(u2*)(ayDataByte);
      ayDataByte += C_2B;
      ayDstByte += C_8B;
      break;
      
    case 0x3: // MC_NUMERIC 3B
      *aiCellValue = 0;
      *ayDstByte++ = *ayDataByte++; // 0
      *ayDstByte++ = *ayDataByte++; // 1
      *ayDstByte++ = *ayDataByte++; // 2
      ayDstByte += C_5B;
      break;
      
    case 0x4: // MC_NUMERIC 4B
      *aiCellValue = *(u4*)(ayDataByte);
      ayDataByte += C_4B;
      ayDstByte += C_8B;
      break;
      
    case 0x5: // MC_NUMERIC 5B
      *aiCellValue = 0;
      *ayDstByte++ = *ayDataByte++; // 0
      *ayDstByte++ = *ayDataByte++; // 1
      *ayDstByte++ = *ayDataByte++; // 2
      *ayDstByte++ = *ayDataByte++; // 3
      *ayDstByte++ = *ayDataByte++; // 4
      ayDstByte += C_3B;
      break;
      
    case 0x6: // MC_NUMERIC 6B
      *aiCellValue = 0;
      *ayDstByte++ = *ayDataByte++; // 0
      *ayDstByte++ = *ayDataByte++; // 1
      *ayDstByte++ = *ayDataByte++; // 2
      *ayDstByte++ = *ayDataByte++; // 3
      *ayDstByte++ = *ayDataByte++; // 4
      *ayDstByte++ = *ayDataByte++; // 5
      ayDstByte += C_2B;
      break;
      
    case 0x7: // MC_NUMERIC 7B
      //*aiCellValue = 0;
      *ayDstByte++ = *ayDataByte++; // 0
      *ayDstByte++ = *ayDataByte++; // 1
      *ayDstByte++ = *ayDataByte++; // 2
      *ayDstByte++ = *ayDataByte++; // 3
      *ayDstByte++ = *ayDataByte++; // 4
      *ayDstByte++ = *ayDataByte++; // 5
      *ayDstByte++ = *ayDataByte++; // 6
      *ayDstByte++ = 0; // 7
      break;
      
    case 0x8: // MC_NUMERIC 8B
      *aiCellValue = *(u8*)(ayDataByte);
      ayDstByte += C_8B;
      ayDataByte += C_8B;
      break;
      
    case 0x9: // MC_NUMERIC 9B
      NEVER_REACH;
      break;
      
    case 0xA: // MC_NUMERIC 10B
      NEVER_REACH;
      break;
      
    case 0xB: // MC_NUMERIC 11B
      NEVER_REACH;
      break;
      
    case 0xC: // MC_NUMERIC 12B
      NEVER_REACH;
      break;
      
    case 0xD: // MC_NUMERIC 13B
      NEVER_REACH;
      break;
      
    case 0xE: // MC_NUMERIC 14B
      NEVER_REACH;
      break;
      
    case 0xF: // MC_NUMERIC 15B
      NEVER_REACH;
      break;
      
  }
  goto L_ColumnInflated;
      
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // MC_DATE_E0: date era-0
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_MC_DATE_E0:

  *aiCellValue = 0;
  *ayDstByte++ = *ayDataByte++; // 0
  *ayDstByte++ = *ayDataByte++; // 1
  ayDstByte += 6;

  //vDteFromEdt(aiCellValue,C_ERA_0);
  aEdt = (union Edt_T*)aiCellValue;
  aDte = (union Dte_T*)aiCellValue;

  aDte->m_uYearDelta = aEdt->m_uYearInEra + C_ERA_0;
  aDte->m_uMonth     = aEdt->m_uMonth;
  aDte->m_uDay       = aEdt->m_uDay;

  goto L_ColumnInflated;

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // MC_DATE_E1: date era-1
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_MC_DATE_E1:

  *aiCellValue = 0;
  *ayDstByte++ = *ayDataByte++; // 0
  *ayDstByte++ = *ayDataByte++; // 1
  ayDstByte += 6;

  //vDteFromEdt(aiCellValue,C_ERA_1);
  aEdt = (union Edt_T*)aiCellValue;
  aDte = (union Dte_T*)aiCellValue;

  aDte->m_uYearDelta = aEdt->m_uYearInEra + C_ERA_1;
  aDte->m_uMonth     = aEdt->m_uMonth;
  aDte->m_uDay       = aEdt->m_uDay;

  goto L_ColumnInflated;

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // MC_DATE_E2: date era-2
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_MC_DATE_E2:

  *aiCellValue = 0;
  *ayDstByte++ = *ayDataByte++; // 0
  *ayDstByte++ = *ayDataByte++; // 1
  ayDstByte += 6;

  //vDteFromEdt(aiCellValue,C_ERA_2);
  aEdt = (union Edt_T*)aiCellValue;
  aDte = (union Dte_T*)aiCellValue;

  aDte->m_uYearDelta = aEdt->m_uYearInEra + C_ERA_2;
  aDte->m_uMonth     = aEdt->m_uMonth;
  aDte->m_uDay       = aEdt->m_uDay;

  goto L_ColumnInflated;

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // MC_STRING_x1: short string 0..15B (no null)
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_MC_STRING_x1:

  nStrSize = uLow;

  if (SI_CELLARRAY_ADR == eStrInflateRule) {
    aacCellValue = (char**)ayDstByte;
    *aacCellValue = (char*)ayDataByte;
    ayDstByte += 8;
    
    //vTrace("MC_STRING_x1: [%lld] %s\n"_Stz,nStrSize,ayDataByte);
    ayDataByte += nStrSize;
  }
  else
  if (SI_STRAREA_OFS == eStrInflateRule) {
    // copy into str area, advance data point ptr
    memcpy(racStrArea,ayDataByte,(size_t)nStrSize);
    ayDataByte += nStrSize;

    i8 i_Ofs = - nDifference(racStrArea,ayDstByte);
    aiCellValue = (i8*)ayDstByte;
    *aiCellValue = i_Ofs;
    ayDstByte += 8;

    racStrArea += nStrSize;
  }
  else
  if (SI_STRAREA_ADR == eStrInflateRule) {
    memcpy(acStrArea,ayDataByte,(size_t)nStrSize);
    ayDataByte += nStrSize;
    
    aacCellValue = (char**)ayDstByte;
    *aacCellValue = acStrArea;
    ayDstByte += 8;
    acStrArea += nStrSize;
  }
  else
  if (SI_CELLARRAY_PST == eStrInflateRule) {
    memcpy(ayDstByte,ayDataByte,(size_t)nStrSize);
    ayDataByte += nStrSize;
    
    //vTrace("SI_CELLARRAY_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
    
    nRoundedStrBytes = nWordRoundedStrBytes(nStrSize);
    ayDstByte += nRoundedStrBytes;
    aiCellValue = (i8*)ayDstByte;
    *aiCellValue = nStrSize;
    ayDstByte += 8;
  }
  else {
    NEVER_REACH;
  }

  goto L_ColumnInflated;

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // MC_NULL: null
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_MC_NULL:

  *(i8*)ayDstByte = C_DATUM_NULL;
  ayDstByte += C_BYTES_PER_WORD;

  goto L_ColumnInflated;

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // MC_SPARE0: spare
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_MC_SPARE0:

  NEVER_REACH;

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // MC_OVERFLOW: overflow 2B
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_MC_OVERFLOW:

      
  // calculation to get overflow
  nSizeUnits = GET_u1B(aHeadLine,C_HEADLINE_OFS_WORDS);
  if (nSizeUnits >= 128)
    nSizeUnits -= 128;
  nHeadBytes = nSizeUnits * C_BYTES_PER_WORD;
  i_Over_rel = GET_u2B(aHeadLine,(nHeadBytes - C_2B));
  //aOverLine = RowNub::aGetOver_rel(aHeadLine,i_Over_rel);
  aOverLine = (struct OverLine_T*)UNIT_GET_REL(aHeadLine,i_Over_rel);


  METACODE_SPLIT( (* ADDR_ADD(u1*,aOverLine,C_U_1B) ), uHigh, uLow);
  ayDataByte = ADDR_ADD(byte*,aOverLine,C_U_1B + C_M_1B);

  goto L_HighSwitch;
      
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // MC_STRING_x2: string x 2
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_MC_STRING_x2:

  nStrSize = STRx2(uLow);

  if (SI_CELLARRAY_ADR == eStrInflateRule) {
    aacCellValue = (char**)ayDstByte;
    *aacCellValue = (char*)ayDataByte;
    ayDstByte += 8;
    
    ayDataByte += nStrSize;
  }
  else
  if (SI_STRAREA_OFS == eStrInflateRule) {
    // copy into str area, advance data point ptr
    memcpy(racStrArea,ayDataByte,(size_t)nStrSize);
    ayDataByte += nStrSize;

    i8 i_Ofs = - nDifference(racStrArea,ayDstByte);
    aiCellValue = (i8*)ayDstByte;
    *aiCellValue = i_Ofs;
    ayDstByte += 8;

    racStrArea += nStrSize;
  }
  else
  if (SI_STRAREA_ADR == eStrInflateRule) {
    
    memcpy(acStrArea,ayDataByte,(size_t)nStrSize);
    ayDataByte += nStrSize;
    
    aacCellValue = (char**)ayDstByte;
    *aacCellValue = acStrArea;
    ayDstByte += 8;
    acStrArea += nStrSize;
  }
  else
  if (SI_CELLARRAY_PST == eStrInflateRule) {
    azDst = (char*)ayDstByte;
    memcpy(azDst,ayDataByte,(size_t)nStrSize);
    ayDataByte += nStrSize;
    
    //vTrace("SI_CELLARRAY_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
    
    nRoundedStrBytes = nWordRoundedStrBytes(nStrSize);
    ayDstByte += nRoundedStrBytes;
    aiCellValue = (i8*)ayDstByte;
    *aiCellValue = nStrSize;
    ayDstByte += 8;
  }
  else {
    NEVER_REACH;
  }

  goto L_ColumnInflated;

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // MC_STRING_x4: string x 4
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_MC_STRING_x4:

  nStrSize = STRx4(uLow);

  if (SI_CELLARRAY_ADR == eStrInflateRule) {
    aacCellValue = (char**)ayDstByte;
    *aacCellValue = (char*)ayDataByte;
    ayDstByte += 8;
    
    //vTrace("MC_STRING_x4: [%lld] %s\n"_Stz,nStrSize,ayDataByte);
    ayDataByte += nStrSize;
  }
  else
  if (SI_STRAREA_OFS == eStrInflateRule) {
    // copy into str area, advance data point ptr
    memcpy(racStrArea,ayDataByte,(size_t)nStrSize);
    ayDataByte += nStrSize;

    i8 i_Ofs = - nDifference(racStrArea,ayDstByte);
    aiCellValue = (i8*)ayDstByte;
    *aiCellValue = i_Ofs;
    ayDstByte += 8;

    racStrArea += nStrSize;
  }
  else
  if (SI_STRAREA_ADR == eStrInflateRule) {
    
    memcpy(acStrArea,ayDataByte,(size_t)nStrSize);
    ayDataByte += nStrSize;
    
    aacCellValue = (char**)ayDstByte;
    *aacCellValue = acStrArea;
    ayDstByte += 8;
    acStrArea += nStrSize;
  }
  else
  if (SI_CELLARRAY_PST == eStrInflateRule) {
    memcpy(ayDstByte,ayDataByte,(size_t)nStrSize);
    ayDataByte += nStrSize;
    
    //vTrace("SI_CELLARRAY_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
    
    nRoundedStrBytes = nWordRoundedStrBytes(nStrSize);
    ayDstByte += nRoundedStrBytes;
    aiCellValue = (i8*)ayDstByte;
    *aiCellValue = nStrSize;
    ayDstByte += 8;
  }
  else {
    NEVER_REACH;
  }

  goto L_ColumnInflated;

      
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // MC_STRING_x8: string x 8
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_MC_STRING_x8:

  nStrSize = STRx8(uLow);

  if (SI_CELLARRAY_ADR == eStrInflateRule) {
    aacCellValue = (char**)ayDstByte;
    *aacCellValue = (char*)ayDataByte;
    ayDstByte += 8;
    
    ayDataByte += nStrSize;
  }
  else
  if (SI_STRAREA_OFS == eStrInflateRule) {
    // copy into str area, advance data point ptr
    memcpy(racStrArea,ayDataByte,(size_t)nStrSize);
    ayDataByte += nStrSize;

    i8 i_Ofs = - nDifference(racStrArea,ayDstByte);
    aiCellValue = (i8*)ayDstByte;
    *aiCellValue = i_Ofs;
    ayDstByte += 8;

    racStrArea += nStrSize;
  }
  else
  if (SI_STRAREA_ADR == eStrInflateRule) {
    
    memcpy(acStrArea,ayDataByte,(size_t)nStrSize);
    ayDataByte += nStrSize;
    
    aacCellValue = (char**)ayDstByte;
    *aacCellValue = acStrArea;
    ayDstByte += 8;
    acStrArea += nStrSize;
  }
  else
  if (SI_CELLARRAY_PST == eStrInflateRule) {
    memcpy(ayDstByte,ayDataByte,(size_t)nStrSize);
    ayDataByte += nStrSize;
    
    //vTrace("SI_CELLARRAY_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
    
    nRoundedStrBytes = nWordRoundedStrBytes(nStrSize);
    ayDstByte += nRoundedStrBytes;
    aiCellValue = (i8*)ayDstByte;
    *aiCellValue = nStrSize;
    ayDstByte += 8;
  }
  else {
    NEVER_REACH;
  }

  goto L_ColumnInflated;

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // MC_STRING_TAIL: string in tail
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_MC_STRING_TAIL:

  u_Tail_rel = *(u2*)ayDataByte;
  //aTailLine = RowNub::aGetTail_rel(aHeadLine,u_Tail_rel);
  aTailLine = (struct TailLine_T*)UNIT_GET_REL(aHeadLine,u_Tail_rel);

  nStrSize = GET_u1B(aTailLine,(+ C_U_1B));


  if (SI_CELLARRAY_ADR == eStrInflateRule) {

    aacCellValue = (char**)ayDstByte;
    *aacCellValue = ( (char*)aTailLine + C_U_1B + C_L_1B );
    
    ayDstByte += 8;
    ayDataByte += 2;
  }
  else
  if (SI_STRAREA_ADR == eStrInflateRule) {
    memcpy(acStrArea,( (char*)aTailLine + C_U_1B + C_L_1B ),(size_t)nStrSize);
    
    aacCellValue = (char**)ayDstByte;
    *aacCellValue = acStrArea;
    ayDstByte += 8;
    ayDataByte += 2;
    acStrArea += nStrSize;
  }
  else
  if (SI_STRAREA_OFS == eStrInflateRule) {

    // copy into str area, advance data point ptr
    memcpy(acStrArea,( (char*)aTailLine + C_U_1B + C_L_1B ),(size_t)nStrSize);

    i8 i_Ofs = - nDifference(acStrArea,ayDstByte);
    aiCellValue = (i8*)ayDstByte;
    *aiCellValue = i_Ofs;
    ayDstByte += 8;
    ayDataByte += 2;

    acStrArea += nStrSize;
  }
  else {
    NEVER_REACH;
  }

  goto L_ColumnInflated;

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // MC_BLOB_0: blob type 0
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_MC_BLOB_0:

  NEVER_REACH;

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // MC_BLOB_1: blob type 1
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_MC_BLOB_1:

  NEVER_REACH;

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // MC_NEGATIVE: -ve digit -15..-0
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_MC_NEGATIVE:

  aiCellValue = (i8*)ayDstByte;

  *ayDstByte++ = (i8)uLow; // 0
  *ayDstByte++ = 0; // 1
  *ayDstByte++ = 0; // 2
  *ayDstByte++ = 0; // 3
  *ayDstByte++ = 0; // 4
  *ayDstByte++ = 0; // 5
  *ayDstByte++ = 0; // 6
  *ayDstByte++ = 0; // 7

  *aiCellValue = - *aiCellValue;

  goto L_ColumnInflated;

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  // MC_POSITIVE: +ve digit 0..15
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
L_MC_POSITIVE:

  *ayDstByte++ = (i8)uLow; // 0
  *ayDstByte++ = 0; // 1
  *ayDstByte++ = 0; // 2
  *ayDstByte++ = 0; // 3
  *ayDstByte++ = 0; // 4
  *ayDstByte++ = 0; // 5
  *ayDstByte++ = 0; // 6
  *ayDstByte++ = 0; // 7

  goto L_ColumnInflated;

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

L_ColumnInflated:
  goto *aSubReturnLabel;
      
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
