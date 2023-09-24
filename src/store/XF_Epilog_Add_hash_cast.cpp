/*============================================================================*/
/* XF_Epilog_Add_hash_cast.cpp -- Add Hash (opt) and Cast                     */
/*============================================================================*/

#include "XF_Epilog_Add_hash_cast.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Add_hash_cast, CN_XF_EPILOG_ADD_HASH_CAST)
__aNewFunction(XF_Epilog_Add_hash_cast, CN_XF_EPILOG_ADD_HASH_CAST)
__vDeleteFunction(XF_Epilog_Add_hash_cast, CN_XF_EPILOG_ADD_HASH_CAST)
__aFooterFunction(XF_Epilog_Add_hash_cast)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Add_hash_cast::XF_Epilog_Add_hash_cast(CardIter* pCI)
: XF_EPILOG(CN_XF_EPILOG_ADD_HASH_CAST, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ADD_HASH_CAST, _FOOTER_);

  _checkInitialised_(CN_XF_EPILOG_ADD_HASH_CAST);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ADD_HASH_CAST, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Add_hash_cast::~XF_Epilog_Add_hash_cast(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ADD_HASH_CAST, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Add_hash_cast::vAdd(CardIter* pCI)
{
  _static_

  XF_Epilog_Add_hash_cast* pXF_stage_cast = new XF_Epilog_Add_hash_cast(pCI);

  pXF_stage_cast->vAdd();

  delete pXF_stage_cast;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Add_hash_cast::vAdd(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ADD_HASH_CAST, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_ADD_HASH_CAST, "add stage casts"_Stz);

  m_pCI->G.pDeck->vAddStageCasts(m_pCI);

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
