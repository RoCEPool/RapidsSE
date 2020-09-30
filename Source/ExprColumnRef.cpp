/*============================================================================*/
/* ExprColumnRef.hpp - a reference to a column                                */
/*============================================================================*/

#include "Parser.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Parser,ExprColumnRef,CN_EXPRCOLUMNREF)
__aNewFunction(ExprColumnRef,CN_EXPRCOLUMNREF)
__vDeleteFunction(ExprColumnRef,CN_EXPRCOLUMNREF)
__aFooterFunction(ExprColumnRef)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ExprColumnRef::ExprColumnRef(Text* pCatalogName, Text* pSchemaName,
                             Text* pTableName, Text* pColumnName)
: ExprAtom(CN_EXPRCOLUMNREF),
  m_pCatalogName(pCatalogName),
  m_pSchemaName(pSchemaName),
  m_pTableName(pTableName),
  m_pColumnName(pColumnName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPRCOLUMNREF,_FOOTER_);
  
  _checkInitialised_(CN_EXPRCOLUMNREF);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_EXPRCOLUMNREF,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ExprColumnRef::~ExprColumnRef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPRCOLUMNREF,_FOOTER_);
  
  if (m_pCatalogName) delete m_pCatalogName;
  if (m_pSchemaName)  delete m_pSchemaName;
  if (m_pTableName)   delete m_pTableName;
  if (m_pColumnName)  delete m_pColumnName;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void ExprColumnRef::vDescribe(s2K& buf)
{
  buf.vConcat("[ColumnRef]: "_Stz);
  
  if (m_pCatalogName) {
    ASSERT(m_pSchemaName);
    ASSERT(m_pTableName);
    ASSERT(m_pColumnName);
    buf.vConcat(m_pCatalogName->szGet());
    buf.vConcat("."_Stz);
    buf.vConcat(m_pSchemaName->szGet());
    buf.vConcat("."_Stz);
    buf.vConcat(m_pTableName->szGet());
    buf.vConcat("."_Stz);
    buf.vConcat(m_pColumnName->szGet());
    
  } else if (m_pSchemaName) {
    ASSERT(m_pTableName);
    ASSERT(m_pColumnName);
    buf.vConcat(m_pSchemaName->szGet());
    buf.vConcat("."_Stz);
    buf.vConcat(m_pTableName->szGet());
    buf.vConcat("."_Stz);
    buf.vConcat(m_pColumnName->szGet());
    
  } else if (m_pTableName) {
    ASSERT(m_pColumnName);
    buf.vConcat(m_pTableName->szGet());
    buf.vConcat("."_Stz);
    buf.vConcat(m_pColumnName->szGet());
    
  } else {
    buf.vConcat(m_pColumnName->szGet());
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
