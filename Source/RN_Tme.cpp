/*============================================================================*/
/* RN_Tme.cpp - Tme sprint helper                                             */
/*============================================================================*/

#include "Datums.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RN_Tme::RN_Tme(void)
: Rand(CN_RN_RAND)
{
  _precondition_(_MAGIC_,CN_RN_RAND,_FOOTER_);
  
  _checkInitialised_(CN_RN_RAND);
  
  _postcondition_(_MAGIC_,CN_RN_RAND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RN_Tme::RN_Tme(Datum_h* pDatum, Column* pCol)
: Rand(CN_RN_RAND)
{
  _precondition_(_MAGIC_,CN_RN_RAND,_FOOTER_);
  
  _checkInitialised_(CN_RN_RAND);
  
  vRnd(pDatum,pCol);
  
  _postcondition_(_MAGIC_,CN_RN_RAND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RN_Tme::RN_Tme(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax)
: Rand(CN_RN_RAND)
{
  _precondition_(_MAGIC_,CN_RN_RAND,_FOOTER_);
  
  _checkInitialised_(CN_RN_RAND);
  
  vRnd(pDatum,pMin,pMax);
  
  _postcondition_(_MAGIC_,CN_RN_RAND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RN_Tme::~RN_Tme(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_RN_RAND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* generate a random tme                                                      */
/*----------------------------------------------------------------------------*/
void RN_Tme::vRnd(Datum_h* pDatum, Column* pCol)
{
  _precondition_(_MAGIC_,CN_RN_RAND,_FOOTER_);
  
  Datum_h* pMin = pCol->pMinVal();
  Datum_h* pMax = pCol->pMaxVal();
  
  vRnd(pDatum,pMin,pMax);
}

/*----------------------------------------------------------------------------*/
/* generate a random tme                                                      */
/*----------------------------------------------------------------------------*/
void RN_Tme::vRnd(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax)
{
  _precondition_(_MAGIC_,CN_RN_RAND,_FOOTER_);
  
  i8 iMin = pMin->iInt();
  i8 iMax = pMax->iInt();
  
  //i8 iRnd = RndLib::iInteger(iMin,iMax); // inclusive
  i8 iRnd = iMin; // hack

  pDatum->vInt(iRnd);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(RN_Tme)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
