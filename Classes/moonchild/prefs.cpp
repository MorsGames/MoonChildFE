#include <prefs.hpp>
#include <globals.hpp>

/* PREFS_CALCVALS:
   routine to calculate values which stay the same untill the preferences
   change
*/

void prefs_calcvals(void)
{
  prefs_ahead = 0;

  prefs_aheadmax = 240;

#if 0
  if (prefs->reso == PREFS_LORES)
    {
      prefs_aheadmax = 120;
    }
  else
    {
      prefs_aheadmax = 240;
    }
#endif
}


