/*============================================================================*/
/* Column.cpp - Column                                                        */
/*============================================================================*/

#include "StorageEngine.h"
#include "MetaData.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Column* MetaData::_pColumn(void* aAddr)
{
  Column* pColumn = reinterpret_cast<Column*>(aAddr);
  
  RASSERT_TYPE(pColumn,CN_COLUMN);
 
  return pColumn;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Column::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_COLUMN);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Column::operator delete(void* pAddress)
{
  Column* pColumn = reinterpret_cast<Column*>(pAddress);
  
  HeapMem::vFree(pColumn);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Column::Column(void)
: Object(CN_COLUMN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  s32 sName = ""_Stz;
  m_iColumnNo     = -1;
  m_sColumnName   = sName;
  m_eColumnType   = CT_NONE;
  m_iColumnLength = 0;
  m_iColumnScale  = 0;
  m_eDatumType    = DM_NONE;
  m_iFirstDate    = C_UNKNOWN;
  m_bIsNullable   = true;
  m_bIsUnique     = false;
  m_bIsPartitionKey = false;
  
  m_bIsLastColumn = false;
  
  m_pColVec       = nullptr;
  
  _checkInitialised_(CN_COLUMN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Column::Column(Jobs::Job_CreateTable* pJob, i8 iColumnNo)
: Object(CN_COLUMN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_iColumnNo     = iColumnNo;
  m_sColumnName   = pJob->m_xColumns[iColumnNo].m_sColumnName;
  m_eColumnType   = pJob->m_xColumns[iColumnNo].m_eColumnType;
  m_iColumnLength = pJob->m_xColumns[iColumnNo].m_iColumnLength;
  m_iColumnScale  = pJob->m_xColumns[iColumnNo].m_iColumnScale;
  m_eDatumType    = pJob->m_xColumns[iColumnNo].m_eDatumType;
   m_iFirstDate   = pJob->m_xColumns[iColumnNo].m_iFirstDate;
  m_bIsNullable   = pJob->m_xColumns[iColumnNo].m_bIsNullable;
  m_bIsUnique     = pJob->m_xColumns[iColumnNo].m_bIsUnique;
  m_bIsPartitionKey = pJob->m_xColumns[iColumnNo].m_bIsPartitionKey;
  m_bIsLastColumn   = false;
  m_pColVec         = nullptr;

  _checkInitialised_(CN_COLUMN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Column::~Column(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
void Column::vColumnName(Stz* aColumnName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_sColumnName = aColumnName;
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
/* set column vector                                                          */
/*----------------------------------------------------------------------------*/
void Column::vColVec(ColumnVec* pColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_pColVec = pColVec;
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
/* get first date column no                                                   */
/*----------------------------------------------------------------------------*/
i8 Column::iFirstDate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  return m_iFirstDate;
}

/*----------------------------------------------------------------------------*/
/* set first date column no                                                   */
/*----------------------------------------------------------------------------*/
void Column::vFirstDate(i8 iFirstDate)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_iFirstDate = iFirstDate;
}

/*----------------------------------------------------------------------------*/
/* get is last column?                                                        */
/*----------------------------------------------------------------------------*/
bool Column::bIsLastColumn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  return m_bIsLastColumn;
}

/*----------------------------------------------------------------------------*/
/* set is last column?                                                        */
/*----------------------------------------------------------------------------*/
void Column::vIsLastColumn(bool bIsLastColumn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_bIsLastColumn = bIsLastColumn;
}

/*----------------------------------------------------------------------------*/
/* is column the first date in the column vector?                             */
/*----------------------------------------------------------------------------*/
bool Column::bIsFirstDate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  if (CT_DATE == m_eColumnType && m_iFirstDate == m_iColumnNo)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is column the given name?                                                  */
/*----------------------------------------------------------------------------*/
bool Column::bIsName(s32& sName)
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
/* get is unique?                                                             */
/*----------------------------------------------------------------------------*/
bool Column::bIsUnique(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  return m_bIsUnique;
}

/*----------------------------------------------------------------------------*/
/* set is unique?                                                           */
/*----------------------------------------------------------------------------*/
void Column::vIsUnique(bool bIsUnique)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_bIsUnique = bIsUnique;
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
/* show column contents                                                       */
/*----------------------------------------------------------------------------*/
void Column::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  vTrace("Column %02lld %s type %s length %lld scale %lld datum %s %s %s\n"_Stz,
         m_iColumnNo,
         m_sColumnName.aStz(),
         aCT[m_eColumnType],
         m_iColumnLength,
         m_iColumnScale,
         aDM[m_eDatumType],
         m_bIsNullable ? "nullable"_Stz : "not null"_Stz,
         m_bIsPartitionKey? "partitionKey"_Stz : ""_Stz);
}

/*----------------------------------------------------------------------------*/
/* column value                                                               */
/*----------------------------------------------------------------------------*/
char* Column::aValue(i8& nBytes, char** aaCursor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);

  Stz* aElem;
  char* aValue = *aaCursor;
  /*
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
  */
  
  if (DM_VCH == m_eDatumType) {
    aElem = (Stz*)aValue;
    nBytes = 0;
    
    while (*aElem->aChar() != '\0') {
      nBytes++;
      aElem++;
    }
    nBytes++;    
  }
  else
    nBytes = sizeof(i8);
  
  *aaCursor = aAdd<char*>(aValue,nBytes);
  
  return aValue;
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
