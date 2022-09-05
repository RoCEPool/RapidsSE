/*============================================================================*/
/* MarkerMgr.cpp -- Marker Manager                                            */
/*============================================================================*/

#include "BASE.h"

#include "MarkerMgr.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,MarkerMgr,CN_MARKERMGR)
__aNewFunction(MarkerMgr,CN_MARKERMGR)
__vDeleteFunction(MarkerMgr,CN_MARKERMGR)
__aFooterFunction(MarkerMgr)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MarkerMgr::MarkerMgr(void)
: Object(CN_MARKERMGR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MARKERMGR,_FOOTER_);
  
  m_paMarker[0] = nullptr;
  for (i8 i_Marker = 1; i_Marker < OM_ENUMS; i_Marker++) {
    Marker* pMarker = new (MG_MANAGER)Marker((eOM)i_Marker);
    pMarker->vMarker(MG_MANAGER);
    m_paMarker[i_Marker] = pMarker;
  }
  
  _checkInitialised_(CN_MARKERMGR);
    
  _postcondition_(_ADDRESS_,_MAGIC_,CN_MARKERMGR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MarkerMgr::~MarkerMgr(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MARKERMGR,_FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* free a marker                                                              */
/*----------------------------------------------------------------------------*/
void MarkerMgr::vFreeMarker(Marker* pMarker)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MARKERMGR,_FOOTER_);

  RASSERT(pMarker == this->pMarker(pMarker->eOpMarker()))

  // do nothing
}

/*----------------------------------------------------------------------------*/
/* get a marker by eOM                                                        */
/*----------------------------------------------------------------------------*/
Marker* MarkerMgr::pMarker(eOM eMarker)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MARKERMGR,_FOOTER_);
  
  return m_paMarker[eMarker];
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
