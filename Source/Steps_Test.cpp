/*============================================================================*/
/* Steps_Test.cpp - test step group                                           */
/*============================================================================*/

#include "StorageEngine.h"
#include "Test.hpp"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Test(void)
{
  vAddStep("test::test0"_Stz,  &Step::vTestTest0);
  vAddStep("test::test1"_Stz,  &Step::vTestTest1);
  vAddStep("test::test2"_Stz,  &Step::vTestTest2);
  vAddStep("test::test3"_Stz,  &Step::vTestTest3);
  vAddStep("test::test4"_Stz,  &Step::vTestTest4);
  vAddStep("test::test5"_Stz,  &Step::vTestTest5);
  vAddStep("test::test6"_Stz,  &Step::vTestTest6);
  vAddStep("test::test7"_Stz,  &Step::vTestTest7);
  vAddStep("test::test8"_Stz,  &Step::vTestTest8);
  vAddStep("test::test9"_Stz,  &Step::vTestTest9);
  vAddStep("test::test10"_Stz, &Step::vTestTest10);
  vAddStep("test::test11"_Stz, &Step::vTestTest11);
  vAddStep("test::test12"_Stz, &Step::vTestTest12);
  vAddStep("test::test13"_Stz, &Step::vTestTest13);
  vAddStep("test::test14"_Stz, &Step::vTestTest14);
  vAddStep("test::test15"_Stz, &Step::vTestTest15);
  vAddStep("test::test16"_Stz, &Step::vTestTest16);
  vAddStep("test::test17"_Stz, &Step::vTestTest17);
  vAddStep("test::test18"_Stz, &Step::vTestTest18);
  vAddStep("test::test19"_Stz, &Step::vTestTest19);
  vAddStep("test::test20"_Stz, &Step::vTestTest20);
  vAddStep("test::test21"_Stz, &Step::vTestTest21);
  vAddStep("test::test22"_Stz, &Step::vTestTest22);
  vAddStep("test::test23"_Stz, &Step::vTestTest23);
  vAddStep("test::test24"_Stz, &Step::vTestTest24);
  vAddStep("test::test25"_Stz, &Step::vTestTest25);
  
}

/*----------------------------------------------------------------------------*/
/* test::test0 -- analyse llc misses                                          */
/*----------------------------------------------------------------------------*/
void Step::vTestTest0(MP_Task mpTask)
{
  ENTRY("vTestTest0"_Stz);
  
  LlcTest::vTest(mpTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test1 -- test1                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTestTest1(MP_Task mpTask)
{
  ENTRY("vTestTest1"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test2 -- test2                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTestTest2(MP_Task mpTask)
{
  ENTRY("vTestTest2"_Stz);
  
  vTest2();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test3 -- test3                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTestTest3(MP_Task mpTask)
{
  ENTRY("vTestTest3"_Stz);
  
  vTest3();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test4 -- test4                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTestTest4(MP_Task mpTask)
{
  ENTRY("vTestTest4"_Stz);
  
  vTest4();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test5 -- test5                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTestTest5(MP_Task mpTask)
{
  ENTRY("vTestTest5"_Stz);
  
  vTest5();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test6 -- test6                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTestTest6(MP_Task mpTask)
{
  ENTRY("vTestTest6"_Stz);
  
  vTest6();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test7 -- test7                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTestTest7(MP_Task mpTask)
{
  ENTRY("vTestTest7"_Stz);
  
  vTest7();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test8 -- test8                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTestTest8(MP_Task mpTask)
{
  ENTRY("vTestTest8"_Stz);
  
  vTest8();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test9 -- test9                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTestTest9(MP_Task mpTask)
{
  ENTRY("vTestTest9 "_Stz);
  
  //vTest9();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test10 -- test1                                                      */
/*----------------------------------------------------------------------------*/
void Step::vTestTest10(MP_Task mpTask)
{
  ENTRY("vTestTest10 "_Stz);
  
  vTest10();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test11 -- test1                                                      */
/*----------------------------------------------------------------------------*/
void Step::vTestTest11(MP_Task mpTask)
{
  ENTRY("vTestTest11 "_Stz);
  
  vTest11();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test12 -- test12                                                      */
/*----------------------------------------------------------------------------*/
void Step::vTestTest12(MP_Task mpTask)
{
  ENTRY("vTestTest12 "_Stz);
  
  vTest12();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test13 -- test13                                                     */
/*----------------------------------------------------------------------------*/
void Step::vTestTest13(MP_Task mpTask)
{
  ENTRY("vTestTest13 "_Stz);
  
  vTest13();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test14 -- test14                                                      */
/*----------------------------------------------------------------------------*/
void Step::vTestTest14(MP_Task mpTask)
{
  ENTRY("vTestTest14 "_Stz);
  
  vTest14();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test15 -- test15                                                      */
/*----------------------------------------------------------------------------*/
void Step::vTestTest15(MP_Task mpTask)
{
  ENTRY("vTestTest15 "_Stz);
  
  vTest15();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test16 -- test16                                                     */
/*----------------------------------------------------------------------------*/
void Step::vTestTest16(MP_Task mpTask)
{
  ENTRY("vTestTest16 "_Stz);
  
  vTest16();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test17 -- test17                                                     */
/*----------------------------------------------------------------------------*/
void Step::vTestTest17(MP_Task mpTask)
{
  ENTRY("vTestTest17 "_Stz);
  
  vTest17();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test18 -- test18                                                      */
/*----------------------------------------------------------------------------*/
void Step::vTestTest18(MP_Task mpTask)
{
  ENTRY("vTestTest18 "_Stz);
  
  vTest18();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test19 -- test19                                                      */
/*----------------------------------------------------------------------------*/
void Step::vTestTest19(MP_Task mpTask)
{
  ENTRY("vTestTest19 "_Stz);
  
  vTest19();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test20 -- test20                                                     */
/*----------------------------------------------------------------------------*/
void Step::vTestTest20(MP_Task mpTask)
{
  ENTRY("vTestTest20 "_Stz);
  
  vTest20();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test21 -- test21                                                     */
/*----------------------------------------------------------------------------*/
void Step::vTestTest21(MP_Task mpTask)
{
  ENTRY("vTestTest21 "_Stz);
  
  vTest21();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test22 -- test22                                                     */
/*----------------------------------------------------------------------------*/
void Step::vTestTest22(MP_Task mpTask)
{
  ENTRY("vTestTest22 "_Stz);
  
  vTest22();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test23 -- test23                                                     */
/*----------------------------------------------------------------------------*/
void Step::vTestTest23(MP_Task mpTask)
{
  ENTRY("vTestTest23 "_Stz);
  
  vTest23();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test24 -- test24                                                     */
/*----------------------------------------------------------------------------*/
void Step::vTestTest24(MP_Task mpTask)
{
  ENTRY("vTestTest24 "_Stz);
  
  vTest24();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test::test25 -- test25                                                     */
/*----------------------------------------------------------------------------*/
void Step::vTestTest25(MP_Task mpTask)
{
  ENTRY("vTestTest25 "_Stz);
  
  vTest25();
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
