
extern void assem_test(BYTE **linestart, COMP_SPRITE compspr, 
				 UINT16 x);
#pragma aux assem_test = \
  "call eax" \
  parm [ebx] [eax] [ecx];
