/*============================================================================*/
/* Column.cpp -- Column                                                       */
/*============================================================================*/

#include "BASE.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,Column,CN_COLUMN)
__aNewFunction(Column,CN_COLUMN)
__vDeleteFunction(Column,CN_COLUMN)
__aFooterFunction(Column)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Column::Column(void)
: Object(CN_COLUMN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  m_iColumnNo     = C_UNKNOWN;
  m_sColumnNm     = C_NO_NAME;
  m_eColumnType   = CT_NONE;

  m_bIsUnique     = false;
  m_bIsLastColumn = false;

  m_eDatum        = DM_NONE;
  m_iColumnLength = 0;
  m_iColumnScale  = 0;
  m_bIsNullable   = true;
  m_bIsPtnKey     = false;

  m_xColPix.m_uAsU8 = 0;
  
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
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert datum to column type                                               */
/*----------------------------------------------------------------------------*/
eCT Column::eColumnType(eDM eDatum)
{
  switch(eDatum) {
    case DM_BOL     : return CT_BOOLEAN;
    case DM_VCH     : return CT_VARCHAR;
    case DM_DTE     : return CT_DATE;
    case DM_BCD     : return CT_DECIMAL;
    case DM_FLT     : return CT_FLOAT;
    case DM_INT     : return CT_INTEGER;
    case DM_IVL     : return CT_INTERVAL_DT;
    case DM_IVY     : return CT_INTERVAL_YM;
    case DM_TME     : return CT_TIME;
    case DM_TSP     : return CT_TIMESTAMP;
    case DM_UNK     : return CT_UNKNOWN;
    default:
      RASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* convert column type to datum                                                */
/*----------------------------------------------------------------------------*/
eDM Column::eDatum(eCT eColumnType)
{
  switch(eColumnType) {
    case CT_BOOLEAN     : return DM_BOL;
    case CT_CHAR        : return DM_VCH;
    case CT_DATE        : return DM_DTE;
    case CT_DECIMAL     : return DM_BCD;
    case CT_FLOAT       : return DM_FLT;
    case CT_INTEGER     : return DM_INT;
    case CT_INTERVAL_DT : return DM_IVL;
    case CT_INTERVAL_YM : NEVER_REACH;
    case CT_TIME        : return DM_TME;
    case CT_TIMESTAMP   : return DM_TSP;
    case CT_VARCHAR     : return DM_VCH;
    case CT_UNKNOWN     : return DM_UNK;
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
Stz* Column::szColNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);

  return m_sColumnNm.szStz();
}

/*----------------------------------------------------------------------------*/
/* set column name                                                            */
/*----------------------------------------------------------------------------*/
void Column::vColumnNm(Stz* aColumnName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);

  m_sColumnNm = aColumnName;
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
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
void Column::vCopy(Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);

  m_iColumnNo       = pCol->m_iColumnNo;
  m_sColumnNm       = pCol->m_sColumnNm;
  m_eColumnType     = pCol->m_eColumnType;
  m_iColumnLength   = pCol->m_iColumnLength;
  m_iColumnScale    = pCol->m_iColumnScale;
  m_eDatum          = pCol->m_eDatum;
  m_bIsNullable     = pCol->m_bIsNullable;
  m_bIsUnique       = pCol->m_bIsUnique;
  m_bIsPtnKey       = pCol->m_bIsPtnKey;
  m_bIsLastColumn   = pCol->m_bIsLastColumn;
  
  _checkInitialised_(CN_COLUMN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
void Column::vCopy(Column* pCol, Stz* sColPrefix)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);

  m_iColumnNo       = pCol->m_iColumnNo;

  m_sColumnNm       = sColPrefix;
  m_sColumnNm.vConcat(pCol->m_sColumnNm);

  m_eColumnType     = pCol->m_eColumnType;
  m_iColumnLength   = pCol->m_iColumnLength;
  m_iColumnScale    = pCol->m_iColumnScale;
  m_eDatum          = pCol->m_eDatum;
  m_bIsNullable     = pCol->m_bIsNullable;
  m_bIsUnique       = pCol->m_bIsUnique;
  m_bIsPtnKey       = pCol->m_bIsPtnKey;
  m_bIsLastColumn   = pCol->m_bIsLastColumn;

  _checkInitialised_(CN_COLUMN);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* set datum type                                                             */
/*----------------------------------------------------------------------------*/
void Column::vDatum(eDM eDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);

  m_eDatum = eDatum;
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
/* is column the given name?                                                  */
/*----------------------------------------------------------------------------*/
bool Column::bIsName(s32& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);

  bool b = m_sColumnNm.bSame(sName);
  
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
/* set is unique?                                                             */
/*----------------------------------------------------------------------------*/
void Column::vIsUnique(bool bIsUnique)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);

  m_bIsUnique = bIsUnique;
}

/*----------------------------------------------------------------------------*/
/* get is partition key?                                                      */
/*----------------------------------------------------------------------------*/
bool Column::bIsPtnKey(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);

  return m_bIsPtnKey;
}

/*----------------------------------------------------------------------------*/
/* set is partition key?                                                      */
/*----------------------------------------------------------------------------*/
void Column::vIsPtnKey(bool bIsPtnKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);

  m_bIsPtnKey = bIsPtnKey;
}

/*----------------------------------------------------------------------------*/
/* show column contents                                                       */
/*----------------------------------------------------------------------------*/
void Column::vListContents(i8 i_Depth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);

  vTrace("%sColumn %02lld %s type %s"
         " length %lld scale %lld datum %s %s %s\n"_Stz,
         Stz::szTab(i_Depth),

         m_iColumnNo,
         m_sColumnNm.szStz(),
         aCT[m_eColumnType],
         m_iColumnLength,
         m_iColumnScale,
         aDM[m_eDatum],
         m_bIsNullable ? "nullable"_Stz : "not null"_Stz,
         m_bIsPtnKey ? "partitionKey"_Stz : ""_Stz);
}

/*----------------------------------------------------------------------------*/
/* sset the column                                                            */
/*----------------------------------------------------------------------------*/
void Column::vSet(Stz* sColumnNm, eCT eColumnType, i8 iColumnLength, i8 iColumnScale) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLUMN, _FOOTER_);

  m_sColumnNm     = sColumnNm;
  m_eColumnType   = eColumnType;
  m_iColumnLength = iColumnLength;
  m_iColumnScale  = iColumnScale;

}

/*----------------------------------------------------------------------------*/
/* show column contents                                                       */
/*----------------------------------------------------------------------------*/
void Column::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);

  vTrace("Column %02lld %s type %s length %lld scale %lld datum %s %s %s\n"_Stz,
         m_iColumnNo,
         m_sColumnNm.szStz(),
         aCT[m_eColumnType],
         m_iColumnLength,
         m_iColumnScale,
         aDM[m_eDatum],
         m_bIsNullable ? "nullable"_Stz : "not null"_Stz,
         m_bIsPtnKey ? "partitionKey"_Stz : ""_Stz);
}

/*----------------------------------------------------------------------------*/
/* get type short form                                                       */
/*----------------------------------------------------------------------------*/
char Column::cTypeShort(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  switch (m_eColumnType) {
    case CT_BOOLEAN     : return 'b';
    case CT_CHAR        : return 'c';
    case CT_DATE        : return 'd';
    case CT_DECIMAL     : return 'm';
    case CT_FLOAT       : return 'f';
    case CT_INTEGER     : return 'i';
    case CT_INTERVAL_DT : return 'l';
    case CT_INTERVAL_YM : return 'y';
    case CT_TIME        : return 't';
    case CT_TIMESTAMP   : return 's';
    case CT_VARCHAR     : return 'v';
    case CT_UNKNOWN     : return 'u';
    default:
      RASSERT(0);
  }
  
  return '?';
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
