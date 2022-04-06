/*============================================================================*/
/* ShardDef.cpp -- Shard Definition                                           */
/*============================================================================*/

#include "BASE.h"

#include "PtnDef.hpp"

#include "eCX.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,PtnDef,CN_PTNDEF)
__aNewFunction(PtnDef,CN_PTNDEF)
__vDeleteFunction(PtnDef,CN_PTNDEF)
__aFooterFunction(PtnDef)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PtnDef::PtnDef(void)
: Definition(CN_PTNDEF)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
  //m_sShardDefNm;
 
  m_nColumns = 0;
  //m_xaColumn[C_MAX_COLUMNS];

  //m_oRowPix;

  m_nFixedSized = 0;

  _checkInitialised_(CN_PTNDEF);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PtnDef::PtnDef(PtnDef* pPtnDef)
: Definition(CN_PTNDEF)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
  m_sShardDefNm = pPtnDef->m_sShardDefNm;
  m_nColumns = pPtnDef->m_nColumns;
    
  for (i8 i_DefCol = 0; i_DefCol < m_nColumns; i_DefCol++) {
    m_xaColumn[i_DefCol].vCopy(&pPtnDef->m_xaColumn[i_DefCol]);
  }

  //m_oRowPix;

  m_nFixedSized = pPtnDef->m_nFixedSized;

  //_checkInitialised_(CN_PTNDEF);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PtnDef::~PtnDef(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* post recv function                                                         */
/*----------------------------------------------------------------------------*/
void PtnDef::vPostRecv(void* aShardDef)
{
  _static_
  
  PtnDef* pPtnDef = _pPtnDef(aShardDef);
  
  pPtnDef->vResetPointers();
}

/*----------------------------------------------------------------------------*/
/* repair the shardDef                                                        */
/*----------------------------------------------------------------------------*/
void PtnDef::vRepair(Object* pObject)
{
  _static_

  PtnDef* pPtnDef = _pPtnDef(pObject);

  // delete the shardDef
  delete pPtnDef;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* add a column to the stream def to the current def set                      */
/*----------------------------------------------------------------------------*/
void PtnDef::vAddColumn(Stz* sColumnNm, eCT eColumnType, i8 iColumnLength, i8 iColumnScale)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
  m_xaColumn[m_nColumns].vColumnNo(m_nColumns);
  m_xaColumn[m_nColumns].vColumnNm(sColumnNm);
  m_xaColumn[m_nColumns].vColumnType(eColumnType);
  m_xaColumn[m_nColumns].vColumnLength(iColumnLength);
  m_xaColumn[m_nColumns].vColumnScale(iColumnScale);
  m_xaColumn[m_nColumns].vDatum(Column::eDatum(eColumnType));

  m_nColumns++;
}

/*----------------------------------------------------------------------------*/
/* convert streamDef to a string                                              */
/*----------------------------------------------------------------------------*/
void PtnDef::vAsString(String* pString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
  pString->vSprint("streamDef %s: ("_Stz,m_sShardDefNm.szStz());
  
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
i8 PtnDef::nColumns(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);

  return m_nColumns;
}

/*----------------------------------------------------------------------------*/
/* find column with given name                                                */
/*----------------------------------------------------------------------------*/
Column* PtnDef::pFindColumn(Stz* szColumnNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);

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
Column* PtnDef::pGetColumn(i8 iColumn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
  return &m_xaColumn[iColumn];
}

/*----------------------------------------------------------------------------*/
/* list contents                                                              */
/*----------------------------------------------------------------------------*/
void PtnDef::vListContents(i8 i_Depth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
  vTrace("%sShardDef %s\n"_Stz,Stz::szTab(i_Depth),m_sShardDefNm.szStz());

  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    Column* pColumn = pGetColumn(i_Col);

    pColumn->vListContents(i_Depth+1);
  }
}

/*----------------------------------------------------------------------------*/
/* get location                                                               */
/*----------------------------------------------------------------------------*/
Location* PtnDef::pLocation(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
  return & m_oLocation;
}

/*----------------------------------------------------------------------------*/
/* get stream name                                                            */
/*----------------------------------------------------------------------------*/
Stz* PtnDef::szShardDefNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
  return m_sShardDefNm;
}

/*----------------------------------------------------------------------------*/
/* get column at index                                                        */
/*----------------------------------------------------------------------------*/
void PtnDef::vShardDefNm(Stz* sShardDefNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
  m_sShardDefNm = sShardDefNm;
}

#define dPT()
/*----------------------------------------------------------------------------*/
/* parse Create Table command                                                 */
/*----------------------------------------------------------------------------*/
eEC PtnDef::eParse(Stz* szCmd)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
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
  m_sShardDefNm = pTkn->szChars();
  
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
/* print the stream def                                                       */
/*----------------------------------------------------------------------------*/
void PtnDef::vPrint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
  vTrace("ShardDef %s\n"_Stz,m_sShardDefNm.szStz());

  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    Column* pColumn = pGetColumn(i_Col);

    pColumn->vShow();
  }
}

/*----------------------------------------------------------------------------*/
/* convert streamDef to condensed streamPix format                            */
/*----------------------------------------------------------------------------*/
RowPix* PtnDef::pRowPix(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
  vToRowPix(&m_oRowPix,false);
  
  return & m_oRowPix;
}

/*----------------------------------------------------------------------------*/
/* reset members which are pointers (after being received)                    */
/*----------------------------------------------------------------------------*/
void PtnDef::vResetPointers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
  // no pointers need to be reset
}

/*----------------------------------------------------------------------------*/
/* convert columns of this streamDef to colPix (opt. adding a ptnhash column) */
/*----------------------------------------------------------------------------*/
void PtnDef::vToColPix(RowPix* pRowPixDst, bool bAddPtnHash)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
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
    
    StrLib::vCopy(azColNmDst,"__keyHash");
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
void PtnDef::vToRowPix(RowPix* pRowPixDst, bool bAddPtnHash)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
  if (pRowPixDst->m_bInitialised)
    return;
  
  ASSERT(! pRowPixDst->m_bPtnHash);

  pRowPixDst->m_sPixNm   = m_sShardDefNm;
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
  
  if (AM::bDeveloper()) {
    pRowPixDst->vPrint();

    vDebugPoint();
  }

}

/*----------------------------------------------------------------------------*/
/* convert streamDef to its metadat format                                    */
/*----------------------------------------------------------------------------*/
void PtnDef::vToMetadata(Stz* sMetadata)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
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

/*----------------------------------------------------------------------------*/
/* parse column                                                               */
/*----------------------------------------------------------------------------*/
eEC PtnDef::eParseColumn(TokenVec* pTknVec, i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
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
  eCT eColumn = Column::eColumn(pTkn->szChars());
  
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
eEC PtnDef::eParseColumnBoolean(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  UNUSED(pTknVec);
  
  pCol->vColumnType(CT_BOOLEAN);
  pCol->vDatum(DM_BOL);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse date column                                                          */
/*----------------------------------------------------------------------------*/
eEC PtnDef::eParseColumnDate(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  UNUSED(pTknVec);

  pCol->vColumnType(CT_DATE);
  pCol->vDatum(DM_DTE);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse decimal column                                                       */
/*----------------------------------------------------------------------------*/
eEC PtnDef::eParseColumnDecimal(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
  // set default precision and scale
  i8 iPrecision = C_MAX_BCD_PRECISION;
  i8 iScale = 0;
  
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
eEC PtnDef::eParseColumnFloat(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
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
eEC PtnDef::eParseColumnInteger(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  UNUSED(pTknVec);

  pCol->vColumnType(CT_INTEGER);
  pCol->vDatum(DM_INT);
  pCol->vColumnLength(C_MAX_INT_PRECISION);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse interval column                                                      */
/*----------------------------------------------------------------------------*/
eEC PtnDef::eParseColumnInterval(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  UNUSED(pTknVec);

  pCol->vColumnType(CT_INTERVAL_DT);
  pCol->vDatum(DM_IVL);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse time column                                                          */
/*----------------------------------------------------------------------------*/
eEC PtnDef::eParseColumnTime(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  UNUSED(pTknVec);

  pCol->vColumnType(CT_TIME);
  pCol->vDatum(DM_TME);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse timestamp column                                                     */
/*----------------------------------------------------------------------------*/
eEC PtnDef::eParseColumnTimestamp(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  UNUSED(pTknVec);

  pCol->vColumnType(CT_TIMESTAMP);
  pCol->vDatum(DM_TSP);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse varchar column                                                       */
/*----------------------------------------------------------------------------*/
eEC PtnDef::eParseColumnVarchar(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
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
eEC PtnDef::eParseColumnClauses(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
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
eEC PtnDef::eParseNotNull(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
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
eEC PtnDef::eParseTableClauses(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
  bool bSawPartition = false;
  
  eEC eError = EC_NONE;
  
  for (;;) {
    
    Token* pTkn = pTknVec->pNextToken(); dPT();
    
    if ( pTkn->bSame("rowsize"_Stz)) {
      
      eError = eParseRowSize(pTknVec);
      if (EC_NONE != eError)
        return eError;
      
      continue;
    }
    
    if ( pTkn->bSame("partition"_Stz)) {
      
      eError = eParsePartitionBy(pTknVec);
      if (EC_NONE != eError)
        return eError;
      
      bSawPartition = true;
      
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
eEC PtnDef::eParsePartitionBy(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
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
/* parse rowsize clause                                                       */
/*----------------------------------------------------------------------------*/
eEC PtnDef::eParseRowSize(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNDEF,_FOOTER_);
  
  // next token should be integer
  Token* pTkn = pTknVec->pNextToken(); dPT();
  
  // check if it was numeric
  if ( ! pTkn->bIsInteger() ) {
    return EC_INVALID_COMMAND;
  }
  
  // get size
  i8 nRowSize = pTkn->iToI8();
  
  m_nFixedSized = nRowSize;
  
  return EC_NONE;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
