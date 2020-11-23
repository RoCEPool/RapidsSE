/*============================================================================*/
/* RowGenerator.cpp -- Row Generator                                          */
/*============================================================================*/

#include "Rows.h"


/*----------------------------------------------------------------------------*/
/*-- Public Static Functions -------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* generate a random row                                                      */
/*----------------------------------------------------------------------------*/
void RowGenerator::vRandom(RowSingle* pRow, StreamDef* pStreamDef)
{
  i8 nCols = pStreamDef->nColumns(C_ALL);
  
  Datum oDatum;
  
  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++) {
    Column* pCol = pStreamDef->pGetColumn(i_Col);
    pRow->vSetValue(&oDatum,pCol);
  }

  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    Column* pCol = pStreamDef->pGetColumn(i_Col);
    
    eDM eDatum = pCol->eDatum();
    
    switch (eDatum) {
      case DM_BCD: vRndBcd(pRow,pCol); break;
      case DM_BOL: vRndBol(pRow,pCol); break;
      case DM_DTE: vRndDte(pRow,pCol); break;
      case DM_FLT: vRndFlt(pRow,pCol); break;
      case DM_INT: vRndInt(pRow,pCol); break;
      case DM_IVL: vRndIvl(pRow,pCol); break;
      case DM_TME: vRndTme(pRow,pCol); break;
      case DM_TSP: vRndTsp(pRow,pCol); break;
      case DM_VCH: vRndVch(pRow,pCol); break;
      default:
        vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
               "RowGenerator::vRandom: Invalid eDatum : %lld\n"_Stz,eDatum);
    }
  }
}

/*----------------------------------------------------------------------------*/
/*-- Private Static Functions ------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* generate a random bcd column                                               */
/*----------------------------------------------------------------------------*/
void RowGenerator::vRndBcd(RowSingle* pRow, Column* pCol)
{
  Datum oDatum;
  
  DatumRandom::vRndBcd(&oDatum,pCol);
  
  pRow->vSetValue(&oDatum,pCol);
}

/*----------------------------------------------------------------------------*/
/* generate a random bol column                                               */
/*----------------------------------------------------------------------------*/
void RowGenerator::vRndBol(RowSingle* pRow, Column* pCol)
{
  Datum oDatum;
  
  DatumRandom::vRndBol(&oDatum,pCol);
  
  pRow->vSetValue(&oDatum,pCol);
}

/*----------------------------------------------------------------------------*/
/* generate a random dte column                                               */
/*----------------------------------------------------------------------------*/
void RowGenerator::vRndDte(RowSingle* pRow, Column* pCol)
{
  Datum oDatum;
  
  DatumRandom::vRndDte(&oDatum,pCol);
  
  pRow->vSetValue(&oDatum,pCol);
}

/*----------------------------------------------------------------------------*/
/* generate a random flt column                                               */
/*----------------------------------------------------------------------------*/
void RowGenerator::vRndFlt(RowSingle* pRow, Column* pCol)
{
  Datum oDatum;
  
  DatumRandom::vRndFlt(&oDatum,pCol);
  
  pRow->vSetValue(&oDatum,pCol);
}

/*----------------------------------------------------------------------------*/
/* generate a random int column                                               */
/*----------------------------------------------------------------------------*/
void RowGenerator::vRndInt(RowSingle* pRow, Column* pCol)
{
  Datum oDatum;
  
  DatumRandom::vRndInt(&oDatum,pCol);
  
  pRow->vSetValue(&oDatum,pCol);
}

/*----------------------------------------------------------------------------*/
/* generate a random ivl column                                               */
/*----------------------------------------------------------------------------*/
void RowGenerator::vRndIvl(RowSingle* pRow, Column* pCol)
{
  Datum oDatum;
  
  DatumRandom::vRndIvl(&oDatum,pCol);
  
  pRow->vSetValue(&oDatum,pCol);
}

/*----------------------------------------------------------------------------*/
/* generate a random tme column                                               */
/*----------------------------------------------------------------------------*/
void RowGenerator::vRndTme(RowSingle* pRow, Column* pCol)
{
  Datum oDatum;
  
  DatumRandom::vRndTme(&oDatum,pCol);
  
  pRow->vSetValue(&oDatum,pCol);
}

/*----------------------------------------------------------------------------*/
/* generate a random tsp column                                               */
/*----------------------------------------------------------------------------*/
void RowGenerator::vRndTsp(RowSingle* pRow, Column* pCol)
{
  Datum oDatum;
  
  DatumRandom::vRndTsp(&oDatum,pCol);
  
  pRow->vSetValue(&oDatum,pCol);
}

/*----------------------------------------------------------------------------*/
/* generate a random vch column                                               */
/*----------------------------------------------------------------------------*/
void RowGenerator::vRndVch(RowSingle* pRow, Column* pCol)
{
  Datum oDatum;
  
  DatumRandom::vRndVch(&oDatum,pCol);
  
  pRow->vSetValue(&oDatum,pCol);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
