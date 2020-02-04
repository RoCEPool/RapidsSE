/*============================================================================*/
/* MetaBlock.hpp - Meta Block                                                 */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MetaBlock : public Object,
                    public ListItem_t<MetaBlock>,
                    public List_t<MetaMember>
  {
  public:
    MetaBlock* m_p_ParentBlock;  // parent meta block

    s32 m_sBlockNm;  // name of this meta block
    eMS m_eBlock;    // kind of meta block

    i8  m_nSize;     // rolled up size of this block

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaBlock(Stz* szBlockNm, eMS eBlock, MetaBlock* p_ParentBlock);
   ~MetaBlock(void) noexcept(false);
  
    eMS  eBlock(void);
    void vBlock(eMS eBlock);

    MetaBlock* p_ParentBlock(void);
  
    void vPrint(i8 i_Tab);
    void vRecursivePrint(i8 i_Depth);
  
    Stz* szBlockNm(void);
    void vBlockNm(Stz* szBlockNm);

    i8   nSize(void);
  
    Footer* aFooter(void);
  };

  MetaBlock* _pMetaBlock(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
