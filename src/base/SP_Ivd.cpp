/*============================================================================*/
/* SP_Ivd.cpp - Ivd sprint helper                                             */
/*============================================================================*/

#include "BASE.h"

#include "SP_Ivd.hpp"
#include "eIV.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Ivd::SP_Ivd(void)
{
  _precondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
  
  _checkInitialised_(CN_SPSMALL);
  
  _postcondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Ivd::~SP_Ivd(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint Ivd and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_Ivd::sz(i8 iIvd)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return sz(iIvd,0);
}

/*----------------------------------------------------------------------------*/
/* sprint Ivl and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_Ivd::sz(i8 iIvd, i8 iScale)
{
  _precondition_(_MAGIC_,_FOOTER_);
  UNUSED(iScale);
  
  Ivl wIvd{IV_SECOND};
  wIvd = iIvd;

  u8 uDays;
  u8 uHours;
  u8 uMins;
  u8 uSecs;
  u8 uMicros;

  switch(wIvd.T.m_xIvl.m_eType) {
    case IV_DAY:
      uDays = wIvd.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_SECS_PER_DAY * C_1M);

      if (wIvd.bIsNeg() && uDays)
        StrLib::vSprint(m_caResult,
                        sizeof(m_caResult),
                        (Stz *) "INTERVAL -'%lld' %s",
                        uDays,
                        aIV_field[wIvd.T.m_xIvl.m_eType]);
      else
        StrLib::vSprint(m_caResult,
                        sizeof(m_caResult),
                        (Stz *) "INTERVAL '%lld' %s",
                        uDays,
                        aIV_field[wIvd.T.m_xIvl.m_eType]);
      break;

    case IV_HOUR:
      uHours = wIvd.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_SECS_PER_HR * C_1M);

      if (wIvd.bIsNeg() && uHours)
        StrLib::vSprint(m_caResult,
                        sizeof(m_caResult),
                        (Stz *) "INTERVAL -'%lld' %s",
                        uHours,
                        aIV_field[wIvd.T.m_xIvl.m_eType]);
      else
        StrLib::vSprint(m_caResult,
                        sizeof(m_caResult),
                        (Stz *) "INTERVAL '%lld' %s",
                        uHours,
                        aIV_field[wIvd.T.m_xIvl.m_eType]);
      break;

    case IV_MINUTE:
      uMins = wIvd.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_SECS_PER_MIN * C_1M);

      if (wIvd.bIsNeg() && uMins)
        StrLib::vSprint(m_caResult,
                        sizeof(m_caResult),
                        (Stz *) "INTERVAL -'%lld' %s",
                        uMins,
                        aIV_field[wIvd.T.m_xIvl.m_eType]);
      else
        StrLib::vSprint(m_caResult,
                        sizeof(m_caResult),
                        (Stz *) "INTERVAL '%lld' %s",
                        uMins,
                        aIV_field[wIvd.T.m_xIvl.m_eType]);
      break;

    case IV_SECOND:
      uSecs = wIvd.T.m_xIvl.u.m_xIvlDT.m_uMicros / C_1M;
      uMicros = wIvd.T.m_xIvl.u.m_xIvlDT.m_uMicros % C_1M;

      if (wIvd.bIsNeg() && (uSecs || uMicros)) {

        if (uMicros > 0)
          StrLib::vSprint(m_caResult,
                          sizeof(m_caResult),
                          (Stz *) "INTERVAL -'%lld.%06lld' %s",
                          uSecs,
                          uMicros,
                          aIV_field[wIvd.T.m_xIvl.m_eType]);
        else
          StrLib::vSprint(m_caResult,
                          sizeof(m_caResult),
                          (Stz *) "INTERVAL -'%lld' %s",
                          uSecs,
                          aIV_field[wIvd.T.m_xIvl.m_eType]);
      }
      else {
        if (uMicros > 0)
          StrLib::vSprint(m_caResult,
                          sizeof(m_caResult),
                          (Stz *) "INTERVAL '%lld.%06lld' %s",
                          uSecs,
                          uMicros,
                          aIV_field[wIvd.T.m_xIvl.m_eType]);
        else
          StrLib::vSprint(m_caResult,
                          sizeof(m_caResult),
                          (Stz *) "INTERVAL '%lld' %s",
                          uSecs,
                          aIV_field[wIvd.T.m_xIvl.m_eType]);
      }
      break;

    case IV_DAY_TO_HOUR:
      uDays  =  wIvd.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_SECS_PER_DAY * C_1M);
      uHours = (wIvd.T.m_xIvl.u.m_xIvlDT.m_uMicros % (C_SECS_PER_DAY * C_1M)) / (C_SECS_PER_HR * C_1M);

      if (wIvd.bIsNeg() && (uDays || uHours))
        StrLib::vSprint(m_caResult,
                        sizeof(m_caResult),
                        (Stz *) "INTERVAL -'%lld %lld' %s",
                        uDays,
                        uHours,
                        aIV_field[wIvd.T.m_xIvl.m_eType]);
      else
        StrLib::vSprint(m_caResult,
                        sizeof(m_caResult),
                        (Stz *) "INTERVAL '%lld %lld' %s",
                        uDays,
                        uHours,
                        aIV_field[wIvd.T.m_xIvl.m_eType]);
      break;

    case IV_DAY_TO_MINUTE:
      uDays  =  wIvd.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_SECS_PER_DAY * C_1M);
      uHours = (wIvd.T.m_xIvl.u.m_xIvlDT.m_uMicros % (C_SECS_PER_DAY * C_1M)) / (C_SECS_PER_HR * C_1M);
      uMins  = (wIvd.T.m_xIvl.u.m_xIvlDT.m_uMicros % (C_SECS_PER_HR * C_1M)) / (C_SECS_PER_MIN * C_1M);

      if (wIvd.bIsNeg() && (uDays || uHours || uMins))
        StrLib::vSprint(m_caResult,
                        sizeof(m_caResult),
                        (Stz *) "INTERVAL -'%lld %lld:%lld' %s",
                        uDays,
                        uHours,
                        uMins,
                        aIV_field[wIvd.T.m_xIvl.m_eType]);
      else
        StrLib::vSprint(m_caResult,
                        sizeof(m_caResult),
                        (Stz *) "INTERVAL '%lld %lld:%lld' %s",
                        uDays,
                        uHours,
                        uMins,
                        aIV_field[wIvd.T.m_xIvl.m_eType]);
      break;

    case IV_DAY_TO_SECOND:
      uDays   =  wIvd.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_SECS_PER_DAY * C_1M);
      uHours  = (wIvd.T.m_xIvl.u.m_xIvlDT.m_uMicros % (C_SECS_PER_DAY * C_1M)) / (C_SECS_PER_HR * C_1M);
      uMins   = (wIvd.T.m_xIvl.u.m_xIvlDT.m_uMicros % (C_SECS_PER_HR  * C_1M)) / (C_SECS_PER_MIN * C_1M);
      uSecs   = (wIvd.T.m_xIvl.u.m_xIvlDT.m_uMicros % (C_SECS_PER_MIN * C_1M)) / C_1M;
      uMicros =  wIvd.T.m_xIvl.u.m_xIvlDT.m_uMicros % C_1M;

      if (wIvd.bIsNeg() && (uDays || uHours || uMins || uSecs || uMicros)) {
        if (uMicros > 0)
          StrLib::vSprint(m_caResult,
                          sizeof(m_caResult),
                          (Stz *) "INTERVAL -'%lld %lld:%lld:%lld.%06lld' %s",
                          uDays,
                          uHours,
                          uMins,
                          uSecs,
                          uMicros,
                          aIV_field[wIvd.T.m_xIvl.m_eType]);
        else
          StrLib::vSprint(m_caResult,
                          sizeof(m_caResult),
                          (Stz *) "INTERVAL -'%lld %lld:%lld:%lld' %s",
                          uDays,
                          uHours,
                          uMins,
                          uSecs,
                          aIV_field[wIvd.T.m_xIvl.m_eType]);
      }
      else {
        if (uMicros > 0)
          StrLib::vSprint(m_caResult,
                          sizeof(m_caResult),
                          (Stz *) "INTERVAL '%lld %lld:%lld:%lld.%06lld' %s",
                          uDays,
                          uHours,
                          uMins,
                          uSecs,
                          uMicros,
                          aIV_field[wIvd.T.m_xIvl.m_eType]);
        else
          StrLib::vSprint(m_caResult,
                          sizeof(m_caResult),
                          (Stz *) "INTERVAL '%lld %lld:%lld:%lld' %s",
                          uDays,
                          uHours,
                          uMins,
                          uSecs,
                          aIV_field[wIvd.T.m_xIvl.m_eType]);
      }
      break;

    case IV_HOUR_TO_MINUTE:
      uHours =  wIvd.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_SECS_PER_HR * C_1M);
      uMins  = (wIvd.T.m_xIvl.u.m_xIvlDT.m_uMicros % (C_SECS_PER_HR * C_1M)) / (C_SECS_PER_MIN * C_1M);

      if (wIvd.bIsNeg() && (uHours || uMins))
        StrLib::vSprint(m_caResult,
                        sizeof(m_caResult),
                        (Stz *) "INTERVAL -'%lld:%lld' %s",
                        uHours,
                        uMins,
                        aIV_field[wIvd.T.m_xIvl.m_eType]);
      else
        StrLib::vSprint(m_caResult,
                        sizeof(m_caResult),
                        (Stz *) "INTERVAL '%lld:%lld' %s",
                        uHours,
                        uMins,
                        aIV_field[wIvd.T.m_xIvl.m_eType]);
      break;

    case IV_HOUR_TO_SECOND:
      uHours  =  wIvd.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_SECS_PER_HR  * C_1M);
      uMins   = (wIvd.T.m_xIvl.u.m_xIvlDT.m_uMicros % (C_SECS_PER_HR  * C_1M)) / (C_SECS_PER_MIN * C_1M);
      uSecs   = (wIvd.T.m_xIvl.u.m_xIvlDT.m_uMicros % (C_SECS_PER_MIN * C_1M)) / C_1M;
      uMicros =  wIvd.T.m_xIvl.u.m_xIvlDT.m_uMicros % C_1M;

      if (wIvd.bIsNeg() && (uHours || uMins || uSecs || uMicros)) {
        if (uMicros > 0)
          StrLib::vSprint(m_caResult,
                          sizeof(m_caResult),
                          (Stz *) "INTERVAL -'%lld:%lld:%lld.%06lld' %s",
                          uHours,
                          uMins,
                          uSecs,
                          uMicros,
                          aIV_field[wIvd.T.m_xIvl.m_eType]);
        else
          StrLib::vSprint(m_caResult,
                          sizeof(m_caResult),
                          (Stz *) "INTERVAL -'%lld:%lld:%lld' %s",
                          uHours,
                          uMins,
                          uSecs,
                          aIV_field[wIvd.T.m_xIvl.m_eType]);
      }
      else {
        if (uMicros > 0)
          StrLib::vSprint(m_caResult,
                          sizeof(m_caResult),
                          (Stz *) "INTERVAL '%lld:%lld:%lld.%06lld' %s",
                          uHours,
                          uMins,
                          uSecs,
                          uMicros,
                          aIV_field[wIvd.T.m_xIvl.m_eType]);
        else
          StrLib::vSprint(m_caResult,
                          sizeof(m_caResult),
                          (Stz *) "INTERVAL '%lld:%lld:%lld' %s",
                          uHours,
                          uMins,
                          uSecs,
                          aIV_field[wIvd.T.m_xIvl.m_eType]);
      }
      break;

    case IV_MINUTE_TO_SECOND:
      uMins   =  wIvd.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_SECS_PER_MIN * C_1M);
      uSecs   = (wIvd.T.m_xIvl.u.m_xIvlDT.m_uMicros % (C_SECS_PER_MIN * C_1M)) / C_1M;
      uMicros =  wIvd.T.m_xIvl.u.m_xIvlDT.m_uMicros % C_1M;

      if (wIvd.bIsNeg() && (uMins || uSecs || uMicros)) {
        if (uMicros > 0)
          StrLib::vSprint(m_caResult,
                          sizeof(m_caResult),
                          (Stz *) "INTERVAL -'%lld:%lld.%06lld' %s",
                          uMins,
                          uSecs,
                          uMicros,
                          aIV_field[wIvd.T.m_xIvl.m_eType]);
        else
          StrLib::vSprint(m_caResult,
                          sizeof(m_caResult),
                          (Stz *) "INTERVAL -'%lld:%lld' %s",
                          uMins,
                          uSecs,
                          aIV_field[wIvd.T.m_xIvl.m_eType]);
      }
      else {
        if (uMicros > 0)
          StrLib::vSprint(m_caResult,
                          sizeof(m_caResult),
                          (Stz *) "INTERVAL '%lld:%lld.%06lld' %s",
                          uMins,
                          uSecs,
                          uMicros,
                          aIV_field[wIvd.T.m_xIvl.m_eType]);
        else
          StrLib::vSprint(m_caResult,
                          sizeof(m_caResult),
                          (Stz *) "INTERVAL '%lld:%lld' %s",
                          uMins,
                          uSecs,
                          aIV_field[wIvd.T.m_xIvl.m_eType]);
      }
      break;

    default:
      NEVER_REACH;
  }

  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/*-- Samples -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* max                                                                        */
/*----------------------------------------------------------------------------*/
Stz* SP_Ivd::szMax(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return sz(0x1UL << 53,1023);
}

/*----------------------------------------------------------------------------*/
/* mid                                                                        */
/*----------------------------------------------------------------------------*/
Stz* SP_Ivd::szMid(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return sz(73,73);
}

/*----------------------------------------------------------------------------*/
/* min                                                                        */
/*----------------------------------------------------------------------------*/
Stz* SP_Ivd::szMin(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return sz(0,0);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SP_Ivd::aFooter(void)
{
  _precondition_(_MAGIC_,CN_SPSMALL);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
