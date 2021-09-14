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
/* generate pu table data                                                     */
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
  else
    NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* generate Pu10k data                                                        */
/*----------------------------------------------------------------------------*/
void PuGen::vPu10k(ToRow* pToRow, i8& riNextKey)
{
  pToRow->vNewRowBuffer();
  RowBuffer* pRowBuffer = pToRow->pRowBuffer();
  
  while (riNextKey && pRowBuffer->pAllotRow()) {
  
    i8 iPtnKeyHash = 0;
    
    pRowBuffer->vDepositColVal(riNextKey);              // pu10k_10k

    vPrePst(pRowBuffer);
    vGroups(pRowBuffer);
    vOther(pRowBuffer,riNextKey);

    HshLib::vRunningHash(&iPtnKeyHash,riNextKey);
    pRowBuffer->vDepositColVal(iPtnKeyHash);

    if (C_10K + 1 == ++riNextKey)
      riNextKey = 0;
  }
}

/*----------------------------------------------------------------------------*/
/* generate Pu100k data                                                       */
/*----------------------------------------------------------------------------*/
void PuGen::vPu100k(ToRow* pToRow, i8& riNextKey)
{
  pToRow->vNewRowBuffer();
  RowBuffer* pRowBuffer = pToRow->pRowBuffer();
  
  while (riNextKey && pRowBuffer->pAllotRow()) {
  
    i8 iPtnKeyHash = 0;
    
    pRowBuffer->vDepositColVal(riNextKey);              // pu100k_100k
    pRowBuffer->vDepositColVal(riNextKey % C_10K + 1);  // pu100k_10k

    vPrePst(pRowBuffer);
    vGroups(pRowBuffer);
    vOther(pRowBuffer,riNextKey);

    HshLib::vRunningHash(&iPtnKeyHash,riNextKey);
    pRowBuffer->vDepositColVal(iPtnKeyHash);

    if (C_100K + 1 == ++riNextKey)
      riNextKey = 0;
  }
}

/*----------------------------------------------------------------------------*/
/* generate Pu1m data                                                         */
/*----------------------------------------------------------------------------*/
void PuGen::vPu1m(ToRow* pToRow, i8& riNextKey)
{
  pToRow->vNewRowBuffer();
  RowBuffer* pRowBuffer = pToRow->pRowBuffer();
  
  while (riNextKey && pRowBuffer->pAllotRow()) {
  
    i8 iPtnKeyHash = 0;
    
    pRowBuffer->vDepositColVal(riNextKey);               // pu1m_1m
    pRowBuffer->vDepositColVal(riNextKey % C_100K + 1);  // pu1m_100k
    pRowBuffer->vDepositColVal(riNextKey % C_10K  + 1);  // pu1m_10k

    vPrePst(pRowBuffer);
    vGroups(pRowBuffer);
    vOther(pRowBuffer,riNextKey);

    HshLib::vRunningHash(&iPtnKeyHash,riNextKey);
    pRowBuffer->vDepositColVal(iPtnKeyHash);

    if (C_1M + 1 == ++riNextKey)
      riNextKey = 0;
  }

}

/*----------------------------------------------------------------------------*/
/* generate Pu10m data                                                        */
/*----------------------------------------------------------------------------*/
void PuGen::vPu10m(ToRow* pToRow, i8& riNextKey)
{
  pToRow->vNewRowBuffer();
  RowBuffer* pRowBuffer = pToRow->pRowBuffer();
  
  while (riNextKey && pRowBuffer->pAllotRow()) {
  
    i8 iPtnKeyHash = 0;
    
    pRowBuffer->vDepositColVal(riNextKey);               // pu10m_10m
    pRowBuffer->vDepositColVal(riNextKey % C_1M   + 1);  // pu10m_1m
    pRowBuffer->vDepositColVal(riNextKey % C_100K + 1);  // pu10m_100k
    pRowBuffer->vDepositColVal(riNextKey % C_10K  + 1);  // pu10m_10k

    vPrePst(pRowBuffer);
    vGroups(pRowBuffer);
    vOther(pRowBuffer,riNextKey);

    HshLib::vRunningHash(&iPtnKeyHash,riNextKey);
    pRowBuffer->vDepositColVal(iPtnKeyHash);

    if (C_10M + 1 == ++riNextKey)
      riNextKey = 0;
  }

}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PuGen::vGroups(RowBuffer* pRowBuffer)
{
  pRowBuffer->vDepositColVal(RndLib::iInteger(1,1000));  // _1000g
  pRowBuffer->vDepositColVal(RndLib::iInteger(1,100));   // _100g
  pRowBuffer->vDepositColVal(RndLib::iInteger(1,10));    // _10g
  pRowBuffer->vDepositColVal((i8)1);                     // _1g
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PuGen::vOther(RowBuffer* pRowBuffer, i8 iKey)
{
  pRowBuffer->vDepositColVal(RndLib::iInteger(iKey));    // _int
  
  Dte iColVal_date;
  iColVal_date.m_uYearDelta = 2021 - C_YEAR_BASE;
  iColVal_date.m_uMonth     = 9;
  iColVal_date.m_uDay       = 14;
  iColVal_date.m_uSpare     = 0;
  pRowBuffer->vDepositColVal(iColVal_date);              // _dte

  s4K sRandom;
  RndLib::vString(sRandom,RndLib::iInteger(1,128));
  pRowBuffer->vDepositColVal(sRandom);                   // _str
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PuGen::vPrePst(RowBuffer* pRowBuffer)
{
  pRowBuffer->vDepositColVal(RndLib::iInteger(1,1000));  // _pre
  pRowBuffer->vDepositColVal(RndLib::iInteger(1,1000));  // _pst
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
