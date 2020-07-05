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
  
  MemLib::vSet(m_caModel,' ',sizeof(m_caModel),CN_ROWSLAB);
  m_caModel[sizeof(m_caModel) - 1] = '\0';
  
  StrLib::vSprint(m_caModel,"RapidsSE %s FORMAT %s"_Stz,
                  RAPIDSSE_VERSION_STRING,
                  RAPIDSSE_FORMAT_STRING);
  
  m_iVersion = RAPIDSSE_VERSION_NUMBER;
  m_iFormat  = RAPIDSSE_FORMAT_NUMBER;
  m_nBytes   = sizeof(RowSlab);
  m_nTrimmed = 0;
  m_i_Trim   = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_nItems   = 0;
  m_i_Row    = C_UNKNOWN;
  m_i_Cell   = C_UNKNOWN;
 
  // m_oStreamPix
  
  MemLib::vZero(m__PadToCacheLine,sizeof(m__PadToCacheLine),CN_ROWSLAB);
  
  // m_oNmlStr

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
/* get number of columns                                                      */
/*----------------------------------------------------------------------------*/
i8 RowSlab::nCols(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  return m_oStreamPix.m_nCols;
}

/*----------------------------------------------------------------------------*/
/* get the condensed column pix array                                         */
/*----------------------------------------------------------------------------*/
ColPix* RowSlab::axaColPix(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  return m_oStreamPix.m_xaColPix;
}

/*----------------------------------------------------------------------------*/
/* copy given stream pix into the stream pix in this rsb                      */
/*----------------------------------------------------------------------------*/
void RowSlab::vCopyPix(StreamPix* pStreamPixSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  MemLib::vCopy(&m_oStreamPix,pStreamPixSrc,sizeof(StreamPix),CN_ROWSLAB);
}

/*----------------------------------------------------------------------------*/
/* copy given source rsb into this current row of this row slab               */
/*----------------------------------------------------------------------------*/
void RowSlab::vCopyRsb(Rsb* aRsbSrc, bool bSrcHasPtnHash, bool bCopyPtnHash)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  i8 iVal;
  Stz* szSrcStz;
  
  i8 nCols = m_oStreamPix.m_nCols;
  
  i8 i_Col;
  i8 i_Cell_ntv;
  
  for (i_Col = 0; i_Col < nCols; i_Col++) {
    
    eDM eDatum = (eDM)m_oStreamPix.m_xaColPix[i_Col].m_uDatum;
    
    
    i_Cell_ntv = aRsbSrc->i_ColToCell_ntv(i_Col);
    
    
    if (DM_VCH == eDatum) {
       // vch datum

      if (C_DATUM_NULL == aRsbSrc->iVal_ntv(i_Cell_ntv)) {
        vDeposit(C_DATUM_NULL);
      }
      else {
        szSrcStz = aRsbSrc->szVal_ntv(i_Cell_ntv);
        vDeposit(szSrcStz);
      }
    }
    else {
      // all others
     
      iVal = aRsbSrc->iVal_ntv(i_Cell_ntv);
      vDeposit(iVal);
    }
  }

  if (bSrcHasPtnHash && bCopyPtnHash) {
    i_Cell_ntv = aRsbSrc->i_ColToCell_ntv(i_Col);
    iVal = aRsbSrc->iVal_ntv(i_Cell_ntv);
    vDeposit(iVal);
  }
  
//  Rsb* aRsbDst = aRsbAdr(nRows() - 1);
//  SP_RowPix spPix(nCols,m_oStreamPix.m_xaColPix);
//  vTrace("CopyRsb: %s\n"_Stz,spPix.sz(aRsbDst));
}

/*----------------------------------------------------------------------------*/
/* return column value from rsb for given column number                       */
/*----------------------------------------------------------------------------*/
i8 RowSlab::iColumn(Rsb* aRsb, i8 iColumn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  i8 iVal;
  Stz* szSrcStz;
    
  eDM eDatum = (eDM)m_oStreamPix.m_xaColPix[iColumn].m_uDatum;
  
  i8 i_Cell_ntv = aRsb->i_ColToCell_ntv(iColumn);
  
  if (DM_VCH == eDatum) {
    // vch datum
    
    if (C_DATUM_NULL == aRsb->iVal_ntv(i_Cell_ntv)) {
      return C_DATUM_NULL;
    }
    else {
      szSrcStz = aRsb->szVal_ntv(i_Cell_ntv);
      return (i8)szSrcStz;
    }
  }
  else {
    // all others
    
    iVal = aRsb->iVal_ntv(i_Cell_ntv);
    return iVal;
  }
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
/* dump normalised string table                                               */
/*----------------------------------------------------------------------------*/
void RowSlab::vDumpNst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  m_oNmlStr.vDump();
}

/*----------------------------------------------------------------------------*/
/* dump given row                                                             */
/*----------------------------------------------------------------------------*/
void RowSlab::vDumpRow(i8 i_Row)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  SP_RowPix spRowPix(this->nCols(),this->axaColPix());
  
  Rsb* aRsb = this->aRsbAdr(i_Row);
  Stz* szRow = spRowPix.sz(aRsb);
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
        
  pStreamDef->vToStreamPix(&m_oStreamPix,bPtnHash);
}

/*----------------------------------------------------------------------------*/
/* start of new row                                                           */
/*----------------------------------------------------------------------------*/
bool RowSlab::bNewRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  i8 nBytesAvail = m_xWsp.nFree() - C_NML_STR_SPACE - (i8)C_BYTES_PER_CACHE_LINE;
  
  if (nBytesAvail < m_oStreamPix.m_nMaxRowSize)
    return false;
  
  if (C_UNKNOWN == m_i_Cell)
    m_i_Cell = C_FIRST_LGL_CELL + 1;
  else {
    //ASSERT(m_i_Cell == m_i_Row - nCols());
    
    m_i_Cell = m_i_Row - nCols();
  }
  
  m_i_Row = m_i_Cell;
  
  m_nItems++;
  
  i8 nCellBytes = m_oStreamPix.m_nCols * C_BYTES_PER_WORD;
  m_xWsp.vTakeTail(nCellBytes);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get normalised string set                                                  */
/*----------------------------------------------------------------------------*/
NmlStr* RowSlab::pNmlStr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  return & m_oNmlStr;
}

/*----------------------------------------------------------------------------*/
/* normalize repeating strings                                                */
/*----------------------------------------------------------------------------*/
void RowSlab::vNormaliseStrings(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  //m_oNmlStr.vDump();

  i8 nTimes = 0;
  i8 nRows  = this->nRows();
  i8 nCols  = this->nCols();
  
//for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
  for (i8 i_Col = 13; i_Col < 14; i_Col++) {

    eDM eDatum = (eDM)m_oStreamPix.m_xaColPix[i_Col].m_uDatum;
    
    if (DM_VCH != eDatum)
      continue;
    

    for (i8 i_OuterRow = 0; i_OuterRow < 100; i_OuterRow++) {
      
      Rsb* aOuterRsb = aRsbAdr(i_OuterRow);
      i8 i_OuterCell_ntv = aOuterRsb->i_ColToCell_ntv(i_Col);
      
      if (C_DATUM_NULL == aOuterRsb->iVal_ntv(i_OuterCell_ntv))
        continue;

      Stz* szOuterStz = aOuterRsb->szVal_ntv(i_OuterCell_ntv);
      //vTrace("Outer: %s\n"_Stz,szOuterStz);
      
      i8 nLength = szOuterStz->nLength();
      if (nLength < 8)
        continue;
      
      
    //for (i8 i_InnerRow = i_OuterRow + 1; i_InnerRow < nRows; i_InnerRow++) {
      for (i8 i_InnerRow = i_OuterRow + 1; i_InnerRow < i_OuterRow + 99; i_InnerRow++) {

        Rsb* aInnerRsb = aRsbAdr(i_InnerRow);
        i8 i_InnerCell_ntv = aInnerRsb->i_ColToCell_ntv(i_Col);

        if (C_DATUM_NULL == aInnerRsb->iVal_ntv(i_InnerCell_ntv))
          continue;

        Stz* szInnerStz = aInnerRsb->szVal_ntv(i_InnerCell_ntv);
        //vTrace("Inner: %s\n"_Stz,szInnerStz);

        if (! szInnerStz->bEqual(szOuterStz))
          continue;

        //vTrace("Testing : %lld - %lld : %s\n"_Stz,i_OuterRow,i_InnerRow,szOuterStz);
        if (! m_oNmlStr.szFind(szOuterStz)) {
          m_oNmlStr.szAdd(szOuterStz);
          if (++nTimes >= 3)
            goto L_Exit;
        }
        
        break;
        
      } // each inner row
      
    } // each outer row

  } // each col
  
L_Exit:
  ; //m_oNmlStr.vDump();
}

/*----------------------------------------------------------------------------*/
/* position on given cell in current rsb                                      */
/*----------------------------------------------------------------------------*/
void RowSlab::vPositionOnCell(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  m_i_Cell = m_i_Row - i_Col;
}

/*----------------------------------------------------------------------------*/
/* is there a ptn hash?                                                       */
/*----------------------------------------------------------------------------*/
bool RowSlab::bPtnHash(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  return m_oStreamPix.m_bPtnHash;
}

/*----------------------------------------------------------------------------*/
/* get the partition of rsb                                                   */
/*----------------------------------------------------------------------------*/
i8 RowSlab::iPtnOfRsb(Rsb* aRsb)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  i8 i_HshCol = m_oStreamPix.m_nCols - 1;
  
  i8 i_HashCell_ntv = aRsb->i_ColToCell_ntv(i_HshCol);
  
  i8 i_Ptn = aRsb->iVal_ntv(i_HashCell_ntv);
  
  return i_Ptn;
}

/*----------------------------------------------------------------------------*/
/* get address of an rsb                                                      */
/*----------------------------------------------------------------------------*/
Rsb* RowSlab::aRsbAdr(i8 i_Row)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  i8 iLglRow = i_Row * m_oStreamPix.m_nCols;
  
  i8 i_RowCells = C_FIRST_LGL_CELL - iLglRow;

  m8* amCellVec = amSlabCellVec();

  Rsb* aRsb = (Rsb*) & amCellVec[i_RowCells];
  
  return aRsb;
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
/* get the condensed stream def                                               */
/*----------------------------------------------------------------------------*/
StreamPix* RowSlab::aStreamPix(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  return & m_oStreamPix;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the cell vector for this slab                                          */
/*----------------------------------------------------------------------------*/
m8* RowSlab::amSlabCellVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  m8* amCellArea = (m8*)m_xWsp.ayArea();
  
  return amCellArea;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
