/*============================================================================*/
/* SP_Node.hpp - node sprint helper                                           */
/*============================================================================*/
#pragma once

namespace Grids
{
  class SP_Node : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Node(void);
   ~SP_Node(void) noexcept(false);
  
    Stz* sz(Node* pNode);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
