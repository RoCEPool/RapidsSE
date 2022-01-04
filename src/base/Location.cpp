/*============================================================================*/
/* Location.cpp - Logical Location of Object                                  */
/*============================================================================*/

#include "BASE.h"

#include "Location.hpp"
#include "MY.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,Location,CN_LOCATION)
__aNewFunction(Location,CN_LOCATION)
__vDeleteFunction(Location,CN_LOCATION)
__aFooterFunction(Location)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Location::Location(void)
: Object(CN_LOCATION)
{
  _precondition_(_MAGIC_,CN_LOCATION,_FOOTER_);
    
  _checkInitialised_(CN_LOCATION);
  
  //vSet();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Location::~Location(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_LOCATION,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print location                                                             */
/*----------------------------------------------------------------------------*/
void Location::vPrint(void)
{
  _precondition_(_MAGIC_,CN_LOCATION,_FOOTER_);

  vTrace("Location: %s %s %s %s\n"_Stz,
         m_sTenant.szStz(),m_sCatalog.szStz(),m_sSchema.szStz(),m_sTable.szStz());
}

/*----------------------------------------------------------------------------*/
/* set all parts of location                                                  */
/*----------------------------------------------------------------------------*/
void Location::vSet(void)
{
  _precondition_(_MAGIC_,CN_LOCATION,_FOOTER_);

  m_sTenant  = MY::szTenantNm();
  m_sCatalog = MY::szCatalogNm();
  m_sSchema  = MY::szSchemaNm();
  m_sTable   = MY::szTableNm();
}

/*----------------------------------------------------------------------------*/
/* get tenant                                                                 */
/*----------------------------------------------------------------------------*/
Stz* Location::szTenant(void)
{
  _precondition_(_MAGIC_,CN_LOCATION,_FOOTER_);
  
  return m_sTenant;
}

/*----------------------------------------------------------------------------*/
/* set tenant                                                                 */
/*----------------------------------------------------------------------------*/
void Location::vTenant(Stz* szTenant)
{
  _precondition_(_MAGIC_,CN_LOCATION,_FOOTER_);
  
  m_sTenant = szTenant;
}

/*----------------------------------------------------------------------------*/
/* get catalog                                                                */
/*----------------------------------------------------------------------------*/
Stz* Location::szCatalog(void)
{
  _precondition_(_MAGIC_,CN_LOCATION,_FOOTER_);
  
  return m_sCatalog;
}

/*----------------------------------------------------------------------------*/
/* set catalog                                                                */
/*----------------------------------------------------------------------------*/
void Location::vCatalog(Stz* szCatalog)
{
  _precondition_(_MAGIC_,CN_LOCATION,_FOOTER_);
  
  m_sCatalog = szCatalog;
}

/*----------------------------------------------------------------------------*/
/* get schema                                                                 */
/*----------------------------------------------------------------------------*/
Stz* Location::szSchema(void)
{
  _precondition_(_MAGIC_,CN_LOCATION,_FOOTER_);
  
  return m_sSchema;
}

/*----------------------------------------------------------------------------*/
/* set schema                                                                 */
/*----------------------------------------------------------------------------*/
void Location::vSchema(Stz* szSchema)
{
  _precondition_(_MAGIC_,CN_LOCATION,_FOOTER_);
  
  m_sSchema = szSchema;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Stz* Location::szTable(void)
{
  _precondition_(_MAGIC_,CN_LOCATION,_FOOTER_);
  
  return m_sTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void Location::vTable(Stz* szTable)
{
  _precondition_(_MAGIC_,CN_LOCATION,_FOOTER_);
  
  m_sTable = szTable;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
