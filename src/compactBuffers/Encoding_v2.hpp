/*============================================================================*/
/* Encoding_v2.hpp -- Row Encoding_v2 (intermediate representation of compact row)  */
/*============================================================================*/
#pragma once

namespace CompactBuffers
{
  // how a column is to be compacted
  struct _ColEnc {
    
    Column* m_pCol;    // column
    
    u1   m_uMetaCode;  // meta code v2 describing the column value
    Dtm  m_wDataCode;  // column value data bytes
    i8   m_nDataSize;  // complete number of data code bytes, incl <L> etc
    i8   m_i_Offset;   // first byte offset in head+over where the data code is
    i8   m_i_Till;     // last byte offset in head+over where the data code is
    bool m_bSplit;     // overflow occurs at this point
    
    i8   m_i_Tail;     // absolute index of tail
    
    TailLine *m_aTailLine;  // the tail line
  };

  class Encoding_v2 : public Object,
                      private RowCoder
  {
  public:
    StreamDef* m_pStreamDef;
  
    i8  m_nColumns;
    i8  m_nHeadSize;
    i8  m_nMetaSize;
    i8  m_i_BaseOffsetData;
  
    i8  m_nBytesNeeded;
    i8  m_nShiftOver;  // by how much a shift occurs
  
    i8  m_i_Offset_next;

    _ColEnc xaColEnc[C_MAX_COLUMNS];

    SingleWord m__PadToCacheLine[1]; // padding to cache line

    Wsp_t<C_MAX_RECORD> m_xWsp;  // sz workspace (multiple vch)
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Encoding_v2(StreamDef* pStreamDef);
   ~Encoding_v2(void) noexcept(false);
  
    i8   nBytesNeeded(void);
  
    void vEncodeRow(Row* pRow);
    void vEncodeRsb(Rsb* aRsb);
    void vEncodeSrw(RowSingle* pSrw);
  
    void vMorphEncoding(_ColEnc* aColEnc);
    bool bOverFlowed(void);
    i8   nOverSize(void);
    void vSplitOver(void);

  private:
    // encoding (structural order)
    void vInitialEncodingIrw(Row* pRow);
    void vInitialEncodingRsb(Rsb* aRsb);
    void vInitialEncodingSrw(RowSingle* pSrw);
 
    // support functions
    Column* pNonFitting(void);
    void vReset(void);
    void vSplitAt(i8 i_Col);
    bool bWithinHead(i8 i_Col);
  
  private:
    Footer* aFooter(void);
  
  /*--------------------------------------------------------------------------*/
  /* possible null encoding                                                   */
  /*--------------------------------------------------------------------------*/
  inline bool bEncodeNul(_ColEnc& xColEnc, Dtm wDtm)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
    
    if (! wDtm.bIsNull())
      return false;
    
    // null case : encode as: MC_NULL + {}
    xColEnc.m_uMetaCode = METACODE_FORM(MC_NULL,0);

    xColEnc.m_wDataCode.vSetNull();
    xColEnc.m_nDataSize = C_0B;
    
    return true;
  }

  /*----------------------------------------------------------------------------*/
  /* encode a bcd column value                                                  */
  /*----------------------------------------------------------------------------*/
  inline void vEncodeBcd(_ColEnc& xColEnc, Dtm wColVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
    
    // null case
    if (bEncodeNul(xColEnc,wColVal))
      return;

    xColEnc.m_wDataCode.m_wBcd.m_iAsI8 = wColVal.m_wBcd.m_iAsI8;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // -1 case -- encode as: MC_BCD_M_ONE + {}
    if (-1 == wColVal.m_wBcd.m_iAsI8) {
      xColEnc.m_nDataSize = C_0B;
      
      xColEnc.m_uMetaCode = METACODE_FORM(MC_NEGATIVE,1);
      return;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // 0..10 case -- encode as MC_BCD_ONE..TEN + {}
    if (wColVal.m_wBcd.m_iAsI8 >= 0 && wColVal.m_wBcd.m_iAsI8 <= 15) {
      xColEnc.m_nDataSize = C_0B;
      
      xColEnc.m_uMetaCode = METACODE_FORM(MC_POSITIVE,wColVal.m_iWrd);

      return;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // general case -- encode as: MC_BCD_1..8B + {D}
    i8 nD = wColVal.iByteMagn();
    xColEnc.m_nDataSize = nD;
    
    xColEnc.m_uMetaCode = METACODE_FORM(MC_NUMERIC,nD);
  }

  /*----------------------------------------------------------------------------*/
  /* encode a bol column value                                                  */
  /*----------------------------------------------------------------------------*/
  inline void vEncodeBol(_ColEnc& xColEnc, Dtm wColVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
    
    // null case
    if (bEncodeNul(xColEnc,wColVal))
      return;
    
    xColEnc.m_wDataCode.m_wBol.m_iAsI8 = wColVal.m_wBol.m_iAsI8;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // encode as: MC_BOL_TRUE or MC_BOL_FALSE
    if (wColVal.m_wBol.m_iAsI8) {
      xColEnc.m_nDataSize = C_0B;
      
      xColEnc.m_uMetaCode = METACODE_FORM(MC_POSITIVE,1);
    }
    else {
      xColEnc.m_nDataSize = C_0B;
      xColEnc.m_uMetaCode = METACODE_FORM(MC_POSITIVE,0);
    }
  }

  /*----------------------------------------------------------------------------*/
  /* encode a dte column value                                                  */
  /*----------------------------------------------------------------------------*/
  inline void vEncodeDte(_ColEnc& xColEnc, Dtm wColVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
    
    // null case
    if (bEncodeNul(xColEnc,wColVal))
      return;
    
    xColEnc.m_wDataCode.m_wDte.m_iAsI8 = wColVal.m_wDte.m_iAsI8;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // encode as: MC_DTE_En_2B + {D}
    
    i8 iEra = 0;
    vDteIntoEdt(wColVal.m_wDte.m_iAsI8,&xColEnc.m_wDataCode.m_wEdt.m_iAsI8,iEra);
    
    if (C_ERA_0 == iEra) {
      xColEnc.m_uMetaCode = METACODE_FORM(MC_DATE_E0,2);
    }
    else
    if (C_ERA_1 == iEra) {
      xColEnc.m_uMetaCode = METACODE_FORM(MC_DATE_E1,2);
    }
    else {
      xColEnc.m_uMetaCode = METACODE_FORM(MC_DATE_E2,2);
    }
    
    xColEnc.m_nDataSize = C_2B;
    
  }

  /*----------------------------------------------------------------------------*/
  /* encode a flt column value                                                  */
  /*----------------------------------------------------------------------------*/
  inline void vEncodeFlt(_ColEnc& xColEnc, Dtm wColVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
    
    // null case
    if (bEncodeNul(xColEnc,wColVal))
      return;
    
    xColEnc.m_wDataCode.m_wFlt.m_dFlt = wColVal.m_wFlt.m_dFlt;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // general case -- encode as: MC_FLT_1..8B + {D}
    i8 nD = wColVal.iByteMagn();
    xColEnc.m_nDataSize = nD;
    
    xColEnc.m_uMetaCode = METACODE_FORM(MC_NUMERIC,nD);
  }

  /*----------------------------------------------------------------------------*/
  /* encode an int column value                                                 */
  /*----------------------------------------------------------------------------*/
  inline void vEncodeInt(_ColEnc& xColEnc, Dtm wColVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   
    // general case -- encode as: MC_INT_1..8B + {D}
    xColEnc.m_wDataCode.m_iWrd = wColVal.m_iWrd;
  
    if (wColVal.m_iWrd > 10) {
      xColEnc.m_nDataSize = (u8)IntLib::iByteMagnitude(wColVal.m_iWrd);
   
      xColEnc.m_uMetaCode = METACODE_FORM(MC_NUMERIC,xColEnc.m_nDataSize);
      return;
    }
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // 0..10 case -- encode as MC_INT_ONE..TEN + {}
    if (wColVal.m_iWrd >= 0 && wColVal.m_iWrd <= 10) {
      xColEnc.m_nDataSize = C_0B;
      
      u1 uHigh = MC_POSITIVE;
      uHigh <<= 4;
      u1 uLow = (u1)wColVal.m_iWrd;
      u1 eTest = (uHigh | uLow);
      
      xColEnc.m_uMetaCode = METACODE_FORM(MC_POSITIVE,wColVal.m_iWrd);
      return;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // null case
    if (bEncodeNul(xColEnc,wColVal))
      return;
        
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // -1 case -- encode as: MC_INT_M_ONE + {}
    if (-1 == wColVal.m_iWrd) {
      xColEnc.m_nDataSize = C_0B;
      
      xColEnc.m_uMetaCode = METACODE_FORM(MC_NEGATIVE,1);
      return;
    }
  
    xColEnc.m_nDataSize = IntLib::iByteMagnitude(wColVal.m_iWrd);

    xColEnc.m_uMetaCode = METACODE_FORM(MC_NUMERIC,xColEnc.m_nDataSize);

  }

  /*----------------------------------------------------------------------------*/
  /* encode an ivl column value                                                 */
  /*----------------------------------------------------------------------------*/
  inline void vEncodeIvl(_ColEnc& xColEnc, Dtm wColVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
    
    // null case
    if (bEncodeNul(xColEnc,wColVal))
      return;
    
    xColEnc.m_wDataCode.m_wIvl.m_iAsI8 = wColVal.m_wIvl.m_iAsI8;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // general case -- encode as: MC_IVL_1..8B : {D}
    i8 nD = wColVal.iByteMagn();
    xColEnc.m_nDataSize = nD;
    
    xColEnc.m_uMetaCode = METACODE_FORM(MC_NUMERIC,nD);
  }

  /*----------------------------------------------------------------------------*/
  /* encode a tme column value                                                  */
  /*----------------------------------------------------------------------------*/
  inline void vEncodeTme(_ColEnc& xColEnc, Dtm wColVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
    
    // null case
    if (bEncodeNul(xColEnc,wColVal))
      return;
    
    xColEnc.m_wDataCode.m_wTme.m_iAsI8 = wColVal.m_wTme.m_iAsI8;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // general case -- encode as: MC_TME_1..8B : {D}
    i8 nD = wColVal.iByteMagn();
    xColEnc.m_nDataSize = nD;
    
    xColEnc.m_uMetaCode = METACODE_FORM(MC_NUMERIC,nD);
  }

  /*----------------------------------------------------------------------------*/
  /* encode a tsp column value                                                  */
  /*----------------------------------------------------------------------------*/
  inline void vEncodeTsp(_ColEnc& xColEnc, Dtm wColVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
    
    // null case
    if (bEncodeNul(xColEnc,wColVal))
      return;
    
    xColEnc.m_wDataCode.m_wTsp.m_iAsI8 = wColVal.m_wTsp.m_iAsI8;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // encode as: MC_1..8B : {D}
    i8 nD = 8;
    xColEnc.m_nDataSize = nD;
    
    xColEnc.m_uMetaCode = METACODE_FORM(MC_NUMERIC,nD);
  }

  /*----------------------------------------------------------------------------*/
  /* encode a vch column value                                                  */
  /*----------------------------------------------------------------------------*/
  inline void vEncodeVch(_ColEnc& xColEnc, Dtm wColVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
    
    u1 nQuanta;
    
    // null case
    if (bEncodeNul(xColEnc,wColVal))
      return;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // copy chars + null into wsp, nS will be size  (i.e. noOf(chars)+1 )
    char* azDst = (char*)m_xWsp.ayLatter();
    //i8 nS = StrLib::nCopy(azDst,(char*)wColVal.m_szVch) + 1;
    i8 nS = StrLib::nLength((char*)wColVal.m_szVch) + 1;

    // have copied nS bytes into wsp
    //m_xWsp.vTake(nS);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      
    // 1) MC_STRING_x1
    if (nS <= 15) {
      nQuanta = nS; //MemLib::nQuantaRequired(nS,1);
      
      xColEnc.m_wDataCode.m_azStr = (char*)wColVal.m_szVch;
      xColEnc.m_nDataSize = STRx1(nQuanta);
      
      xColEnc.m_uMetaCode = METACODE_FORM(MC_STRING_x1,nQuanta);
      return;
    }
    
    // 2) MC_STRINGx2
    if (nS <= 47) {
      nQuanta = MemLib::nQuantaRequired(nS-16,2);
      
      xColEnc.m_wDataCode.m_azStr = (char*)wColVal.m_szVch;
      xColEnc.m_nDataSize = STRx2(nQuanta);
      
      xColEnc.m_uMetaCode = METACODE_FORM(MC_STRING_x2,nQuanta);

      return;
    }
    
    // 3) MC_STRINGx4
    if (nS <= 111) {
      nQuanta = MemLib::nQuantaRequired(nS-48,4);
      

      xColEnc.m_wDataCode.m_azStr = (char*)wColVal.m_szVch;
      xColEnc.m_nDataSize = STRx4(nQuanta);
      
      xColEnc.m_uMetaCode = METACODE_FORM(MC_STRING_x4,nQuanta);

      return;
    }

    // 4) MC_STRINGx8
    if (nS <= 239) {
      nQuanta = MemLib::nQuantaRequired(nS-112,8);
      
      xColEnc.m_wDataCode.m_azStr = (char*)wColVal.m_szVch;
      xColEnc.m_nDataSize = STRx8(nQuanta);
      
      xColEnc.m_uMetaCode = METACODE_FORM(MC_STRING_x8,nQuanta);

      return;
    }

    // 5) MC_STRING_TAIL
    xColEnc.m_wDataCode.m_azStr = (char*)wColVal.m_szVch;
    xColEnc.m_nDataSize = nS;
      
    xColEnc.m_uMetaCode = METACODE_FORM(MC_STRING_TAIL,2);
  }

  /*--------------------------------------------------------------------------*/
  /* compute initial offset and till for column encoding                      */
  /*--------------------------------------------------------------------------*/
  inline void vOffsetTill(_ColEnc& xrColEnc)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING_V2,_FOOTER_);
    
    // set the offset of this column
    xrColEnc.m_i_Offset = m_i_Offset_next;
    
    // get the base size of the encoding
    i8 iHigh = METACODE_HIGH(xrColEnc.m_uMetaCode);
    i8 iLow  = METACODE_LOW(xrColEnc.m_uMetaCode);
    //i8 nDataBytes = nDataSizeOfCode(iHigh,iLow);
    
    // if 0B case then till is same as offset
    if (! xrColEnc.m_nDataSize) {
      xrColEnc.m_i_Till = m_i_Offset_next;
      return;
    }
      
//    if (MC_STRING_TAIL == (eMC)iHigh) {
//      i8 nStrLen = StrLib::nLength(xrColEnc.m_wDataCode.m_szVch->azChar());
//      nDataBytes += nStrLen + 1;
//    }
    
    // set where the next offset will be
    m_i_Offset_next += xrColEnc.m_nDataSize;
    
    // set the till byte of the column encoding
    xrColEnc.m_i_Till = m_i_Offset_next; // - 1;
  }


  };
  
  Encoding_v2* _pEncoding_v2(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
