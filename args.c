/*
 * args.c - (C) David Riesz 2022
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published
 *   by the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with this program; if not, write to the Free Software Foundation,
 *   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <asm-generic/errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "rigsync.h"

void usage(int argc, char **argv);

void read_args(int argc, char **argv)
{
  int ii;
  struct rs_rig *rig = NULL;
  for(ii=1 ; ii<argc ; ii++)
  {
    if(!(strcmp(argv[ii], "-m")))
    {
      if((++ii)>=argc) { fprintf(stderr, "Argument error.\n"); exit(1); }
      rig = add_rig(atoi(argv[ii]));
    }
    else if(!(strcmp(argv[ii], "-s")))
    {
      if(rig == NULL)
      {
        fprintf(stderr, "Rig definition must begin with a \"-m\" argument.\n");
        exit(1);
      }
      if((++ii)>=argc) { fprintf(stderr, "Argument error.\n"); exit(1); }
      set_rig_speed(rig, atoi(argv[ii]));
    }
    else if(!(strcmp(argv[ii], "-r")))
    {
      if(rig == NULL)
      {
        fprintf(stderr, "Rig definition must begin with a \"-m\" argument.\n");
        exit(1);
      }
      if((++ii)>=argc) { fprintf(stderr, "Argument error.\n"); exit(1); }
      set_rig_port(rig, argv[ii]);
    }
    else if(!(strcmp(argv[ii], "-n")))
    {
      if((++ii)>=argc) { fprintf(stderr, "Argument error.\n"); exit(1); }
      set_rig_sync_init(atoi(argv[ii])-1);
      if(get_rig_sync_init() >= rig_count())
      {
        fprintf(stderr, "Sync startup argument error.\n");
        exit(1);
      }
    }
    else if(!(strcmp(argv[ii], "--ignore-digital")))
    {
      if (rig != NULL)
      {
        rig->ignore_digital_mode = true;
        gprintf(1, "ignoring special digital modes of TRX\n");
      }
    }
#ifdef DEBUG
    else if(!(strcmp(argv[ii], "-d")))
    {
      if((++ii)>=argc) { fprintf(stderr, "Argument error.\n"); exit(1); }
      set_debug_level(atoi(argv[ii]));
    }
#endif
    else if(!(strcmp(argv[ii], "-h")))
    {
      usage(argc, argv);
    }
    else { usage(argc, argv); }
  }
  if(rig_count() < 2)
  {
    fprintf(stderr, "At least two rigs must be defined.\n");
    exit(1);
  }
  gprintf(1, "rig count = %d\n", rig_count());
}

void usage(int argc, char **argv)
{
    fprintf(stderr, "Usage: %s <RIG_DEF> <RIG_DEF> [<RIG_DEF>] [OPTIONS]\n", argv[0]);
    fprintf(stderr, "\n");
    fprintf(stderr, "Keeps defined rigs in sync according to options\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "OPTIONS:\n");
    fprintf(stderr, "  -h                 display this help\n");
    fprintf(stderr, "  -n <rig number>    number of the rig the others are synced to on startup (default: 1)\n");
#ifdef DEBUG
    fprintf(stderr, "  -d <level>         debug output verbosity\n");
#endif
    fprintf(stderr, "\n");
    fprintf(stderr, "Rig Definition:\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  RIG_DEF = -m <model_number> [-r <port/conn.>] [-s <baud rate>] [--ignore-digital]\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "RIG_DEF options:\n");
    fprintf(stderr, "  -m <model number>    Set the model number according to hamlib ( rigctl -l )\n");
    fprintf(stderr, "  -r <port/conn.>      Connection to the rig e.g. /dev/ttyUSB0 or IP:PORT (HamlibNet)\n");
    fprintf(stderr, "  -s <baud rate>       baud rate using serial connection\n");
    fprintf(stderr, "  --ignore-digital     to convert PKTLSB/PKTUSB to normal LSB/USB.\n");
    fprintf(stderr, "                       Helps when syncing e.g. Icom IC-7300 and QrpLabs QDX\n");
    fprintf(stderr, "                       (QDX is not capable of PKTUSB/PKTLSB)\n");
  exit(1);
}
