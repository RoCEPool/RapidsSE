/*============================================================================*/
/* Row.cpp -- Row                                                             */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Row* DBobjects::_pRow(void* aAddr)
{
  Row* pRow = reinterpret_cast<Row*>(aAddr);
  
  return pRow;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Row::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: Row class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Row::Row(void)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Row::~Row(void)
{
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extract row from given raw record                                          */
/*----------------------------------------------------------------------------*/
bool Row::bFormatRow(ColumnVec* pColVec,
                     Formatters* pFormatters,
                     Stz* aRawData, i8 iRawLength,
                     Stz* aRowData, i8& iRowLength,
                     bool &bBlankLine)
{
  bool bValid = RowData::bFormatData(pColVec,
                                     pFormatters,
                                     aRawData, iRawLength,
                                     aRowData, iRowLength,
                                     bBlankLine);
  
  return bValid;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert row to binary string                                               */
/*----------------------------------------------------------------------------*/
void Row::vAsBinaryString(s48K& sDataString, ColumnVec* pColVec)
{
  i8 i_RowLength = 0;
  char* pOffset;
  Stz* aData;
  
  i8 nColumns = pColVec->nItems();
  
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {
    
    Column* pColumn = pColVec->pGet(i_Column);
    
    if (i_Column != 0)
      sDataString.vConcat(","_Stz);
    
    unsigned char* pOffset = (unsigned char*)(this) + i_Column*C_COLUMN_OFFSET_SIZE;
    unsigned char iOffset = *(pOffset);
    
    // move to offset
    aData = (Stz*)((unsigned char*)(this) + iOffset);
    
    s256 s;
    DtmLib::vSprint(pColumn->eDatumType(), s, aData, pColumn);
    if (i_Column == 0)
      sDataString.vCopy(s);
    else
      sDataString.vConcat(s);
  }
  
  
}

/*----------------------------------------------------------------------------*/
/* convert row to escaped string                                              */
/*----------------------------------------------------------------------------*/
void Row::vAsEscapedString(s48K& sDataString, ColumnVec* pColVec)
{
  RowData::vRowAsEscapedString(sDataString,(Stz*)this,pColVec);
}

/*----------------------------------------------------------------------------*/
/* get address of a column datum                                              */
/*----------------------------------------------------------------------------*/
char* Row::aColumnDatum(i8 iColumnNo)
{
  char* aDatum = RowData::aColumnDatum((char*)this,iColumnNo);
  
  return aDatum;
}

/*----------------------------------------------------------------------------*/
/* dump row                                                                   */
/*----------------------------------------------------------------------------*/
void Row::vDump(ColumnVec* pColVec)
{
  RowData::vDump((Stz*)this,pColVec);
}

/*----------------------------------------------------------------------------*/
/* show row                                                                   */
/*----------------------------------------------------------------------------*/
void Row::vShow(ColumnVec* pColVec)
{
  RowData::vShow_TBR((Stz*)this,pColVec);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
