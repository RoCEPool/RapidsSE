#include "BASE.h"
#include "BktMapGet.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,BktMapGet,CN_BKTMAPGET)
__aNewFunction(BktMapGet,CN_BKTMAPGET)
__vDeleteFunction(BktMapGet,CN_BKTMAPGET)
__aFooterFunction(BktMapGet)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BktMapGet::BktMapGet(void)
: Operand(CN_BKTMAPGET) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_BKTMAPGET, _FOOTER_);

  m_sTableName = ""_Stz;


  _checkInitialised_(CN_BKTMAPGET);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_BKTMAPGET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BktMapGet::BktMapGet(s32 sTableNm)
: Operand(CN_BKTMAPGET) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_BKTMAPGET, _FOOTER_);

  m_sTableName = sTableNm;


  _checkInitialised_(CN_BKTMAPGET);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_BKTMAPGET,_FOOTER_);
}



/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
BktMapGet::~BktMapGet(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_, CN_BKTMAPGET,_FOOTER_);


}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair the bucket map get                                                  */
/*----------------------------------------------------------------------------*/
void BktMapGet::vRepair(Object* pObject)
{
  _static_

  if (pObject) {
    BktMapGet * pBktMapGet = _pBktMapGet(pObject);

    // delete the BktMapGet
    delete pBktMapGet;
  }
}
