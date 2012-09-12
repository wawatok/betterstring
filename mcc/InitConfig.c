/***************************************************************************

 BetterString.mcc - A better String gadget MUI Custom Class
 Copyright (C) 1997-2000 Allan Odgaard
 Copyright (C) 2005-2009 by BetterString.mcc Open Source Team

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 BetterString class Support Site:  http://www.sf.net/projects/bstring-mcc/

 $Id$

***************************************************************************/

#include <string.h>

#include <clib/alib_protos.h>
#include <proto/intuition.h>
#include <proto/muimaster.h>
#include <proto/graphics.h>
#include <proto/diskfont.h>
#include <proto/dos.h>

#include "BetterString_mcp.h"
#include "private.h"

#include "Debug.h"

static ULONG GetCol(Object *obj, ULONG item, struct MUI_PenSpec *defaultcol)
{
  struct MUI_PenSpec *spec;

  if(DoMethod(obj, MUIM_GetConfigItem, item, &spec) == 0)
    spec = defaultcol;

  return MUI_ObtainPen(muiRenderInfo(obj), spec, 0L);
}

void InitConfig(Object *obj, struct InstData *data)
{
  IPTR setting;

  ENTER();

  if(isFlagSet(data->Flags, FLG_SetFrame) && MUIMasterBase->lib_Version >= 20)
  {
    // don't remember the string from the configuration but copy it
    // with MUI4's realtime prefs the configuration might change upon
    // canceling MUI prefs and hence we would operate on invalid data.
    if(DoMethod(obj, MUIM_GetConfigItem, MUICFG_BetterString_Frame, &setting))
      set(obj, MUIA_Frame, setting);
    else
      set(obj, MUIA_Frame, "302211");
  }

  data->InactiveText = GetCol(obj, MUICFG_BetterString_InactiveText, (struct MUI_PenSpec *)"m4");
  data->ActiveText = GetCol(obj, MUICFG_BetterString_ActiveText, (struct MUI_PenSpec *)"m5");
  data->CursorColor = GetCol(obj, MUICFG_BetterString_Cursor, (struct MUI_PenSpec *)"m0");
  data->MarkedColor = GetCol(obj, MUICFG_BetterString_MarkedBack, (struct MUI_PenSpec *)"m6");
  data->MarkedTextColor = GetCol(obj, MUICFG_BetterString_MarkedText, (struct MUI_PenSpec *)"m5");

  if(DoMethod(obj, MUIM_GetConfigItem, MUICFG_BetterString_InactiveBack, &setting))
  {
    // don't remember the string from the configuration but copy it
    // with MUI4's realtime prefs the configuration might change upon
    // canceling MUI prefs and hence we would operate on invalid data.
    strlcpy(data->InactiveBackgroundBuffer, (STRPTR)setting, sizeof(data->InactiveBackgroundBuffer));
    data->InactiveBackground = data->InactiveBackgroundBuffer;
  }
  else
    data->InactiveBackground = (STRPTR)MUII_BACKGROUND;

  if(DoMethod(obj, MUIM_GetConfigItem, MUICFG_BetterString_ActiveBack, &setting))
  {
    // don't remember the string from the configuration but copy it
    // with MUI4's realtime prefs the configuration might change upon
    // canceling MUI prefs and hence we would operate on invalid data.
    strlcpy(data->ActiveBackgroundBuffer, (STRPTR)setting, sizeof(data->ActiveBackgroundBuffer));
    data->ActiveBackground = data->ActiveBackgroundBuffer;
  }
  else
    data->ActiveBackground = (STRPTR)"2:m1";

  if(DoMethod(obj, MUIM_GetConfigItem, MUICFG_BetterString_SelectOnActive, &setting))
    data->SelectOnActive = *(IPTR*)setting;
  else
    data->SelectOnActive = FALSE;

  if(DoMethod(obj, MUIM_GetConfigItem, MUICFG_BetterString_SelectPointer, &setting))
    data->SelectPointer = *(IPTR*)setting;
  else
    data->SelectPointer = FALSE;

  if(isFlagClear(data->Flags, FLG_OwnBackground))
    set(obj, MUIA_Background, data->InactiveBackground);

  LEAVE();
}

VOID FreeConfig(struct MUI_RenderInfo *mri, struct InstData *data)
{
  MUI_ReleasePen(mri, data->InactiveText);
  MUI_ReleasePen(mri, data->ActiveText);
  MUI_ReleasePen(mri, data->CursorColor);
  MUI_ReleasePen(mri, data->MarkedColor);
  MUI_ReleasePen(mri, data->MarkedTextColor);
}

