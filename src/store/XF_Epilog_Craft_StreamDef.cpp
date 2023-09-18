/*============================================================================*/
/* XF_Epilog_Craft_StreamDef.cpp -- Craft Stream Def's (arriving & emitted)   */
/*============================================================================*/

#include "XF_Epilog_Craft_StreamDef.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Craft_StreamDef, CN_XF_EPILOG_CRAFT_STREAMDEF)
__aNewFunction(XF_Epilog_Craft_StreamDef, CN_XF_EPILOG_CRAFT_STREAMDEF)
__vDeleteFunction(XF_Epilog_Craft_StreamDef, CN_XF_EPILOG_CRAFT_STREAMDEF)
__aFooterFunction(XF_Epilog_Craft_StreamDef)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Craft_StreamDef::XF_Epilog_Craft_StreamDef(CardIter* pCI)
: XF_EPILOG(CN_XF_EPILOG_CRAFT_STREAMDEF, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_CRAFT_STREAMDEF, _FOOTER_);

  _checkInitialised_(CN_XF_EPILOG_CRAFT_STREAMDEF);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_CRAFT_STREAMDEF, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Craft_StreamDef::~XF_Epilog_Craft_StreamDef(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_CRAFT_STREAMDEF, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Craft_StreamDef::vEpilog(CardIter* pCI)
{
  _static_

  XF_Epilog_Craft_StreamDef* pXF_Columns_required = new XF_Epilog_Craft_StreamDef(pCI);

  pXF_Columns_required->vXF_Columns_required();

  delete pXF_Columns_required;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* columns required                                                           */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Craft_StreamDef::vXF_Columns_required(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_CRAFT_STREAMDEF, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_COLUMNS_REQUIRED, "columns required"_Stz);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain_NONE()) {
  }

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
