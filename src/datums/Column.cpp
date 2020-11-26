/*============================================================================*/
/* Column.cpp - Column                                                        */
/*============================================================================*/

#include "Datums.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Column* Datums::_pColumn(void* aAddr)
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
  
  m_iColumnNo     = C_UNKNOWN;
  m_sColumnNm     = C_NO_NAME;
  m_eDatum        = DM_NONE;
  
  m_eColumnType   = CT_NONE;
  m_iColumnLength = 0;
  m_iColumnScale  = 0;

  m_bIsNullable   = true;
  m_bIsUnique     = false;
  m_bIsPtnKey     = false;
  m_bIsLastColumn = false;
  
  m_eGeneration   = DG_NONE;
//  m_xMaxVal.vInit();
//  m_xMinVal.vInit();
  
  m_eFunApplied = FA_NONE;
  m_iFunColArg  = C_UNKNOWN;

  _checkInitialised_(CN_COLUMN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Column::~Column(void) noexcept(false)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
}



/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is the bcd size valid ?                                                    */
/*----------------------------------------------------------------------------*/
eDM Column::eDatum(eCT eColumnType)
{
  switch(eColumnType) {
    case CT_BOOLEAN   : return DM_BOL;
    case CT_CHAR      : return DM_VCH;
    case CT_DATE      : return DM_DTE;
    case CT_DECIMAL   : return DM_BCD;
    case CT_FLOAT     : return DM_FLT;
    case CT_INTEGER   : return DM_INT;
    case CT_INTERVAL  : return DM_IVL;
    case CT_TIME      : return DM_TME;
    case CT_TIMESTAMP : return DM_TSP;
    case CT_VARCHAR   : return DM_VCH;
    default:
      RASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* is the bcd size valid ?                                                    */
/*----------------------------------------------------------------------------*/
bool Column::bValidBcd(i8 iPrecision, i8 iScale)
{
  if (iPrecision > C_MAX_BCD_PRECISION)
    return false;
  
  if (iScale > iPrecision)
    return false;
  
  if (iScale > C_MAX_BCD_PRECISION)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* is the flt size valid ?                                                    */
/*----------------------------------------------------------------------------*/
bool Column::bValidFlt(i8 iPrecision)
{
  if (iPrecision > C_MAX_FLT_PRECISION)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* is the vch size valid ?                                                    */
/*----------------------------------------------------------------------------*/
bool Column::bValidVch(i8 iSize)
{
  if (iSize > C_MAX_VCH_PRECISION)
    return false;
  
  return true;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set column no                                                              */
/*----------------------------------------------------------------------------*/
void Column::vColumnNo(i8 iColumnNo)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_iColumnNo = iColumnNo;
}

/*----------------------------------------------------------------------------*/
/* get column length                                                          */
/*----------------------------------------------------------------------------*/
i8 Column::iColumnLength(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  return m_iColumnLength;
}

/*----------------------------------------------------------------------------*/
/* set column length                                                          */
/*----------------------------------------------------------------------------*/
void Column::vColumnLength(i8 iColumnLength)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_iColumnLength = iColumnLength;
}

/*----------------------------------------------------------------------------*/
/* get column precision                                                       */
/*----------------------------------------------------------------------------*/
i8 Column::iColumnScale(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  return m_iColumnScale;
}

/*----------------------------------------------------------------------------*/
/* set column precision                                                       */
/*----------------------------------------------------------------------------*/
void Column::vColumnScale(i8 iColumnScale)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_iColumnScale = iColumnScale;
}

/*----------------------------------------------------------------------------*/
/* get column name                                                            */
/*----------------------------------------------------------------------------*/
Stz* Column::szColNm(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  return m_sColumnNm.szStz();
}

/*----------------------------------------------------------------------------*/
/* set column name                                                            */
/*----------------------------------------------------------------------------*/
void Column::vColumnNm(Stz* aColumnName)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_sColumnNm = aColumnName;
}

/*----------------------------------------------------------------------------*/
/* get column type                                                            */
/*----------------------------------------------------------------------------*/
eCT Column::eColumnType(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  return m_eColumnType;
}

/*----------------------------------------------------------------------------*/
/* set column type                                                            */
/*----------------------------------------------------------------------------*/
void Column::vColumnType(eCT eColumnType)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_eColumnType = eColumnType;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
void Column::vCopy(Column* pCol)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_iColumnNo       = pCol->m_iColumnNo;
  m_sColumnNm       = pCol->m_sColumnNm;
  m_eColumnType     = pCol->m_eColumnType;
  m_iColumnLength   = pCol->m_iColumnLength;
  m_iColumnScale    = pCol->m_iColumnScale;
  m_eDatum          = pCol->m_eDatum;
  m_eGeneration     = pCol->m_eGeneration;
  m_bIsNullable     = pCol->m_bIsNullable;
  m_bIsUnique       = pCol->m_bIsUnique;
  m_bIsPtnKey       = pCol->m_bIsPtnKey;
  m_bIsLastColumn   = pCol->m_bIsLastColumn;
  
  //_checkInitialised_(CN_COLUMN);
  
  //_postcondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* set datum type                                                             */
/*----------------------------------------------------------------------------*/
void Column::vDatum(eDM eDatum)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_eDatum = eDatum;
}

/*----------------------------------------------------------------------------*/
/* get data generation style                                                  */
/*----------------------------------------------------------------------------*/
eDG Column::eGeneration(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  return m_eGeneration;
}

/*----------------------------------------------------------------------------*/
/* set data generation style                                                  */
/*----------------------------------------------------------------------------*/
void Column::vGeneration(eDG eGeneration)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_eGeneration = eGeneration;
}

/*----------------------------------------------------------------------------*/
/* get is last column?                                                        */
/*----------------------------------------------------------------------------*/
bool Column::bIsLastColumn(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  return m_bIsLastColumn;
}

/*----------------------------------------------------------------------------*/
/* set is last column?                                                        */
/*----------------------------------------------------------------------------*/
void Column::vIsLastColumn(bool bIsLastColumn)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_bIsLastColumn = bIsLastColumn;
}

/*----------------------------------------------------------------------------*/
/* is column the given name?                                                  */
/*----------------------------------------------------------------------------*/
bool Column::bIsName(s32& sName)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  bool b = m_sColumnNm.bSame(sName);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get is nullable?                                                           */
/*----------------------------------------------------------------------------*/
bool Column::bIsNullable(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  return m_bIsNullable;
}

/*----------------------------------------------------------------------------*/
/* set is nullable?                                                           */
/*----------------------------------------------------------------------------*/
void Column::vIsNullable(bool bIsNullable)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_bIsNullable = bIsNullable;
}

/*----------------------------------------------------------------------------*/
/* get is unique?                                                             */
/*----------------------------------------------------------------------------*/
bool Column::bIsUnique(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  return m_bIsUnique;
}

/*----------------------------------------------------------------------------*/
/* set is unique?                                                             */
/*----------------------------------------------------------------------------*/
void Column::vIsUnique(bool bIsUnique)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_bIsUnique = bIsUnique;
}

/*----------------------------------------------------------------------------*/
/* get is partition key?                                                      */
/*----------------------------------------------------------------------------*/
bool Column::bIsPtnKey(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  return m_bIsPtnKey;
}

/*----------------------------------------------------------------------------*/
/* set is partition key?                                                      */
/*----------------------------------------------------------------------------*/
void Column::vIsPtnKey(bool bIsPtnKey)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_bIsPtnKey = bIsPtnKey;
}

/*----------------------------------------------------------------------------*/
/* get m_MaxVal                                                               */
/*----------------------------------------------------------------------------*/
Datum_h* Column::pMaxVal(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);

  return & m_xMaxVal;
}

/*----------------------------------------------------------------------------*/
/* get m_MinVal                                                               */
/*----------------------------------------------------------------------------*/
Datum_h* Column::pMinVal(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  return & m_xMinVal;
}


/*----------------------------------------------------------------------------*/
/* show column contents                                                       */
/*----------------------------------------------------------------------------*/
void Column::vShow(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  vTrace("Column %02lld %s type %s length %lld scale %lld datum %s %s %s\n"_Stz,
         m_iColumnNo,
         m_sColumnNm.szStz(),
         aCT[m_eColumnType],
         m_iColumnLength,
         m_iColumnScale,
         aDM[m_eDatum],
         m_bIsNullable ? "nullable"_Stz : "not null"_Stz,
         m_bIsPtnKey? "partitionKey"_Stz : ""_Stz);
}


/*----------------------------------------------------------------------------*/
/* column value                                                               */
/*----------------------------------------------------------------------------*/
byte* Column::ayValue(i8& nBytes, byte** aayCursor)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);

  Stz* szElem;
  byte* ayValue = *aayCursor;
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
      
    
    case DM_VCH:
      aElem = (Stz*)aValue;
      nBytes = 0;
      
      while (*aElem->azChar() != '\0') {
        nBytes++;
        aElem++;
      }
      nBytes++;
      break;
      
    default:
      NEVER_REACH;
  }
  */
  
//  if (DM_VCH == m_eDatum) {
//    szElem = (Stz*)ayValue;
//    nBytes = 0;
//
//    while (*szElem->azChar() != '\0') {
//      nBytes++;
//      szElem++;
//    }
//    nBytes++;
//  }
//  else
    nBytes = sizeof(i8);
  
  *aayCursor = aAdd<byte*>(ayValue,nBytes);
  
  return ayValue;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Column::aFooter(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
