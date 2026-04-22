#ifndef _TRIGGER_H
#define _TRIGGER_H

#include <objects.hpp>

void madeit_00(void);
void reallymadeit_00(void);
void trigger(void);
void bordwissel_11(void);
void sneaktransfer_11(void);
void madeit_11(void);
void reallymadeit_11(void);
void madeit_12(void);
void reallymadeit_12(void);
void warp_13(void);
void madeit_13(void);
void reallymadeit_13(void);
void sneaktransfer_13(void);
void sneaktransfer2_13(void);
void sneaktransfer3_13(void);
void sneaktransfer4_13(void);
void earthquake_13(void);
void loginitter_13(void);
void warp_14(void);
void stopautoscroll_14(void);
void stopautoscroll2_14(void);
void startautoscroll_14(void);
void stopfastrun_14(void);
void madeit_14(void);
void reallymadeit_14(void);
void notkneeldood_21(void);
void madeit_21(void);
void reallymadeit_21(void);
void warp_22(void);
void sneaktransfer_22(void);
void floatingobj_22(void);
void fanl1_22(void);
void fanr1_22(void);
void fanr2_22(void);
void fanb1_22(void);
void fanb2_22(void);
void drunk_22(void);
void madeit_22(void);
void reallymadeit_22(void);
void sneaktransfer_23(void);
void sneaktransfer2_23(void);
void sneaktransfer3_23(void);
void tyres_23(void);
void madeit_23(void);
void reallymadeit_23(void);
void changepara_31(void);
void changepara2_31(void);
void changepara3_31(void);
void opendoor_31(void);
void opendoor2_31(void);
void restart_31(void);
void madeit_31(void);
void reallymadeit_31(void);
void shootenable_31(void);
void verkleur_32(void);
void resetsoko_32(void);
void madeit_32(void);
void reallymadeit_32(void);
void restartcnt_33(void);
void restart_33(void);
void restart2_33(void);
void madeit_33(void);
void reallymadeit_33(void);
void shootenable_34(void);
void restart_34(void);
void madeit_34(void);
void reallymadeit_34(void);
void changepara_41(void);
void changepara2_41(void);
void changepara3_41(void);
void changepara4_41(void);
void changepara5_41(void);
void changepara8_41(void);
void camcircle_41(void);
void flipcam_41(void);
void madeit_41(void);
void reallymadeit_41(void);



struct TRIGGER_ITEM
{
  UINT16      x1;
  UINT16      y1;
  UINT16      x2;
  UINT16      y2;
  TRIGGER_FN  trigger_fn;
};


#endif
