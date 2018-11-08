/*============================================================================*/
/* SkipCursor.hpp -- Cursor over Skip List                                    */
/*============================================================================*/
#pragma once

namespace Structures
{
  class SkipCursor : public Object
  {
    friend class SkipList;
    
  private:
    SkipNode* m_pCurrentNode;
    
    // must be last
    Footer m_xFooter;
    
  public:
    SkipCursor(void);
    ~SkipCursor(void) noexcept(false);
    
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
