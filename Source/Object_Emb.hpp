/*============================================================================*/
/* Obj.hpp -- Base Class for Proper Objects (short form)                      */
/*============================================================================*/
#pragma once

namespace CorrectnessAlgebra
{
  class Object_Emb :public Magic,
  public Class
  {
    private:
    bool m_bWordBody;
    char m_sFiller[3];
    
    public:
    Object_Emb(eCN eClass);
    
    bool bWordBody(void);
    void vWordBody(bool bIsWordBody);
    
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
