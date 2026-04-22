/* Asset description file parser */

#include <frm_wrk.hpp>
#include <asset.hpp>
#include <globals.hpp>
#include <anim.hpp>


static char *asset_nextstring(void);
static char *asset_zoek(char *string);
static void  asset_rewind();
static void  asset_close(void);
static int   asset_open(char * name);

static int seqcontext_add(char *string);
static int anmcontext_add(char *string);
static int seqcontext_open(char *string);
static int anmcontext_open(char *string);
static int seqcontext_close(void);
static int anmcontext_close(void);

static char *asset_util_return_string(char *srcstring);
static char *asset_util_copy_string(char *srcstring);

char *seqname;   /* name of the current sequence */

ANIM *curanim;   /* pointer to current anim */


FILE *assetfp;
char assetstring[256];
char utilstring[200];

Cspr_frame *seqframes[100];
int seqframescnt;


static int asset_open(char * name)
{
  assetfp = fopen(name, "rb");
  if (!assetfp)
    {
      return 0;
    }


  return 1;
}


static void asset_close(void)
{
  fclose(assetfp);
}


static void asset_rewind(void)
{
  fseek( assetfp, 0L, SEEK_SET );
//  rewind(assetfp);
}


static char *asset_zoek(char *string)
{
  char *dumstring = NULL;
  short int cmpcnt;

  do
    {
      cmpcnt = 0;
      dumstring = asset_nextstring();

      while (string[cmpcnt])
  {
    if (string[cmpcnt] != dumstring[cmpcnt]) goto nextstring;
    cmpcnt++;
  }
      return dumstring;
nextstring:
      cmpcnt++;   /* dummy instruction so the label can be used */
    } while (dumstring != NULL);
  return NULL;
}


static char *asset_nextstring(void)
{
  do
    {
      if (!fgets(assetstring,250,assetfp))
  {
    return NULL;
  }
    } while (assetstring[0] == '#'); /* strip comments */
  return assetstring;
}


/*****************************************************************************/

int asset_parse(char * assetname)
{
  char *passstring;
  int inseq = 0;
  int inanm = 0;


  if (!asset_open(assetname))
    {
//      log_out("Couldn't load file %s\n",assetname);
      return 0;
    }

  log_out("PARSE STARTED\n");

  while (passstring = asset_nextstring())
    {

      if (passstring[0] == ' ')   /* we stay in the context we're in */
  {
    if (inseq == 1)
      {
        if(!seqcontext_add(passstring))
    {
      asset_close();
      log_out("Error in adding to seqcontext");
      return 0;     
    }
      }
    else if (inanm == 1)
      {
        if(!anmcontext_add(passstring))
    {
      asset_close();
      log_out("Error in adding to anmcontext");
      return 0;     
    }
      }
    else
      {
        asset_close();
        log_out("Error in script\n");
        return 0;
      }

  }
      else
  {
    if (inseq == 1)
      {
        seqcontext_close();
        inseq = 0;
      }
    else if (inanm == 1)
      {
        anmcontext_close();
        inanm = 0;
      }

    switch (passstring[0])
      {
      case 'S':
        if(!seqcontext_open(passstring))
    {
      asset_close();
      log_out("Error in opening seqcontext");
      return 0;     
    }
        inseq = 1;
        break;
      case 'A':
        if(!anmcontext_open(passstring))
    {
      asset_close();
      log_out("Error in opening anmcontext");
      return 0;     
    }
        inanm = 1;
        break;
      case 'E':
        asset_close();
        log_out("file parsed successfully");
        return 1;
      }
  }
    }

  asset_close();
  log_out("unexpected end of file");
  return 0;
}



static int seqcontext_add(char *string)
{
  char *frmname;
  Cspr_frame *newframe;
  static int totalmem = 0;

  log_out("enter: seqcontext_add");
  /* this must be a frame so lets add him */

  frmname = asset_util_return_string(string);
  newframe = new Cspr_frame(frmname, 0, 0, 0); /* make new frame */

  totalmem += (newframe->get_width() * newframe->get_height());
//  log_out("Total memory used for sprites so far: %dKb",totalmem/1024);

  vg_dll_add(allframes, newframe, VG_DLL_LAST); /* add to the total list */

  seqframes[seqframescnt] = newframe; /* add to the temp seq list */
  seqframescnt++;

  return 1;
}

static int anmcontext_add(char *string)
{
  char *seqname;
  SEQUENCE *mysequence;

  seqname = asset_util_return_string(string);

//  log_out("enter: anmcontext_add seqname: %s",seqname);

  /* lets seek the referenced sequence */

  if (!vg_dll_first(allsequences))
    {
      log_out("ERROR: No sequences specified yet");
      return 0;
    }

  do
    {
      mysequence = (SEQUENCE *) vg_dll_collect(allsequences);
      if (strcmp(mysequence->name, seqname) == 0)
  {
    curanim->sequences[curanim->cursequence] = mysequence;

    if (curanim->cursequence < SEQ_DUMMY + 1)
      {
        curanim->cursequence++;
      }
    else
      {
        log_out("warning: too much sequences for the animation specifed");
      }

    goto endofwhile;
  }
    } while (vg_dll_next(allsequences));

//  log_out("sequence %s was not yet specified",seqname);
  return 0;

endofwhile:

  return 1;
}

static int seqcontext_open(char *string)
{
  char *tempname;

  log_out("enter: seqcontext_open");
  seqframescnt = 0;
  tempname = asset_util_return_string(string);
  seqname = asset_util_copy_string(tempname);
  return 1;
}

static int anmcontext_open(char *string)
{
  char *tempname;

//  curanim = (ANIM *) malloc(sizeof(ANIM));
  curanim = (ANIM *) calloc(sizeof(ANIM), 1);

  vg_dll_add(allanims, curanim, VG_DLL_LAST);

  tempname = asset_util_return_string(string);

//  log_out("enter: anmcontext_open: animname: %s",tempname);

  curanim->name = asset_util_copy_string(tempname);
  curanim->cursequence = 0;
  curanim->stepnr = 0;
  curanim->maxstep = 0;

  return 1;
}

static int seqcontext_close(void)
{
  SEQUENCE *newsequence;

  log_out("enter: seqcontext_close");
  newsequence = (SEQUENCE *) malloc(sizeof(SEQUENCE) + (sizeof(void *)*seqframescnt));
  vg_dll_add(allsequences, newsequence, VG_DLL_LAST);  /* add to the list off all seqs */

  newsequence->nrofsteps = seqframescnt;
  
  memcpy((unsigned char *) newsequence->frame, (unsigned char *)seqframes,
   seqframescnt*sizeof(void *));

  newsequence->name = seqname; /* during the open a copy was made of the string */
  return 1;
}

static int anmcontext_close(void)
{
  log_out("enter: anmcontext_close");

  curanim->cursequence = 0;
  return 1;
}


/* function will return a string pointer with only the
   contents of what was between the quotes */

static char *asset_util_return_string(char *srcstring)
{
  int srccnt,destcnt;

  srccnt = 0;
  destcnt = 0;
  
  while(srcstring[srccnt] != '\"')
    {
      srccnt++;
    }
  
  srccnt++;
  
  while(srcstring[srccnt] != '\"')
    {
      utilstring[destcnt] = srcstring[srccnt];
      srccnt++;
      destcnt++;
    }
  utilstring[destcnt] = 0;
  return utilstring;
}


void asset_removeall(void)
{
  Cspr_frame  *frame;
  SEQUENCE    *sequence;
  ANIM        *anim;

// remove all frames;

  if(vg_dll_first(allframes))
  {
    do
    {
      frame = (Cspr_frame *) vg_dll_collect(allframes);
      if (frame)
      {
        delete frame;
        vg_dll_remcur(allframes);
      }

    } while (frame);
  }

  free(allframes);

// remove all sequences

  if(vg_dll_first(allsequences))
  {
    do
    {
      sequence = (SEQUENCE *) vg_dll_collect(allsequences);
      if (sequence)
      {
//        log_out("freeing sequence %s",sequence->name);
        free(sequence->name);
        free(sequence);
        vg_dll_remcur(allsequences);
      }
    } while (sequence);
  }

  free(allsequences);

// remove all anims

  if(vg_dll_first(allanims))
  {
    do
    {
      anim = (ANIM *) vg_dll_collect(allanims);
      if (anim)
      {
//        log_out("freeing anim %s",anim->name);
        free(anim->name);
        free(anim);
        vg_dll_remcur(allanims);
      }
    } while (anim);
  }

  free(allanims);
}


/* function will return a copy of the string */

static char *asset_util_copy_string(char *srcstring)
{
  char *newstring;
  newstring = (char *) malloc(strlen(srcstring)+1);
  strcpy(newstring,srcstring);
  return newstring;
}
