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
    vGen10k(riNextKey,pToRow,pStreamDef);
  else
  if (szStreamDefNm->bSame("Pu100k"_Stz))
    vGen100k(riNextKey,pToRow,pStreamDef);
  else
  if (szStreamDefNm->bSame("Pu1m"_Stz))
    vGen1m(riNextKey,pToRow,pStreamDef);
  else
  if (szStreamDefNm->bSame("Pu10m"_Stz))
    vGen10m(riNextKey,pToRow,pStreamDef);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PuGen::vGen10k(i8& riNextKey, ToRow* pToRow, StreamDef* pStreamDef)
{
  Stz* szStreamDefNm = pStreamDef->szStreamDefNm();

  vTrace("vPuGenX: %p ; %s ; %lld\n"_Stz
         ,pToRow,szStreamDefNm,riNextKey);
  
  //  create a new buffer
  pToRow->vNewRowBuffer();
  RowBuffer* pRowBuffer = pToRow->pRowBuffer();
  
  while (riNextKey && pRowBuffer->pAllotRow()) {
  
    i8 iPtnKeyHash = 0;
    
    pRowBuffer->vDepositColVal(riNextKey);
    
    
    
    HshLib::vRunningHash(&iPtnKeyHash,riNextKey);
    pRowBuffer->vDepositColVal(iPtnKeyHash);

    if (10001 == ++riNextKey)
      riNextKey = 0;
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PuGen::vGen100k(i8& riNextKey, ToRow* pToRow, StreamDef* pStreamDef)
{
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PuGen::vGen1m(i8& riNextKey, ToRow* pToRow, StreamDef* pStreamDef)
{
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PuGen::vGen10m(i8& riNextKey, ToRow* pToRow, StreamDef* pStreamDef)
{
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
