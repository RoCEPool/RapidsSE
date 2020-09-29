/*============================================================================*/
/* TableRef.cpp - a named table                                               */
/*============================================================================*/

#include "Parser.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Parser,TableRef,CN_TABLEREF)
__aNewFunction(TableRef,CN_TABLEREF)
__vDeleteFunction(TableRef,CN_TABLEREF)
__aFooterFunction(TableRef)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TableRef::TableRef(Text* pCatalogName, Text* pSchemaName, Text* pTableName)
: Object(CN_TABLEREF),
    m_pCatalogName(pCatalogName),
    m_pSchemaName(pSchemaName),
    m_pTableName(pTableName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEREF,_FOOTER_);
  
  _checkInitialised_(CN_TABLEREF);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TABLEREF,_FOOTER_);
  
  ASSERT(m_pTableName != nullptr);
  ASSERT(m_pCatalogName == nullptr || m_pSchemaName != nullptr);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TableRef::~TableRef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEREF,_FOOTER_);
  
  if (m_pCatalogName)    delete m_pCatalogName;
  if (m_pSchemaName)     delete m_pSchemaName;
  if (m_pTableName)      delete m_pTableName;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Print the name of the table reference to stdout                            */
/*----------------------------------------------------------------------------*/
void TableRef::vPrint()
{
  s2K fullname = ""_Stz;
  
  if (m_pCatalogName != nullptr) {
    fullname.vConcat(m_pCatalogName->szGet());
    fullname.vConcat("."_Stz);
  }
  
  if (m_pSchemaName != nullptr) {
    fullname.vConcat(m_pSchemaName->szGet());
    fullname.vConcat("."_Stz);
  }
  
  fullname.vConcat(m_pTableName->szGet());
  
  vTrace("Got table reference: %s\n"_Stz, fullname.azChar());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
