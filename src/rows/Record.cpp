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
Record::Record(void)
: Object(CN_RECORD)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
  
  m_azCursor   = az();
  m_pStreamDef = nullptr;
  m_iSubSet    = C_UNKNOWN;

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
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Record::Record(StreamDef* pStreamDef, i8 iSubset)
: Object(CN_RECORD)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
  
  m_azCursor   = az();
  m_pStreamDef = pStreamDef;
  m_iSubSet    = iSubset;

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
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Record::Record(RowSingle* pRowSingle)
: Object(CN_RECORD)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
  
  // set default enclosing char
  m_azCursor   = az();
  m_pStreamDef = nullptr;
  m_iSubSet    = C_UNKNOWN;
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caRecord,sizeof(m_caRecord),CN_RECORD);
#else
  m_caRecord[0] = '\0';
#endif
  
  //! FIX RAY
  //_checkInitialised_(CN_RECORD);
  
  vToRecord(pRowSingle);
  
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

  vReset();

  for (i8 i_Col = 0; i_Col < m_pStreamDef->nColumns(C_ALL); i_Col++) {
    Column* pCol = m_pStreamDef->pGetColumn(i_Col);

    if (i_Col)
      StrLib::vAppendString(&m_azCursor,",");

    if (DM_VCH == pCol->eDatum()) {
      
      if (aRow->bIsColNull(i_Col)) {
        vAddCol_col(pCol->eDatum(),pCol->iColumnScale(),C_DATUM_NULL);
        continue;
      }
      
      Stz* acSrcString = aRow->szColVal_ofs(i_Col);
      vAddCol_col(pCol->eDatum(),pCol->iColumnScale(),(i8)acSrcString);
      continue;
    }
    
    vAddCol_col(pCol->eDatum(),pCol->iColumnScale(),
                aRow->iColVal_int(i_Col));
  }
  
  return (Stz*) &m_caRecord[0];
}


/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* Record::sz(RowSingle* pRowSingle)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);

  vToRecord(pRowSingle);
  
  return (Stz*) &m_caRecord[0];
}


/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
void Record::vToRecord(RowSingle* pRowSingle)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
  
  vReset();

  sz(pRowSingle->pRowCurrent());
  
//  for (i8 i_Col = 0; i_Col < pRowSingle->m_nColumns; i_Col++) {
//
//    Column* pCol = pRowSingle->m_pStreamDef->pGetColumn(i_Col);
//    eDM eDatum   = pCol->eDatum();
//    i8 iScale    = pCol->iColumnScale();
//
//    if (i_Col)
//      StrLib::vAppendString(&m_azCursor,",");
//
//
//    vAppendCol(eDatum,iScale,i_Col,pRowSingle);
//
//  }
  
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
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a value                                                                */
/*----------------------------------------------------------------------------*/
void Record::vAddCol_col(eDM eDatum, i8 iScale, i8 iVal)
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

/*----------------------------------------------------------------------------*/
/* append ascii of datum to record                                            */
/*----------------------------------------------------------------------------*/
void Record::vAppendCol(eDM eDatum, i8 iScale, i8 i_Col,
                        RowSingle* pRowSingle)
{
  Datum oDatum;
  
  oDatum = pRowSingle->iColVal(i_Col);
  
  if (oDatum.bIsNull()) {
    StrLib::vAppendString(&m_azCursor,"null");
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  switch (eDatum) {
    case DM_BCD: vAppendBcd(&oDatum,iScale); break;
    case DM_BOL: vAppendBol(&oDatum); break;
    case DM_DTE: vAppendDte(&oDatum); break;
    case DM_FLT: vAppendFlt(&oDatum); break;
    case DM_INT: vAppendInt(&oDatum); break;
    case DM_IVL: vAppendIvl(&oDatum); break;
    case DM_TME: vAppendTme(&oDatum); break;
    case DM_TSP: vAppendTsp(&oDatum); break;
      
    case DM_VCH:
      //vTrace("DM_VCH: %s\n"_Stz,pRowSingle->szColVal(i_Col));
      oDatum.vStz(pRowSingle->szColVal(i_Col));
      //oDatum.vStzDump();
      
      vAppendVch(&oDatum);
      break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* append ascii of decimal datum to record                                    */
/*----------------------------------------------------------------------------*/
void Record::vAppendBcd(Datum* pDatum, i8 iScale)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Bcd spBcd;
  
  StrLib::vAppendString(&m_azCursor,
                        (char*)spBcd.sz(pDatum->iBcd(),iScale));
}

/*----------------------------------------------------------------------------*/
/* append ascii of boolean datum to record                                    */
/*----------------------------------------------------------------------------*/
void Record::vAppendBol(Datum* pDatum)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Bol spBol;
  
  StrLib::vAppendString(&m_azCursor,
                        (char*)spBol.sz(pDatum->iBol()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of date datum to record                                       */
/*----------------------------------------------------------------------------*/
void Record::vAppendDte(Datum* pDatum)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Dte spDte;
  StrLib::vAppendString(&m_azCursor,
                        (char*)spDte.sz(pDatum->iDte()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of float datum to record                                      */
/*----------------------------------------------------------------------------*/
void Record::vAppendFlt(Datum* pDatum)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Flt spFlt;
  StrLib::vAppendString(&m_azCursor,
                        (char*)spFlt.sz(pDatum->dFlt()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of integer datum to record                                    */
/*----------------------------------------------------------------------------*/
void Record::vAppendInt(Datum* pDatum)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Int spInt;
  StrLib::vAppendString(&m_azCursor,
                        (char*)spInt.sz(pDatum->iInt()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of interval datum to record                                   */
/*----------------------------------------------------------------------------*/
void Record::vAppendIvl(Datum* pDatum)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Ivl spIvl;
  StrLib::vAppendString(&m_azCursor,
                        (char*)spIvl.sz(pDatum->iIvl()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of time datum to record                                       */
/*----------------------------------------------------------------------------*/
void Record::vAppendTme(Datum* pDatum)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Tme spTme;
  StrLib::vAppendString(&m_azCursor,
                        (char*)spTme.sz(pDatum->iTme()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of timestamp datum to record                                  */
/*----------------------------------------------------------------------------*/
void Record::vAppendTsp(Datum* pDatum)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Tsp spTsp;
  StrLib::vAppendString(&m_azCursor,
                        (char*)spTsp.sz(pDatum->iTsp()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of varchar datum to record                                    */
/*----------------------------------------------------------------------------*/
void Record::vAppendVch(Datum* pDatum)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Vch spVch;
  StrLib::vAppendString(&m_azCursor,
                        (char*)spVch.sz(pDatum->azChar()));
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void Record::vReset(void)
{
  _precondition_(_MAGIC_,CN_RECORD);
  
  m_caRecord[0] = '\0';
  
  m_azCursor = az();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
