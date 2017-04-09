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
  Data* pData = reinterpret_cast<Data*>(pDataAddress);
  
  Heap::vFree(pData);
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
  m_eDatumType  = DM_NONE;
  m_bIsNullable = false;
  
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
  m_eDatumType  = DM_NONE;
  m_bIsNullable = false;
  
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
  
}

/*----------------------------------------------------------------------------*/
/* show column contents                                                       */
/*----------------------------------------------------------------------------*/
void Column::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
  
  vTrace("Column %02lld %s type %s datum %s %s\n"_Str,
         m_iColumnNo,
         m_sColumnName.aStr(),
         aCT[m_eColumnType],
         aDM[m_eDatumType],
         m_bIsNullable ? "nullable"_Str : "not null"_Str);
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
