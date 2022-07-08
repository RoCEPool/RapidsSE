/*============================================================================*/
/* Encoding.hpp -- Row Encoding (intermediate representation of compact row)  */
/*============================================================================*/
#pragma once

namespace STORE
{
  // how a column is to be compacted
  struct _ColEnc {
    
    Column* m_pCol;    // column
    
    union {
      u1   m_uMetaCode;  // meta code v2 describing the column value
      
      struct {
        u1  m_uLow  : 4;
        eMC m_uHigh : 4;
      };
    };
    
    Dtm  m_wDataCode;  // column value data bytes
    i8   m_jDataBytes; // complete number of data code bytes, incl <L> etc
    i8   m_o_DataFrom; // first byte offset in head+over where the data code is
    i8   m_o_DataBreak;// byte after last byte offset in head+over where the data code is
    b8   m_bSplit;     // overflow occurs at this point
    
    i8   m_i_Tail;     // absolute index of tail
    b8   m_bDuplicate; // tail is a duplicate
    i8   m_jTailBytes;  // tail size required (for non-duplicate tails)

    TailLine *m_aTailLine;  // the tail line
  };

  class Encoding : public Object,
                      private RowCoder
  {
  public:
    StreamDef* m_pStreamDef;
  
    i8  m_nColumns;
    i8  m_jHeadSize;
    i8  m_jMetaSize;
    i8  m_jOverSize;
  
    i8  m_o_DataOffsetBase;
    i8  m_o_DataOffsetNext;

    i8  m_jBytesNeeded;
    i8  m_nShiftOver;  // by how much a shift occurs
  
    i8  m_nExtraTails;
    i8  m_jExtraSize;
  
    _ColEnc xaColEnc[C_MAX_COLUMNS];

    PadWord m__PadToCacheLine[1]; // padding to cache line

    Wsp_t<C_MAX_RECORD> m_xWsp;  // sz workspace (multiple vch)
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Encoding(StreamDef* pStreamDef);
   ~Encoding(void) noexcept(false);
  
    i8   jBytesNeeded(void);
  
    void vDumpEncoding(void);
    
    // encoding (structural order)
    void vInitialEncodingRow(Row* pRow);
  
  
    void vMorphEncoding(_ColEnc& rxColEnc);
    bool bOverFlowed(void);
    i8   jOverSize(void);
    void vSplitOver(void);

  private:
 
    // support functions
    Column* pNonFitting(void);
    void vReset(void);
    bool bWithinHead(i8 i_Col);
  
  private:
    Footer* aFooter(void);
  
  /*--------------------------------------------------------------------------*/
  /* possible null encoding                                                   */
  /*--------------------------------------------------------------------------*/
  inline bool bEncodeNul(_ColEnc& xColEnc, Dtm wDtm)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
    
    if (! wDtm.bIsNull())
      return false;
    
    // null case : encode as: MC_NULL + {}
    xColEnc.m_uMetaCode = METACODE_FORM(MC_NULL,0);

    xColEnc.m_wDataCode.vSetNull();
    xColEnc.m_jDataBytes = C_0B;
    
    return true;
  }

  /*----------------------------------------------------------------------------*/
  /* encode a bcd column value                                                  */
  /*----------------------------------------------------------------------------*/
  inline void vEncodeBcd(_ColEnc& xColEnc, Dtm wColVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
    
    // null case
    if (bEncodeNul(xColEnc,wColVal))
      return;

    xColEnc.m_wDataCode.m_wBcd.T.m_iAsI8 = wColVal.m_wBcd.T.m_iAsI8;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // -1 case -- encode as: MC_BCD_M_ONE + {}
    if (-1 == wColVal.m_wBcd.T.m_iAsI8) {
      xColEnc.m_jDataBytes = C_0B;
      
      xColEnc.m_uMetaCode = METACODE_FORM(MC_NEGATIVE,1);
      return;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // 0..10 case -- encode as MC_BCD_ONE..TEN + {}
    if (wColVal.m_wBcd.T.m_iAsI8 >= 0 && wColVal.m_wBcd.T.m_iAsI8 <= 15) {
      xColEnc.m_jDataBytes = C_0B;
      
      xColEnc.m_uMetaCode = METACODE_FORM(MC_POSITIVE,wColVal.m_iWrd);

      return;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // general case -- encode as: MC_BCD_1..8B + {D}
    i8 nD = wColVal.iByteMagn();
    xColEnc.m_jDataBytes = nD;
    
    xColEnc.m_uMetaCode = METACODE_FORM(MC_NUMERIC,nD);
  }

  /*----------------------------------------------------------------------------*/
  /* encode a bol column value                                                  */
  /*----------------------------------------------------------------------------*/
  inline void vEncodeBol(_ColEnc& xColEnc, Dtm wColVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
    
    // null case
    if (bEncodeNul(xColEnc,wColVal))
      return;
    
    xColEnc.m_wDataCode.m_wBol.T.m_iAsI8 = wColVal.m_wBol.T.m_iAsI8;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // encode as: MC_BOL_TRUE or MC_BOL_FALSE
    if (wColVal.m_wBol.T.m_iAsI8) {
      xColEnc.m_jDataBytes = C_0B;
      
      xColEnc.m_uMetaCode = METACODE_FORM(MC_POSITIVE,1);
    }
    else {
      xColEnc.m_jDataBytes = C_0B;
      xColEnc.m_uMetaCode = METACODE_FORM(MC_POSITIVE,0);
    }
  }

  /*----------------------------------------------------------------------------*/
  /* encode a dte column value                                                  */
  /*----------------------------------------------------------------------------*/
  inline void vEncodeDte(_ColEnc& xColEnc, Dtm wColVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
    
    // null case
    if (bEncodeNul(xColEnc,wColVal))
      return;
    
    xColEnc.m_wDataCode.m_wDte.T.m_iAsI8 = wColVal.m_wDte.T.m_iAsI8;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // encode as: MC_DTE_En_2B + {D}
    
    i8 iEra = 0;
    vDteIntoEdt(wColVal.m_wDte.T.m_iAsI8,
                &xColEnc.m_wDataCode.m_wEdt.T.m_iAsI8,iEra);
    
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
    
    xColEnc.m_jDataBytes = C_2B;
    
  }

  /*----------------------------------------------------------------------------*/
  /* encode a flt column value                                                  */
  /*----------------------------------------------------------------------------*/
  inline void vEncodeFlt(_ColEnc& xColEnc, Dtm wColVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
    
    // null case
    if (bEncodeNul(xColEnc,wColVal))
      return;
    
    xColEnc.m_wDataCode.m_wFlt.T.m_dFlt = wColVal.m_wFlt.T.m_dFlt;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // general case -- encode as: MC_FLT_1..8B + {D}
    i8 nD = wColVal.iByteMagn();
    xColEnc.m_jDataBytes = nD;
    
    xColEnc.m_uMetaCode = METACODE_FORM(MC_NUMERIC,nD);
  }

  /*----------------------------------------------------------------------------*/
  /* encode an int column value                                                 */
  /*----------------------------------------------------------------------------*/
  inline void vEncodeInt(_ColEnc& xColEnc, Dtm wColVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   
    // general case -- encode as: MC_INT_1..8B + {D}
    xColEnc.m_wDataCode.m_iWrd = wColVal.m_iWrd;
  
    if (wColVal.m_iWrd > 10) {
      xColEnc.m_jDataBytes = (u8)IntLib::iByteMagnitude(wColVal.m_iWrd);
   
      xColEnc.m_uMetaCode = METACODE_FORM(MC_NUMERIC,xColEnc.m_jDataBytes);
      return;
    }
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // 0..10 case -- encode as MC_INT_ONE..TEN + {}
    if (wColVal.m_iWrd >= 0 && wColVal.m_iWrd <= 10) {
      xColEnc.m_jDataBytes = C_0B;
      
      u1 uHigh = MC_POSITIVE;
      uHigh <<= 4;
      u1 uLow = (u1)wColVal.m_iWrd;
      u1 eTest = (uHigh | uLow);
      UNUSED(eTest);
      
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
      xColEnc.m_jDataBytes = C_0B;
      
      xColEnc.m_uMetaCode = METACODE_FORM(MC_NEGATIVE,1);
      return;
    }
  
    xColEnc.m_jDataBytes = IntLib::iByteMagnitude(wColVal.m_iWrd);

    xColEnc.m_uMetaCode = METACODE_FORM(MC_NUMERIC,xColEnc.m_jDataBytes);

  }

  /*----------------------------------------------------------------------------*/
  /* encode an ivl column value                                                 */
  /*----------------------------------------------------------------------------*/
  inline void vEncodeIvl(_ColEnc& xColEnc, Dtm wColVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
    
    // null case
    if (bEncodeNul(xColEnc,wColVal))
      return;
    
    xColEnc.m_wDataCode.m_wIvl.T.m_iAsI8 = wColVal.m_wIvl.T.m_iAsI8;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // general case -- encode as: MC_IVL_1..8B : {D}
    i8 nD = wColVal.iByteMagn();
    xColEnc.m_jDataBytes = nD;
    
    xColEnc.m_uMetaCode = METACODE_FORM(MC_NUMERIC,nD);
  }

  /*----------------------------------------------------------------------------*/
  /* encode a tme column value                                                  */
  /*----------------------------------------------------------------------------*/
  inline void vEncodeTme(_ColEnc& xColEnc, Dtm wColVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
    
    // null case
    if (bEncodeNul(xColEnc,wColVal))
      return;
    
    xColEnc.m_wDataCode.m_wTme.m_iAsI8 = wColVal.m_wTme.m_iAsI8;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // general case -- encode as: MC_TME_1..8B : {D}
    i8 nD = wColVal.iByteMagn();
    xColEnc.m_jDataBytes = nD;
    
    xColEnc.m_uMetaCode = METACODE_FORM(MC_NUMERIC,nD);
  }

  /*----------------------------------------------------------------------------*/
  /* encode a tsp column value                                                  */
  /*----------------------------------------------------------------------------*/
  inline void vEncodeTsp(_ColEnc& xColEnc, Dtm wColVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
    
    // null case
    if (bEncodeNul(xColEnc,wColVal))
      return;
    
    xColEnc.m_wDataCode.m_wTsp.T.m_iAsI8 = wColVal.m_wTsp.T.m_iAsI8;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // encode as: MC_1..8B : {D}
    i8 nD = 8;
    xColEnc.m_jDataBytes = nD;
    
    xColEnc.m_uMetaCode = METACODE_FORM(MC_NUMERIC,nD);
  }

  /*----------------------------------------------------------------------------*/
  /* encode a vch column value                                                  */
  /*----------------------------------------------------------------------------*/
  inline void vEncodeVch(_ColEnc& xColEnc, Dtm wColVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
    
    u1 nQuanta;
    
    // null case
    if (bEncodeNul(xColEnc,wColVal))
      return;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // copy chars + null into wsp, nS will be size  (i.e. noOf(chars)+1 )
    char* azDst = (char*)m_xWsp.ayLatter();
    UNUSED(azDst);
    //i8 nS = StrLib::nCopy(azDst,(char*)wColVal.m_szVch) + 1;
    i8 jVchSize = StrLib::nLength((char*)wColVal.m_szVch) + 1;

    // have copied nS bytes into wsp
    //m_xWsp.vTake(nS);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      
    // 1) MC_STRING_x1
    if (jVchSize <= 15) {
      nQuanta = jVchSize; //MemLib::nQuantaRequired(nS,1);
      
      xColEnc.m_wDataCode.m_azStr = (char*)wColVal.m_szVch;
      xColEnc.m_jDataBytes = STRx1(nQuanta);
      
      xColEnc.m_uMetaCode = METACODE_FORM(MC_STRING_x1,nQuanta);
      return;
    }
    
    // 2) MC_STRINGx2
    if (jVchSize <= 45) {
      nQuanta = MemLib::nQuantaRequired(jVchSize-15,2);
      
      xColEnc.m_wDataCode.m_azStr = (char*)wColVal.m_szVch;
      xColEnc.m_jDataBytes = STRx2(nQuanta);
      
      xColEnc.m_uMetaCode = METACODE_FORM(MC_STRING_x2,nQuanta);

      return;
    }
    
    // 3) MC_STRINGx4
    if (jVchSize <= 105) {
      nQuanta = MemLib::nQuantaRequired(jVchSize-45,4);
      

      xColEnc.m_wDataCode.m_azStr = (char*)wColVal.m_szVch;
      xColEnc.m_jDataBytes = STRx4(nQuanta);
      
      xColEnc.m_uMetaCode = METACODE_FORM(MC_STRING_x4,nQuanta);

      return;
    }

    // 4) MC_STRINGx8
    if (jVchSize <= 225) {
      nQuanta = MemLib::nQuantaRequired(jVchSize-105,8);
      
      xColEnc.m_wDataCode.m_azStr = (char*)wColVal.m_szVch;
      xColEnc.m_jDataBytes = STRx8(nQuanta);
      
      xColEnc.m_uMetaCode = METACODE_FORM(MC_STRING_x8,nQuanta);

      return;
    }

    // 5) MC_STRING_TAIL
    xColEnc.m_wDataCode.m_azStr = (char*)wColVal.m_szVch;
    xColEnc.m_jDataBytes = jVchSize;
      
    xColEnc.m_uMetaCode = METACODE_FORM(MC_STRING_TAIL,2);
  }

  /*--------------------------------------------------------------------------*/
  /* compute initial offset and till for column encoding                      */
  /*--------------------------------------------------------------------------*/
//  inline void vOffsetTill(_ColEnc& xrColEnc)
//  {
//    _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
//    
//    // set the offset of this column
//    xrColEnc.m_i_Offset = m_i_Offset_next;
//    
//    // get the base size of the encoding
//    //i8 iHigh = METACODE_HIGH(xrColEnc.m_uMetaCode);
//    //i8 iLow  = METACODE_LOW(xrColEnc.m_uMetaCode);
//    //i8 nDataBytes = nDataSizeOfCode(iHigh,iLow);
//    
//    // if 0B case then till is same as offset
//    if (! xrColEnc.m_jDataSize) {
//      xrColEnc.m_i_Till = m_i_Offset_next;
//      return;
//    }
//      
////    if (MC_STRING_TAIL == (eMC)iHigh) {
////      i8 nStrLen = StrLib::nLength(xrColEnc.m_wDataCode.m_szVch->azChar());
////      nDataBytes += nStrLen + 1;
////    }
//    
//    // set where the next offset will be
//    m_i_Offset_next += xrColEnc.m_jDataSize;
//    
//    // set the till byte of the column encoding
//    xrColEnc.m_i_Till = m_i_Offset_next; // - 1;
//  }


  };
  
  Encoding* _pEncoding(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
