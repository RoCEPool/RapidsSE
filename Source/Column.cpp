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
  m_sColumnName = "<notSet>"_Str;
  m_eColumnType = CT_NONE;
  m_iColumnLength = 0;
  m_iColumnPrecision = 0;
  m_eDatumType  = DM_NONE;
  m_bIsNullable = true;
  
  _checkInitialised_(CN_COLUMN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Column::Column(Str* aName)
: Object(CN_COLUMN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_iColumnNo   = 0;
  m_sColumnName = aName;
  m_eColumnType = CT_INTEGER;
  m_iColumnLength = 0;
  m_iColumnPrecision = 0;
  m_eDatumType  = DM_INT;
  m_bIsNullable = true;
  
  _checkInitialised_(CN_COLUMN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Column::Column(Str* aName, Str* aDefinition)
: Object(CN_COLUMN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_iColumnNo   = 0;
  m_sColumnName = aName;
  m_eColumnType = CT_NONE;
  m_iColumnLength = 0;
  m_iColumnPrecision = 0;
  m_eDatumType  = DM_NONE;
  m_bIsNullable = true;
  
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
i8 Column::iColumnPrecision(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  return m_iColumnPrecision;
}

/*----------------------------------------------------------------------------*/
/* set column precision                                                       */
/*----------------------------------------------------------------------------*/
void Column::vColumnPrecision(i8 iColumnPrecision)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_iColumnPrecision = iColumnPrecision;
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
/* parse definition and set                                                   */
/*----------------------------------------------------------------------------*/
void Column::vSet(Str* aDefinition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  bool b;
  char aBuffer[128];
  
  char* aCursor = (char*)aDefinition;
  b = StrLib::bExtractWord(&aCursor,aBuffer);
  
  //vTrace("Type: %s\n"_Str,aBuffer);
  
  if (StrLib::bSame(aBuffer,"boolean")) {
    m_eColumnType = CT_BOOLEAN;
    m_eDatumType = DM_BOL;
  }
  else if (StrLib::bSame(aBuffer,"char")) {
    m_eColumnType = CT_CHAR;
    m_eDatumType = DM_CHR;
  }
  else if (StrLib::bSame(aBuffer,"date")) {
    m_eColumnType = CT_DATE;
    m_eDatumType = DM_DTE;
  }
  else if (StrLib::bSame(aBuffer,"decimal")) {
    m_eColumnType = CT_DECIMAL;
    m_eDatumType = DM_BCD;
  }
  else if (StrLib::bSame(aBuffer,"float")) {
    m_eColumnType = CT_FLOAT;
    m_eDatumType = DM_FLT;
  }
  else if (StrLib::bSame(aBuffer,"integer")) {
    m_eColumnType = CT_INTEGER;
    m_eDatumType = DM_INT;
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
    m_eColumnType = CT_VARBINARY;
    m_eDatumType = DM_VBN;
  }
  else if (StrLib::bSame(aBuffer,"varchar")) {
    m_eColumnType = CT_VARCHAR;
    m_eDatumType = DM_VCH;
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
      b = StrLib::bExtractI8(&aCursor,&m_iColumnPrecision);
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
  
  vTrace("Column %02lld %s type %s length %lld precision %lld datum %s %s\n"_Str,
         m_iColumnNo,
         m_sColumnName.aStr(),
         aCT[m_eColumnType],
         m_iColumnLength,
         m_iColumnPrecision,
         aDM[m_eDatumType],
         m_bIsNullable ? "nullable"_Str : "not null"_Str);
}

/*----------------------------------------------------------------------------*/
/* validate column field                                                      */
/*----------------------------------------------------------------------------*/
bool Column::bValidateField(Str** aaSubRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  char* aSubRow = (char*)*aaSubRow;

  while (*aSubRow && '|' != *aSubRow)
    aSubRow++;
  
  *aaSubRow = (Str*)aSubRow;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* column value                                                               */
/*----------------------------------------------------------------------------*/
char* Column::aValue(i8& nBytes, char** aaCursor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);

  char* aValue = *aaCursor;
  nBytes = sizeof(i8);
  
  *aaCursor = pAdd<char*>(aValue,nBytes);
    
  return aValue;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Column::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
