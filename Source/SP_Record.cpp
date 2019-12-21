/*============================================================================*/
/* SP_Record.cpp - Record sprint helper                                       */
/*============================================================================*/

#include "Records.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Record::SP_Record(void)
: Sprint(CN_RECORD)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
  
  // set default enclosing char
  vFormat(F_VCH_enclose);

  m_azAppendCursor = azRecord();
  m_pStreamDef     = nullptr;
  m_iSubSet        = C_UNKNOWN;

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caRecord,sizeof(m_caRecord),CN_RECORD);
#else
  m_caRecord[0] = '\0';
#endif
  
  //! FIX RAY
  //_checkInitialised_(CN_RECORD);
  
  _postcondition_(_MAGIC_,CN_RECORD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Record::~SP_Record(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append ascii of datum to record                                            */
/*----------------------------------------------------------------------------*/
void SP_Record::vAppendCol(Column* pCol, Row* pRow)
{
  Datum oDatum;
  
  i8 i_Col = pCol->iColumnNo();
  oDatum = pRow->m_waColVal[i_Col].m_iWrd;
  
  if (oDatum.bIsNull()) {
    StrLib::vAppendString(&m_azAppendCursor,"null");
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  eDM eDatum = pCol->eDatum();
  
  switch (eDatum) {
    case DM_BCD: vAppendBcd(pCol,&oDatum); break;
    case DM_BOL: vAppendBol(pCol,&oDatum); break;
    case DM_DTE: vAppendDte(pCol,&oDatum); break;
    case DM_FLT: vAppendFlt(pCol,&oDatum); break;
    case DM_INT: vAppendInt(pCol,&oDatum); break;
    case DM_IVL: vAppendIvl(pCol,&oDatum); break;
    case DM_TME: vAppendTme(pCol,&oDatum); break;
    case DM_TSP: vAppendTsp(pCol,&oDatum); break;
      
    case DM_VCH:
      oDatum.vStz(pRow->m_waColVal[i_Col].m_szVch);
      //oDatum.vStzDump();
      
      vAppendVch(pCol,&oDatum);
      break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* append ascii of datum to record                                            */
/*----------------------------------------------------------------------------*/
void SP_Record::vAppendDtm(Column* pCol, Datum* pDatum)
{
  if (pDatum->bIsNull()) {
    StrLib::vAppendString(&m_azAppendCursor,"null");
    return;
  }
  
  eDM eDatum = pCol->eDatum();
  
  switch (eDatum) {
    case DM_BCD: vAppendBcd(pCol,pDatum); break;
    case DM_BOL: vAppendBol(pCol,pDatum); break;
    case DM_DTE: vAppendDte(pCol,pDatum); break;
    case DM_FLT: vAppendFlt(pCol,pDatum); break;
    case DM_INT: vAppendInt(pCol,pDatum); break;
    case DM_IVL: vAppendIvl(pCol,pDatum); break;
    case DM_TME: vAppendTme(pCol,pDatum); break;
    case DM_TSP: vAppendTsp(pCol,pDatum); break;
    case DM_VCH: vAppendVch(pCol,pDatum); break;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* append ascii of decimal datum to record                                    */
/*----------------------------------------------------------------------------*/
void SP_Record::vAppendBcd(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  s32 sAscii;
  bool bNegative = false;
  
  i8 iDigits = 1;
  for (i8 i_Digit = 0; i_Digit < pCol->iColumnScale(); i_Digit++)
    iDigits *= 10;
  
  i8 iBcd_Whole    = pDatum->iBcd() / iDigits;
  i8 iBcd_Fraction = pDatum->iBcd() % iDigits;
  
  if (iBcd_Fraction < 0 || iBcd_Whole < 0) {
    bNegative     = true;
    iBcd_Fraction = iBcd_Fraction * -1;
    iBcd_Whole    = iBcd_Whole * -1;
  }
  
  if (pCol->iColumnScale())
    StrLib::vSprint(sAscii.azChar(),"%s%lld.%.*lld"_Stz,
                    bNegative ? "-" : "",
                    iBcd_Whole,
                    pCol->iColumnScale(),
                    iBcd_Fraction);
  else
    StrLib::vSprint(sAscii.azChar(),"%s%lld"_Stz,
                    bNegative ? "-" : "",
                    iBcd_Whole);
  
  //m_sRecStr.vConcat((Stz*)sAscii.azChar());
  StrLib::vAppendString(&m_azAppendCursor,sAscii.azChar());
}

/*----------------------------------------------------------------------------*/
/* append ascii of boolean datum to record                                    */
/*----------------------------------------------------------------------------*/
void SP_Record::vAppendBol(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Bol SP_Bol;
  
  StrLib::vAppendString(&m_azAppendCursor,(char*)SP_Bol.sz(pDatum->iBol()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of date datum to record                                       */
/*----------------------------------------------------------------------------*/
void SP_Record::vAppendDte(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  s32 sAscii;
  
  StrLib::vSprint(sAscii.azChar(),
                  "%02lld-%02lld-%02lld"_Stz,
                  (i8)pDatum->iDte_year(),
                  (i8)pDatum->iDte_month(),
                  (i8)pDatum->iDte_day());
  
  //m_sRecStr.vConcat((Stz*)sAscii.azChar());
  StrLib::vAppendString(&m_azAppendCursor,sAscii.azChar());
}

/*----------------------------------------------------------------------------*/
/* append ascii of float datum to record                                      */
/*----------------------------------------------------------------------------*/
void SP_Record::vAppendFlt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  s32 sAscii;
  
  StrLib::vSprint(sAscii.azChar(),"%g"_Stz,pDatum->dFlt());
  
  //m_sRecStr.vConcat((Stz*)sAscii.azChar());
  StrLib::vAppendString(&m_azAppendCursor,sAscii.azChar());
}

/*----------------------------------------------------------------------------*/
/* append ascii of integer datum to record                                    */
/*----------------------------------------------------------------------------*/
void SP_Record::vAppendInt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Int spInt;
  
  StrLib::vAppendString(&m_azAppendCursor,(char*)spInt.sz(pDatum->iInt()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of interval datum to record                                   */
/*----------------------------------------------------------------------------*/
void SP_Record::vAppendIvl(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Ivl spIvl;
  
  StrLib::vAppendString(&m_azAppendCursor,(char*)spIvl.sz(pDatum->iIvl()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of time datum to record                                       */
/*----------------------------------------------------------------------------*/
void SP_Record::vAppendTme(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Tme SP_Tme;
  
  StrLib::vAppendString(&m_azAppendCursor,(char*)SP_Tme.sz(pDatum->iTme()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of timestamp datum to record                                  */
/*----------------------------------------------------------------------------*/
void SP_Record::vAppendTsp(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Tsp SP_Tsp;
  
  StrLib::vAppendString(&m_azAppendCursor,(char*)SP_Tsp.sz(pDatum->iTsp()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of varchar datum to record                                    */
/*----------------------------------------------------------------------------*/
void SP_Record::vAppendVch(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Vch SP_Vch;
  
  StrLib::vAppendString(&m_azAppendCursor,(char*)SP_Vch.sz(pDatum->azChar()));
}

/*----------------------------------------------------------------------------*/
/* return char* of result                                                     */
/*----------------------------------------------------------------------------*/
char* SP_Record::azRecord(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return & m_caRecord[0];
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void SP_Record::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD);
  
  m_caRecord[0] = '\0';
  
  m_azAppendCursor = azRecord();
}

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
void SP_Record::vRow(Row* pRow)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  char* szCrs = &m_caRecord[0];
  
  for (i8 i_Col = 0; i_Col < pRow->m_nColumns; i_Col++) {
  
    Column* pCol = pRow->m_pStreamDef->pGetColumn(i_Col);
    eDM eDatum   = pCol->eDatum();
    Stz* szDatum = (Stz*)aDM[eDatum];

    StrLib::vAppendString(&szCrs,(char*)"  "_Stz);

    StrLib::vAppendString(&szCrs,(char*)pCol->szColNm());
    StrLib::vAppendString(&szCrs,(char*)" "_Stz);
    StrLib::vAppendString(&szCrs,(char*)szDatum);
    StrLib::vAppendString(&szCrs,(char*)" "_Stz);
    
    if (pRow->m_waColVal[i_Col].bIsNull()) {
      StrLib::vAppendString(&szCrs,(char*)"null"_Stz);
      continue;
    }
      
    switch (eDatum) {
      case DM_BCD: {
        SP_Bcd SP_Bcd;
        StrLib::vAppendString(&szCrs,
                              (char*)SP_Bcd.sz(pRow->m_waColVal[i_Col]));
        break;
      }

      case DM_BOL: {
        SP_Bol SP_Bol;
        StrLib::vAppendString(&szCrs,
                              (char*)SP_Bol.sz(pRow->m_waColVal[i_Col]));
        break;
      }
        
      case DM_DTE: {
        SP_Dte SP_Dte;
        StrLib::vAppendString(&szCrs,
                              (char*)SP_Dte.sz(pRow->m_waColVal[i_Col]));
        break;
      }
        
      case DM_FLT: {
        SP_Flt SP_Flt;
        StrLib::vAppendString(&szCrs,
                              (char*)SP_Flt.sz(*(d8*)&pRow->m_waColVal[i_Col]));
        break;
      }
        
      case DM_INT: {
        SP_Int spInt;
        StrLib::vAppendString(&szCrs,
                              (char*)spInt.sz(pRow->m_waColVal[i_Col]));
        break;
      }
        
      case DM_IVL: {
        SP_Ivl spIvl;
        StrLib::vAppendString(&szCrs,
                              (char*)spIvl.sz(pRow->m_waColVal[i_Col]));
        break;
      }
        
      case DM_TME: {
        SP_Tme SP_Tme;
        StrLib::vAppendString(&szCrs,
                              (char*)SP_Tme.sz(pRow->m_waColVal[i_Col]));
        break;
      }
        
      case DM_TSP: {
        SP_Tsp SP_Tsp;
        StrLib::vAppendString(&szCrs,
                              (char*)SP_Tsp.sz(pRow->m_waColVal[i_Col]));
        break;
      }
        
      case DM_VCH: {
        SP_Vch SP_Vch;
        StrLib::vAppendString(&szCrs,
                              (char*)SP_Vch.sz(pRow->m_waColVal[i_Col].m_azStr));
        break;
      }
        
      default:
        NEVER_REACH;
    }

    StrLib::vAppendString(&szCrs,(char*)"\n"_Stz);
  }
  
}

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
Stz* SP_Record::sz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*) &m_caRecord[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_Record::sz(Row* pRow)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vRow(pRow);
  
  return (Stz*) &m_caRecord[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SP_Record::aFooter(void)
{
  _precondition_(_MAGIC_,CN_RECORD);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
