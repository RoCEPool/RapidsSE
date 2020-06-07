/*============================================================================*/
/* RowSlab.hpp -- Row Slab                                                    */
/*============================================================================*/

#include "Rows.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Rows,RowSlab,CN_ROWSLAB)
__aNewFunction(RowSlab,CN_ROWSLAB)
__vDeleteFunction(RowSlab,CN_ROWSLAB)
__aFooterFunction(RowSlab)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowSlab::RowSlab(void)
: Object(CN_ROWSLAB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  MemLib::vSet(caModel,' ',sizeof(caModel),CN_ROWSLAB);
  caModel[31] = '\0';
  
  StrLib::vSprint(caModel,"RapidsSE %s FORMAT %s"_Stz,
                  RAPIDSSE_VERSION_STRING,
                  RAPIDSSE_FORMAT_STRING);
  
  m_iVersion = RAPIDSSE_VERSION_NUMBER;
  m_iFormat  = RAPIDSSE_FORMAT_NUMBER;
  m_nBytes   = sizeof(RowSlab);
  m_nTrimmed = 0;
  m_i_Trim   = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_nItems      = 0;
  m_nMaxRowSize = 256;  // FOR TESTING PURPOSES
  m_nCols       = C_UNKNOWN;
  m_nExtraCols  = 0;
  m_bPtnHash    = false;
  
  m_i_Cell      = C_UNKNOWN;
  
  for (i8 i_Col = 0; i_Col < C_MAX_DEFCOLUMNS; i_Col++)
    m_xaColPix[i_Col].m_uAsI4 = 0;
  
#ifdef WITH_DEBUG_VERSION
  m_xWsp.vZero(CN_ROWSLAB);
#endif

  _checkInitialised_(CN_ROWSLAB);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowSlab::~RowSlab(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get a row cell                                                             */
/*----------------------------------------------------------------------------*/
i8 RowSlab::iCell(i8 iRow, i8 iCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  i8* iaCells = (i8*)m_xWsp.ayArea();

  i8 iLglCell = iRow * m_nCols + iCell;
  
  i8 i_Cell = C_FIRST_CELL - iLglCell;
  
  i8 iVal =iaCells[i_Cell];
  
  return iVal;
}

/*----------------------------------------------------------------------------*/
/* get address of a row cell                                                  */
/*----------------------------------------------------------------------------*/
m8* RowSlab::amCell(i8 i_Cell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  m8* amCellVec = amCellVector();

  m8* amCell = &amCellVec[i_Cell];
  
  return amCell;
}

/*----------------------------------------------------------------------------*/
/* get the entire cell vector                                                 */
/*----------------------------------------------------------------------------*/
m8* RowSlab::amCellVector(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  m8* amCellArea = (m8*)m_xWsp.ayArea();
  
  return amCellArea;
}

/*----------------------------------------------------------------------------*/
/* get number of columns                                                      */
/*----------------------------------------------------------------------------*/
i8 RowSlab::nCols(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  return m_nCols;
}

/*----------------------------------------------------------------------------*/
/* get row cells for given row                                                */
/*----------------------------------------------------------------------------*/
ColPix* RowSlab::axaColPix(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  return m_xaColPix;
}

/*----------------------------------------------------------------------------*/
/* copy given source rsb into this current row of this row slab               */
/*----------------------------------------------------------------------------*/
void RowSlab::vCopyRsb(Rsb* aRsbSrc, bool bCopyPtnHash)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* deposit an d8 value into next cell                                         */
/*----------------------------------------------------------------------------*/
void RowSlab::vDeposit(d8 dVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  // move to previous cell
  m_i_Cell--;
  
  d8* daCells = (d8*)m_xWsp.ayArea();
  
  daCells[m_i_Cell] = dVal;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* deposit an i8 value into next cell                                         */
/*----------------------------------------------------------------------------*/
void RowSlab::vDeposit(i8 iVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  // move to previous cell
  m_i_Cell--;
  
  i8* iaCells = (i8*)m_xWsp.ayArea();
  
  iaCells[m_i_Cell] = iVal;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* begin deposit of string                                                    */
/*----------------------------------------------------------------------------*/
char* RowSlab::azDepositString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  char* azStzCell = (char*)m_xWsp.ayLatterHead();
  
  m_i_Cell--;

  i8* iaCells = (i8*)m_xWsp.ayArea();
  
  iaCells[m_i_Cell] = (i8)azStzCell;

  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  return azStzCell;
}

/*----------------------------------------------------------------------------*/
/* take bytes for deposited string                                            */
/*----------------------------------------------------------------------------*/
void RowSlab::vDepositedString(i8 nSize)
{
   _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  m_xWsp.vTakeHead(nSize);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump given row                                                             */
/*----------------------------------------------------------------------------*/
void RowSlab::vDumpAllRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  i8 nRows = this->nRows();
  vTrace("RowSlab of %lld rows...\n"_Stz,nRows);
  for (i8 i_Row = 0; i_Row < nRows; i_Row++)
    vDumpRow(i_Row);
}

/*----------------------------------------------------------------------------*/
/* dump given row                                                             */
/*----------------------------------------------------------------------------*/
void RowSlab::vDumpRow(i8 i_Row)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  SP_RowPix spRowPix(this->nCols(),this->axaColPix());
  
  m8* amRowCells = this->amRowCells(i_Row);
  Stz* szRow = spRowPix.sz(amRowCells);
  vTrace("[%lld] %s\n"_Stz,i_Row,szRow);
}

/*----------------------------------------------------------------------------*/
/* get number of rows                                                         */
/*----------------------------------------------------------------------------*/
i8 RowSlab::nFreeSpace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  i8 nFree = m_xWsp.nFree();
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* initilize col pix                                                          */
/*----------------------------------------------------------------------------*/
void RowSlab::vInitPix(StreamDef* pStreamDef, bool bPtnHash)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  m_nCols       = pStreamDef->nColumns(C_ALL);
  m_bPtnHash    = bPtnHash;
  if (m_bPtnHash)
    m_nCols++;
  m_i_Cell      = C_UNKNOWN;
  
  //                                                           optimise this
  for (i8 i_Col = 0; i_Col < C_MAX_DEFCOLUMNS; i_Col++)
    m_xaColPix[i_Col].m_uAsI4 = 0;
  
  pStreamDef->vToColPix(m_xaColPix,bPtnHash);
}

/*----------------------------------------------------------------------------*/
/* start of new row                                                           */
/*----------------------------------------------------------------------------*/
bool RowSlab::bNewRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  i8 nBytesAvail = m_xWsp.nFree() - C_NML_STR_SPACE - (i8)C_BYTES_PER_CACHE_LINE;
  
  if (nBytesAvail < m_nMaxRowSize)
    return false;
  
  if (C_UNKNOWN == m_i_Cell)
    m_i_Cell = C_FIRST_CELL + 1;
    
  m_nItems++;
  
  i8 nCellBytes = m_nCols * C_BYTES_PER_WORD;
  m_xWsp.vTakeTail(nCellBytes);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* is there a ptn hash?                                                       */
/*----------------------------------------------------------------------------*/
bool RowSlab::bPtnHash(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  return m_bPtnHash;
}

/*----------------------------------------------------------------------------*/
/* get row cells for given row                                                */
/*----------------------------------------------------------------------------*/
Rsb* RowSlab::aRsb(i8 i_Row)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  Rsb* aRsb = (Rsb*)amRowCells(i_Row);
  
  return aRsb;
}

/*----------------------------------------------------------------------------*/
/* get row cells for given row                                                */
/*----------------------------------------------------------------------------*/
m8* RowSlab::amRowCells(i8 i_Row)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  i8 iLglRow = i_Row * m_nCols;
  
  i8 i_RowCells = C_FIRST_CELL - iLglRow;

  m8* amCellVec = amCellVector();

  m8* amRowCells = &amCellVec[i_RowCells];
  
  return amRowCells;
}

/*----------------------------------------------------------------------------*/
/* get number of rows                                                         */
/*----------------------------------------------------------------------------*/
i8 RowSlab::nRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* dump slap and report                                                       */
/*----------------------------------------------------------------------------*/
void RowSlab::vSlabReport(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
 
  s32 sSlab_magn;  MagLib::vByteMag(C_WSP_ROWSLAB, sSlab_magn);
  s32 sUsed_magn;  MagLib::vByteMag(m_xWsp.nUsed(),sUsed_magn);
  s32 sFree_magn;  MagLib::vByteMag(m_xWsp.nFree(),sFree_magn);

  vTrace("RowSlab space %s, used %s, free %s %lld%%; "
         "rows %lld, %lld(B) per row\n"_Stz,
         
         sSlab_magn.szStz(),
         sUsed_magn.szStz(),
         sFree_magn.szStz(),
         (m_xWsp.nFree() * 100) / C_WSP_ROWSLAB,
         
         nRows(),
         m_xWsp.nUsed() / nRows()
         );
}



/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate column hints                                                      */
/*----------------------------------------------------------------------------*/
void RowSlab::vColHints(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
