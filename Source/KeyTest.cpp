/*============================================================================*/
/* KeyTest.cpp -- key library / test                                          */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/* key testing                                                                */
/*----------------------------------------------------------------------------*/
void KeyTest::vTest(void)
{
  vTrace("\n\nKeyTest:*************************************************\n"_Str);

  KeyWsp xKeyWsp;
  ValWsp xValWsp;
  KeyVal* pKeyVal;
  
  Column* pCol = new Column("Col0"_Str);
  ColumnVec* pColVec = new ColumnVec;
  pColVec->vAdd(pCol);
  
  KeyVec* pKeyVec = new KeyVec;
  
  for (i8 i = 0; i < 10; i++) {
  
    xValWsp = i;
    
    xKeyWsp.vReset();
    xKeyWsp.vAddElement(xValWsp);
    
    pKeyVal = new KeyVal(xKeyWsp);
    
    pKeyVec->vAdd(pKeyVal);
  }
  
  i8 v;
  i8 nBytes;
  char* aCursor;
  
  for (i8 i = 0; i < 10; i++) {
    
    pKeyVal = pKeyVec->pGet(i);
    
//    v = *(i8*)pKeyVal->m_xKey.aWspArea();
    aCursor = pKeyVal->m_xKey.aWspArea();
    v = *(i8*)pKeyVal->aElement(pCol,nBytes,&aCursor);
    
    vTrace("%lld: %lld\n"_Str,i,v);
  }
  
  delete pKeyVec;
  
  vTrace("\n\nEnd KeyTest**********************************************\n"_Str);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
