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
SP_Record::SP_Record(Row* pRow)
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
  
  vToRecord(pRow);
  
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
Stz* SP_Record::sz(Row* pRow)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vToRecord(pRow);
  
  return (Stz*) &m_caRecord[0];
}


/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
void SP_Record::vToRecord(Row* pRow)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vReset();

  for (i8 i_Col = 0; i_Col < pRow->m_nColumns; i_Col++) {
    
    Column* pCol = pRow->m_pStreamDef->pGetColumn(i_Col);
    eDM eDatum   = pCol->eDatum();
   
    if (i_Col)
      StrLib::vAppendString(&m_azCursor,",");
    
    if (pRow->m_waColVal[i_Col].bIsNull()) {
      StrLib::vAppendString(&m_azCursor,(char*)"null"_Stz);
      continue;
    }
    
    switch (eDatum) {
      case DM_BCD: {
        SP_Bcd spBcd;
        StrLib::vAppendString(&m_azCursor,
                              (char*)spBcd.sz(pRow->m_waColVal[i_Col]));
        break;
      }
        
      case DM_BOL: {
        SP_Bol spBol;
        StrLib::vAppendString(&m_azCursor,
                              (char*)spBol.sz(pRow->m_waColVal[i_Col]));
        break;
      }
        
      case DM_DTE: {
        SP_Dte spDte;
        StrLib::vAppendString(&m_azCursor,
                              (char*)spDte.sz(pRow->m_waColVal[i_Col]));
        break;
      }
        
      case DM_FLT: {
        SP_Flt spFlt;
        StrLib::vAppendString(&m_azCursor,
                              (char*)spFlt.sz(*(d8*)&pRow->m_waColVal[i_Col]));
        break;
      }
        
      case DM_INT: {
        SP_Int spInt;
        StrLib::vAppendString(&m_azCursor,
                              (char*)spInt.sz(pRow->m_waColVal[i_Col]));
        break;
      }
        
      case DM_IVL: {
        SP_Ivl spIvl;
        StrLib::vAppendString(&m_azCursor,
                              (char*)spIvl.sz(pRow->m_waColVal[i_Col]));
        break;
      }
        
      case DM_TME: {
        SP_Tme spTme;
        StrLib::vAppendString(&m_azCursor,
                              (char*)spTme.sz(pRow->m_waColVal[i_Col]));
        break;
      }
        
      case DM_TSP: {
        SP_Tsp spTsp;
        StrLib::vAppendString(&m_azCursor,
                              (char*)spTsp.sz(pRow->m_waColVal[i_Col]));
        break;
      }
        
      case DM_VCH: {
        SP_Vch spVch;
        StrLib::vAppendString(&m_azCursor,
                              (char*)spVch.sz(pRow->m_waColVal[i_Col].m_azStr));
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
