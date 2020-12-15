/*============================================================================*/
/* StreamDef.cpp -- Stream Definition                                         */
/*============================================================================*/

#include "MetaData.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
StreamDef* MetaData::_pStreamDef(void* aAddr)
{
  StreamDef* pStreamDef = reinterpret_cast<StreamDef*>(aAddr);
  
  RASSERT_TYPE(pStreamDef,CN_STREAMDEF);
  
  return pStreamDef;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* StreamDef::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_STREAMDEF);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void StreamDef::operator delete(void* aAddr)
{
  StreamDef* pStreamDef = reinterpret_cast<StreamDef*>(aAddr);
  
  HeapMem::vFree(pStreamDef);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StreamDef::StreamDef(void)
: Definition(CN_STREAMDEF)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  //m_sName;
  //m_sPath;
  
  m_iStreamId = AM::iNextStreamId();
  m_nDefSets = 0;
  
  //m_saDefName[C_MAX_STREAMDEFS];
  
  for (i8 i_StreamDefs = 0; i_StreamDefs < C_MAX_STREAMDEFS; i_StreamDefs++) {
    m_i_DefStart[i_StreamDefs] = C_UNKNOWN;
    m_nDefColumns[i_StreamDefs] = 0;
  }
  //m_xaColumn[C_MAX_DEFCOLUMNS];
  
  m_bTemp = false;
  //vTrace("StreamDef permanent %p\n"_Stz, this);
  
  m_nTotalColumns = 0;
  m_nFixedSized = 0;
  
  _checkInitialised_(CN_STREAMDEF);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StreamDef::StreamDef(StreamDef* pStreamDef)
: Definition(CN_STREAMDEF)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  m_sName = pStreamDef->m_sName;
  m_sPath = pStreamDef->m_sPath;
  
  m_iStreamId = AM::iNextStreamId();
  m_nDefSets = pStreamDef->m_nDefSets;
  
  for (i8 i_StreamDef = 0; i_StreamDef < m_nDefSets; i_StreamDef++) {
    m_nDefColumns[i_StreamDef] = pStreamDef->m_nDefColumns[i_StreamDef];
    m_i_DefStart[i_StreamDef] = pStreamDef->m_i_DefStart[i_StreamDef];
    
    for (i8 i_DefCol = 0; i_DefCol < m_nDefColumns[i_StreamDef]; i_DefCol++) {
      m_xaColumn[i_DefCol].vCopy(&pStreamDef->m_xaColumn[i_DefCol]);
    }
    
  }
  
  m_bTemp = true;
  
  m_nTotalColumns = pStreamDef->m_nTotalColumns;
  m_nFixedSized = pStreamDef->m_nFixedSized;
  
  //_checkInitialised_(CN_STREAMDEF);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StreamDef::~StreamDef(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert string into column type                                            */
/*----------------------------------------------------------------------------*/
eCT StreamDef::eColumn(Stz* szType)
{
  i8 nEntries = sizeof(aCT) / sizeof (Stz*);
  
  for (i8 i_Type = 0; i_Type < nEntries ; i_Type++) {
    
    if (aCT[i_Type]->bSame(szType))
      return (eCT)i_Type;
    
  }
  
  return (eCT)CT_NONE;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add column vec from stream def                                             */
/*----------------------------------------------------------------------------*/
void StreamDef::vAdd(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  m_nDefSets++;
  
  for (i8 i_StreamDef = 0; i_StreamDef < pStreamDef->m_nDefSets; i_StreamDef++) {
    m_nDefColumns[i_StreamDef] = m_nDefColumns[i_StreamDef];
    m_i_DefStart[i_StreamDef] = pStreamDef->m_i_DefStart[i_StreamDef];
    
    for (i8 i_DefCol = 0; i_DefCol < m_nDefColumns[i_StreamDef]; i_DefCol++) {
      m_xaColumn[i_DefCol].vCopy(&pStreamDef->m_xaColumn[i_DefCol]);
    }
    
  }
  
  m_nTotalColumns= pStreamDef->m_nTotalColumns;
  
  _checkInitialised_(CN_STREAMDEF);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* add a def set to the stream def                                            */
/*----------------------------------------------------------------------------*/
void StreamDef::vAddDefSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  m_nDefSets++;
  m_i_DefStart[m_nDefSets-1] = 0;
}

/*----------------------------------------------------------------------------*/
/* add a column to the stream def to the current def set                      */
/*----------------------------------------------------------------------------*/
void StreamDef::vAddColumn(Stz* sColumnNm, eCT eColumnType)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  //! ALKA come back to this
  i8 i_DefCol = m_nDefColumns[m_nDefSets-1];
 
  m_xaColumn[i_DefCol].vColumnNo(m_nDefColumns[i_DefCol]);
  m_xaColumn[i_DefCol].vColumnNm(sColumnNm);
  m_xaColumn[i_DefCol].vColumnType(eColumnType);
  m_xaColumn[i_DefCol].vDatum(Column::eDatum(eColumnType));
  
  m_nDefColumns[m_nDefSets-1]++;
  m_nTotalColumns++;
}

/*----------------------------------------------------------------------------*/
/* add a column to the stream def to the current def set                      */
/*----------------------------------------------------------------------------*/
void StreamDef::vAddColumn(Stz* sColumnNm, eCT eColumnType, i8 iColumnLength, i8 iColumnScale)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  //! ALKA come back to this
  i8 i_DefCol = m_nDefColumns[m_nDefSets-1];
  
  m_xaColumn[i_DefCol].vColumnNo(i_DefCol);
  m_xaColumn[i_DefCol].vColumnNm(sColumnNm);
  m_xaColumn[i_DefCol].vColumnType(eColumnType);
  m_xaColumn[i_DefCol].vColumnLength(iColumnLength);
  m_xaColumn[i_DefCol].vColumnScale(iColumnScale);
  m_xaColumn[i_DefCol].vDatum(Column::eDatum(eColumnType));
  
  m_nDefColumns[m_nDefSets-1]++;
  m_nTotalColumns++;
}

/*----------------------------------------------------------------------------*/
/* add all columns from given stream definition                               */
/*----------------------------------------------------------------------------*/
void StreamDef::vAddColumns(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  for (i8 i_StreamDef = 0; i_StreamDef < m_nDefSets; i_StreamDef++) {
    m_nDefColumns[i_StreamDef] = pStreamDef->m_nDefColumns[i_StreamDef];
    m_i_DefStart[i_StreamDef] = pStreamDef->m_i_DefStart[i_StreamDef];
    
    for (i8 i_DefCol = 0; i_DefCol < m_nDefColumns[i_StreamDef]; i_DefCol++) {
      m_xaColumn[i_DefCol].vCopy(&pStreamDef->m_xaColumn[i_DefCol]);
    }
    
  }
  m_nTotalColumns = pStreamDef->m_nTotalColumns;
  
  //m_nFixedSized = pStreamDef->m_nFixedSized;
}

/*----------------------------------------------------------------------------*/
/* convert streamDef to a string                                              */
/*----------------------------------------------------------------------------*/
void StreamDef::vAsString(String* pString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  pString->vSprint("streamDef %s: ("_Stz,m_sName.szStz());
  
  for (i8 i_DefSet = 0; i_DefSet < m_nDefSets; i_DefSet++) {
    
    if (i_DefSet != 0)
      pString->vConcat(","_Stz);
    
    for (i8 i_DefColumn = 0; i_DefColumn < m_nDefColumns[i_DefSet]; i_DefColumn++) {
        
      if (i_DefColumn != 0)
        pString->vConcat(","_Stz);
      
      pString->vCatSprint("%s %s"_Stz,
                          m_xaColumn[i_DefColumn].szColNm(),
                          aCT[m_xaColumn[i_DefColumn].eColumnType()]);
    }
  }
  pString->vConcat(")"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get no of columns in subset of streamDef                                   */
/*----------------------------------------------------------------------------*/
i8 StreamDef::nColumns(i8 iSubSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  i8 nColumns = 0;
  
  if (C_ALL == iSubSet) {
    for (i8 i_StreamDef = 0; i_StreamDef < m_nDefSets; i_StreamDef++) {
      nColumns +=  m_nDefColumns[i_StreamDef];
    }
  }
  else {
    nColumns +=  m_nDefColumns[iSubSet];
  }
  return nColumns;
  
}


/*----------------------------------------------------------------------------*/
/* get column vector at index                                                 */
/*----------------------------------------------------------------------------*/
i8 StreamDef::iFindColumn(char* sColumnNm, i8& i_ColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);

// ALKA_TO_REVIEW
//  for (i_ColVec = 0; i_ColVec < m_oColVecAry.nItems(); i_ColVec++) {
//    ColumnVec* pColumnVec = m_oColVecAry.pGet(i_ColVec);
//    i8 nColumns = pColumnVec->nItems();
//    for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
//      Column* pCol = pColumnVec->pGet(i_Col);
//      if (StrLib::bEqualTo(sColumnNm,pCol->sColumnName().azChar(),C_NO_CASE)) {
//        return i_Col;
//      }
//    }
//  }
  
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* get column at index                                                        */
/*----------------------------------------------------------------------------*/
Column* StreamDef::pGetColumn(i8 iColumn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  return &m_xaColumn[iColumn];
}

/*----------------------------------------------------------------------------*/
/* get stream name                                                            */
/*----------------------------------------------------------------------------*/
Stz* StreamDef::szName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  return m_sName;
}

/*----------------------------------------------------------------------------*/
/* get column at index                                                        */
/*----------------------------------------------------------------------------*/
void StreamDef::vName(Stz* sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  m_sName = sName;
}

#define dPT()
/*----------------------------------------------------------------------------*/
/* parse Create Table command                                                 */
/*----------------------------------------------------------------------------*/
eEC StreamDef::eParse(Stz* szCmd)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  eEC eError = EC_NONE;
  i8 i_Col = C_UNKNOWN;
  Column* pLastCol = nullptr;
  
  // retain the Create Table command
  //m_sCmd = szCmd;
  
  // tokenise the command
  TokenVec* pTknVec = TokenVec::pTokenise(szCmd);
  
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
  m_sName = pTkn->szChars();
  
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
  m_nDefSets = 1;
  m_i_DefStart[0] = 0;
  m_nDefColumns[0] = 0;
  
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
    m_nTotalColumns++;
    
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
void StreamDef::vPrint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  vTrace("StreamDef %s\n"_Stz,m_sName.szStz());
  
  i8 nCols = this->nColumns(C_ALL);
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column* pColumn = pGetColumn(i_Col);

    pColumn->vShow();
  }
}

/*----------------------------------------------------------------------------*/
/* convert streamDef to condensed streamPix format                            */
/*----------------------------------------------------------------------------*/
RowPix* StreamDef::aRowPix(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  vToRowPix(&m_oRowPix,false);
  
  return & m_oRowPix;
}

/*----------------------------------------------------------------------------*/
/* convert streamDef to colPix format                                         */
/*----------------------------------------------------------------------------*/
void StreamDef::vToColPix(ColPix* axaColPix, bool bPtnHash)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  i8 i_DefColumn = 0;
  
  for (i8 i_DefSet = 0; i_DefSet < m_nDefSets; i_DefSet++) {
        
    for ( ; i_DefColumn < m_nDefColumns[i_DefSet]; i_DefColumn++) {
      
      Column* pColumn  = &m_xaColumn[i_DefColumn];
      ColPix* axColPix = &axaColPix[i_DefColumn];
      
      axColPix->m_uDatum    = (u4)pColumn->eDatum();
      axColPix->m_uLength   = (u4)pColumn->iColumnLength();
      axColPix->m_uScale    = (u4)pColumn->iColumnScale();
      axColPix->m_bNullable = (u4)pColumn->bIsNullable();
      axColPix->m_bPtnKey   = (u4)pColumn->bIsPtnKey();
      axColPix->m_uKind     = (u4)CX_USERCOL;
      axColPix->m_uSpare    = (u4)0;
    }
  }
  
  if (bPtnHash) {

    ColPix* axColPix = &axaColPix[i_DefColumn];
  
    axColPix->m_uDatum    = (u4)DM_INT;
    axColPix->m_uLength   = (u4)C_MAX_INT_PRECISION;
    axColPix->m_uScale    = (u4)0;
    axColPix->m_bNullable = (u4)false;
    axColPix->m_bPtnKey   = (u4)false;
    axColPix->m_uKind     = (u4)CX_PTNHASH;
    axColPix->m_uSpare    = (u4)0;
  }
}

/*----------------------------------------------------------------------------*/
/* convert streamDef to condensed streamPix format                            */
/*----------------------------------------------------------------------------*/
void StreamDef::vToRowPix(RowPix* aRowPix, bool bPtnHash)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  if (aRowPix->m_bInitialised)
    return;
  
  //ASSERT(! aRowPix->m_bInitialised);
  
  aRowPix->m_nColumns = nColumns(C_ALL);
  aRowPix->m_bPtnHash = bPtnHash;
  if (aRowPix->m_bPtnHash) {
    aRowPix->m_nColumns++;
    aRowPix->m_nExtraCols = 1;
  }
  vToColPix(aRowPix->m_xaColPix,bPtnHash);
  
  aRowPix->m_bInitialised = true;
}

/*----------------------------------------------------------------------------*/
/* convert streamDef to its metadat format                                    */
/*----------------------------------------------------------------------------*/
void StreamDef::vToMetadata(Stz* sMetadata)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  sMetadata->vCopy("metadata: ("_Stz);
  
  for (i8 i_DefSet = 0; i_DefSet < m_nDefSets; i_DefSet++) {
    
    if (i_DefSet != 0)
      sMetadata->vConcat(","_Stz);
    
    for (i8 i_DefColumn = 0; i_DefColumn < m_nDefColumns[i_DefSet]; i_DefColumn++) {
        
      if (i_DefColumn != 0)
        sMetadata->vConcat(","_Stz);
      
      sMetadata->vCatSprint("%s %s"_Stz, m_xaColumn[i_DefColumn].szColNm(),
                            aCT[m_xaColumn[i_DefColumn].eColumnType()]);
    }
  }
  sMetadata->vConcat(")"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get total no of columns in streamDef                                       */
/*----------------------------------------------------------------------------*/
i8 StreamDef::nTotalColumns(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  return m_nTotalColumns;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse column                                                               */
/*----------------------------------------------------------------------------*/
eEC StreamDef::eParseColumn(TokenVec* pTknVec, i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
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
  eCT eColumn = StreamDef::eColumn(pTkn->szChars());
  
  switch (eColumn) {
      
    case CT_BOOLEAN:   eError = eParseColumnBoolean   (pTknVec,pCol);  break;
    case CT_DATE:      eError = eParseColumnDate      (pTknVec,pCol);  break;
    case CT_DECIMAL:   eError = eParseColumnDecimal   (pTknVec,pCol);  break;
    case CT_FLOAT:     eError = eParseColumnFloat     (pTknVec,pCol);  break;
    case CT_INTEGER:   eError = eParseColumnInteger   (pTknVec,pCol);  break;
    case CT_INTERVAL:  eError = eParseColumnInterval  (pTknVec,pCol);  break;
    case CT_TIME:      eError = eParseColumnTime      (pTknVec,pCol);  break;
    case CT_TIMESTAMP: eError = eParseColumnTimestamp (pTknVec,pCol);  break;
    case CT_VARCHAR:   eError = eParseColumnVarchar   (pTknVec,pCol);  break;
      
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
  
  m_nDefColumns[0]++;
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse boolean column                                                       */
/*----------------------------------------------------------------------------*/
eEC StreamDef::eParseColumnBoolean(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  pCol->vColumnType(CT_BOOLEAN);
  pCol->vDatum(DM_BOL);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse date column                                                          */
/*----------------------------------------------------------------------------*/
eEC StreamDef::eParseColumnDate(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  pCol->vColumnType(CT_DATE);
  pCol->vDatum(DM_DTE);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse decimal column                                                       */
/*----------------------------------------------------------------------------*/
eEC StreamDef::eParseColumnDecimal(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
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
  if ( ! pTkn->bIsNumeric() )
    return EC_INVALID_COMMAND;
  
  iPrecision = pTkn->iToI8();
  
  pTkn = pTknVec->pNextToken(); dPT();
  
  if (!pTkn->bSame(","_Stz))
    return EC_INVALID_COMMAND;
  
  pTkn = pTknVec->pNextToken(); dPT();
  
  // check if it was numeric
  if ( ! pTkn->bIsNumeric() )
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
eEC StreamDef::eParseColumnFloat(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
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
  if ( ! pTkn->bIsNumeric() ) {
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
eEC StreamDef::eParseColumnInteger(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  pCol->vColumnType(CT_INTEGER);
  pCol->vDatum(DM_INT);
  pCol->vColumnLength(C_MAX_INT_PRECISION);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse interval column                                                      */
/*----------------------------------------------------------------------------*/
eEC StreamDef::eParseColumnInterval(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  pCol->vColumnType(CT_INTERVAL);
  pCol->vDatum(DM_IVL);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse time column                                                          */
/*----------------------------------------------------------------------------*/
eEC StreamDef::eParseColumnTime(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  pCol->vColumnType(CT_TIME);
  pCol->vDatum(DM_TME);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse timestamp column                                                     */
/*----------------------------------------------------------------------------*/
eEC StreamDef::eParseColumnTimestamp(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  pCol->vColumnType(CT_TIMESTAMP);
  pCol->vDatum(DM_TSP);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse varchar column                                                       */
/*----------------------------------------------------------------------------*/
eEC StreamDef::eParseColumnVarchar(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
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
  if (! pTkn->bIsNumeric())
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
eEC StreamDef::eParseColumnClauses(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  eEC eError = EC_NONE;
  
  for (;;) {
    
    Token* pTkn = pTknVec->pNextToken(); dPT();
    
    if (pTkn->bSame("asc"_Stz)) {
      
      eError = eParseAsc(pTknVec,pCol);
      if (EC_NONE != eError)
        return eError;
      
      continue;
    }
    
    if (pTkn->bSame("desc"_Stz)) {
      
      eError = eParseDesc(pTknVec,pCol);
      if (EC_NONE != eError)
        return eError;
      
      continue;
    }
    
    if (pTkn->bSame("fixed"_Stz)) {
      
      eError = eParseFixed(pTknVec,pCol);
      if (EC_NONE != eError)
        return eError;
      
      continue;
    }
    
    if (pTkn->bSame("max"_Stz)) {
      
      eError = eParseMax(pTknVec,pCol);
      if (EC_NONE != eError)
        return eError;
      
      continue;
    }
    
    if (pTkn->bSame("min"_Stz)) {
      
      eError = eParseMin(pTknVec,pCol);
      if (EC_NONE != eError)
        return eError;
      
      continue;
    }
    
    if (pTkn->bSame("not"_Stz)) {
      
      eError = eParseNotNull(pTknVec,pCol);
      if (EC_NONE != eError) {
        pCol->vIsNullable(false);
        return eError;
      }
      
      continue;
    }
    
    if (pTkn->bSame("random"_Stz)) {
      
      eError = eParseRandom(pTknVec,pCol);
      if (EC_NONE != eError)
        return eError;
      
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
/* parse asc column clause                                                    */
/*----------------------------------------------------------------------------*/
eEC StreamDef::eParseAsc(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  pCol->vGeneration(DG_ASCENDING);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse desc column clause                                                   */
/*----------------------------------------------------------------------------*/
eEC StreamDef::eParseDesc(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  pCol->vGeneration(DG_DESCENDING);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse fixed column clause                                                  */
/*----------------------------------------------------------------------------*/
eEC StreamDef::eParseFixed(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  pCol->vGeneration(DG_FIXED);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse max column clause                                                    */
/*----------------------------------------------------------------------------*/
eEC StreamDef::eParseMax(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  Token* pTkn = pTknVec->pNextToken(); dPT();
  
  Datum_h* pDatum_max = pCol->pMaxVal();
  
  DatumToken::vAssign(pDatum_max,pTkn,pCol);
  
  //  if (!b)
  //    return EC_INVALID_COLUMN;
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse min column clause                                                    */
/*----------------------------------------------------------------------------*/
eEC StreamDef::eParseMin(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  Token* pTkn = pTknVec->pNextToken(); dPT();
  
  Datum_h* pDatum_min = pCol->pMinVal();
  
  DatumToken::vAssign(pDatum_min,pTkn,pCol);
  
  //  if (!b)
  //    return EC_INVALID_COLUMN;
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse not null column clause                                               */
/*----------------------------------------------------------------------------*/
eEC StreamDef::eParseNotNull(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  Token* pTkn = pTknVec->pNextToken(); dPT();
  
  if (! pTkn->bSame(C_NULL))
    return EC_INVALID_COMMAND;
  
  pCol->vIsNullable(false);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse random column clause                                                 */
/*----------------------------------------------------------------------------*/
eEC StreamDef::eParseRandom(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  pCol->vGeneration(DG_RANDOM);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse table clauses                                                        */
/*----------------------------------------------------------------------------*/
eEC StreamDef::eParseTableClauses(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
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
eEC StreamDef::eParsePartitionBy(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
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
    for (i8 i_Col = 0; i_Col < nColumns(C_ALL); i_Col++) {
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
eEC StreamDef::eParseRowSize(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  // next token should be integer
  Token* pTkn = pTknVec->pNextToken(); dPT();
  
  // check if it was numeric
  if ( ! pTkn->bIsNumeric() ) {
    return EC_INVALID_COMMAND;
  }
  
  // get size
  i8 nRowSize = pTkn->iToI8();
  
  m_nFixedSized = nRowSize;
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(StreamDef);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
