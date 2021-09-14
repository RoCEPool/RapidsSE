/*============================================================================*/
/* PuGen.cpp -- Performance Unit Data Generator                               */
/*============================================================================*/

#include "BASE.h"

#include "PuGen.hpp"

#include "StreamDef.hpp"
#include "ToRow.hpp"
#include "RowBuffer.hpp"


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PuGen::vGen(i8& riNextKey, ToRow* pToRow, StreamDef* pStreamDef)
{
  Stz* szStreamDefNm = pStreamDef->szStreamDefNm();
  
  if (szStreamDefNm->bSame("Pu10k"_Stz))
    vPu10k(pToRow,riNextKey);
  else
  if (szStreamDefNm->bSame("Pu100k"_Stz))
    vPu100k(pToRow,riNextKey);
  else
  if (szStreamDefNm->bSame("Pu1m"_Stz))
    vPu1m(pToRow,riNextKey);
  else
  if (szStreamDefNm->bSame("Pu10m"_Stz))
    vPu10m(pToRow,riNextKey);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PuGen::vPu10k(ToRow* pToRow, i8& riNextKey)
{
  pToRow->vNewRowBuffer();
  RowBuffer* pRowBuffer = pToRow->pRowBuffer();
  
  while (riNextKey && pRowBuffer->pAllotRow()) {
  
    i8 iPtnKeyHash = 0;
    
    pRowBuffer->vDepositColVal(riNextKey);

    vPrePst(pRowBuffer);
    vOther(pRowBuffer,riNextKey);

    HshLib::vRunningHash(&iPtnKeyHash,riNextKey);
    pRowBuffer->vDepositColVal(iPtnKeyHash);

    if (10001 == ++riNextKey)
      riNextKey = 0;
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PuGen::vPu100k(ToRow* pToRow, i8& riNextKey)
{
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PuGen::vPu1m(ToRow* pToRow, i8& riNextKey)
{
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PuGen::vPu10m(ToRow* pToRow, i8& riNextKey)
{
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PuGen::vOther(RowBuffer* pRowBuffer, i8 iKey)
{
  pRowBuffer->vDepositColVal(RndLib::iInteger(iKey));
  
  Dte iColVal_date;
  iColVal_date.m_uYearDelta = 2021 - C_YEAR_BASE;
  iColVal_date.m_uMonth     = 9;
  iColVal_date.m_uDay       = 14;
  iColVal_date.m_uSpare     = 0;
  pRowBuffer->vDepositColVal(iColVal_date);

  s4K sRandom;
  RndLib::vString(sRandom,RndLib::iInteger(1,128));
  pRowBuffer->vDepositColVal(sRandom);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PuGen::vPrePst(RowBuffer* pRowBuffer)
{
  pRowBuffer->vDepositColVal(RndLib::iInteger(1000));
  pRowBuffer->vDepositColVal(RndLib::iInteger(1000));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
