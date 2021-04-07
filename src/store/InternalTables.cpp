/*============================================================================*/
/* InternalTables.cpp -- Internal Tables                                      */
/*============================================================================*/

#include "BASE.h"

#include "InternalTables.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,InternalTables,CN_INTERNALTABLES)
__aNewFunction(InternalTables,CN_INTERNALTABLES)
__vDeleteFunction(InternalTables,CN_INTERNALTABLES)
__aFooterFunction(InternalTables)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
InternalTables::InternalTables(void)
: Object(CN_INTERNALTABLES)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTERNALTABLES,_FOOTER_);
  
  for (i8 i_StreamDef = 0; i_StreamDef < TI_ENUMS; i_StreamDef++)
    m_apStreamDef[i_StreamDef] = nullptr;
    
  _checkInitialised_(CN_INTERNALTABLES);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INTERNALTABLES,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
InternalTables::~InternalTables(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTERNALTABLES,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get stream def for given internal table                                    */
/*----------------------------------------------------------------------------*/
StreamDef* InternalTables::pStreamDef(eTI eTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTERNALTABLES,_FOOTER_);
  
  return m_apStreamDef[eTable];
}

/*----------------------------------------------------------------------------*/
/* set stream def for given internal table                                    */
/*----------------------------------------------------------------------------*/
void InternalTables::vStreamDef(eTI eTable, StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTERNALTABLES,_FOOTER_);
  
  m_apStreamDef[eTable] = pStreamDef;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
