//
// Created on 4/26/21.
//

#include "BASE.h"
#include "MetaGet.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,MetaGet,CN_METAGET)
__aNewFunction(MetaGet,CN_METAGET)
__vDeleteFunction(MetaGet,CN_METAGET)
__aFooterFunction(MetaGet)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MetaGet::MetaGet(void)
    : Operand(CN_METAGET) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_METAGET, _FOOTER_);

  m_sTableName = ""_Stz;


  _checkInitialised_(CN_METAGET);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_METAGET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MetaGet::MetaGet(s32 sTableNm)
    : Operand(CN_METAGET) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_METAGET, _FOOTER_);

  m_sTableName = sTableNm;


  _checkInitialised_(CN_METAGET);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_METAGET,_FOOTER_);
}



/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MetaGet::~MetaGet(void) noexcept(false)
{
_precondition_(_ADDRESS_,_MAGIC_, CN_METAGET,_FOOTER_);


}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair the meta get                                                        */
/*----------------------------------------------------------------------------*/
void MetaGet::vRepair(Object* pObject)
{
  _static_

  MetaGet* pMetaGet = _pMetaGet(pObject);

  // delete the metaGet
  delete pMetaGet;
}
