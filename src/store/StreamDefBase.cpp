/*============================================================================*/
/* StreamDefBase.cpp -- Stream Definition                                     */
/*============================================================================*/

#include "BASE.h"

#include "StreamDefBase.hpp"

#include "eCX.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
StreamDefBase* STORE::_pStreamDefBase(void* aAddr)
{
  StreamDefBase* pStreamDef = reinterpret_cast<StreamDefBase*>(aAddr);

  return pStreamDef;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StreamDefBase::StreamDefBase(eCN eClassNo)
: Definition(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);

  m_bReplicated = false;
  m_sStreamDefNm = ""_Stz;

  m_nColumns = 0;
  //m_xaColumn[C_MAX_DEFCOLUMNS];

  //m_oRowPix;

  m_jRowSize = 0;
  m_jTableSize = 0;

  m_nOrders = 0;
  MemLib::vZero(m_iaOrder,sizeof(m_iaOrder),eClassNo);
  
  //_checkInitialised_(eClassNo);

  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StreamDefBase::StreamDefBase(eCN eClassNo, StreamDefBase* pStreamDefBase)
: Definition(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);

  m_bReplicated = false;
  m_sStreamDefNm = pStreamDefBase->m_sStreamDefNm;
  m_nColumns = pStreamDefBase->m_nColumns;

  for (i8 i_DefCol = 0; i_DefCol < m_nColumns; i_DefCol++) {
    m_xaColumn[i_DefCol].vCopy(&pStreamDefBase->m_xaColumn[i_DefCol]);
  }

  //m_oRowPix;

  m_jRowSize = pStreamDefBase->m_jRowSize;

  m_nOrders = 0;
  MemLib::vZero(m_iaOrder,sizeof(m_iaOrder),eClassNo);

  //_checkInitialised_(CN_STREAMDEF);

  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StreamDefBase::~StreamDefBase(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);

}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert string into column type                                            */
/*----------------------------------------------------------------------------*/
eCT StreamDefBase::eColumn(Stz* szType)
{
  _static_
  
  i8 nEntries = sizeof(aCT) / sizeof (Stz*);

  for (i8 i_Type = 0; i_Type < nEntries ; i_Type++) {

    if (aCT[i_Type]->bSame(szType))
      return (eCT)i_Type;

  }

  if (szType->bSame("interval"_Stz))
    return CT_INTERVAL_DT;

  return (eCT)CT_NONE;
}

/*----------------------------------------------------------------------------*/
/* convert string into datum type                                             */
/*----------------------------------------------------------------------------*/
eDM StreamDefBase::eDatum(Stz* szType)
{
  _static_
  
  i8 nEntries = sizeof(aDM) / sizeof (Stz*);

  for (i8 i_Type = 0; i_Type < nEntries ; i_Type++) {

    if (aDM[i_Type]->bSame(szType))
      return (eDM)i_Type;
  }

  if (szType->bSame("interval"_Stz))
    return DM_IVL;

  return (eDM)DM_UNK;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a column to the stream def to the current def set                      */
/*----------------------------------------------------------------------------*/
void StreamDefBase::vAddColumn(Stz* sColumnNm, eCT eColumnType, i8 iColumnLength, i8 iColumnScale)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_xaColumn[m_nColumns].vColumnNo(m_nColumns);
  m_xaColumn[m_nColumns].vColumnNm(sColumnNm);
  m_xaColumn[m_nColumns].vColumnType(eColumnType);
  m_xaColumn[m_nColumns].vColumnLength(iColumnLength);
  m_xaColumn[m_nColumns].vColumnScale(iColumnScale);
  m_xaColumn[m_nColumns].vDatum(Column::eDatum(eColumnType));

  m_nColumns++;
}

/*----------------------------------------------------------------------------*/
/* add a column to the stream order                                           */
/*----------------------------------------------------------------------------*/
void StreamDefBase::vAddOrder(i8 i_Column)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iaOrder[m_nOrders++] = i_Column;
}

/*----------------------------------------------------------------------------*/
/* append streamDef                                                           */
/*----------------------------------------------------------------------------*/
void StreamDefBase::vAppendStreamDef(StreamDefBase* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  for (i8 i_StreamCol = 0; i_StreamCol < pStreamDef->m_nColumns; i_StreamCol++) {
    
    m_xaColumn[m_nColumns].vCopy(&pStreamDef->m_xaColumn[i_StreamCol]);
    m_xaColumn[m_nColumns].vColumnNo(m_nColumns);
    
    m_nColumns++;
  }

  m_jRowSize += pStreamDef->m_jRowSize;

  _postcondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* append streamDef                                                           */
/*----------------------------------------------------------------------------*/
void StreamDefBase::vAppendStreamDef(StreamDefBase* pStreamDef, Stz* sColPrefix)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  for (i8 i_StreamCol = 0; i_StreamCol < pStreamDef->m_nColumns; i_StreamCol++) {
    
    m_xaColumn[m_nColumns].vCopy(&pStreamDef->m_xaColumn[i_StreamCol], sColPrefix);
    m_xaColumn[m_nColumns].vColumnNo(m_nColumns);
    
    m_nColumns++;
  }

  m_jRowSize += pStreamDef->m_jRowSize;

  _postcondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* apply the order of given streamDef                                         */
/*----------------------------------------------------------------------------*/
void StreamDefBase::vApplyOrder(StreamDefBase* pStreamDef_apply)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 nOrders = pStreamDef_apply->nOrders();
  for (i8 i_Ord = 0; i_Ord < nOrders; i_Ord++) {
    Column* pOrderColumn = pStreamDef_apply->pGetOrder(i_Ord);
    Stz* szOrderColumnNm = pOrderColumn->szColNm();

    Column* pStreamColumn = pFindColumn(szOrderColumnNm);
    if (! pStreamColumn)
      break;
    
    i8 iColNo = pStreamColumn->iColumnNo();
    vAddOrder(iColNo);
  }
}
                                
/*----------------------------------------------------------------------------*/
/* convert streamDef to a string                                              */
/*----------------------------------------------------------------------------*/
void StreamDefBase::vAsString(String* pString)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  pString->vSprint("streamDef %s: ("_Stz,m_sStreamDefNm.szStz());
  
  i8 i_DefSet = 0;
    
  if (i_DefSet != 0)
    pString->vConcat(","_Stz);

  for (i8 i_DefColumn = 0; i_DefColumn < m_nColumns; i_DefColumn++) {

    if (i_DefColumn != 0)
      pString->vConcat(","_Stz);

    pString->vCatSprint("%s %s"_Stz,
                        m_xaColumn[i_DefColumn].szColNm(),
                        aCT[m_xaColumn[i_DefColumn].eColumnType()]);
  }

  pString->vConcat(")"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get no of columns in subset of streamDef                                   */
/*----------------------------------------------------------------------------*/
i8 StreamDefBase::nColumns(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_nColumns;
}

/*----------------------------------------------------------------------------*/
/* check if column name is a duplicate in the streamDef                       */
/*----------------------------------------------------------------------------*/
bool StreamDefBase::bIsDuplicateName(Column* pColumn)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  bool bFound = false;

  for (i8 i_Column = 0; i_Column < m_nColumns; i_Column++) {

    Column* pCol = & m_xaColumn[i_Column];

    if (pCol->szColNm()->bSame(pColumn->szColNm())) {

      if (bFound)
        return true;

      bFound = true;
    }
  }
  return false;
}

/*----------------------------------------------------------------------------*/
/* find column with given name                                                */
/*----------------------------------------------------------------------------*/
Column* StreamDefBase::pFindColumn(Stz* szColumnNm)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  for (i8 i_Column = 0; i_Column < m_nColumns; i_Column++) {
    
    Column* pColumn = & m_xaColumn[i_Column];
    
    if (pColumn->szColNm()->bSame(szColumnNm))
      return pColumn;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get column at index                                                        */
/*----------------------------------------------------------------------------*/
Column* StreamDefBase::pGetColumn(i8 i_Column)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return &m_xaColumn[i_Column];
}

/*----------------------------------------------------------------------------*/
/* get order column                                                           */
/*----------------------------------------------------------------------------*/
Column* StreamDefBase::pGetOrder(i8 i_Order)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 i_Column = m_iaOrder[i_Order];
  
  return &m_xaColumn[i_Column];
}

/*----------------------------------------------------------------------------*/
/* list contents                                                              */
/*----------------------------------------------------------------------------*/
void StreamDefBase::vListContents(i8 i_Depth)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  vTrace("%sStreamDef %s\n"_Stz,Stz::szTab(i_Depth),m_sStreamDefNm.szStz());

  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    Column* pColumn = pGetColumn(i_Col);

    pColumn->vListContents(i_Depth+1);
  }
}

/*----------------------------------------------------------------------------*/
/* get location                                                               */
/*----------------------------------------------------------------------------*/
Location* StreamDefBase::pLocation(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return & m_oLocation;
}

/*----------------------------------------------------------------------------*/
/* get no of columns in stream order                                          */
/*----------------------------------------------------------------------------*/
i8 StreamDefBase::nOrders(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_nOrders;
}

/*----------------------------------------------------------------------------*/
/* parse Create Table command                                                 */
/*----------------------------------------------------------------------------*/
eEC StreamDefBase::eParseCmd(Stz* szCmd)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  eEC eError = eParse(szCmd);
  
  return eError;
}

/*----------------------------------------------------------------------------*/
/* print the stream def                                                       */
/*----------------------------------------------------------------------------*/
void StreamDefBase::vPrint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  vTrace("StreamDefBase %s\n"_Stz,m_sStreamDefNm.szStz());

  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    Column* pColumn = pGetColumn(i_Col);

    pColumn->vShow();
  }
}

/*----------------------------------------------------------------------------*/
/* get replicated                                                             */
/*----------------------------------------------------------------------------*/
bool StreamDefBase::bReplicated(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_bReplicated;
}

/*----------------------------------------------------------------------------*/
/* set replicated                                                             */
/*----------------------------------------------------------------------------*/
void StreamDefBase::vReplicated(bool bReplicated)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_bReplicated = bReplicated;
}

/*----------------------------------------------------------------------------*/
/* reset members which are pointers (after being received)                    */
/*----------------------------------------------------------------------------*/
void StreamDefBase::vResetPointers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  // no pointers need to be reset
}

/*----------------------------------------------------------------------------*/
/* convert streamDef to condensed streamPix format                            */
/*----------------------------------------------------------------------------*/
RowPix* StreamDefBase::pRowPix(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  vToRowPix(&m_oRowPix,false);
  
  return & m_oRowPix;
}

/*----------------------------------------------------------------------------*/
/* get fixed row size                                                         */
/*----------------------------------------------------------------------------*/
i8 StreamDefBase::jRowSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_jRowSize;
}

/*----------------------------------------------------------------------------*/
/* get stream name                                                            */
/*----------------------------------------------------------------------------*/
Stz* StreamDefBase::szStreamDefNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_sStreamDefNm;
}

/*----------------------------------------------------------------------------*/
/* get column at index                                                        */
/*----------------------------------------------------------------------------*/
void StreamDefBase::vStreamDefNm(Stz* sStreamDefNm)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_sStreamDefNm = sStreamDefNm;
}

/*----------------------------------------------------------------------------*/
/* get fixed table size                                                       */
/*----------------------------------------------------------------------------*/
i8 StreamDefBase::jTableSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_jTableSize;
}

/*----------------------------------------------------------------------------*/
/* convert columns of this streamDef to colPix (opt. adding a ptnhash column) */
/*----------------------------------------------------------------------------*/
void StreamDefBase::vToColPix(RowPix* pRowPixDst, bool bAddPtnHash)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  ColPix* axaColPixDst = pRowPixDst->m_xaColPix;
  
  i8 nNameSize;
  i8 nStzOfs;
  
  
  pRowPixDst->m_nNameOffset = sizeof(ColPix)
                                * ( m_nColumns + (bAddPtnHash ? 1 : 0) );
  pRowPixDst->m_nTotalChars = 0;

  char* azNames = aAdd<char*>(axaColPixDst,pRowPixDst->m_nNameOffset);
  char* azColNmDst = azNames;

  i8 i_Col;
          
  for (i_Col = 0; i_Col < m_nColumns; i_Col++) {

    Column* pColumn = &m_xaColumn[i_Col];
    
    ColPix* axColPixDst = &axaColPixDst[i_Col];

    axColPixDst->m_uNumber   = (u8)i_Col;
    // name
    axColPixDst->m_uKind     = (u8)CX_USERCOL;
    axColPixDst->m_uType     = (u8)pColumn->eColumnType();
    axColPixDst->m_uDatum    = (u8)pColumn->eDatum();
    axColPixDst->m_uLength   = (u8)pColumn->iColumnLength();
    axColPixDst->m_uScale    = (u8)pColumn->iColumnScale();
    axColPixDst->m_bNullable = (u8)pColumn->bIsNullable();
    axColPixDst->m_bPtnKey   = (u8)pColumn->bIsPtnKey();
    axColPixDst->m_uSpare    = (u8)0;
    
    StrLib::vCopy(azColNmDst,pColumn->szColNm()->azChar());
    nNameSize = StrLib::nSize(azColNmDst);

    nStzOfs = nDifference(axColPixDst,azColNmDst);
    axColPixDst->m_uName = nStzOfs;
    
    azColNmDst += nNameSize;
    pRowPixDst->m_nTotalChars += nNameSize;
  }

  // i_Col falls thru
  
  if (bAddPtnHash) {

    //pRowPixDst->vPrint();
    ASSERT(! pRowPixDst->m_nExtraCols);
    
    pRowPixDst->m_nColumns++;
    pRowPixDst->m_nExtraCols = 1;
    pRowPixDst->m_bPtnHash = true;

    ColPix* axColPixDst = &axaColPixDst[i_Col];
  
    axColPixDst->m_uNumber   = (u8)i_Col;
    // name
    axColPixDst->m_uKind     = (u8)CX_PTNHASH;
    axColPixDst->m_uType     = (u8)CT_INTEGER;
    axColPixDst->m_uDatum    = (u8)DM_INT;
    axColPixDst->m_uLength   = (u8)C_MAX_INT_PRECISION;
    axColPixDst->m_uScale    = (u8)0;
    axColPixDst->m_bNullable = (u8)false;
    axColPixDst->m_bPtnKey   = (u8)false;
    axColPixDst->m_uSpare    = (u8)0;
    
    StrLib::vCopy(azColNmDst,"__ptnHsh");
    nNameSize = StrLib::nSize(azColNmDst);

    nStzOfs = nDifference(axColPixDst,azColNmDst);
    axColPixDst->m_uName = nStzOfs;

    azColNmDst += nNameSize;
    pRowPixDst->m_nTotalChars += nNameSize;
    
    //pRowPixDst->vPrint();
  }
  
  pRowPixDst->m_nTotalChars++;
  * azColNmDst = '\0';
}

/*----------------------------------------------------------------------------*/
/* convert this streamDef to RowPix (optionally adding a ptnhash column)      */
/*----------------------------------------------------------------------------*/
void StreamDefBase::vToRowPix(RowPix* pRowPixDst, bool bAddPtnHash)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  if (pRowPixDst->m_bInitialised)
    return;
  
  ASSERT(! pRowPixDst->m_bPtnHash);

  pRowPixDst->m_sPixNm   = m_sStreamDefNm;
  pRowPixDst->m_nColumns = m_nColumns;

//  if (bAddPtnHash) {
//    ASSERT(! pRowPixDst->m_nExtraCols);
//
//    pRowPixDst->m_nColumns++;
//    pRowPixDst->m_nExtraCols = 1;
//    pRowPixDst->m_bPtnHash = true;
//  }
  
  vToColPix(pRowPixDst,bAddPtnHash);
  
  pRowPixDst->m_bInitialised = true;
  
  //if (AM::bDeveloper()) {
  //  pRowPixDst->vPrint();
  //
  //  vDebugPoint();
  //}

}

/*----------------------------------------------------------------------------*/
/* convert streamDef to its metadat format                                    */
/*----------------------------------------------------------------------------*/
void StreamDefBase::vToMetadata(Stz* sMetadata)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  sMetadata->vCopy("metadata: ("_Stz);
  
  i8 i_DefSet = 0;
    
  if (i_DefSet != 0)
    sMetadata->vConcat(","_Stz);

  for (i8 i_DefColumn = 0; i_DefColumn < m_nColumns; i_DefColumn++) {

    if (i_DefColumn != 0)
      sMetadata->vConcat(","_Stz);

    sMetadata->vCatSprint("%s %s"_Stz, m_xaColumn[i_DefColumn].szColNm(),
                          aCT[m_xaColumn[i_DefColumn].eColumnType()]);
  }

  sMetadata->vConcat(")"_Stz);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

#define dPT()
/*----------------------------------------------------------------------------*/
/* parse Create Table command                                                 */
/*----------------------------------------------------------------------------*/
eEC StreamDefBase::eParse(Stz* szCmd)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  eEC eError = EC_NONE;
  i8 i_Col = C_UNKNOWN;
  Column* pLastCol = nullptr;
  
  // retain the Create Table command
  //m_sCmd = szCmd;
  
  // tokenise the command
  TokenVec* pTknVec = TokenVec::pTokeniseSen(szCmd);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Parse: Create Table <tableNm>
  
  // -- -- -- -- -- -- -- -- -- --
  // assert we got a create table
  // -- -- -- -- -- -- -- -- -- --
  
  Token* pTkn = pTknVec->pFirstToken(); dPT();
  ASSERT(pTkn->bSame("create"_Stz));
  
  pTkn = pTknVec->pNextToken(); dPT();
  ASSERT(pTkn->bSame("table"_Stz));
  
  // -- -- -- --
  // table name
  // -- -- -- --
  
  pTkn = pTknVec->pNextToken(); dPT();
  m_sStreamDefNm = pTkn->szChars();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Parse: ( <columnDef> ,... )
  
  // -- -- -- -- --
  // open parenth (
  // -- -- -- -- --
  
  pTkn = pTknVec->pNextToken(); dPT();
  if (! pTkn->bSame("("_Stz)) {
    eError = EC_INVALID_COMMAND;
    goto L_RETURN;
  }
  
  // create the streamDef for this table def
  
  // -- -- -- -- -- -- -- -- -- -- --
  // iterate from first column till )
  // -- -- -- -- -- -- -- -- -- -- --
  
  pTkn = pTknVec->pNextToken(); dPT();
  for (i_Col = 0; ! pTkn->bSame(")"_Stz); i_Col++) {
    
    // -- -- -- -- -- -- -- --
    // exceeded column limit?
    // -- -- -- -- -- -- -- --
    if (i_Col >= C_MAX_COLUMNS) {
      eError = EC_MAX_COLUMNS;
      goto L_RETURN;
    }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // subsequent column names, skipping over comma
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    if (i_Col) {
      if (pTkn->bSame(","_Stz)) {
        pTkn = pTknVec->pNextToken(); dPT();
      }
      else {
        eError = EC_INVALID_COMMAND;
        goto L_RETURN;
      }
    }
    
    // -- -- -- -- --
    // parse column
    // -- -- -- -- --
    eError = eParseColumn(pTknVec,i_Col);
    if (EC_NONE != eError) {
      goto L_RETURN;
    }
    m_nColumns++;
    
    pTkn = pTknVec->pCurrToken(); dPT();
  }
  
  // did we get any columns ?
  if (i_Col == 0) {
    eError = EC_INVALID_COMMAND;
    goto L_RETURN;
  }
  
  // mark the last column
  pLastCol = pGetColumn(i_Col-1);
  pLastCol->vIsLastColumn(true);
  
  eError = eParseTableClauses(pTknVec);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
L_RETURN:
  
  delete pTknVec;
  
  return eError;
}

/*----------------------------------------------------------------------------*/
/* parse column                                                               */
/*----------------------------------------------------------------------------*/
eEC StreamDefBase::eParseColumn(TokenVec* pTknVec, i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  eEC eError = EC_NONE;
  
  Token* pTkn = pTknVec->pCurrToken(); dPT();
  // -- -- -- --
  
  // create column
  // -- -- -- --
  Column* pCol = pGetColumn(i_Col);
  
  // set column number
  pCol->vColumnNo(i_Col);
  
  // set column name
  pCol->vColumnNm(pTkn->szChars());
  
  // -- -- -- -- -- -- -- -- -- -- --
  // check for duplicate column name
  // -- -- -- -- -- -- -- -- -- -- --
  
  for (i8 i_Search = 0; i_Search < i_Col; i_Search++) {
    Column* pCol_search = pGetColumn(i_Search++);
    if (pCol_search->szColNm()->bSame(pCol->szColNm())) {
      eError = EC_DUPLICATE_COLUMN;
      return eError;
    }
  }
  
  // -- -- -- --
  // column type
  // -- -- -- --
  
  pTkn = pTknVec->pNextToken(); dPT();
  eCT eColumn = StreamDefBase::eColumn(pTkn->szChars());
  
  switch (eColumn) {
      
    case CT_BOOLEAN:   eError = eParseColumnBoolean   (pTknVec,pCol);  break;
    case CT_DATE:      eError = eParseColumnDate      (pTknVec,pCol);  break;
    case CT_DECIMAL:   eError = eParseColumnDecimal   (pTknVec,pCol);  break;
    case CT_FLOAT:     eError = eParseColumnFloat     (pTknVec,pCol);  break;
    case CT_INTEGER:   eError = eParseColumnInteger   (pTknVec,pCol);  break;
    case CT_INTERVAL_DT: eError = eParseColumnInterval(pTknVec,pCol);  break;
    case CT_INTERVAL_YM: NEVER_REACH;                                  break;
    case CT_TIME:      eError = eParseColumnTime      (pTknVec,pCol);  break;
    case CT_TIMESTAMP: eError = eParseColumnTimestamp (pTknVec,pCol);  break;
    case CT_VARCHAR:   eError = eParseColumnVarchar   (pTknVec,pCol);  break;
    case CT_UNKNOWN:   NEVER_REACH;                                    break;

    default:
      if (pTkn->bSame("char"_Stz) ||
          pTkn->bSame("varbinary"_Stz)) {
        eError = eParseColumnVarchar(pTknVec,pCol);
      }
      else
        eError = EC_INVALID_COMMAND;
      break;
      
  }
  
  if (EC_NONE != eError)
    return eError;
  
  eError = eParseColumnClauses(pTknVec,pCol);
  if (EC_NONE != eError)
    return eError;
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse boolean column                                                       */
/*----------------------------------------------------------------------------*/
eEC StreamDefBase::eParseColumnBoolean(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  UNUSED(pTknVec);
  
  pCol->vColumnType(CT_BOOLEAN);
  pCol->vDatum(DM_BOL);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse date column                                                          */
/*----------------------------------------------------------------------------*/
eEC StreamDefBase::eParseColumnDate(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  UNUSED(pTknVec);

  pCol->vColumnType(CT_DATE);
  pCol->vDatum(DM_DTE);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse decimal column                                                       */
/*----------------------------------------------------------------------------*/
eEC StreamDefBase::eParseColumnDecimal(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  // set default precision and scale
  i8 iPrecision = C_MAX_BCD_PRECISION;
  i8 iScale = 2;
  
  // set column type in column
  pCol->vColumnType(CT_DECIMAL);
  pCol->vDatum(DM_BCD);
  
  Token* pTkn = pTknVec->pNextToken(); dPT();
  
  // extract scale and precison if specified
  if (! pTkn->bSame("("_Stz)) {
    pCol->vColumnLength(iPrecision);
    pCol->vColumnScale(iScale);
    
    IGNORE_RETURN(pTknVec->pPrevToken());
    
    return EC_NONE;
  }
  
  pTkn = pTknVec->pNextToken(); dPT();
  
  // check if it was numeric
  if ( ! pTkn->bIsInteger() )
    return EC_INVALID_COMMAND;
  
  iPrecision = pTkn->iToI8();
  
  pTkn = pTknVec->pNextToken(); dPT();
  
  if (!pTkn->bSame(","_Stz))
    return EC_INVALID_COMMAND;
  
  pTkn = pTknVec->pNextToken(); dPT();
  
  // check if it was numeric
  if ( ! pTkn->bIsInteger() )
    return EC_INVALID_COMMAND;
  
  // get the scale
  iScale = pTkn->iToI8();
  
  pTkn = pTknVec->pNextToken(); dPT();
  if (! pTkn->bSame(")"_Stz))
    return EC_INVALID_COMMAND;

  if (iScale != 2)
    return EC_DECIMAL_NOT_SUPPORTED;

  if (! Column::bValidBcd(iPrecision, iScale))
    return EC_INVALID_COMMAND;
  
  // set scale and length in column
  pCol->vColumnLength(iPrecision);
  pCol->vColumnScale(iScale);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse float column                                                         */
/*----------------------------------------------------------------------------*/
eEC StreamDefBase::eParseColumnFloat(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  // set default size
  i8 iSize = C_MAX_FLT_PRECISION;
  
  // set column type
  pCol->vColumnType(CT_FLOAT);
  pCol->vDatum(DM_FLT);
  
  Token* pTkn = pTknVec->pNextToken(); dPT();
  
  // extract size if specified
  if (! pTkn->bSame("("_Stz)) {
    // set size in column
    pCol->vColumnLength(iSize);
    
    IGNORE_RETURN(pTknVec->pPrevToken());
    
    return EC_NONE;
  }
  
  pTkn = pTknVec->pNextToken(); dPT();
  
  // check if it was numeric
  if ( ! pTkn->bIsInteger() ) {
    return EC_INVALID_COMMAND;
  }
  
  // get size
  iSize = pTknVec->pNextToken()->iToI8();
  
  pTkn = pTknVec->pNextToken(); dPT();
  if (!pTkn->bSame(")"_Stz)) {
    return EC_INVALID_COMMAND;
  }
  
  if (! Column::bValidFlt(iSize)) {
    return EC_INVALID_COMMAND;
  }
  
  // set size in column
  pCol->vColumnLength(iSize);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse integer column                                                       */
/*----------------------------------------------------------------------------*/
eEC StreamDefBase::eParseColumnInteger(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  UNUSED(pTknVec);

  pCol->vColumnType(CT_INTEGER);
  pCol->vDatum(DM_INT);
  pCol->vColumnLength(C_MAX_INT_PRECISION);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse interval column                                                      */
/*----------------------------------------------------------------------------*/
eEC StreamDefBase::eParseColumnInterval(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  UNUSED(pTknVec);
  UNUSED(pCol);

  return EC_INTERVAL_NOT_SUPPORTED;

  //pCol->vColumnType(CT_INTERVAL_DT);
  //pCol->vDatum(DM_IVL);
  
  //return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse time column                                                          */
/*----------------------------------------------------------------------------*/
eEC StreamDefBase::eParseColumnTime(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  UNUSED(pTknVec);

  pCol->vColumnType(CT_TIME);
  pCol->vDatum(DM_TME);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse timestamp column                                                     */
/*----------------------------------------------------------------------------*/
eEC StreamDefBase::eParseColumnTimestamp(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  UNUSED(pTknVec);

  pCol->vColumnType(CT_TIMESTAMP);
  pCol->vDatum(DM_TSP);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse varchar column                                                       */
/*----------------------------------------------------------------------------*/
eEC StreamDefBase::eParseColumnVarchar(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  // set default size
  i8 iSize = C_64K;
  
  // set column type
  pCol->vColumnType(CT_VARCHAR);
  pCol->vDatum(DM_VCH);
  
  Token* pTkn = pTknVec->pNextToken(); dPT();
  
  // extract size if specified
  if (! pTkn->bSame("("_Stz)) {
    // set size in column
    pCol->vColumnLength(iSize);
    
    IGNORE_RETURN(pTknVec->pPrevToken());
    
    return EC_NONE;
  }
  
  //pCol->vColumnLength(pTknVec->pNextToken()->iToI8());
  
  pTkn = pTknVec->pNextToken(); dPT();
  
  // check if it was numeric
  if (! pTkn->bIsInteger())
    return EC_INVALID_COMMAND;
  
  // get size
  iSize = pTkn->iToI8();
  
  pTkn = pTknVec->pNextToken(); dPT();
  
  if (! pTkn->bSame(")"_Stz))
    return EC_INVALID_COMMAND;
  
  // valid the size
  if (! Column::bValidVch(iSize))
    return EC_INVALID_COMMAND;
  
  // set size in column
  pCol->vColumnLength(iSize);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse column clause                                                        */
/*----------------------------------------------------------------------------*/
eEC StreamDefBase::eParseColumnClauses(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  eEC eError = EC_NONE;
  
  for (;;) {
    
    Token* pTkn = pTknVec->pNextToken(); dPT();
        
    if (pTkn->bSame("not"_Stz)) {
      
      eError = eParseNotNull(pTknVec,pCol);
      if (EC_NONE != eError) {
        pCol->vIsNullable(false);
        return eError;
      }
      
      continue;
    }
        
    break;
  }
  
  
  //!
  //! ??? discuss with Alka
  //!
  
  // check for PRIMARY KEY
  //    if (m_xTokenVec[i_Tkn]->bSame("primary"_Stz)) {
  //
  //      pJob->pCol->m_sPrimaryKey = m_xTokenVec[i_Tkn++]->aText();
  //
  //      if (!m_xTokenVec[i_Tkn]->bSame("key"_Stz)) {
  //        m_pSession->vSetLastError(EC_INVALID_COMMAND);
  //        return false;
  //      }
  //    }
  
  // check for UNIQUE
  //    if (pTkn->bSame("unique"_Stz)) {
  //      pCol->vIsUnique(true);
  //
  //      pTkn = pTknVec->pNextToken(); dPT();
  //    }
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse not null column clause                                               */
/*----------------------------------------------------------------------------*/
eEC StreamDefBase::eParseNotNull(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Token* pTkn = pTknVec->pNextToken(); dPT();
  
  if (! pTkn->bSame(C_NULL))
    return EC_INVALID_COMMAND;
  
  pCol->vIsNullable(false);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse table clauses                                                        */
/*----------------------------------------------------------------------------*/
eEC StreamDefBase::eParseTableClauses(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  bool bSawPartition = false;
  
  eEC eError = EC_NONE;
  
  for (;;) {
    
    Token* pTkn = pTknVec->pNextToken(); dPT();
    
    if ( pTkn->bSame("partition"_Stz)) {
      
      eError = eParsePartitionBy(pTknVec);
      if (EC_NONE != eError)
        return eError;
      
      bSawPartition = true;
      
      continue;
    }
    
    if ( pTkn->bSame("replicated"_Stz)) {
      
      eError = eParseReplicated(pTknVec);
      if (EC_NONE != eError)
        return eError;
      
      continue;
    }
    
    if ( pTkn->bSame("rowsize"_Stz)) {
      
      eError = eParseRowSize(pTknVec);
      if (EC_NONE != eError)
        return eError;
      
      continue;
    }
    
    if ( pTkn->bSame("tablesize"_Stz)) {
      
      eError = eParseTableSize(pTknVec);
      if (EC_NONE != eError)
        return eError;
      
      continue;
    }
    
    break;
  }
  
  // if no partition by clause was specified
  //   we create default partitioning using the first column
  
  if (! bSawPartition)
    pGetColumn(0)->vIsPtnKey(true);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse partition clause                                                     */
/*----------------------------------------------------------------------------*/
eEC StreamDefBase::eParsePartitionBy(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  // next token should be 'by'
  Token* pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn->bSame("by"_Stz) ) {
    return EC_INVALID_COMMAND;
  }
  
  // next token should be '('
  pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn->bSame("("_Stz)) {
    return EC_INVALID_COMMAND;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the partition columns
  
  i8 iPartKeys = 0;
  bool bFound = false;
  
  pTkn = pTknVec->pNextToken(); dPT();
  while ( ! pTkn->bSame(")"_Stz) ) {
    
    iPartKeys++;
    
    if ( ! pTkn ) {
      return EC_INVALID_COMMAND;
    }
    
    if ( pTkn->bSame(";"_Stz) ) {
      return EC_INVALID_COMMAND;
    }
    
    // find column and set as partition column
    for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
      Column* pCol = pGetColumn(i_Col);
      if (pCol->szColNm()->bSame(pTkn->szChars())) {
        pCol->vIsPtnKey(true);
        bFound = true;
        break;
      }
    }
    if ( ! bFound) {
      return EC_INVALID_PARTITION_KEY;
    }
    pTkn = pTknVec->pNextToken(); dPT();
    if ( ! pTkn->bSame(","_Stz)) {
      continue;
    }
    pTkn = pTknVec->pNextToken(); dPT();
    bFound = false;
  }
  
  if ( ! iPartKeys ) {
    return EC_INVALID_COMMAND;
  }
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse replicate clasue                                                     */
/*----------------------------------------------------------------------------*/
eEC StreamDefBase::eParseReplicated(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_bReplicated = true;
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse rowsize clause                                                       */
/*----------------------------------------------------------------------------*/
eEC StreamDefBase::eParseRowSize(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  // next token should be integer
  Token* pTkn = pTknVec->pNextToken(); dPT();
  
  // check if it was numeric
  if ( ! pTkn->bIsInteger() ) {
    return EC_INVALID_COMMAND;
  }
  
  // get size
  m_jRowSize = pTkn->iToI8();
    
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse tablesize clause                                                     */
/*----------------------------------------------------------------------------*/
eEC StreamDefBase::eParseTableSize(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  // next token should be integer
  Token* pTkn = pTknVec->pNextToken(); dPT();
  
  // check if it was numeric
  if ( ! pTkn->bIsInteger() ) {
    return EC_INVALID_COMMAND;
  }
  
  // get size
  m_jTableSize = pTkn->iToI8();
    
  return EC_NONE;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
