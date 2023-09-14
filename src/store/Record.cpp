/*============================================================================*/
/* Record.cpp - Record sprint helper                                          */
/*============================================================================*/

#include "BASE.h"

#include "Row.hpp"

#include "Record.hpp"

#include "SP_Bcd.hpp"
#include "SP_Bol.hpp"
#include "SP_Dte.hpp"
#include "SP_Flt.hpp"
#include "SP_Int.hpp"
#include "SP_Ivd.hpp"
#include "SP_Ivy.hpp"
#include "SP_Tme.hpp"
#include "SP_Tsp.hpp"
#include "SP_Vch.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Record,CN_RECORD)
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
  
  ASSERT(pRowPix->m_nColumns);
  
  m_pRowPix = pRowPix;
    
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
Stz* Record::sz(Row* aRow, bool bNewLine)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);

  vAddRow(aRow);

  if (bNewLine)
    StrLib::vAppendString(&m_azCursor, azRecordLastChar(), "\n");

  return (Stz*) &m_caRecord[0];
}

/*----------------------------------------------------------------------------*/
/* return size of the string buffer size                                      */
/*----------------------------------------------------------------------------*/
i8 Record::iStrBufSize(void)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);

  return sizeof(m_caRecord);
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
void Record::vPrintAsRow(Stz* szLabel)
{
  _precondition_(_MAGIC_,CN_RECORD);
  
  if (szLabel)
    vTrace("%s (%s)\n"_Stz,szLabel,az());
  else
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
/* write record as csv to file                                                */
/*----------------------------------------------------------------------------*/
void Record::vWriteAsCsv(File* pFile)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
 
  pFile->vPrint("%s\n"_Stz,az());
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

  Bcd xBcd;

  //vBeginEach_forward();
  m_caRecord[0] = '\0';
  m_azCursor = & m_caRecord[0];
  
  for (i8 i_Col = 0;
       i_Col < m_pRowPix->m_nColumns - m_pRowPix->m_nExtraCols;
       i_Col++) {

    eDM eDatum = (eDM)m_pRowPix->m_xaColPix[i_Col].m_uDatum; //pCol->eDatum();
    //i8  iScale = m_pRowPix->m_xaColPix[i_Col].m_uScale; //pCol->iColumnScale();


    if (i_Col)
      StrLib::vAppendString(&m_azCursor, azRecordLastChar(), ",");

    if (DM_VCH == eDatum) {
      
      if (aRow->bIsColNull(i_Col)) {
        vAppendCsvVal(eDatum,0,C_DATUM_NULL);
        continue;
      }
      
      Stz* acSrcString = aRow->szColVal_ofs(i_Col);
      vAppendCsvVal(eDatum,0,(i8)acSrcString);
      continue;
    }

    if (eDatum == DM_BCD) {

      if (aRow->bIsColNull(i_Col)) {
        vAppendCsvVal(eDatum,0,C_DATUM_NULL);
        continue;
      }

      xBcd = aRow->iColVal_int(i_Col);
      vAppendCsvVal(eDatum,xBcd.T.m_iScale,xBcd.T.m_iMantissa);
      continue;
    }

    vAppendCsvVal(eDatum,0,aRow->iColVal_int(i_Col));
  }
}

/*----------------------------------------------------------------------------*/
/* add a value                                                                */
/*----------------------------------------------------------------------------*/
void Record::vAppendCsvVal(eDM eDatum, i8 iScale, i8 iVal)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);

  const char* azLastChar = azRecordLastChar();

  if (C_DATUM_NULL == iVal) {
    StrLib::vAppendString(&m_azCursor, azLastChar, C_NULL->azChar());
    return;
  }
    
  switch (eDatum) {
    case DM_BCD: {
      SP_Bcd spBcd;
      StrLib::vAppendString(&m_azCursor, azLastChar, (char*)spBcd.sz(iVal,iScale));                 // FIX
      break;
    }
      
    case DM_BOL: {
      SP_Bol spBol;
      StrLib::vAppendString(&m_azCursor, azLastChar, (char*)spBol.sz(iVal));
      break;
    }
      
    case DM_DTE: {
      SP_Dte spDte;
      StrLib::vAppendString(&m_azCursor, azLastChar, (char*)spDte.sz(iVal));
      break;
    }
      
    case DM_FLT: {
      SP_Flt spFlt;
      d8* pdVal = (d8*) &iVal;
      StrLib::vAppendString(&m_azCursor, azLastChar, (char*)spFlt.sz(*pdVal));
      break;
    }
      
    case DM_INT: {
      SP_Int spInt;
      StrLib::vAppendString(&m_azCursor, azLastChar, (char*)spInt.sz(iVal));
      break;
    }
      
    case DM_IVD: {
      SP_Ivd spIvd;
      StrLib::vAppendString(&m_azCursor, azLastChar, (char*)spIvd.sz(iVal));
      break;
    }

    case DM_IVY: {
      SP_Ivy spIvy;
      StrLib::vAppendString(&m_azCursor, azLastChar, (char*)spIvy.sz(iVal));
      break;
    }

    case DM_TME: {
      SP_Tme spTme;
      StrLib::vAppendString(&m_azCursor, azLastChar, (char*)spTme.sz(iVal));
      break;
    }
      
    case DM_TSP: {
      SP_Tsp spTsp;
      StrLib::vAppendString(&m_azCursor, azLastChar, (char*)spTsp.sz(iVal));
      break;
    }
      
    case DM_VCH: {
      SP_Vch spVch;
      StrLib::vAppendString(&m_azCursor, azLastChar, (char*)spVch.sz((char*)iVal));
      break;
    }
      
    case DM_UNK: {
      NEVER_REACH;
      break;
    }
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* Return a pointer to the last valid char in the m_caRecord char array       */
/*----------------------------------------------------------------------------*/
const char* Record::azRecordLastChar(void)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);

  return m_caRecord + sizeof(m_caRecord) - 1;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
