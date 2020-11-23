/*============================================================================*/
/* MetaCell.hpp - Meta Block Cell                                             */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MetaCell : public Object,
                   public ListItem_t<MetaCell>
  {
  public:
    MetaBlock*  m_p_WithinBlock; // this cell is within which meta block
    MetaMember* m_p_MetaMember;  // which member (in meta structure)

    s32         m_sCellNm;       // name of the cell
    i8          m_i_Cell;        // cell in block
    i8          m_i_Offset;      // byte offset in block
    i8          m_nSize;         // size of the cell
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaCell(Stz* szCellNm, i8 i_Cell, i8 i_Offset, i8 nSize,
             MetaBlock* p_WithinBlock, MetaMember* p_MetaMember);
   ~MetaCell(void) noexcept(false);
  
    Stz*         szCellNm(void);
    MetaBlock*   p_WithinBlock(void);
    MetaMember*  p_MetaMember(void);
    void         vPrint(i8 i_Tab);
    i8           nSize(void);

    Footer* aFooter(void);
  };

  MetaCell* _pMetaCell(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
