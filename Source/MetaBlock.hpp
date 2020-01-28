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
 
    MetaGraph* m_p_MetaGraph;    // the _p_ sequence means not owned by
    MetaBlock* m_p_ParentBlock;  // parent member set

    s32 m_sBlockNm;    // name of this meta block
    i8  m_nSize;       // rolled up size of this block
    eMB m_eBlock;      // kind of meta block

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaBlock(MetaGraph* p_MetaGraph, MetaBlock* p_ParentBlock);
   ~MetaBlock(void) noexcept(false);

    MetaMember* pAddMember(Stz* szVarNm);
  
    eMB  eBlock(void);
    void vBlock(eMB eBlock);

    MetaBlock* pParentBlock(void);
  
    void vPrint(i8 i_Tab);
    void vRecPrint(i8 i_Depth);
  
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
