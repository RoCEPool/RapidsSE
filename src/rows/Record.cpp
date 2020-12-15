/*============================================================================*/
/* Record.cpp - Record sprint helper                                          */
/*============================================================================*/

#include "Rows.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Rows,Record,CN_RECORD)
__aNewFunction(Record,CN_RECORD)
__vDeleteFunction(Record,CN_RECORD)
__aFooterFunction(Record)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Record::Record(RowPix* pRowPix)
: Object(CN_RECORD)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
  
  m_oRowPix.vCopyRowPix(pRowPix);
  
  m_azCursor = & m_caRecord[0];
  
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
Record::~Record(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* return char* of result                                                     */
/*----------------------------------------------------------------------------*/
char* Record::az(void)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
  
  return & m_caRecord[0];
}

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
Stz* Record::sz(void)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
  
  return (Stz*) &m_caRecord[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* Record::sz(Row* aRow)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);

  vAddRow(aRow);

  return (Stz*) &m_caRecord[0];
}

/*----------------------------------------------------------------------------*/
/* print record string as csv,...                                              */
/*----------------------------------------------------------------------------*/
void Record::vPrintAsCsv(void)
{
  _precondition_(_MAGIC_,CN_RECORD);
  
  vTrace("%s"_Stz,az());
}

/*----------------------------------------------------------------------------*/
/* print record string as row (...)                                           */
/*----------------------------------------------------------------------------*/
void Record::vPrintAsRow(void)
{
  _precondition_(_MAGIC_,CN_RECORD);
  
  vTrace("row: (%s)\n"_Stz,az());
}

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
void Record::vToCsvRecord(Row* aRow)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
  
  vAddRow(aRow);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
void Record::vAddRow(Row* aRow)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);

  //vReset();
  m_caRecord[0] = '\0';
  m_azCursor = & m_caRecord[0];

  for (i8 i_Col = 0; i_Col < m_oRowPix.m_nColumns; i_Col++) {

    eDM eDatum = (eDM)m_oRowPix.m_xaColPix[i_Col].m_uDatum; //pCol->eDatum();
    i8  iScale = m_oRowPix.m_xaColPix[i_Col].m_uScale; //pCol->iColumnScale();

    if (i_Col)
      StrLib::vAppendString(&m_azCursor,",");

    if (DM_VCH == eDatum) {
      
      if (aRow->bIsColNull(i_Col)) {
        vAppendCsvVal(eDatum,iScale,C_DATUM_NULL);
        continue;
      }
      
      Stz* acSrcString = aRow->szColVal_ofs(i_Col);
      vAppendCsvVal(eDatum,iScale,(i8)acSrcString);
      continue;
    }
    
    vAppendCsvVal(eDatum,iScale,aRow->iColVal_int(i_Col));
  }
}

/*----------------------------------------------------------------------------*/
/* add a value                                                                */
/*----------------------------------------------------------------------------*/
void Record::vAppendCsvVal(eDM eDatum, i8 iScale, i8 iVal)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
  
  if (C_DATUM_NULL == iVal) {
    StrLib::vAppendString(&m_azCursor,(char*)C_NULL);
    return;
  }
    
  switch (eDatum) {
    case DM_BCD: {
      SP_Bcd spBcd;
      StrLib::vAppendString(&m_azCursor,(char*)spBcd.sz(iVal,iScale));                 // FIX
      break;
    }
      
    case DM_BOL: {
      SP_Bol spBol;
      StrLib::vAppendString(&m_azCursor,(char*)spBol.sz(iVal));
      break;
    }
      
    case DM_DTE: {
      SP_Dte spDte;
      StrLib::vAppendString(&m_azCursor,(char*)spDte.sz(iVal));
      break;
    }
      
    case DM_FLT: {
      SP_Flt spFlt;
      StrLib::vAppendString(&m_azCursor,(char*)spFlt.sz(*(d8*)&iVal));
      break;
    }
      
    case DM_INT: {
      SP_Int spInt;
      StrLib::vAppendString(&m_azCursor,(char*)spInt.sz(iVal));
      break;
    }
      
    case DM_IVL: {
      SP_Ivl spIvl;
      StrLib::vAppendString(&m_azCursor,(char*)spIvl.sz(iVal));
      break;
    }
      
    case DM_TME: {
      SP_Tme spTme;
      StrLib::vAppendString(&m_azCursor,(char*)spTme.sz(iVal));
      break;
    }
      
    case DM_TSP: {
      SP_Tsp spTsp;
      StrLib::vAppendString(&m_azCursor,(char*)spTsp.sz(iVal));
      break;
    }
      
    case DM_VCH: {
      SP_Vch spVch;
      StrLib::vAppendString(&m_azCursor,(char*)spVch.sz((char*)iVal));
      break;
    }
      
    default:
      NEVER_REACH;
  }
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
