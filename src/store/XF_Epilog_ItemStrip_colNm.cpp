/*============================================================================*/
/* XF_Epilog_ItemStrip_colNm.cpp -- Strip itemise prefix off col names Cn_Abc */
/*============================================================================*/

#include "XF_Epilog_ItemStrip_colNm.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_ItemStrip_colNm, CN_XF_EPILOG_ITEMSTRIP_COLNM)
__aNewFunction(XF_Epilog_ItemStrip_colNm, CN_XF_EPILOG_ITEMSTRIP_COLNM)
__vDeleteFunction(XF_Epilog_ItemStrip_colNm, CN_XF_EPILOG_ITEMSTRIP_COLNM)
__aFooterFunction(XF_Epilog_ItemStrip_colNm)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_ItemStrip_colNm::XF_Epilog_ItemStrip_colNm(CardIter* pCI)
: XF_EPILOG(CN_XF_EPILOG_ITEMSTRIP_COLNM, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ITEMSTRIP_COLNM, _FOOTER_);

  _checkInitialised_(CN_XF_EPILOG_ITEMSTRIP_COLNM);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ITEMSTRIP_COLNM, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_ItemStrip_colNm::~XF_Epilog_ItemStrip_colNm(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ITEMSTRIP_COLNM, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* de-duplicate as (column names)                                             */
/*----------------------------------------------------------------------------*/
void XF_Epilog_ItemStrip_colNm::vEpilog(CardIter* pCI)
{
  _static_

  XF_Epilog_ItemStrip_colNm* pXF_Dedupe_colNm = new XF_Epilog_ItemStrip_colNm(pCI);

  pXF_Dedupe_colNm->vEpilog();

  delete pXF_Dedupe_colNm;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* de-duplicate as (column names)                                             */
/*----------------------------------------------------------------------------*/
void XF_Epilog_ItemStrip_colNm::vEpilog(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ITEMSTRIP_COLNM, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_ITEMSTRIP_COLNM, "strip C_ suffix"_Stz);


  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
