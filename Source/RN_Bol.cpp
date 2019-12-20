/*============================================================================*/
/* RN_Bol.cpp - Bol sprint helper                                             */
/*============================================================================*/

#include "Datums.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RN_Bol::RN_Bol(void)
: Rand(CN_RN_RAND)
{
  _precondition_(_MAGIC_,CN_RN_RAND,_FOOTER_);
  
  _checkInitialised_(CN_RN_RAND);
  
  _postcondition_(_MAGIC_,CN_RN_RAND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RN_Bol::RN_Bol(Datum_h* pDatum, Column* pCol)
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
RN_Bol::RN_Bol(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax)
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
RN_Bol::~RN_Bol(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_RN_RAND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* generate a random bol                                                      */
/*----------------------------------------------------------------------------*/
void RN_Bol::vRnd(Datum_h* pDatum, Column* pCol)
{
  _precondition_(_MAGIC_,CN_RN_RAND,_FOOTER_);
  
  Datum_h* pMin = pCol->pMinVal();
  Datum_h* pMax = pCol->pMaxVal();
  
  vRnd(pDatum,pMin,pMax);
}

/*----------------------------------------------------------------------------*/
/* generate a random bol                                                      */
/*----------------------------------------------------------------------------*/
void RN_Bol::vRnd(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax)
{
  _precondition_(_MAGIC_,CN_RN_RAND,_FOOTER_);
  
  i8 iMin = pMin->iBol();
  i8 iMax = pMax->iBol();
  
  i8 iRnd = RndLib::iInteger(iMin,iMax); // inclusive
  
  pDatum->vInt(iRnd);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(RN_Bol)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
