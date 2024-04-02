/*============================================================================*/
/* MetaDataTables.cpp -- Internal Tables                                      */
/*============================================================================*/

#include "BASE.h"

#include "MetaDataTables.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,MetaDataTables,CN_METADATATABLES)
__aNewFunction(MetaDataTables,CN_METADATATABLES)
__vDeleteFunction(MetaDataTables,CN_METADATATABLES)
__aFooterFunction(MetaDataTables)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MetaDataTables::MetaDataTables(void)
    : Object(CN_METADATATABLES)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATATABLES,_FOOTER_);

  for (i8 i_Table = 0; i_Table < MD_ENUMS; i_Table++)
    m_apStreamDef[i_Table] = nullptr;

  _checkInitialised_(CN_METADATATABLES);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_METADATATABLES,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MetaDataTables::~MetaDataTables(void) noexcept(false)
{
_precondition_(_ADDRESS_,_MAGIC_,CN_METADATATABLES,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get stream def for given metadata table                                    */
/*----------------------------------------------------------------------------*/
StreamDef* MetaDataTables::pStreamDef(eMD eTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATATABLES,_FOOTER_);

  return m_apStreamDef[eTable];
}

/*----------------------------------------------------------------------------*/
/* set stream def for given metadata table                                    */
/*----------------------------------------------------------------------------*/
void MetaDataTables::vStreamDef(eMD eTable, StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATATABLES,_FOOTER_);

  m_apStreamDef[eTable] = pStreamDef;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
