#ifndef _LVLINITS_H
#define _LVLINITS_H

void init_level_00(void);
void init_level_11(void);
void init_level_12(void);
void init_level_13(void);
void init_level_14(void);
void init_level_21(void);
void init_level_22(void);
void init_level_23(void);
void init_level_24(void);
void init_level_31(void);
void init_level_32(void);
void init_level_33(void);
void init_level_34(void);
void init_level_41(void);


void wavegoodday(void);
void flying_objects(void);
void flying_objects2(void);
void falling_objects1(void);
void falling_objects2(void);
void floating_objects(void);
void fast_objects1(void);
void circle_objects1(void);
void gen_doolhof(void);

struct SCR_TRIGGER_ITEM
{
  UINT16      x;
  UINT16      wholeregionflg;    // if 1 then the whole region triggers the function
								 // if 0 then only the exact screen-x position triggers the function
  SCR_TRIGGER_FN  scr_trigger_fn;
};


#endif

