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

  m_azCursor = azRecord();
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
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Record::SP_Record(StreamDef* pStreamDef)
: Sprint(CN_RECORD)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
  
  // set default enclosing char
  vFormat(F_VCH_enclose);

  m_azCursor   = azRecord();
  m_pStreamDef = pStreamDef;
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
SP_Record::SP_Record(RowSingle* pRowSingle)
: Sprint(CN_RECORD)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
  
  // set default enclosing char
  vFormat(F_VCH_enclose);
  
  m_azCursor   = azRecord();
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
SP_Record::~SP_Record(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_RECORD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a value                                                                */
/*----------------------------------------------------------------------------*/
void SP_Record::vAddCol(Column* pCol, i8 iVal)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  if (C_DATUM_NULL == iVal) {
    StrLib::vAppendString(&m_azCursor,(char*)C_NULL);
    return;
  }
  
  eDM eDatum = pCol->eDatum();
  
  switch (eDatum) {
    case DM_BCD: {
      SP_Bcd spBcd;
      StrLib::vAppendString(&m_azCursor,(char*)spBcd.sz(iVal));
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
void SP_Record::vAppendCol(Column* pCol, RowSingle* pRowSingle)
{
  Datum oDatum;
  
  i8 i_Col = pCol->iColumnNo();
  oDatum = pRowSingle->iColVal(i_Col);
  
  if (oDatum.bIsNull()) {
    StrLib::vAppendString(&m_azCursor,"null");
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
      vTrace("DM_VCH: %s\n"_Stz,pRowSingle->szColVal(i_Col));
      oDatum.vStz(pRowSingle->szColVal(i_Col));
      oDatum.vStzDump();
      
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
    StrLib::vAppendString(&m_azCursor,"null");
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
  
  SP_Bcd spBcd;
  
  StrLib::vAppendString(&m_azCursor,(char*)spBcd.sz(pDatum->iBcd(), pCol->iColumnScale()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of boolean datum to record                                    */
/*----------------------------------------------------------------------------*/
void SP_Record::vAppendBol(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Bol spBol;
  
  StrLib::vAppendString(&m_azCursor,(char*)spBol.sz(pDatum->iBol()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of date datum to record                                       */
/*----------------------------------------------------------------------------*/
void SP_Record::vAppendDte(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Dte spDte;
  
  StrLib::vAppendString(&m_azCursor,(char*)spDte.sz(pDatum->iDte()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of float datum to record                                      */
/*----------------------------------------------------------------------------*/
void SP_Record::vAppendFlt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Flt spFlt;
  StrLib::vAppendString(&m_azCursor,(char*)spFlt.sz(pDatum->dFlt()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of integer datum to record                                    */
/*----------------------------------------------------------------------------*/
void SP_Record::vAppendInt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Int spInt;
  
  StrLib::vAppendString(&m_azCursor,(char*)spInt.sz(pDatum->iInt()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of interval datum to record                                   */
/*----------------------------------------------------------------------------*/
void SP_Record::vAppendIvl(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Ivl spIvl;
  
  StrLib::vAppendString(&m_azCursor,(char*)spIvl.sz(pDatum->iIvl()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of time datum to record                                       */
/*----------------------------------------------------------------------------*/
void SP_Record::vAppendTme(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Tme spTme;
  
  StrLib::vAppendString(&m_azCursor,(char*)spTme.sz(pDatum->iTme()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of timestamp datum to record                                  */
/*----------------------------------------------------------------------------*/
void SP_Record::vAppendTsp(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Tsp spTsp;
  
  StrLib::vAppendString(&m_azCursor,(char*)spTsp.sz(pDatum->iTsp()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of varchar datum to record                                    */
/*----------------------------------------------------------------------------*/
void SP_Record::vAppendVch(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Vch spVch;
  
  StrLib::vAppendString(&m_azCursor,(char*)spVch.sz(pDatum->azChar()));
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
  _precondition_(_MAGIC_,CN_RECORD);
  
  m_caRecord[0] = '\0';
  
  m_azCursor = azRecord();
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
Stz* SP_Record::sz(Row* aRow)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vReset();

  for (i8 i_Col = 0; i_Col < m_pStreamDef->nColumns(C_ALL); i_Col++) {
    Column* pCol = m_pStreamDef->pGetColumn(i_Col);

    if (i_Col)
      StrLib::vAppendString(&m_azCursor,",");

    if (DM_VCH == pCol->eDatum()) {
      
      if (aRow->bIsColNull(i_Col)) {
        vAddCol(pCol, C_DATUM_NULL);
        continue;
      }
      
      Stz* acSrcString = aRow->szColVal_ofs(i_Col);
      vAddCol(pCol, (i8)acSrcString);
      continue;
    }
    vAddCol(pCol, aRow->iColVal_int(i_Col));
  }
  
  return (Stz*) &m_caRecord[0];
}


/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_Record::sz(RowSingle* pRowSingle)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vToRecord(pRowSingle);
  
  return (Stz*) &m_caRecord[0];
}


/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
void SP_Record::vToRecord(RowSingle* pRowSingle)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vReset();

  for (i8 i_Col = 0; i_Col < pRowSingle->m_nColumns; i_Col++) {
    
    Column* pCol = pRowSingle->m_pStreamDef->pGetColumn(i_Col);
    eDM eDatum   = pCol->eDatum();
   
    if (i_Col)
      StrLib::vAppendString(&m_azCursor,",");
    
    if (pRowSingle->bIsColNull(i_Col)) {
      StrLib::vAppendString(&m_azCursor,(char*)C_NULL);
      continue;
    }
    
    switch (eDatum) {
      case DM_BCD: {
        SP_Bcd spBcd;
        StrLib::vAppendString(&m_azCursor,
                              (char*)spBcd.sz(pRowSingle->iColVal(i_Col)));
        break;
      }
        
      case DM_BOL: {
        SP_Bol spBol;
        StrLib::vAppendString(&m_azCursor,
                              (char*)spBol.sz(pRowSingle->iColVal(i_Col)));
        break;
      }
        
      case DM_DTE: {
        SP_Dte spDte;
        StrLib::vAppendString(&m_azCursor,
                              (char*)spDte.sz(pRowSingle->iColVal(i_Col)));
        break;
      }
        
      case DM_FLT: {
        SP_Flt spFlt;
        StrLib::vAppendString(&m_azCursor,
                              (char*)spFlt.sz(pRowSingle->dColVal(i_Col)));
        break;
      }
        
      case DM_INT: {
        SP_Int spInt;
        StrLib::vAppendString(&m_azCursor,
                              (char*)spInt.sz(pRowSingle->iColVal(i_Col)));
        break;
      }
        
      case DM_IVL: {
        SP_Ivl spIvl;
        StrLib::vAppendString(&m_azCursor,
                              (char*)spIvl.sz(pRowSingle->iColVal(i_Col)));
        break;
      }
        
      case DM_TME: {
        SP_Tme spTme;
        StrLib::vAppendString(&m_azCursor,
                              (char*)spTme.sz(pRowSingle->iColVal(i_Col)));
        break;
      }
        
      case DM_TSP: {
        SP_Tsp spTsp;
        StrLib::vAppendString(&m_azCursor,
                              (char*)spTsp.sz(pRowSingle->iColVal(i_Col)));
        break;
      }
        
      case DM_VCH: {
        SP_Vch spVch;
        StrLib::vAppendString(&m_azCursor,
                              (char*)spVch.sz(pRowSingle->szColVal(i_Col)));
        break;
      }
        
      default:
        NEVER_REACH;
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_Record)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
