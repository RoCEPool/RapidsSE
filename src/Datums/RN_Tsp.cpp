/*============================================================================*/
/* RN_Tsp.cpp - Tsp sprint helper                                             */
/*============================================================================*/

#include "Datums.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RN_Tsp::RN_Tsp(void)
: Rand(CN_RN_RAND)
{
  _precondition_(_MAGIC_,CN_RN_RAND,_FOOTER_);
  
  _checkInitialised_(CN_RN_RAND);
  
  _postcondition_(_MAGIC_,CN_RN_RAND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RN_Tsp::RN_Tsp(Datum_h* pDatum, Column* pCol)
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
RN_Tsp::RN_Tsp(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax)
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
RN_Tsp::~RN_Tsp(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_RN_RAND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* generate a random tsp                                                      */
/*----------------------------------------------------------------------------*/
void RN_Tsp::vRnd(Datum_h* pDatum, Column* pCol)
{
  _precondition_(_MAGIC_,CN_RN_RAND,_FOOTER_);
  
  Datum_h* pMin = pCol->pMinVal();
  Datum_h* pMax = pCol->pMaxVal();
  
  vRnd(pDatum,pMin,pMax);
}

/*----------------------------------------------------------------------------*/
/* generate a random tsp                                                      */
/*----------------------------------------------------------------------------*/
void RN_Tsp::vRnd(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax)
{
  _precondition_(_MAGIC_,CN_RN_RAND,_FOOTER_);
  
  i8 iMin = pMin->iInt();
  i8 iMax = pMax->iInt();
  
  //i8 iRnd = RndLib::iInteger(iMin,iMax); // inclusive
  i8 iRnd = iMin; //hack

  pDatum->vInt(iRnd);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(RN_Tsp)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
