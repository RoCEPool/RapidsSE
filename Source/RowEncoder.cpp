/*============================================================================*/
/* RowEncoder.hpp -- Row Encoder                                              */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowEncoder::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_ROWENCODER);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RowEncoder::operator delete(void* pAddr)
{
  RowEncoder* pRowEncoder = reinterpret_cast<RowEncoder*>(pAddr);
  
  HeapMem::vFree(pRowEncoder);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowEncoder::RowEncoder(RowParts* pRowParts)
: Object(CN_ROWENCODER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODER,_FOOTER_);
  
  m_pRowParts = pRowParts;
  m_i_Offset = 8;
  
  _checkInitialised_(CN_ROWENCODER);
  
  m_pRowParts->vReset();
  i8 i_Head = m_pRowParts->iAllotHead();
  RASSERT(0 == i_Head);
  
  CacheLine* aCacheLine = m_pRowParts->aGet(i_Head);
  
  RowCoder::vCacheLineInit(aCacheLine);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowEncoder::~RowEncoder(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* encode given datum into cache line                                         */
/*----------------------------------------------------------------------------*/
void RowEncoder::vEncode(Column* pColumn, Datum* pDatum, i8& iHash)
{
  i8 i_Column = pColumn->iColumnNo();
  
  //! don't encode columns 16+                                             (fix)
  if (i_Column >= 16)
    return;
  
  CacheLine* aCacheLine = m_pRowParts->aGet(0);
  
  i8 i_Tail = C_CACHELINES - 1;

  eDM eDatum = pColumn->eDatumType();
  switch (eDatum) {
    case DM_BCD:
      RowCoder::vEncodeBcd(aCacheLine,i_Tail,m_i_Offset,pColumn,pDatum,iHash);
      return;
    case DM_BOL: RowCoder::vEncodeBol(aCacheLine,i_Tail,m_i_Offset,pColumn,pDatum,iHash);
      return;
    case DM_DTE: RowCoder::vEncodeDte(aCacheLine,i_Tail,m_i_Offset,pColumn,pDatum,iHash);
      return;
    case DM_FLT: RowCoder::vEncodeFlt(aCacheLine,i_Tail,m_i_Offset,pColumn,pDatum,iHash);
      return;
    case DM_INT: RowCoder::vEncodeInt(aCacheLine,i_Tail,m_i_Offset,pColumn,pDatum,iHash);
      return;
    case DM_IVL: RowCoder::vEncodeIvl(aCacheLine,i_Tail,m_i_Offset,pColumn,pDatum,iHash);
      return;
    case DM_TME: RowCoder::vEncodeTme(aCacheLine,i_Tail,m_i_Offset,pColumn,pDatum,iHash);
      return;
    case DM_TSP: RowCoder::vEncodeTsp(aCacheLine,i_Tail,m_i_Offset,pColumn,pDatum,iHash);
      return;
    case DM_VCH: RowCoder::vEncodeVch(aCacheLine,i_Tail,m_i_Offset,pColumn,pDatum,iHash);
      return;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* return the row parts tray                                                  */
/*----------------------------------------------------------------------------*/
RowParts* RowEncoder::pRowParts(void)
{
  return m_pRowParts;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* RowEncoder::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
