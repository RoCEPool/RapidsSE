/*============================================================================*/
/* ToRsb.hpp -- Csv Record to Rsb                                             */
/*============================================================================*/
#pragma once

namespace Load
{
  static i8 iDaysPerMonth[12]     = { 31,28,31,30,31,30,31,31,30,31,30,31};
  static i8 iDaysPerMonthLeap[12] = { 31,29,31,30,31,30,31,31,30,31,30,31};
  
  class ToRsb : public Object
  {
  private:
    StreamDef* m_pStreamDef;
    Format*    m_pFmt;
    bool       m_bOrdCsv;

    char*      m_azSrc;
    char*      m_acChr;

    char       m_cRecEnd;
    char       m_cFldSep;
  
    i8         m_iLine;
    i8         m_iCol;
    i8         m_i_Fld;
    i8         m_i_Rec;
      
    Column*    m_pCol;
    bool       m_bPtnHash;
  
    bool       m_bTrace;
    d8         m_dCalc;
    i8         m_nChars;
    bool       m_bSawFldSep;
  
    Datum      m_oDatum;
  
    RowSlab*   m_pRowSlab;
    RsbBuffer* m_pRsbBuffer;
  
    bool       m_bFirst;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ToRsb(StreamDef* pStreamDef, Format* pFmt, bool bPtnHash);
   ~ToRsb(void) noexcept(false);
  
    bool bConvertCrw(CrwBuffer* pCrwBuffer);
    bool bConvertCsv(char* azSrc);

    RsbBuffer* pDanglingRsbBuffer(void);
    RsbBuffer* pFullRsbBuffer(void);
    void vNewRsbBuffer(void);

    RsbBuffer* pRsbBuffer(void);
    void vRsbBuffer(RsbBuffer* pRsbBuffer);
  

  private:
    Footer* aFooter(void);

  #define CALC ((m_dCalc * 3.141 + (d8)*m_acChr) / 1.618)

  /*--------------------------------------------------------------------------*/
  /*                               - SUPPORT -                                */
  /*--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* test for more data                                                       */
  /*--------------------------------------------------------------------------*/
  inline bool bMoreData(void)
  {
    bool bMore = (*m_acChr);
    
    return bMore;
  }
  
  /*--------------------------------------------------------------------------*/
  /* test for more fields                                                     */
  /*--------------------------------------------------------------------------*/
  inline bool bMoreFields(void)
  {
    bool bMore = (*m_acChr && m_cRecEnd != *m_acChr);
    
    return bMore;
  }
  
  /*--------------------------------------------------------------------------*/
  /* test for more fields                                                     */
  /*--------------------------------------------------------------------------*/
  inline bool bInField(void)
  {
    bool bMore = (*m_acChr && m_cFldSep != *m_acChr && m_cRecEnd != *m_acChr);
    
    return bMore;
  }
  
  /*--------------------------------------------------------------------------*/
  /* test blankspace                                                          */
  /*--------------------------------------------------------------------------*/
  inline bool bIsBlankspace(char cChr)
  {
    if (cChr == ' ')  return true;
    if (cChr == '\t') return true;
    
    return false;
  }
  
  /*--------------------------------------------------------------------------*/
  /* skip whitespace                                                          */
  /*--------------------------------------------------------------------------*/
  inline void vSkipBlankspace(void)
  {
    if ( (*m_acChr && bIsBlankspace(*m_acChr)) ) {
      while (*m_acChr && StrLib::bIsWhite(*m_acChr)) {
        m_acChr++;
        m_iCol++;
        m_nChars++;
      }
    }
  }

  /*--------------------------------------------------------------------------*/
  /* extract boolean                                                          */
  /*--------------------------------------------------------------------------*/
  inline void vFieldTrace(eCT eCol)
  {
    if (m_bTrace)
      vTrace("[%lld,%lld @%02lld,%02lld %10s] "_Stz,
             m_i_Rec,
             m_i_Fld,
             
             m_iLine,
             m_iCol,

             aCT[eCol]);
  }
  
  /*--------------------------------------------------------------------------*/
  /*                              - FIELD SPRINT -                            */
  /*--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* sprint boolean                                                           */
  /*--------------------------------------------------------------------------*/
  inline void vSprintBoolean(void)
  {
    if (! m_bTrace)
      return;
  
    SP_Bol spBol;
  
    vTrace("Boolean : %s\n"_Stz,spBol.sz(m_oDatum.iBol()));
  }

  /*--------------------------------------------------------------------------*/
  /* sprint date                                                              */
  /*--------------------------------------------------------------------------*/
  inline void vSprintDate(void)
  {
    if (! m_bTrace)
      return;
  
    SP_Dte spDte;
  
    vTrace("Date : %s\n"_Stz,spDte.sz(m_oDatum.iDte()));
  }

  /*--------------------------------------------------------------------------*/
  /* sprint decimal                                                           */
  /*--------------------------------------------------------------------------*/
  inline void vSprintDecimal(void)
  {
    if (! m_bTrace)
      return;
  
    SP_Bcd spBcd;
  
    vTrace("Decimal : %s\n"_Stz,
           spBcd.sz(m_oDatum.iBcd(),
                    (i8)m_pRowSlab->axaColPix()[m_i_Fld].m_uScale));
  }

  /*--------------------------------------------------------------------------*/
  /* sprint float                                                             */
  /*--------------------------------------------------------------------------*/
  inline void vSprintFloat(void)
  {
    if (! m_bTrace)
      return;
  
    SP_Flt spFlt;
  
    vTrace("Float : %s\n"_Stz,spFlt.sz(m_oDatum.dFlt()));
  }

  /*--------------------------------------------------------------------------*/
  /* sprint integer                                                           */
  /*--------------------------------------------------------------------------*/
  inline void vSprintInteger(void)
  {
    if (! m_bTrace)
      return;
  
    SP_Int spInt;
  
    vTrace("Integer : %s\n"_Stz,spInt.sz(m_oDatum.iInt()));
  }

  /*--------------------------------------------------------------------------*/
  /* sprint interval                                                          */
  /*--------------------------------------------------------------------------*/
  inline void vSprintInterval(void)
  {
    if (! m_bTrace)
      return;
  
    SP_Ivl spIvl;
  
    vTrace("Interval : %s\n"_Stz,spIvl.sz(m_oDatum.iIvl()));
  }

  /*--------------------------------------------------------------------------*/
  /* sprint time                                                              */
  /*--------------------------------------------------------------------------*/
  inline void vSprintTime(void)
  {
    if (! m_bTrace)
      return;
  
    SP_Tme spTme;
  
    vTrace("Time : %s\n"_Stz,spTme.sz(m_oDatum.iTme()));
  }

  /*--------------------------------------------------------------------------*/
  /* sprint timestamp                                                         */
  /*--------------------------------------------------------------------------*/
  inline void vSprintTimestamp(void)
  {
    if (! m_bTrace)
      return;
  
    SP_Tsp spTsp;
  
    vTrace("Timestamp : %s\n"_Stz,spTsp.sz(m_oDatum.iTsp()));
  }

  /*--------------------------------------------------------------------------*/
  /* sprint varchar                                                           */
  /*--------------------------------------------------------------------------*/
  inline void vSprintVarchar(void)
  {
    if (! m_bTrace)
      return;
  
    SP_Vch spVch;
  
    vTrace("Varchar : %s (%lldB)\n"_Stz,
           m_oDatum.szStz(),
           StrLib::nLength(m_oDatum.azChar())
           );
  }

  /*--------------------------------------------------------------------------*/
  /*                            - FIELD CONVERSION -                          */
  /*--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* extract boolean                                                          */
  /*--------------------------------------------------------------------------*/
  inline void vConvertBoolean(void)
  {
    vFieldTrace(CT_BOOLEAN);
               
    Bol xBol;
    SN_Bol snBol;

    xBol = snBol.i(&m_acChr,m_pFmt);
  
    m_pRowSlab->vDeposit(xBol);
    m_oDatum.vBol(xBol);

    vSprintBoolean();
  }

  /*--------------------------------------------------------------------------*/
  /* extract date                                                             */
  /*--------------------------------------------------------------------------*/
  inline void vConvertDate(void)
  {
    vFieldTrace(CT_DATE);

    Dte xDte;
    SN_Dte snDte;

    xDte = snDte.i(&m_acChr,m_pFmt);
  
    m_pRowSlab->vDeposit(xDte);
    m_oDatum.vDte(xDte);

    vSprintDate();
  }

  /*--------------------------------------------------------------------------*/
  /* extract date                                                             */
  /*--------------------------------------------------------------------------*/
  inline void vConvertDateFast(void)
  {
    vFieldTrace(CT_DATE);
    
    //Dte xDte;
    //SN_Dte snDte;
    
    //xDte = snDte.i(&m_acChr,m_pCol,m_pFmt);
    i8 iYear = 0, iMonth = 0, iDay = 0;
    
    for (i8 i_Component = 0; i_Component < 3; i_Component++) {
      
      eDC eDateComponent = m_pFmt->m_eDateComponent[i_Component];
      
      switch (eDateComponent) {
          
        case DC_DD: {
          
          u8 uSubField;
            
          char *s = m_acChr;
          i8 iDigits = 2;
          
          if ( ! *s )
            THROW_USER("vConvertDateFast"_Stz);
          
          bool bOnce = false;
          if (*s && isdigit(*s))
            bOnce = true;
          
          u8 v = 0;
          for ( ; *s && iDigits > 0; s++)
          {
            char c = *s;
            
            // decrement no of digits extracted
            iDigits--;
            
            //    if ( ! isdigit(c) )
            if ( c < '0' || c > '9' )
              break;
            
            v = (v * 10) + c - '0';
          }
          
          uSubField = v;
          m_acChr = s;
          
          if (!bOnce)
            THROW_USER("vConvertDateFast"_Stz);
          
          iDay = uSubField;
          break;
        }
          
        case DC_MM: {
          u8 uSubField;
          
          iMonth = 0;

          char *s = m_acChr;
          i8 iDigits = 2;
          
          if ( ! *s )
            THROW_USER("vConvertDateFast"_Stz);
          
          bool bOnce = false;
          if (*s && isdigit(*s))
            bOnce = true;
          
          u8 v = 0;
          for ( ; *s && iDigits > 0; s++)
          {
            char c = *s;
            
            // decrement no of digits extracted
            iDigits--;
            
            //    if ( ! isdigit(c) )
            if ( c < '0' || c > '9' )
              break;
            
            v = (v * 10) + c - '0';
          }
          
          uSubField = v;
          m_acChr = s;
          
          if (!bOnce)
            THROW_USER("vConvertDateFast"_Stz);
          
          if (uSubField < 1 || uSubField > 12)
            THROW_USER("vConvertDateFast"_Stz);
          
          iMonth = uSubField;
          
          break;
        }
          
        case DC_YYYY: {
          i8 nDigits;
          
          // extract year
          //if ( ! StrLib::bExtractInteger(&m_acChr,&iYear,nDigits) )
          //  THROW_USER("vConvertDateFast"_Stz);
          
          char *aSrc = m_acChr;
          
          if ( ! *aSrc )
            THROW_USER("vConvertDateFast"_Stz);
          
          bool bOnce = false;
          if (*aSrc && isdigit(*aSrc))
            bOnce = true;
          
          nDigits = 0;
          
          u8 uUnsignedResult = 0;
          for ( ; *aSrc; aSrc++) {
            
            char c = *aSrc;
            
            if ( c < '0' || c > '9' )
              break;
            
            uUnsignedResult = (uUnsignedResult * 10) + (c - '0');
            nDigits++;
            
            // have we exceeded the max ?
            if (uUnsignedResult > (u8)C_I8_MAX)
              THROW_USER("vConvertDateFast"_Stz);
          }
          
          iYear = uUnsignedResult;
          
          m_acChr = aSrc;
          
          if (!bOnce)
            THROW_USER("vConvertDateFast"_Stz);
          
          if (2 == nDigits) {
            iYear += 2000;
            break;
          }
          
          if (4 == nDigits)
            break;
          
          THROW_USER("vConvertDateFast"_Stz);
          break;
        }
          
        default:
          THROW_USER("vConvertDateFast"_Stz);
      }
      
      // skip separator after components  0 and 1
      if (i_Component < 2) {
        if (m_pFmt->m_cDateSeparator != *m_acChr++)
          THROW_USER("vConvertDateFast"_Stz);
      }
      
    }
    
    if (! StrLib::bValidateMonth(iDay, iMonth, iYear))
      THROW_USER("vConvertDateFast"_Stz);
    
    Dte xDte;
    xDte.m_uYearDelta  = iYear - C_YEAR_BASE;
    xDte.m_uMonth = iMonth;
    xDte.m_uDay   = iDay;
    xDte.m_uSpare = 0;
    
    m_pRowSlab->vAdvanceCursor();
    
    i8* iaCells = (i8*)m_pRowSlab->amCellArray();
    
    iaCells[m_pRowSlab->i_CellCrs()] = xDte.m_iAsI8;

    //m_oDatum.vDte(xDte);
    
    //vSprintDate();
  }
    
  /*--------------------------------------------------------------------------*/
  /* extract date                                                             */
  /*--------------------------------------------------------------------------*/
  inline void vConvertDecimal(void)
  {
    vFieldTrace(CT_DECIMAL);
  
    Bcd xBcd;
    SN_Bcd snBcd;

    xBcd = snBcd.i(&m_acChr,m_pCol->iColumnLength(), m_pCol->iColumnScale(),m_pFmt);
  
    m_pRowSlab->vDeposit(xBcd);
    m_oDatum.vBcd(xBcd);
  
    vSprintDecimal();
  }

  /*--------------------------------------------------------------------------*/
  /* extract date                                                             */
  /*--------------------------------------------------------------------------*/
  inline void vConvertDecimalFast(void)
  {
    vFieldTrace(CT_DECIMAL);
    
    Bcd xBcd;
    //SN_Bcd snBcd;
    //xBcd = snBcd.i(&m_acChr,m_pCol,m_pFmt);
    
    i8 iDecimal;
    i8 iLength = m_pCol->iColumnLength();
    i8 iScale  = m_pCol->iColumnScale();
    i8 iPrecision = iLength - iScale;
    
    u8 uUnsignedResult = 0;
    
    // test for negative or positive sign
    bool bNegative = false;
    if (*m_acChr && '-' == *m_acChr ) {
      bNegative = true;
      (*m_acChr)++;
    }
    else
      if (*m_acChr && '+' == *m_acChr ) {
        (m_acChr)++;
      }
    
    bool bOnce = false;
    if (*m_acChr && (isdigit(*m_acChr) || '.' == *m_acChr))
      bOnce = true;
    
    // ignore leading 0's
    while (isdigit(*m_acChr) && *m_acChr - '0' == 0) {
      m_acChr++;
    }
    
    // collect leading digits (till '.')
    while (isdigit(*m_acChr)) {
      uUnsignedResult = uUnsignedResult * 10 + *m_acChr - '0';
      m_acChr++;
      iPrecision--;
    }
    
    // have we exceeded maximum precision digits?
    if (iPrecision < 0)
      THROW_USER("vConvertDecimalFast"_Stz);
    
    if ('.' == *m_acChr) {
      
      m_acChr++; // skip '.'
      
      while (isdigit(*m_acChr) && iScale) {
        uUnsignedResult = uUnsignedResult * 10 + *m_acChr - '0';
        m_acChr++;
        iScale--;
      }
    }
    
    while (iScale--)
      uUnsignedResult = uUnsignedResult * 10 + 0;
    
    // have we exceeded the max ?
    if (uUnsignedResult > (u8)C_I8_MAX)
      THROW_USER("vConvertDecimalFast"_Stz);
    
    // round up if there any more digits after decimal
    if (isdigit(*m_acChr)) {
      if (*m_acChr - '0' >= 5)
        uUnsignedResult += 1;
      while (isdigit(*m_acChr))
        m_acChr++;
    }
    
    // compute final result
    uUnsignedResult = bNegative ? - uUnsignedResult : uUnsignedResult;
    
    // have we gone beyond the min ?
    if (uUnsignedResult != 0 && bNegative && (uUnsignedResult < (u8)C_I8_MIN))
      THROW_USER("vConvertDecimalFast"_Stz);
    
    iDecimal = uUnsignedResult;
    
    if ( ! bOnce )
      THROW_USER("vConvertDecimalFast"_Stz);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // deposit the decimal
    
    m_pRowSlab->vAdvanceCursor();
    i8* iaCells = (i8*)m_pRowSlab->amCellArray();
    iaCells[m_pRowSlab->i_CellCrs()] = iDecimal;
    
    //m_oDatum.vBcd(xBcd);
    
    //vSprintDecimal();
  }
    
  /*--------------------------------------------------------------------------*/
  /* extract float                                                            */
  /*--------------------------------------------------------------------------*/
  inline void vConvertFloat(void)
  {
    vFieldTrace(CT_FLOAT);

    Flt xFlt;
    SN_Flt snFlt;

    xFlt = snFlt.d(&m_acChr,m_pFmt);
  
    m_pRowSlab->vDeposit(xFlt.m_dFlt);
    m_oDatum.vFlt(xFlt);

    vSprintFloat();
  }

  /*--------------------------------------------------------------------------*/
  /* extract integer                                                          */
  /*--------------------------------------------------------------------------*/
  inline void vConvertInteger(void)
  {
    vFieldTrace(CT_INTEGER);

//    while (bInField()) {
//
//      m_dCalc = (CALC) - 4.0;
//
//      if (m_bTrace)
//        vTrace("%c"_Stz,*m_acChr);
//      m_acChr++;
//      m_iCol++;
//      m_nChars++;
//    }
  
//    i8 iInt;
//    StrLib::bExtractInteger(&m_acChr,&iInt,true);
//
//    m_oDatum.vInt(iInt);
  
    Int xInt;
    SN_Int snInt;

//  if (m_iLine > 1199000) {
//    vTrace("%lld: %c%c%c%c%c%c%c%c\n"_Stz,
//           m_iLine,
//           *(m_acChr+0),
//           *(m_acChr+1),
//           *(m_acChr+2),
//           *(m_acChr+3),
//           *(m_acChr+4),
//           *(m_acChr+5),
//           *(m_acChr+6),
//           *(m_acChr+7)
//           );
//  }
  
    xInt = snInt.i(&m_acChr,m_pFmt);
  
    m_pRowSlab->vDeposit(xInt);
    m_oDatum.vInt(xInt);

    vSprintInteger();
  }
  
  /*--------------------------------------------------------------------------*/
  /* extract integer                                                          */
  /*--------------------------------------------------------------------------*/
  inline void vConvertIntegerFast(void)
  {
    vFieldTrace(CT_INTEGER);
    
    //Int xInt;
    //SN_Int snInt;
    //xInt = snInt.i(&m_acChr,m_pCol,m_pFmt);
    
    char *aSrc = m_acChr;
    
    if ( ! *aSrc )
      THROW_USER("vConvertIntegerFast"_Stz);
    
    // test for negative or positive sign
    bool bNegative = false;
    //if (bWithSign) {
      if (*aSrc && '-' == *aSrc) {
        bNegative = true;
        aSrc++;
      }
      else
        if (*aSrc && '+' == *aSrc) {
          aSrc++;
        }
    //}
    
    bool bOnce = false;
    if (*aSrc && isdigit(*aSrc))
      bOnce = true;
    
    u8 uUnsignedResult = 0;
    for ( ; *aSrc; aSrc++) {
      
      char c = *aSrc;
      
      if ( c < '0' || c > '9' )
        break;
      
      uUnsignedResult = (uUnsignedResult * 10) + (c - '0');
      
      // have we exceeded the max ?
      if (uUnsignedResult > (u8)C_I8_MAX)
        THROW_USER("vConvertIntegerFast"_Stz);
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // deposit the integer
    
    m_pRowSlab->vAdvanceCursor();
    i8* iaCells = (i8*)m_pRowSlab->amCellArray();
    
    if (bNegative)
      iaCells[m_pRowSlab->i_CellCrs()] = - uUnsignedResult;
    else
      iaCells[m_pRowSlab->i_CellCrs()] = uUnsignedResult;
    
    //m_oDatum.vInt(xInt);
    //vSprintInteger();
    
    if ( ! bOnce)
      THROW_USER("vConvertIntegerFast"_Stz);
  }
    
  /*--------------------------------------------------------------------------*/
  /* extract interval                                                         */
  /*--------------------------------------------------------------------------*/
  inline void vConvertInterval(void)
  {
    vFieldTrace(CT_INTERVAL);

    Ivl xIvl;
    SN_Ivl snIvl;

    xIvl = snIvl.i(&m_acChr,m_pFmt);
  
    m_pRowSlab->vDeposit(xIvl);
    m_oDatum.vIvl(xIvl);

    vSprintInterval();
  }
  
  /*--------------------------------------------------------------------------*/
  /* extract time                                                             */
  /*--------------------------------------------------------------------------*/
  inline void vConvertTime(void)
  {
    vFieldTrace(CT_TIME);

    Tme xTme;
    SN_Tme snTme;

    xTme = snTme.i(&m_acChr,m_pFmt);
  
    m_pRowSlab->vDeposit(xTme);
    m_oDatum.vTme(xTme);

    vSprintTime();
  }
  
  /*--------------------------------------------------------------------------*/
  /* extract timestamp                                                        */
  /*--------------------------------------------------------------------------*/
  inline void vConvertTimestamp(void)
  {
    vFieldTrace(CT_TIMESTAMP);

    Tsp xTsp;
    SN_Tsp snTsp;

    xTsp = snTsp.i(&m_acChr,m_pFmt);
  
    m_pRowSlab->vDeposit(xTsp);
    m_oDatum.vTsp(xTsp);

    vSprintTimestamp();
  }
  
  /*--------------------------------------------------------------------------*/
  /* extract varchar                                                          */
  /*--------------------------------------------------------------------------*/
  inline void vConvertVarchar(void)
  {
    vFieldTrace(CT_VARCHAR);
             
    SN_Vch snVch;

    i8 nChars = 0;
    bool bSawEnclosedLeading = CsvValue::bIsEnclosedString(&m_acChr,m_pFmt);

    m_oDatum = snVch.sz(&m_acChr,m_pCol->iColumnLength(),
                        m_pFmt,bSawEnclosedLeading,nChars);
  
    char* szStr = m_pRowSlab->azDepositString_begin();
    StrLib::vCopy(szStr,m_oDatum.azChar());
    m_pRowSlab->vDepositString_end(nChars+1);
    ASSERT(nChars + 1 == StrLib::nSize(szStr));

    vSprintVarchar();
  }

  /*--------------------------------------------------------------------------*/
  /* extract varchar                                                          */
  /*--------------------------------------------------------------------------*/
  inline void vConvertVarcharFast(void)
  {
    vFieldTrace(CT_VARCHAR);
    
    i8 iMaxColumn = m_pCol->iColumnLength();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    char* azStzCell = m_pRowSlab->azDepositString_begin();

  
    //SN_Vch snVch;
    
    i8 nChars = 0;
    //bool bSawEnclosedLeading = CsvValue::bIsEnclosedString(&m_acChr,m_pFmt);
    
    bool bSawEnclosedLeading;
    
    if (! m_pFmt->m_cEnclosedBy_Leading)
      bSawEnclosedLeading = false;
    
    char *aStart = m_acChr;
    
    // find first non-space character
    while (' ' == *m_acChr)
      m_acChr++;
    
    // is it an enclosed varchar?
    if (*m_acChr == m_pFmt->m_cEnclosedBy_Leading) {
      m_acChr++;
      bSawEnclosedLeading = true;
    }
    else {
      m_acChr = aStart;
      bSawEnclosedLeading = false;
    }
    
    //m_oDatum = snVch.sz(&m_acChr,m_pCol,m_pFmt,bSawEnclosedLeading,nChars);
    i8 iLength = 0;
    char *aDst = (char*)azStzCell;
    
    bool bSawEnclosedTrailing = false;
    
    char cSrc = *m_acChr;
    
    while (cSrc) {
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // special case of double enclosedBy
      
      if (m_pFmt->m_cEnclosedBy_Trailing == m_pFmt->m_cEnclosedBy_Leading) {
        if (cSrc == m_pFmt->m_cEnclosedBy_Leading && *(m_acChr+1) == m_pFmt->m_cEnclosedBy_Leading) {
          
          // skip the first enclosed character
          m_acChr++;
          
          // append one instance of the enclosedBy character
          if (iLength++ < iMaxColumn)
            *aDst++ = cSrc;
          
          m_acChr++;
          
          cSrc = *m_acChr;
          
          continue;
        }
        //falls thru if not the case of double enclosedBy
      }
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // test for a non-printable character (e.g. \n or \r)
      
      // Alka: Changed check from ! isprint (not-printable) to isspace and then allowing space
      // this ways we allow for UTF-8 characters to pass this test
      // I think there is more to supporting UTF-8 but this will let us allow for UTF-8 characters to
      // be loaded from files.
      
      //if (! bAllowNonPrintable && ! isprint(**aaSrc) )
      
      // isprint: checks for any printable character including space.
      
      // isspace : checks for white-space characters. In the C and POSIX locales, these are:
      // space, form-feed ('\f'), newline ('\n'), carriage return ('\r'), horizontal tab ('\t'),
      // and vertical tab ('\v').
      
      //if (! m_pFmt->m_bAllowNonPrintable && isspace(cSrc) && (cSrc != ' ') )
      //  break;
      
      if (! m_pFmt->m_bAllowNonPrintable &&
          ( (cSrc == '\f') ||
            (cSrc == '\n') ||
            (cSrc == '\r') ||
            (cSrc == '\t') ||
            (cSrc == '\v') )
          )
        break;
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // test for normal end
      
      if (bSawEnclosedLeading) {
        // we saw a leading enclodedBy so test for trailing enclosedBy
        if (cSrc == m_pFmt->m_cEnclosedBy_Trailing) {
          bSawEnclosedTrailing = true;
          m_acChr++;
          break;
        }
        //falls thru if not trailing enclosedBy
      }
      // test for field separator (because its not enclosed)
      else if (m_pFmt->m_cFieldDelimiter && cSrc == m_pFmt->m_cFieldDelimiter)
        break;
      // test for record terminator (because its not enclosed)
      else if (m_pFmt->m_cRecordTerminator && cSrc == m_pFmt->m_cRecordTerminator)
        break;
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // test for the escape character
      
      if (cSrc == m_pFmt->m_cEscapeChar) {
        
        if (StrLib::bEscapeConvert( *(m_acChr+1),
                                   m_pFmt->m_cEnclosedBy_Leading,
                                   m_pFmt->m_cEnclosedBy_Trailing,
                                   m_pFmt->m_cEscapeChar,
                                   m_pFmt->m_cFieldDelimiter,
                                   m_pFmt->m_cRecordTerminator)) {
          
          // skip the escape character
          m_acChr++;
          
          // append the escaped character
          if (iLength++ < iMaxColumn)
            *aDst++ = StrLib::cEscapeConvert(*m_acChr++);
          else
            m_acChr++;
          
          cSrc = *m_acChr;
          continue;
        }
        else {
          m_acChr++;
          cSrc = *m_acChr;
        }
      }
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // normal character
      
      // append the normal character
      if (iLength++ < iMaxColumn)
        *aDst++ = cSrc;
      
      m_acChr++;
      cSrc = *m_acChr;
    }
    
    // terminate
    *aDst++ = '\0';
    
    // its not an enclosed varchar so we consider it to be valid
    if (iLength < iMaxColumn)
      nChars = iLength;
    else
      nChars = iMaxColumn;
    
    // if this an enclosed varchar we should have expected to
    // see the trailing enclosedBy
    if (bSawEnclosedLeading)
      if ( ! bSawEnclosedTrailing )
        THROW_USER("vConvertVarcharFast"_Stz);
    
    
    //char* szStr = m_pRowSlab->azDepositString_begin();
    //StrLib::vCopy(szStr,m_oDatum.azChar());
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // deposit the string
    
    m_pRowSlab->vDepositString_end(nChars+1);
    //m_pRowSlab->m_xWsp.vTakeHead(nChars+1);
    //ASSERT(nChars + 1 == StrLib::nSize(azStzCell));
    
    //vSprintVarchar();
  }
    
  /*--------------------------------------------------------------------------*/
  /*                            - FIELD SUPPORT -                             */
  /*--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* pre-field actions                                                        */
  /*--------------------------------------------------------------------------*/
  inline void vPreField(eDM eDatum)
  {
    m_bSawFldSep = false;
  
    if (DM_VCH == eDatum)
      return;
  
    vSkipBlankspace();
  }
  
  /*--------------------------------------------------------------------------*/
  /* post-field actions                                                       */
  /*--------------------------------------------------------------------------*/
  inline void vPostField(void)
  {
    if (m_cFldSep == *m_acChr) {
      m_bSawFldSep = true;
  
      if (m_bTrace)
        vTrace(" + fldSep\n"_Stz);
    }
  
    while (bInField()) {
      m_acChr++;
    }
  }
  
  }; // end class --------------------------------------------------------------

  ToRsb* _pToRsb(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
