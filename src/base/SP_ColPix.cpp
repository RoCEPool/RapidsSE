/*============================================================================*/
/* SP_ColPix.cpp - Column Pix Sprint Helper                                   */
/*============================================================================*/

#include "BASE.h"

#include "SP_ColPix.hpp"

#include "eCX.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_ColPix::SP_ColPix(void)
: SP(CN_SPID)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  _checkInitialised_(CN_SPID);
  
  _postcondition_(_MAGIC_,CN_SPID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_ColPix::~SP_ColPix(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint Header and return Stz* to result                                    */
/*----------------------------------------------------------------------------*/
Stz* SP_ColPix::sz(ColPix* pColPix)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
      
  Stz* szColNm = aAdd<Stz*>(pColPix,pColPix->m_uName);
    
  StrLib::vSprint(m_caResult
                  
    ,"%02lld %16s : kind %s type %s datum %s "
     "length %5lld scale %2lld %s %s"_Stz
                  
    ,(i8)pColPix->m_uNumber
    ,szColNm
    ,aCX[pColPix->m_uKind]
    ,aCT[pColPix->m_uType]
    ,aDM[pColPix->m_uDatum]
    ,(i8)pColPix->m_uLength
    ,(i8)pColPix->m_uScale
    ,pColPix->m_bNullable   ? "nullable"_Stz : "not null"_Stz
    ,pColPix->m_bPtnKey     ? "ptnKey"_Stz   : ""_Stz
                  
    );

  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_ColPix)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
