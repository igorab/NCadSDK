/*
** The code in this file is derived from a sample implementation
** found in RFC 4122 which has the following copyright notice:
**
** Copyright (c) 1990- 1993, 1996 Open Software Foundation, Inc.
** Copyright (c) 1989 by Hewlett-Packard Company, Palo Alto, Ca. &
** Digital Equipment Corporation, Maynard, Mass.
** Copyright (c) 1998 Microsoft.
** To anyone who acknowledges that this file is provided "AS IS"
** without any express or implied warranty: permission to use, copy,
** modify, and distribute this file for any purpose is hereby
** granted without fee, provided that the above copyright notices and
** this notice appears in all source code copies, and that none of
** the names of Open Software Foundation, Inc., Hewlett-Packard
** Company, Microsoft, or Digital Equipment Corporation be used in
** advertising or publicity pertaining to distribution of the software
** without specific, written prior permission. Neither Open Software
** Foundation, Inc., Hewlett-Packard Company, Microsoft, nor Digital
** Equipment Corporation makes any representations about the
** suitability of this software for any purpose.
*/

#include <stdio.h>
#include "sysdep.h"
#include "uuid.h"

void uuid_unparse(const uuid_t *u, char *uuid_str)
{
  int i;

  sprintf(
    uuid_str, 
    "%8.8X-%4.4X-%4.4X-%2.2X%2.2X-", 
    u->time_low, 
    u->time_mid,
    u->time_hi_and_version, 
    u->clock_seq_hi_and_reserved,
    u->clock_seq_low
  );
  
  for (i = 0; i < 6; i++)
    sprintf(uuid_str+24+2*i, "%2.2X", u->node[i]);
}
