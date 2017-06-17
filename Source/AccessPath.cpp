/*============================================================================*/
/* AccessPath.cpp - Access Path                                               */
/*============================================================================*/

#include "Plans.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
AccessPath* Plans::_pAccessPath(void* aAddr)
{
  AccessPath* pAccessPath = reinterpret_cast<AccessPath*>(aAddr);
  
  RASSERT( !pAccessPath || CN_ACCESSPATH == pAccessPath->eClass() );
 
  return pAccessPath;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* AccessPath::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(CN_ACCESSPATH);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void AccessPath::operator delete(void* pDataAddress)
{
  AccessPath* pAccessPath = reinterpret_cast<AccessPath*>(pDataAddress);
  
  Heap::vFree(pAccessPath);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
AccessPath::AccessPath(void)
: Object(CN_ACCESSPATH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  m_sTenantName  = "<notSet>"_Str;
  m_sCatalogName = "<notSet>"_Str;
  m_sSchemaName  = "<notSet>"_Str;
  m_sTableName   = "<notSet>"_Str;
  m_sIndexName   = "<notSet>"_Str;
  
  m_pTable     = nullptr;
  m_pPartition = nullptr;
  m_pIndex     = nullptr;
  
  m_sColumnName   = "<notSet>"_Str;
  m_eRelationalOp = RO_NONE;
  //m_xValReg
  
  m_iPathRating = 0;
  
  _checkInitialised_(CN_ACCESSPATH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
AccessPath::~AccessPath(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* initialise access path for simple test                                     */
/*----------------------------------------------------------------------------*/
void AccessPath::vInit(Interface::SEitf* pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);

  m_sTenantName   = "Boray"_Str;
  m_sCatalogName  = "RapidsSE"_Str;
  m_sSchemaName   = "public"_Str;
  m_sTableName    = "jidx"_Str;
  m_sIndexName    = "idx1"_Str;
  
  m_sColumnName   = "jidx_key"_Str;
  m_eRelationalOp = RO_EQUAL_TO;
  m_xValReg       = 10;
  
  // find ptrs to the objects...
}

/*----------------------------------------------------------------------------*/
/* get tenant name                                                            */
/*----------------------------------------------------------------------------*/
s32& AccessPath::sTenantName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  return m_sTenantName;
}

/*----------------------------------------------------------------------------*/
/* set tenant name                                                            */
/*----------------------------------------------------------------------------*/
void AccessPath::vTenantName(s32& sTenantName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  m_sTenantName = sTenantName;
}

/*----------------------------------------------------------------------------*/
/* get catalog name                                                           */
/*----------------------------------------------------------------------------*/
s32& AccessPath::sCatalogName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  return m_sCatalogName;
}

/*----------------------------------------------------------------------------*/
/* set catalog name                                                            */
/*----------------------------------------------------------------------------*/
void AccessPath::vCatalogName(s32& sCatalogName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  m_sCatalogName = sCatalogName;
}

/*----------------------------------------------------------------------------*/
/* get schema name                                                            */
/*----------------------------------------------------------------------------*/
s32& AccessPath::sSchemaName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  return m_sSchemaName;
}

/*----------------------------------------------------------------------------*/
/* set schema name                                                            */
/*----------------------------------------------------------------------------*/
void AccessPath::vSchemaName(s32& sSchemaName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  m_sSchemaName = sSchemaName;
}

/*----------------------------------------------------------------------------*/
/* get table name                                                             */
/*----------------------------------------------------------------------------*/
s32& AccessPath::sTableName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  return m_sTableName;
}

/*----------------------------------------------------------------------------*/
/* set table name                                                             */
/*----------------------------------------------------------------------------*/
void AccessPath::vTableName(s32& sTableName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  m_sTableName = sTableName;
}

/*----------------------------------------------------------------------------*/
/* get index name                                                             */
/*----------------------------------------------------------------------------*/
s32& AccessPath::sIndexName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  return m_sIndexName;
}

/*----------------------------------------------------------------------------*/
/* set index name                                                             */
/*----------------------------------------------------------------------------*/
void AccessPath::vIndexName(s32& sIndexName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  m_sIndexName = sIndexName;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* AccessPath::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void AccessPath::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  m_pTable = pTable;
}

/*----------------------------------------------------------------------------*/
/* get partition                                                              */
/*----------------------------------------------------------------------------*/
Partition* AccessPath::pPartition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  return m_pPartition;
}

/*----------------------------------------------------------------------------*/
/* set partition                                                              */
/*----------------------------------------------------------------------------*/
void AccessPath::vPartition(Partition* pPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  m_pPartition = pPartition;
}

/*----------------------------------------------------------------------------*/
/* get index                                                                  */
/*----------------------------------------------------------------------------*/
Index* AccessPath::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  return m_pIndex;
}

/*----------------------------------------------------------------------------*/
/* set index                                                                  */
/*----------------------------------------------------------------------------*/
void AccessPath::vIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  m_pIndex = pIndex;
}

/*----------------------------------------------------------------------------*/
/* get column name                                                            */
/*----------------------------------------------------------------------------*/
s32& AccessPath::sColumnName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  return m_sColumnName;
}

/*----------------------------------------------------------------------------*/
/* set column name                                                            */
/*----------------------------------------------------------------------------*/
void AccessPath::vColumnName(s32& sColumnName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  m_sColumnName = sColumnName;
}

/*----------------------------------------------------------------------------*/
/* get column                                                                 */
/*----------------------------------------------------------------------------*/
Column* AccessPath::pColumn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  return m_pColumn;
}

/*----------------------------------------------------------------------------*/
/* set column                                                                 */
/*----------------------------------------------------------------------------*/
void AccessPath::vColumn(Column* pColumn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  m_pColumn = pColumn;
}

/*----------------------------------------------------------------------------*/
/* get relational operator                                                    */
/*----------------------------------------------------------------------------*/
eRO AccessPath::eRelationalOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  return m_eRelationalOp;
}

/*----------------------------------------------------------------------------*/
/* set relational name                                                        */
/*----------------------------------------------------------------------------*/
void AccessPath::vRelationalOp(eRO eRelationalOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  m_eRelationalOp = eRelationalOp;
}

/*----------------------------------------------------------------------------*/
/* get value register                                                         */
/*----------------------------------------------------------------------------*/
ValReg& AccessPath::xValReg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  return m_xValReg;
}

/*----------------------------------------------------------------------------*/
/* set value register                                                         */
/*----------------------------------------------------------------------------*/
void AccessPath::vValReg(ValReg& xValReg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  m_xValReg = xValReg;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

_pFooter_(AccessPath)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
