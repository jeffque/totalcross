/*********************************************************************************
 *  TotalCross Software Development Kit                                          *
 *  Copyright (C) 2000-2012 SuperWaba Ltda.                                      *
 *  All Rights Reserved                                                          *
 *                                                                               *
 *  This library and virtual machine is distributed in the hope that it will     *
 *  be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of    *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                         *
 *                                                                               *
 *********************************************************************************/



#include "gfx_ex.h"

void privateScreenChange(int32 w, int32 h)
{
   UNUSED(w)
   UNUSED(h)
}

bool graphicsStartup(ScreenSurface screen)
{
   DFBResult err;
   IDirectFB *_dfb;
   IDirectFBSurface *_primary;
   IDirectFBDisplayLayer *_layer;

   screen->extension = ((ScreenSurfaceEx)deviceCtx);

   if (SCREEN_EX(screen)->layer) return true; // already initialized

   _dfb = SCREEN_EX(screen)->dfb;

   err = _dfb->GetDisplayLayer(_dfb, DLID_PRIMARY, &SCREEN_EX(screen)->layer);
   if (err != DFB_OK) return false;

   _layer = SCREEN_EX(screen)->layer;

   err = _layer->SetCooperativeLevel(_layer, DLSCL_ADMINISTRATIVE);
   if (err != DFB_OK) return false;

   err = _layer->GetSurface(_layer, &SCREEN_EX(screen)->primary);
   if (err != DFB_OK) return false;

   _primary = SCREEN_EX(screen)->primary;
   _primary->SetBlittingFlags(_primary, DSBLIT_BLEND_ALPHACHANNEL);
   _primary->SetDrawingFlags(_primary, DSDRAW_BLEND | DSDRAW_SRC_PREMULTIPLY);

   screen->pixels = (uint8*)1; // fake initialize, will be set on each lock

   DFBSurfacePixelFormat format;
   err = _primary->GetPixelFormat (_primary, &format);
   if (err != DFB_OK) return false;

   screen->bpp = DFB_BITS_PER_PIXEL(format);

   int w, h;
   err = _primary->GetSize (_primary, &w, &h);
   if (err != DFB_OK) return false;

   screen->screenW = w;
   screen->screenH = h;

   return true;
}

bool graphicsCreateScreenSurface(ScreenSurface screen)
{
   return true;
}

static void drawImageLine(ScreenSurface screen, DFBRegion *bounds, int32 minx, int32 miny, int32 maxx, int32 maxy)
{
   bounds->x1 = minx;
   bounds->y1 = miny;
   bounds->x2 = maxx;
   bounds->y2 = maxy;
   SCREEN_EX(screen)->primary->Flip(SCREEN_EX(screen)->primary, bounds, DSFLIP_ONSYNC);
}

void graphicsUpdateScreen(ScreenSurface screen, int32 transitionEffect) // screen's already locked
{
   DFBRegion bounds;
   switch (transitionEffect)
   {
      case TRANSITION_NONE:
         bounds.x1 = screen->dirtyX1;
         bounds.y1 = screen->dirtyY1;
         bounds.x2 = screen->dirtyX2;
         bounds.y2 = screen->dirtyY2;
         SCREEN_EX(screen)->primary->Flip(SCREEN_EX(screen)->primary, &bounds, DSFLIP_ONSYNC);
         break;
      case TRANSITION_CLOSE:
      case TRANSITION_OPEN:
      {
         int32 i0,iinc,i;
         int32 w = screen->screenW;
         int32 h = screen->screenH;
         float incX=1,incY=1;
         int32 n = min32(w,h);
         int32 mx = w/2,ww=1,hh=1;
         int32 my = h/2;
         if (w > h)
            {incX = (float)w/h; ww = (int)incX+1;}
          else
            {incY = (float)h/w; hh = (int)incY+1;}
         i0 = transitionEffect == TRANSITION_CLOSE ? n : 0;
         iinc = transitionEffect == TRANSITION_CLOSE ? -1 : 1;
         for (i =i0; --n >= 0; i+=iinc)
         {
            int32 minx = (int32)(mx - i*incX);
            int32 miny = (int32)(my - i*incY);
            int32 maxx = (int32)(mx + i*incX);
            int32 maxy = (int32)(my + i*incY);
            drawImageLine(screen,&bounds,minx-ww,miny-hh,maxx+ww,miny+hh);
            drawImageLine(screen,&bounds,minx-ww,miny-hh,minx+ww,maxy+hh);
            drawImageLine(screen,&bounds,maxx-ww,miny-hh,maxx+ww,maxy+hh);
            drawImageLine(screen,&bounds,minx-ww,maxy-hh,maxx+ww,maxy+hh);
         }
         break;
      }
   }
}

void graphicsDestroy(ScreenSurface screen, bool isScreenChange)
{
   if (SCREEN_EX(screen)->layer)
      SCREEN_EX(screen)->layer->Release (SCREEN_EX(screen)->layer);
   if (SCREEN_EX(screen)->primary)
      SCREEN_EX(screen)->primary->Release (SCREEN_EX(screen)->primary);
}

bool graphicsLock(ScreenSurface screen, bool on)
{
   IDirectFBSurface *surf = SCREEN_EX(screen)->primary;
   IDirectFBDisplayLayer *_layer = SCREEN_EX(screen)->layer;
   if (on)
   {
      _layer->EnableCursor(_layer, 0);
      return (surf->Lock(surf, DSLF_READ|DSLF_WRITE, (void **)&screen->pixels, &screen->pitch) == DFB_OK);
   }
   else
   {
      bool ok = (surf->Unlock(surf) == DFB_OK);
      _layer->EnableCursor(_layer, 1);
      return ok;
   }
}