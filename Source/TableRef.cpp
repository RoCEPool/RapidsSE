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
void TableRef::vPrintInto(s256& buf)
{  
  if (m_pCatalogName != nullptr) {
    ASSERT(m_pSchemaName != nullptr);
    ASSERT(m_pTableName  != nullptr);
    
    buf.vSprint("%s.%s.%s"_Stz,
                m_pCatalogName->szGet(),
                m_pSchemaName->szGet(),
                m_pTableName->szGet() );
    
  } else if (m_pSchemaName != nullptr) {
    ASSERT(m_pTableName  != nullptr);
    
    buf.vSprint("%s.%s"_Stz,
                m_pSchemaName->szGet(),
                m_pTableName->szGet() );
    
  } else {
    ASSERT(m_pTableName  != nullptr);
    buf.vCopy(m_pTableName->szGet());
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
