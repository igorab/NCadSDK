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

int uuid_parse(const char *uuid_str, uuid_t *u)
{
  int node_tmp[6];

  int parsed=sscanf(
    uuid_str, 
    "%8x-%4x-%4x-%2x%2x-%2x%2x%2x%2x%2x%2x",
    &u->time_low, 
    &u->time_mid, 
    &u->time_hi_and_version, 
    &u->clock_seq_hi_and_reserved, 
    &u->clock_seq_low,
    &node_tmp[0],
    &node_tmp[1],
    &node_tmp[2],
    &node_tmp[3],
    &node_tmp[4],
    &node_tmp[5]
  );

  u->node[0]=(byte)node_tmp[0];
  u->node[1]=(byte)node_tmp[1];
  u->node[2]=(byte)node_tmp[2];
  u->node[3]=(byte)node_tmp[3];
  u->node[4]=(byte)node_tmp[4];
  u->node[5]=(byte)node_tmp[5];

  return parsed;
}
