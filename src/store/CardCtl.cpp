/*============================================================================*/
/* CardCtl.cpp -- Card Control                                                */
/*============================================================================*/

 #include "CardCtl.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,CardCtl,CN_CARDCTL)
__aNewFunction(CardCtl,CN_CARDCTL)
__vDeleteFunction(CardCtl,CN_CARDCTL)
__aFooterFunction(CardCtl)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardCtl::CardCtl(void)
: Object(CN_CARDCTL)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);
        
  m_iDepth = 99;
  
  for (i8 i_Card = 0; i_Card < CK_ENUMS; i_Card++) {
    vForm(i_Card,1);
    vPrint(i_Card,1);
    vSee(i_Card,0);
  }
  
  _checkInitialised_(CN_CARDCTL);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CardCtl::~CardCtl(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get print depth                                                            */
/*----------------------------------------------------------------------------*/
i8 CardCtl::iDepth(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  return m_iDepth;
}

/*----------------------------------------------------------------------------*/
/* set print depth                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vDepth(i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  m_iDepth = iDepth;
}

/*----------------------------------------------------------------------------*/
/* get iForm for card                                                         */
/*----------------------------------------------------------------------------*/
i8 CardCtl::iForm(i8 i_Card)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  i8 iForm = m_iaForm[i_Card];
  
  return iForm;
}

/*----------------------------------------------------------------------------*/
/* set iForm for card                                                         */
/*----------------------------------------------------------------------------*/
void CardCtl::vForm(i8 i_Card, i8 iForm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  m_iaForm[i_Card] = iForm;
}

/*----------------------------------------------------------------------------*/
/* get iPrint for card                                                        */
/*----------------------------------------------------------------------------*/
i8 CardCtl::iPrint(i8 i_Card)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  i8 iPrint = m_iaPrint[i_Card];
  
  return iPrint;
}

/*----------------------------------------------------------------------------*/
/* set iPrint for card                                                        */
/*----------------------------------------------------------------------------*/
void CardCtl::vPrint(i8 i_Card, i8 iPrint)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  m_iaPrint[i_Card] = iPrint;
}

/*----------------------------------------------------------------------------*/
/* get iSee for card                                                          */
/*----------------------------------------------------------------------------*/
i8 CardCtl::iSee(i8 i_Card)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  i8 iSee = m_iaSee[i_Card];
  
  return iSee;
}

/*----------------------------------------------------------------------------*/
/* set iSee for card                                                          */
/*----------------------------------------------------------------------------*/
void CardCtl::vSee(i8 i_Card, i8 iSee)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  m_iaSee[i_Card] = iSee;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
