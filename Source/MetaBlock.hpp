/*============================================================================*/
/* MetaBlock.hpp - Meta Memory Block                                          */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MetaBlock : public Object,
                    public ListItem_t<MetaBlock>,
                    public List_t<MetaCell>
  {
  public:
    s32 m_sBlockNm;  // name of this meta block
    eMS m_eBlock;    // kind of meta block

    i8  m_nSize;     // rolled up size of this block

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaBlock(Stz* szBlockNm, eMS eBlock);
   ~MetaBlock(void) noexcept(false);
  
    eMS  eBlock(void);
    void vBlock(eMS eBlock);
  
    void vPrint(i8 i_Tab);
  
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
