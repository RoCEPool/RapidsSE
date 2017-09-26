/*============================================================================*/
/* Column.cpp - Column                                                        */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Column* DBobjects::_pColumn(void* aAddr)
{
  Column* pColumn = reinterpret_cast<Column*>(aAddr);
  
  RASSERT( !pColumn || CN_COLUMN == pColumn->eClass() );
 
  return pColumn;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Column::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(CN_COLUMN);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Column::operator delete(void* pDataAddress)
{
  Column* pColumn = reinterpret_cast<Column*>(pDataAddress);
  
  Heap::vFree(pColumn);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Column::Column(void)
: Object(CN_COLUMN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_iColumnNo   = 0;
  m_sColumnName = "<notSet>"_Stz;
  m_eColumnType = CT_NONE;
  m_iColumnLength = 0;
  m_iColumnScale  = 0;
  m_eDatumType  = DM_NONE;
  m_bIsNullable = true;
  m_bIsPartitionKey = false;
  
  _checkInitialised_(CN_COLUMN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Column::Column(Stz* aName)
: Object(CN_COLUMN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_iColumnNo   = 0;
  m_sColumnName = aName;
  m_eColumnType = CT_INTEGER;
  m_iColumnLength = 0;
  m_iColumnScale  = 0;
  m_eDatumType  = DM_INT;
  m_bIsNullable = true;
  m_bIsPartitionKey = false;

  _checkInitialised_(CN_COLUMN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Column::Column(Stz* aName, Stz* aDefinition)
: Object(CN_COLUMN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_iColumnNo   = 0;
  m_sColumnName = aName;
  m_eColumnType = CT_NONE;
  m_iColumnLength = 0;
  m_iColumnScale  = 0;
  m_eDatumType  = DM_NONE;
  m_bIsNullable = true;
  m_bIsPartitionKey = false;

  vSet(aDefinition);
  
  _checkInitialised_(CN_COLUMN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Column::~Column(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get column no                                                              */
/*----------------------------------------------------------------------------*/
i8 Column::iColumnNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  return m_iColumnNo;
}

/*----------------------------------------------------------------------------*/
/* set column no                                                              */
/*----------------------------------------------------------------------------*/
void Column::vColumnNo(i8 iColumnNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_iColumnNo = iColumnNo;
}

/*----------------------------------------------------------------------------*/
/* get column length                                                          */
/*----------------------------------------------------------------------------*/
i8 Column::iColumnLength(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  return m_iColumnLength;
}

/*----------------------------------------------------------------------------*/
/* set column length                                                          */
/*----------------------------------------------------------------------------*/
void Column::vColumnLength(i8 iColumnLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_iColumnLength = iColumnLength;
}

/*----------------------------------------------------------------------------*/
/* get column precision                                                       */
/*----------------------------------------------------------------------------*/
i8 Column::iColumnScale(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  return m_iColumnScale;
}

/*----------------------------------------------------------------------------*/
/* set column precision                                                       */
/*----------------------------------------------------------------------------*/
void Column::vColumnScale(i8 iColumnScale)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_iColumnScale = iColumnScale;
}

/*----------------------------------------------------------------------------*/
/* get column name                                                            */
/*----------------------------------------------------------------------------*/
s32& Column::sColumnName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  return m_sColumnName;
}

/*----------------------------------------------------------------------------*/
/* set column name                                                            */
/*----------------------------------------------------------------------------*/
void Column::vColumnName(s32& sColumnName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_sColumnName = sColumnName;
}

/*----------------------------------------------------------------------------*/
/* get column type                                                            */
/*----------------------------------------------------------------------------*/
eCT Column::eColumnType(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  return m_eColumnType;
}

/*----------------------------------------------------------------------------*/
/* set column type                                                            */
/*----------------------------------------------------------------------------*/
void Column::vColumnType(eCT eColumnType)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_eColumnType = eColumnType;
}

/*----------------------------------------------------------------------------*/
/* get datum type                                                             */
/*----------------------------------------------------------------------------*/
eDM Column::eDatumType(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  return m_eDatumType;
}

/*----------------------------------------------------------------------------*/
/* set datum type                                                             */
/*----------------------------------------------------------------------------*/
void Column::vDatumType(eDM eDatumType)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_eDatumType = eDatumType;
}

/*----------------------------------------------------------------------------*/
/* is column the given name?                                                  */
/*----------------------------------------------------------------------------*/
bool Column::bIsName(s32 sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  bool b = m_sColumnName.bSame(sName);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get is nullable?                                                           */
/*----------------------------------------------------------------------------*/
bool Column::bIsNullable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  return m_bIsNullable;
}

/*----------------------------------------------------------------------------*/
/* set is nullable?                                                           */
/*----------------------------------------------------------------------------*/
void Column::vIsNullable(bool bIsNullable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_bIsNullable = bIsNullable;
}
/*----------------------------------------------------------------------------*/
/* get is partition key?                                                      */
/*----------------------------------------------------------------------------*/
bool Column::bIsPartitionKey(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  return m_bIsPartitionKey;
}

/*----------------------------------------------------------------------------*/
/* set is partition key?                                                      */
/*----------------------------------------------------------------------------*/
void Column::vIsPartitionKey(bool bIsPartitionKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_bIsPartitionKey = bIsPartitionKey;
}

/*----------------------------------------------------------------------------*/
/* parse definition and set                                                   */
/*----------------------------------------------------------------------------*/
void Column::vSet(Stz* aDefinition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  bool b;
  char aBuffer[128];
  
  char* aCursor = (char*)aDefinition;
  b = StrLib::bExtractWord(&aCursor,aBuffer);
  
  //vTrace("Type: %s\n"_Stz,aBuffer);
  
  if (StrLib::bSame(aBuffer,"boolean")) {
    m_eColumnType = CT_BOOLEAN;
    m_eDatumType = DM_BOL;
  }
  else if (StrLib::bSame(aBuffer,"char")) {
    m_eColumnType = CT_CHAR;
    m_eDatumType = DM_VCH;
    m_iColumnLength = C_64K;
  }
  else if (StrLib::bSame(aBuffer,"date")) {
    m_eColumnType = CT_DATE;
    m_eDatumType = DM_DTE;
  }
  else if (StrLib::bSame(aBuffer,"decimal")) {
    m_eColumnType = CT_DECIMAL;
    m_eDatumType = DM_BCD;
    m_iColumnLength = 18;
    m_iColumnScale = 0;
  }
  else if (StrLib::bSame(aBuffer,"float")) {
    m_eColumnType = CT_FLOAT;
    m_eDatumType = DM_FLT;
    m_iColumnLength = 16;
  }
  else if (StrLib::bSame(aBuffer,"integer")) {
    m_eColumnType = CT_INTEGER;
    m_eDatumType = DM_INT;
    m_iColumnLength = 19;
  }
  else if (StrLib::bSame(aBuffer,"interval")) {
    m_eColumnType = CT_INTERVAL;
    m_eDatumType = DM_IVL;
  }
  else if (StrLib::bSame(aBuffer,"time")) {
    m_eColumnType = CT_TIME;
    m_eDatumType = DM_TME;
  }
  else if (StrLib::bSame(aBuffer,"timestamp")) {
    m_eColumnType = CT_TIMESTAMP;
    m_eDatumType = DM_TSP;
  }
  else if (StrLib::bSame(aBuffer,"varbinary")) {
    m_eColumnType   = CT_VARBINARY;
    m_eDatumType    = DM_VBN;
    m_iColumnLength = C_64K;
  }
  else if (StrLib::bSame(aBuffer,"varchar")) {
    m_eColumnType = CT_VARCHAR;
    m_eDatumType = DM_VCH;
    m_iColumnLength = C_64K;
  }
  else {
    m_eColumnType = CT_INTEGER;
    m_eDatumType = DM_INT;
  }
  
  StrLib::vSkipWhite(&aCursor);
  
  if ('(' == *aCursor) {
    aCursor++;
    StrLib::vSkipWhite(&aCursor);
    b = StrLib::bExtractI8(&aCursor,&m_iColumnLength);
    
    StrLib::vSkipWhite(&aCursor);
    if (',' == *aCursor) {
      aCursor++;
      StrLib::vSkipWhite(&aCursor);
      b = StrLib::bExtractI8(&aCursor,&m_iColumnScale);
    }
    StrLib::vSkipWhite(&aCursor);
    aCursor++;
    StrLib::vSkipWhite(&aCursor);
  }
  
  if (StrLib::bSame(aCursor,"not null"))
    m_bIsNullable = false;
}

/*----------------------------------------------------------------------------*/
/* show column contents                                                       */
/*----------------------------------------------------------------------------*/
void Column::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  vTrace("Column %02lld %s type %s length %lld scale %lld datum %s %s\n"_Stz,
         m_iColumnNo,
         m_sColumnName.aStr(),
         aCT[m_eColumnType],
         m_iColumnLength,
         m_iColumnScale,
         aDM[m_eDatumType],
         m_bIsNullable ? "nullable"_Stz : "not null"_Stz);
}

/*----------------------------------------------------------------------------*/
/* column value                                                               */
/*----------------------------------------------------------------------------*/
char* Column::aValue(i8& nBytes, char** aaCursor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);

  Stz* aElem;
  char* aValue = *aaCursor;
  
  switch (m_eDatumType) {
      
    case DM_BOL:
      nBytes = sizeof(i8);
      break;
      
    case DM_DTE:
      nBytes = sizeof(i8);
      break;
      
    case DM_BCD:
      nBytes = sizeof(i8);
      break;
      
    case DM_FLT:
      nBytes = sizeof(d8);
      break;
      
    case DM_INT:
      nBytes = sizeof(i8);
      break;
      
    case DM_IVL:
      nBytes = sizeof(i8);
      break;
      
    case DM_TME:
      nBytes = sizeof(i8);
      break;
      
    case DM_TSP:
      nBytes = sizeof(i8);
      break;
      
    case DM_VBN:
      NEVER_REACH;
      break;
    
    case DM_VCH:
      aElem = (Stz*)aValue;
      nBytes = 0;
      
      while (*aElem->aChar() != '\0') {
        nBytes++;
        aElem++;
      }
      nBytes++;
      break;
      
    default:
      NEVER_REACH;
  }
  
  *aaCursor = pAdd<char*>(aValue,nBytes);
  
  return aValue;
}

/*----------------------------------------------------------------------------*/
/* less than                                                                  */
/*----------------------------------------------------------------------------*/
bool Column::bLessThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  bool bLessThan = DtmLib::bLessThan(m_eDatumType,aArgA,nBytesA,aArgB,nBytesB);
  
  return bLessThan;
}

/*----------------------------------------------------------------------------*/
/* less equal                                                                 */
/*----------------------------------------------------------------------------*/
bool Column::bLessEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  bool bLessEqual = DtmLib::bLessEqual(m_eDatumType,aArgA,nBytesA,aArgB,nBytesB);
  
  return bLessEqual;
}

/*----------------------------------------------------------------------------*/
/* equal to                                                                   */
/*----------------------------------------------------------------------------*/
bool Column::bEqualTo(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  bool bEqualTo = DtmLib::bEqualTo(m_eDatumType,aArgA,nBytesA,aArgB,nBytesB);
  
  return bEqualTo;
}

/*----------------------------------------------------------------------------*/
/* not equal to                                                               */
/*----------------------------------------------------------------------------*/
bool Column::bNotEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  bool bNotEqual = DtmLib::bNotEqual(m_eDatumType,aArgA,nBytesA,aArgB,nBytesB);
  
  return bNotEqual;
}

/*----------------------------------------------------------------------------*/
/* greater equal                                                              */
/*----------------------------------------------------------------------------*/
bool Column::bGreaterEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  bool bGreaterEqual = DtmLib::bGreaterEqual(m_eDatumType,aArgA,nBytesA,aArgB,nBytesB);
  
  return bGreaterEqual;
}

/*----------------------------------------------------------------------------*/
/* greater than                                                               */
/*----------------------------------------------------------------------------*/
bool Column::bGreaterThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  bool bGreaterThan = DtmLib::bGreaterThan(m_eDatumType,aArgA,nBytesA,aArgB,nBytesB);
  
  return bGreaterThan;
}

/*----------------------------------------------------------------------------*/
/* cmp                                                                        */
/*----------------------------------------------------------------------------*/
i8 Column::iCmp(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  i8 iCmp = DtmLib::iCmp(m_eDatumType,aArgA,nBytesA,aArgB,nBytesB);
  
  return iCmp;
}

/*----------------------------------------------------------------------------*/
/* cmp                                                                        */
/*----------------------------------------------------------------------------*/
//bool Column::bCmp(eRO eRelnOp, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
//  
//  switch (m_eColumnType) {
//    case CT_BOOLEAN:   NEVER_REACH;
//      
//    case CT_CHAR:      NEVER_REACH;
//      
//    case CT_DATE:      NEVER_REACH;
//      
//    case CT_DECIMAL:   NEVER_REACH;
//      
//    case CT_FLOAT:     NEVER_REACH;
//      
//    case CT_INTEGER:
//      switch(eRelnOp) {
//        case RO_LESS_THAN:    return bLessThan    (aArgA, nBytesA, aArgB, nBytesB);
//        case RO_LESS_EQUAL:   return bLessEqual   (aArgA, nBytesA, aArgB, nBytesB);
//        case RO_EQUAL:        return bEqualTo     (aArgA, nBytesA, aArgB, nBytesB);
//        case RO_NOT_EQUAL:    return bNotEqual    (aArgA, nBytesA, aArgB, nBytesB);
//        case RO_GREATER_EQUAL:return bGreaterEqual(aArgA, nBytesA, aArgB, nBytesB);
//        case RO_GREATER:      return bGreaterThan (aArgA, nBytesA, aArgB, nBytesB);
//        default: NEVER_REACH;
//      }
//      
//    case CT_INTERVAL:  NEVER_REACH;
//      
//    case CT_TIME:      NEVER_REACH;
//      
//    case CT_TIMESTAMP: NEVER_REACH;
//      
//    case CT_VARBINARY: NEVER_REACH;
//      
//    case CT_VARCHAR:
//      switch(eRelnOp) {
//        case RO_LESS_THAN:    return bLessThan    (aArgA, nBytesA, aArgB, nBytesB);
//        case RO_LESS_EQUAL:   return bLessEqual   (aArgA, nBytesA, aArgB, nBytesB);
//        case RO_EQUAL:        return bEqualTo     (aArgA, nBytesA, aArgB, nBytesB);
//        case RO_NOT_EQUAL:    return bNotEqual    (aArgA, nBytesA, aArgB, nBytesB);
//        case RO_GREATER_EQUAL:return bGreaterEqual(aArgA, nBytesA, aArgB, nBytesB);
//        case RO_GREATER:      return bGreaterThan (aArgA, nBytesA, aArgB, nBytesB);
//        default: NEVER_REACH;
//      }
//      
//    default:
//      NEVER_REACH;
//  }
//  return true;
//}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Column::bCmp(eRO eRelnOp, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  switch(eRelnOp) {
    case RO_LESS_THAN:    return bLessThan    (aArgA, nBytesA, aArgB, nBytesB);
    case RO_LESS_EQUAL:   return bLessEqual   (aArgA, nBytesA, aArgB, nBytesB);
    case RO_EQUAL_TO:     return bEqualTo     (aArgA, nBytesA, aArgB, nBytesB);
    case RO_NOT_EQUAL:    return bNotEqual    (aArgA, nBytesA, aArgB, nBytesB);
    case RO_GREATER_EQUAL:return bGreaterEqual(aArgA, nBytesA, aArgB, nBytesB);
    case RO_GREATER_THAN: return bGreaterThan (aArgA, nBytesA, aArgB, nBytesB);
    default: NEVER_REACH;
  }
  
  NEVER_REACH;
  return false;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Column::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
