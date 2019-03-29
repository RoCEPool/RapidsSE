/*============================================================================*/
/* Record.cpp -- Record                                                       */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Record* Data::_pRecord(void* aAddr)
{
  Record* pRecord = reinterpret_cast<Record*>(aAddr);
  
  return pRecord;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Record::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_RECORD);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Record::operator delete(void* aAddr)
{
  Record* pRecord = reinterpret_cast<Record*>(aAddr);
  
  HeapMem::vFree(pRecord);
}


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Record::Record(void)
: Object(CN_RECORD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  _checkInitialised_(CN_RECORD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Record::~Record(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extract                                                                    */
/*----------------------------------------------------------------------------*/
bool Record::bExtract(Column* pColumn, Datum* pDatum, char** aaSrc,
                      bool bLastColumn, Formatters* pFormatters)
{
  bool b = false;
  eDM eDatum = pColumn->eDatumType();
  
  Stz* szDst = pDatum->m_sDatum.aStz();
  
  if (pFormatters->m_bDefault) {
    switch (eDatum) {
      case DM_BCD: b = Bcd::bExtractDefault(aaSrc,szDst,pColumn,pFormatters); break;
      case DM_BOL: b = Bol::bExtractDefault(aaSrc,szDst,pColumn,pFormatters); break;
      case DM_CHR: b = Vch::bExtractDefault(aaSrc,szDst,pColumn,pFormatters); break;
      case DM_DTE: b = Dte::bExtractDefault(aaSrc,szDst,pColumn,pFormatters); break;
      case DM_FLT: b = Flt::bExtractDefault(aaSrc,szDst,pColumn,pFormatters); break;
      case DM_INT: b = Int::bExtractDefault(aaSrc,szDst,pColumn,pFormatters); break;
      case DM_IVL: b = Ivl::bExtractDefault(aaSrc,szDst,pColumn,pFormatters); break;
      case DM_TME: b = Tme::bExtractDefault(aaSrc,szDst,pColumn,pFormatters); break;
      case DM_TSP: b = Tsp::bExtractDefault(aaSrc,szDst,pColumn,pFormatters); break;
      case DM_VCH: b = Vch::bExtractDefault(aaSrc,szDst,pColumn,pFormatters); break;
      default:
        vFatal("Record::bExtract: Invalid eDatum : %lld\n"_Stz,eDatum);
    }
  }
  else {
    switch (eDatum) {
      case DM_BCD: b = Bcd::bExtract(aaSrc,szDst,pColumn,pFormatters); break;
      case DM_BOL: b = Bol::bExtract(aaSrc,szDst,pColumn,pFormatters); break;
      case DM_CHR: b = Vch::bExtract(aaSrc,szDst,pColumn,pFormatters); break;
      case DM_DTE: b = Dte::bExtract(aaSrc,szDst,pColumn,pFormatters); break;
      case DM_FLT: b = Flt::bExtract(aaSrc,szDst,pColumn,pFormatters); break;
      case DM_INT: b = Int::bExtract(aaSrc,szDst,pColumn,pFormatters); break;
      case DM_IVL: b = Ivl::bExtract(aaSrc,szDst,pColumn,pFormatters); break;
      case DM_TME: b = Tme::bExtract(aaSrc,szDst,pColumn,pFormatters); break;
      case DM_TSP: b = Tsp::bExtract(aaSrc,szDst,pColumn,pFormatters); break;
      case DM_VCH: b = Vch::bExtract(aaSrc,szDst,pColumn,pFormatters); break;
      default:
        vFatal("Record::bExtract: Invalid eDatum : %lld\n"_Stz,eDatum);
    }
  }
  
  pDatum->vDatum((byte*)szDst->aChar());
  
  // move past the field delimiter
  //   (but not past the record terminator or record ends)
  
  if (bLastColumn) {
    // do not expect to see a field delimiter
    if ( pFormatters->m_cFieldDelimiter &&
        pFormatters->m_cRecordTerminator &&
        pFormatters->m_cFieldDelimiter != pFormatters->m_cRecordTerminator &&
        *(*aaSrc) == pFormatters->m_cFieldDelimiter ) {
      // saw a field delimiter after last field (incorrect)
      b = false;
    }
    /*
     // did not see a field delimiter (correct)
     if ( pFormatters->m_cFieldDelimiter &&
     *(*aaSrc) == pFormatters->m_cFieldDelimiter )
     (*aaSrc)++;
     */
  }
  else {
    // expect to see a field delimiter
    if ( pFormatters->m_cFieldDelimiter &&
        *(*aaSrc) == pFormatters->m_cFieldDelimiter )
      // saw a field delimiter after non-last field (correct)
      (*aaSrc)++;
    else
      // did not see a field delimiter after non-last field (incorrect)
      b = false;
  }
  
  return b;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append decimal datum to string                                             */
/*----------------------------------------------------------------------------*/
void Record::vAppendBcd(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  m_sRecStr.vCatSprint("%lld"_Stz,(i8)pDatum->wDatum());
}

/*----------------------------------------------------------------------------*/
/* append boolean datum to string                                             */
/*----------------------------------------------------------------------------*/
void Record::vAppendBol(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  m_sRecStr.vConcat(pDatum->wDatum() ? "true"_Stz : "false"_Stz);
}

/*----------------------------------------------------------------------------*/
/* append date datum to string                                                */
/*----------------------------------------------------------------------------*/
void Record::vAppendDte(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  Dte wDte;
  wDte.vAsI8(pDatum->wDatum());
  
  s32 sDte;
  wDte.vSprint(sDte.aStz());
  
  m_sRecStr.vConcat(sDte.aStz());
}

/*----------------------------------------------------------------------------*/
/* append float datum to string                                               */
/*----------------------------------------------------------------------------*/
void Record::vAppendFlt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  m_sRecStr.vCatSprint("%g"_Stz,(d8)pDatum->wDatum());
}

/*----------------------------------------------------------------------------*/
/* append integer datum to string                                             */
/*----------------------------------------------------------------------------*/
void Record::vAppendInt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  m_sRecStr.vCatSprint("%lld"_Stz,(i8)pDatum->wDatum());
}

/*----------------------------------------------------------------------------*/
/* append interval datum to string                                            */
/*----------------------------------------------------------------------------*/
void Record::vAppendIvl(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  m_sRecStr.vCatSprint("%lld"_Stz,(i8)pDatum->wDatum());
}

/*----------------------------------------------------------------------------*/
/* append time datum to string                                                */
/*----------------------------------------------------------------------------*/
void Record::vAppendTme(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  Tme wTme;
  wTme.vAsI8((i8)pDatum->wDatum());
  
  s32 sTme;
  wTme.vSprint(sTme.aStz());
  
  m_sRecStr.vConcat(sTme.aStz());
}

/*----------------------------------------------------------------------------*/
/* append timestamp datum to string                                           */
/*----------------------------------------------------------------------------*/
void Record::vAppendTsp(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  Tsp wTsp;
  wTsp.vAsI8((i8)pDatum->wDatum());
  
  s32 sTsp;
  wTsp.vSprint(sTsp.aStz());
  
  m_sRecStr.vConcat(sTsp.aStz());
}

/*----------------------------------------------------------------------------*/
/* append varchar datum to string                                             */
/*----------------------------------------------------------------------------*/
void Record::vAppendVch(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  s256 sBuf;
  
  Vch::vSprintQuotes(sBuf,pDatum->szDatum());
  m_sRecStr.vConcat(sBuf.aStz());
}


/*----------------------------------------------------------------------------*/
/* get record string                                                          */
/*----------------------------------------------------------------------------*/
Stz* Record::szRecStr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD);
  
  return m_sRecStr.aStz();
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Record::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
