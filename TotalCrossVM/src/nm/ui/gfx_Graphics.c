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



#include "tcvm.h"

#define Graphics_forePixel(o)      makePixelRGB(Graphics_foreColor(o))
#define Graphics_backPixel(o)      makePixelRGB(Graphics_backColor(o))
#define Image_transparentPixel(o)  makePixelRGB(Image_transparentColor(o))

#if defined(darwin)
 #include "darwin/gfx_Graphics_c.h"
#elif defined(__SYMBIAN32__)
 #include "symbian/gfx_Graphics_c.h"
#endif
#include "GraphicsPrimitives_c.h"

#if defined(PALMOS)
 #include "palm/gfx_Graphics_c.h"
#elif defined(WINCE) || defined(WIN32)
 #include "win/gfx_Graphics_c.h"
#elif defined(linux) && !defined(darwin)
 #include "linux/gfx_Graphics_c.h"
#elif defined(ANDROID)
 #include "android/gfx_Graphics_c.h"
#endif

#define R3D_EDIT     1
#define R3D_LOWERED  2
#define R3D_RAISED   3
#define R3D_CHECK    4
#define R3D_SHADED   5
#define ARROW_UP     1
#define ARROW_DOWN   2
#define ARROW_LEFT   3
#define ARROW_RIGHT  4
#define WinCE        0
#define PalmOS       1
#define Flat         2
#define Vista        3
#define Android      4

bool initGraphicsBeforeSettings(Context currentContext) // no thread are running at this point
{
   return startupGraphics() && createScreenSurface(currentContext, false) && (screen.bpp != 8 || createColorPaletteLookupTables());
}

void destroyGraphics()
{
   graphicsDestroyPrimitives();
   if (screen.pixels) graphicsDestroy(&screen, false);
   screen.pixels = null; // disallow future updates of the screen
}

bool initGraphicsAfterSettings(Context currentContext)
{                
   updateScreenSettings(screen.screenW, screen.screenH, screen.hRes, screen.vRes, screen.bpp);
   if (!fontInit(currentContext))
   {
      destroyGraphics();
      return false;
   }
   return true;
}

//////////////////////////////////////////////////////////////////////////
TC_API void tugG_create_g(NMParams p) // totalcross/ui/gfx/Graphics native protected void create(totalcross.ui.gfx.GfxSurface surface);
{
   Object g = p->obj[0];
   Object surface = p->obj[1];
   SurfaceType stype = getSurfaceType(p->currentContext, surface);
   int32 w,h;

   if (stype == SURF_IMAGE)
   {
      w = *getInstanceFieldInt(surface, "width",  "totalcross.ui.image.Image");
      h = *getInstanceFieldInt(surface, "height", "totalcross.ui.image.Image");
   }
   else
   {
#ifdef ENABLE_TEST_SUITE // no thread are running during a test_suite
      w = screen.screenW;
      h = screen.screenH;
#else
      w = *getInstanceFieldInt(surface, "width",  "totalcross.ui.Control");
      h = *getInstanceFieldInt(surface, "height", "totalcross.ui.Control");
#endif
   }
   createGfxSurface(w, h, g, stype);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawDottedCursor_iiii(NMParams p) // totalcross/ui/gfx/Graphics native public void drawDottedCursor(int x, int y, int w, int h);
{
   Object g = p->obj[0];
   drawDottedCursor(g, p->i32[0], p->i32[1], p->i32[2], p->i32[3]);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawEllipse_iiii(NMParams p) // totalcross/ui/gfx/Graphics native public void drawEllipse(int xc, int yc, int rx, int ry);
{
   Object g = p->obj[0];
   ellipseDrawAndFill(g, p->i32[0], p->i32[1], p->i32[2], p->i32[3], Graphics_forePixel(g), Graphics_forePixel(g), false, false);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_fillEllipse_iiii(NMParams p) // totalcross/ui/gfx/Graphics native public void fillEllipse(int xc, int yc, int rx, int ry);
{
   Object g = p->obj[0];
   ellipseDrawAndFill(g, p->i32[0], p->i32[1], p->i32[2], p->i32[3], Graphics_backPixel(g), Graphics_backPixel(g), true, false);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_fillEllipseGradient_iiii(NMParams p) // totalcross/ui/gfx/Graphics native public void fillEllipseGradient(int xc, int yc, int rx, int ry);
{
   Object g = p->obj[0];
   ellipseDrawAndFill(g, p->i32[0], p->i32[1], p->i32[2], p->i32[3], Graphics_forePixel(g), Graphics_backPixel(g), true, true);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawArc_iiidd(NMParams p) // totalcross/ui/gfx/Graphics native public void drawArc(int xc, int yc, int r, double startAngle, double endAngle);
{
   Object g = p->obj[0];
   arcPiePointDrawAndFill(g, p->i32[0], p->i32[1], p->i32[2], p->i32[2], p->dbl[0], p->dbl[1], Graphics_forePixel(g), Graphics_forePixel(g), false, false, false, p->currentContext);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawPie_iiidd(NMParams p) // totalcross/ui/gfx/Graphics native public void drawPie(int xc, int yc, int r, double startAngle, double endAngle);
{
   Object g = p->obj[0];
   arcPiePointDrawAndFill(g, p->i32[0], p->i32[1], p->i32[2], p->i32[2], p->dbl[0], p->dbl[1], Graphics_forePixel(g), Graphics_forePixel(g), false, true, false, p->currentContext);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_fillPie_iiidd(NMParams p) // totalcross/ui/gfx/Graphics native public void fillPie(int xc, int yc, int r, double startAngle, double endAngle);
{
   Object g = p->obj[0];
   arcPiePointDrawAndFill(g, p->i32[0], p->i32[1], p->i32[2], p->i32[2], p->dbl[0], p->dbl[1], Graphics_forePixel(g), Graphics_backPixel(g), true, true, false, p->currentContext);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_fillPieGradient_iiidd(NMParams p) // totalcross/ui/gfx/Graphics native public void fillPieGradient(int xc, int yc, int r, double startAngle, double endAngle);
{
   Object g = p->obj[0];
   arcPiePointDrawAndFill(g, p->i32[0], p->i32[1], p->i32[2], p->i32[2], p->dbl[0], p->dbl[1], Graphics_forePixel(g), Graphics_backPixel(g), true, true, true, p->currentContext);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawEllipticalArc_iiiidd(NMParams p) // totalcross/ui/gfx/Graphics native public void drawEllipticalArc(int xc, int yc, int rx, int ry, double startAngle, double endAngle);
{
   Object g = p->obj[0];
   arcPiePointDrawAndFill(g, p->i32[0], p->i32[1], p->i32[2], p->i32[2], p->dbl[0], p->dbl[1], Graphics_forePixel(g), Graphics_forePixel(g), false, false, false, p->currentContext);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawEllipticalPie_iiiidd(NMParams p) // totalcross/ui/gfx/Graphics native public void drawEllipticalPie(int xc, int yc, int rx, int ry, double startAngle, double endAngle);
{
   Object g = p->obj[0];
   arcPiePointDrawAndFill(g, p->i32[0], p->i32[1], p->i32[2], p->i32[2], p->dbl[0], p->dbl[1], Graphics_forePixel(g), Graphics_forePixel(g), false, true, false, p->currentContext);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_fillEllipticalPie_iiiidd(NMParams p) // totalcross/ui/gfx/Graphics native public void fillEllipticalPie(int xc, int yc, int rx, int ry, double startAngle, double endAngle);
{
   Object g = p->obj[0];
   arcPiePointDrawAndFill(g, p->i32[0], p->i32[1], p->i32[2], p->i32[2], p->dbl[0], p->dbl[1], Graphics_forePixel(g), Graphics_backPixel(g), true, true, false, p->currentContext);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_fillEllipticalPieGradient_i(NMParams p) // totalcross/ui/gfx/Graphics native public void fillEllipticalPieGradient(int xc, int yc, int rx, int ry, double startAngle, double endAngle);
{
   Object g = p->obj[0];
   arcPiePointDrawAndFill(g, p->i32[0], p->i32[1], p->i32[2], p->i32[2], p->dbl[0], p->dbl[1], Graphics_forePixel(g), Graphics_backPixel(g), true, true, true, p->currentContext);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawCircle_iii(NMParams p) // totalcross/ui/gfx/Graphics native public void drawCircle(int xc, int yc, int r);
{
   Object g = p->obj[0];
   ellipseDrawAndFill(g, p->i32[0], p->i32[1], p->i32[2], p->i32[2], Graphics_forePixel(g), Graphics_forePixel(g), false, false);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_fillCircle_iii(NMParams p) // totalcross/ui/gfx/Graphics native public void fillCircle(int xc, int yc, int r);
{
   Object g = p->obj[0];
   ellipseDrawAndFill(g, p->i32[0], p->i32[1], p->i32[2], p->i32[2], Graphics_backPixel(g), Graphics_backPixel(g), true, false);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_fillCircleGradient_iii(NMParams p) // totalcross/ui/gfx/Graphics native public void fillCircleGradient(int xc, int yc, int r);
{
   Object g = p->obj[0];
   ellipseDrawAndFill(g, p->i32[0], p->i32[1], p->i32[2], p->i32[2], Graphics_backPixel(g), Graphics_backPixel(g), true, true);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_getPixel_ii(NMParams p) // totalcross/ui/gfx/Graphics native public int getPixel(int x, int y);
{
   Object g = p->obj[0];
   p->retI = getPixel(g, p->i32[0], p->i32[1]);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_setPixel_ii(NMParams p) // totalcross/ui/gfx/Graphics native public void setPixel(int x, int y);
{
   Object g = p->obj[0];
   setPixel(g, p->i32[0], p->i32[1], Graphics_forePixel(g));
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_eraseRect_iiii(NMParams p) // totalcross/ui/gfx/Graphics native public void eraseRect(int x, int y, int w, int h);
{
   Object g = p->obj[0];
   eraseRect(g, p->i32[0], p->i32[1], p->i32[2], p->i32[3], Graphics_forePixel(g), Graphics_backPixel(g));
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawLine_iiii(NMParams p) // totalcross/ui/gfx/Graphics native public void drawLine(int ax, int ay, int bx, int by);
{
   Object g = p->obj[0];
   drawLine(g, p->i32[0], p->i32[1], p->i32[2], p->i32[3], Graphics_forePixel(g));
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawDots_iiii(NMParams p) // totalcross/ui/gfx/Graphics native public void drawDots(int ax, int ay, int bx, int by);
{
   Object g = p->obj[0];
   drawDottedLine(g, p->i32[0], p->i32[1], p->i32[2], p->i32[3], Graphics_forePixel(g), Graphics_backPixel(g));
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_fillCursor_iiii(NMParams p) // totalcross/ui/gfx/Graphics native public void fillCursor(int x, int y, int w, int h);
{
   fillCursor(p->obj[0], p->i32[0], p->i32[1], p->i32[2], p->i32[3]);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawCursor_iiii(NMParams p) // totalcross/ui/gfx/Graphics native public void drawCursor(int x, int y, int w, int h);
{
   Object g = p->obj[0];
   drawCursor(g, p->i32[0], p->i32[1], p->i32[2], p->i32[3]);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawRect_iiii(NMParams p) // totalcross/ui/gfx/Graphics native public void drawRect(int x, int y, int w, int h);
{
   Object g = p->obj[0];
   drawRect(g, p->i32[0], p->i32[1], p->i32[2], p->i32[3], Graphics_forePixel(g));
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_fillRect_iiii(NMParams p) // totalcross/ui/gfx/Graphics native public void fillRect(int x, int y, int w, int h);
{
   Object g = p->obj[0];
   fillRect(g, p->i32[0], p->i32[1], p->i32[2], p->i32[3], Graphics_backPixel(g));
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawDottedRect_iiii(NMParams p) // totalcross/ui/gfx/Graphics native public void drawDottedRect(int x, int y, int w, int h);
{
   Object g = p->obj[0];
   drawDottedRect(g, p->i32[0], p->i32[1], p->i32[2], p->i32[3], Graphics_forePixel(g), Graphics_backPixel(g));
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_fillPolygon_IIi(NMParams p) // totalcross/ui/gfx/Graphics native public void fillPolygon(int []xPoints, int []yPoints, int nPoints);
{
   Object g = p->obj[0];
   Object xPoints = p->obj[1];
   Object yPoints = p->obj[2];
   int32 nPoints = p->i32[0];
   // fdie@ the vm has a 4bytes pointer!
   int32* xp = (int32 *)ARRAYOBJ_START(xPoints);
   int32* yp = (int32 *)ARRAYOBJ_START(yPoints);

   if (checkArrayRange(p->currentContext, xPoints, 0, nPoints) && checkArrayRange(p->currentContext, yPoints, 0, nPoints))
      fillPolygon(g, xp, yp, nPoints, 0,0,0, Graphics_backPixel(g), Graphics_backPixel(g), false, p->currentContext);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_fillPolygonGradient_IIi(NMParams p) // totalcross/ui/gfx/Graphics native public void fillPolygonGradient(int []xPoints, int []yPoints, int nPoints);
{
   Object g = p->obj[0];
   Object xPoints = p->obj[1];
   Object yPoints = p->obj[2];
   int32 nPoints = p->i32[0];
   // fdie@ the vm has a 4bytes pointer!
   int32* xp = (int32 *)ARRAYOBJ_START(xPoints);
   int32* yp = (int32 *)ARRAYOBJ_START(yPoints);

   if (checkArrayRange(p->currentContext, xPoints, 0, nPoints) && checkArrayRange(p->currentContext, yPoints, 0, nPoints))
      fillPolygon(g, xp, yp, nPoints, 0,0,0, Graphics_forePixel(g), Graphics_backPixel(g), true, p->currentContext);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawPolygon_IIi(NMParams p) // totalcross/ui/gfx/Graphics native public void drawPolygon(int []xPoints, int []yPoints, int nPoints);
{
   Object g = p->obj[0];
   Object xPoints = p->obj[1];
   Object yPoints = p->obj[2];
   int32 nPoints = p->i32[0];
   // fdie@ the vm has a 4bytes pointer!
   int32* xp = (int32 *)ARRAYOBJ_START(xPoints);
   int32* yp = (int32 *)ARRAYOBJ_START(yPoints);

   if (checkArrayRange(p->currentContext, xPoints, 0, nPoints) && checkArrayRange(p->currentContext, yPoints, 0, nPoints))
   {
      drawPolygon(g, xp, yp, nPoints, 0,0,0, Graphics_forePixel(g));
      drawLine(g, xp[0],yp[0],xp[nPoints-1],yp[nPoints-1], Graphics_forePixel(g));
   }
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawPolyline_IIi(NMParams p) // totalcross/ui/gfx/Graphics native public void drawPolyline(int []xPoints, int []yPoints, int nPoints);
{
   Object g = p->obj[0];
   Object xPoints = p->obj[1];
   Object yPoints = p->obj[2];
   int32 nPoints = p->i32[0];
   // fdie@ the vm has a 4bytes pointer!
   int32* xp = (int32 *)ARRAYOBJ_START(xPoints);
   int32* yp = (int32 *)ARRAYOBJ_START(yPoints);

   if (checkArrayRange(p->currentContext, xPoints, 0, nPoints) && checkArrayRange(p->currentContext, yPoints, 0, nPoints))
      drawPolygon(g, xp, yp, nPoints, 0,0,0, Graphics_forePixel(g));
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawText_sii(NMParams p) // totalcross/ui/gfx/Graphics native public void drawText(String text, int x, int y);
{
   Object text;
   Object g = p->obj[0];
   if ((text=p->obj[1]) != null)
      drawText(p->currentContext, g, String_charsStart(text), String_charsLen(text), p->i32[0], p->i32[1], Graphics_forePixel(g),0);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawText_Ciiii(NMParams p) // totalcross/ui/gfx/Graphics native public void drawText(char []chars, int start, int count, int x, int y);
{
   Object chars;
   Object g = p->obj[0];
   if ((chars=p->obj[1]) != null)
   {
      uint32 start = p->i32[0];
      uint32 len   = p->i32[1];
      if (checkArrayRange(p->currentContext, chars, start, len)) // array bound check
         drawText(p->currentContext, g, ((JCharP)ARRAYOBJ_START(chars))+start, len, p->i32[2], p->i32[3], Graphics_forePixel(g),0);
   }
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawText_siiii(NMParams p) // totalcross/ui/gfx/Graphics native public void drawText(StringBuffer sb, int chrStart, int chrCount, int x, int y);
{
   Object sb;
   Object g = p->obj[0];
   if ((sb=p->obj[1]) != null)
   {
      uint32 start = p->i32[0];
      uint32 len   = p->i32[1];
      if (checkArrayRange(p->currentContext, StringBuffer_chars(sb), start, len)) // array bound check
         drawText(p->currentContext, g, StringBuffer_charsStart(sb)+start, len, p->i32[2], p->i32[3], Graphics_forePixel(g),0);
   }
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawText_siiiii(NMParams p) // totalcross/ui/gfx/Graphics native public void drawText(StringBuffer sb, int chrStart, int chrCount, int x, int y, int justifyWidth);
{
   Object sb;
   Object g = p->obj[0];
   if ((sb=p->obj[1]) != null)
   {
      uint32 start = p->i32[0];
      uint32 len   = p->i32[1];
      if (checkArrayRange(p->currentContext, StringBuffer_chars(sb), start, len)) // array bound check
         drawText(p->currentContext, g, StringBuffer_charsStart(sb)+start, len, p->i32[2], p->i32[3], Graphics_forePixel(g),p->i32[4]);
   }
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawText_siii(NMParams p) // totalcross/ui/gfx/Graphics native public void drawText(String text, int x, int y, int justifyWidth);
{
   Object text;
   Object g = p->obj[0];
   if ((text=p->obj[1]) != null)
      drawText(p->currentContext, g, String_charsStart(text), String_charsLen(text), p->i32[0], p->i32[1], Graphics_forePixel(g),p->i32[2]);
}
//////////////////////////////////////////////////////////////////////////
static void drawTextShadowed(Context currentContext, Object g, JCharP text, int32 chrCount, int32 x0, int32 y0, Pixel foreColor, int32 justifyWidth, bool shadow, Pixel shadowColor)
{
   if (shadow)
   {
      drawText(currentContext, g, text, chrCount, x0-1,y0-1, shadowColor, justifyWidth);
      drawText(currentContext, g, text, chrCount, x0+1,y0-1, shadowColor, justifyWidth);
      drawText(currentContext, g, text, chrCount, x0+1,y0+1, shadowColor, justifyWidth);
      drawText(currentContext, g, text, chrCount, x0-1,y0+1, shadowColor, justifyWidth);
   }
   drawText(currentContext, g, text, chrCount, x0,y0, foreColor, justifyWidth);
}

TC_API void tugG_drawText_Ciiiibi(NMParams p) // totalcross/ui/gfx/Graphics native public void drawText(char []chars, int chrStart, int chrCount, int x, int y, boolean shadow, int shadowColor);
{
   Object chars;
   Object g = p->obj[0];
   if ((chars=p->obj[1]) != null)
   {
      uint32 start = p->i32[0];
      uint32 len   = p->i32[1];
      if (checkArrayRange(p->currentContext, chars, start, len)) // array bound check
         drawTextShadowed(p->currentContext, g, ((JCharP)ARRAYOBJ_START(chars))+start, len, p->i32[2], p->i32[3], Graphics_forePixel(g),0,p->i32[4],makePixelRGB(p->i32[5]));
   }
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawText_siiiibi(NMParams p) // totalcross/ui/gfx/Graphics native public void drawText(StringBuffer sb, int chrStart, int chrCount, int x, int y, boolean shadow, int shadowColor);
{
   Object sb;
   Object g = p->obj[0];
   if ((sb=p->obj[1]) != null)
   {
      uint32 start = p->i32[0];
      uint32 len   = p->i32[1];
      if (checkArrayRange(p->currentContext, StringBuffer_chars(sb), start, len)) // array bound check
         drawTextShadowed(p->currentContext, g, StringBuffer_charsStart(sb)+start, len, p->i32[2], p->i32[3], Graphics_forePixel(g),0,p->i32[4],makePixelRGB(p->i32[5]));
   }
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawText_siiiiibi(NMParams p) // totalcross/ui/gfx/Graphics native public void drawText(StringBuffer sb, int chrStart, int chrCount, int x, int y, int justifyWidth, boolean shadow, int shadowColor);
{
   Object sb;
   Object g = p->obj[0];
   if ((sb=p->obj[1]) != null)
   {
      uint32 start = p->i32[0];
      uint32 len   = p->i32[1];
      if (checkArrayRange(p->currentContext, StringBuffer_chars(sb), start, len)) // array bound check
         drawTextShadowed(p->currentContext, g, StringBuffer_charsStart(sb)+start, len, p->i32[2], p->i32[3], Graphics_forePixel(g),p->i32[4],p->i32[5],makePixelRGB(p->i32[6]));
   }
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawText_siibi(NMParams p) // totalcross/ui/gfx/Graphics native public void drawText(String text, int x, int y, boolean shadow, int shadowColor);
{
   Object text;
   Object g = p->obj[0];
   if ((text=p->obj[1]) != null)
      drawTextShadowed(p->currentContext, g, String_charsStart(text), String_charsLen(text), p->i32[0], p->i32[1], Graphics_forePixel(g),0,p->i32[2],makePixelRGB(p->i32[3]));
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawText_siiibi(NMParams p) // totalcross/ui/gfx/Graphics native public void drawText(String text, int x, int y, int justifyWidth, boolean shadow, int shadowColor);
{
   Object text;
   Object g = p->obj[0];
   if ((text=p->obj[1]) != null)
      drawTextShadowed(p->currentContext, g, String_charsStart(text), String_charsLen(text), p->i32[0], p->i32[1], Graphics_forePixel(g),p->i32[2],p->i32[3],makePixelRGB(p->i32[4]));
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawHatchedRect_iiiibb(NMParams p) // totalcross/ui/gfx/Graphics native public void drawHatchedRect(int x, int y, int width, int height, boolean top, boolean bottom);
{
   Object g = p->obj[0];
   drawHatchedRect(g,p->i32[0],p->i32[1],p->i32[2],p->i32[3],(bool)p->i32[4],(bool)p->i32[5],Graphics_forePixel(g));
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_fillHatchedRect_iiiibb(NMParams p) // totalcross/ui/gfx/Graphics native public void fillHatchedRect(int x, int y, int width, int height, boolean top, boolean bottom);
{
   Object g = p->obj[0];
   fillHatchedRect(g,p->i32[0],p->i32[1],p->i32[2],p->i32[3],(bool)p->i32[4],(bool)p->i32[5],Graphics_backPixel(g));
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawRoundRect_iiiii(NMParams p) // totalcross/ui/gfx/Graphics native public void drawRoundRect(int x, int y, int width, int height, int r);
{
   Object g = p->obj[0];
   drawRoundRect(g, p->i32[0], p->i32[1], p->i32[2], p->i32[3], p->i32[4], Graphics_forePixel(g));
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_fillRoundRect_iiiii(NMParams p) // totalcross/ui/gfx/Graphics native public void fillRoundRect(int x, int y, int width, int height, int r);
{
   Object g = p->obj[0];
   fillRoundRect(g, p->i32[0], p->i32[1], p->i32[2], p->i32[3], p->i32[4], Graphics_backPixel(g));
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_setClip_iiii(NMParams p) // totalcross/ui/gfx/Graphics native public void setClip(int x, int y, int w, int h);
{
   Object g = p->obj[0];
   setClip(g, p->i32[0], p->i32[1], p->i32[2], p->i32[3]);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_clip_r(NMParams p) // totalcross/ui/gfx/Graphics native public boolean clip(totalcross.ui.gfx.Rect r);
{
   Object g = p->obj[0];
   Object rect = p->obj[1];
   bool b = false;
   if (rect)
   {
      int32 x,y;
      int32 w,h;
      x = Rect_x(rect);
      y = Rect_y(rect);
      w = Rect_width(rect);
      h = Rect_height(rect);
      b = translateAndClip(g,&x,&y,&w,&h);
      if (b) // intersect?
      {
         Graphics_clipX1(g) = Rect_x(rect) = x - Graphics_transX(g);
         Graphics_clipY1(g) = Rect_y(rect) = y - Graphics_transY(g);
         Rect_width(rect) = w;
         Rect_height(rect) = h;
         Graphics_clipX2(g) = Graphics_clipX1(g)+w;
         Graphics_clipY2(g) = Graphics_clipY1(g)+h;
      }
   }
   p->retI = b;
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_copyRect_giiiiii(NMParams p) // totalcross/ui/gfx/Graphics native public void copyRect(totalcross.ui.gfx.GfxSurface surface, int x, int y, int width, int height, int dstX, int dstY);
{
   Object hDest = p->obj[0];
   Object hOrig = p->obj[1];
   if (hOrig)
      drawSurface(hDest, hOrig, p->i32[0], p->i32[1], p->i32[2], p->i32[3], p->i32[4], p->i32[5], Graphics_drawOp(hDest), Graphics_backPixel(hDest), Graphics_forePixel(hDest), true);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawHighLightFrame_iiiiiib(NMParams p) // totalcross/ui/gfx/Graphics native public void drawHighLightFrame(int x, int y, int w, int h, int topLeftColor, int bottomRightColor, boolean yMirror);
{
   Object g = p->obj[0];
   drawHighLightFrame(g, p->i32[0],p->i32[1],p->i32[2],p->i32[3],makePixelRGB(p->i32[4]),makePixelRGB(p->i32[5]), (bool)p->i32[6]);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawRoundGradient_iiiiiiiii(NMParams p) // totalcross/ui/gfx/Graphics native public void drawRoundGradient(int startX, int startY, int endX, int endY, int topLeftRadius, int topRightRadius, int bottomLeftRadius, int bottomRightRadius,int startColor, int endColor);
{
   Object g = p->obj[0];
   drawRoundGradient(g, p->i32[0],p->i32[1],p->i32[2],p->i32[3],p->i32[4],p->i32[5],p->i32[6],p->i32[7],p->i32[8],p->i32[9], p->i32[10]);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawImage_iiiiib(NMParams p) // totalcross/ui/gfx/Graphics native public void drawImage(totalcross.ui.image.Image image, int x, int y, int drawOp, int backColor, boolean doClip);
{
   Object surfDest = p->obj[0];
   Object surfOrig = p->obj[1];
   if (surfOrig) drawSurface(surfDest, surfOrig, 0, 0, Image_width(surfOrig), Image_height(surfOrig), p->i32[0], p->i32[1], p->i32[2],
                             p->i32[3] >= 0 ? makePixelRGB(p->i32[3]) : Image_transparentPixel(surfOrig), Graphics_forePixel(surfDest), (bool)p->i32[4]);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_copyImageRect_iiiiiiib(NMParams p) // totalcross/ui/gfx/Graphics native public void copyImageRect(totalcross.ui.image.Image image, int x, int y, int width, int height, int drawOp, int backColor, boolean doClip);
{
   Object surfDest = p->obj[0];
   Object surfOrig = p->obj[1];
   if (surfOrig) drawSurface(surfDest, surfOrig, p->i32[0], p->i32[1], p->i32[2], p->i32[3], 0,0, p->i32[4],
                             p->i32[5] >= 0 ? makePixelRGB(p->i32[5]) : Image_transparentPixel(surfOrig), Graphics_forePixel(surfDest), (bool)p->i32[6]);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_setPixels_IIi(NMParams p) // totalcross/ui/gfx/Graphics native public void setPixels(int []xPoints, int []yPoints, int nPoints);
{
   Object g = p->obj[0];
   Object xPoints = p->obj[1];
   Object yPoints = p->obj[2];
   int32 nPoints = p->i32[0];
   Pixel c = Graphics_forePixel(g);

   // fdie@ the vm has a 4bytes pointer!
   int32 * xp = (int32 *)ARRAYOBJ_START(xPoints);
   int32 * yp = (int32 *)ARRAYOBJ_START(yPoints);

   if (checkArrayRange(p->currentContext, xPoints, 0, nPoints) && checkArrayRange(p->currentContext, yPoints, 0, nPoints))
      while (nPoints-- > 0)
         setPixel(g, *xp++, *yp++, c);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_refresh_iiiiiif(NMParams p) // totalcross/ui/gfx/Graphics native public void refresh(int sx, int sy, int sw, int sh, int tx, int ty, totalcross.ui.font.Font f);
{
   Object g = p->obj[0];
   Object surf = Graphics_surface(g);
   int32 scrW,scrH;
   if (Surface_isImage(surf)) // update everything, because a screen rotation may have occured
   {
      scrW = Graphics_clipX2(g) = Graphics_width (g) = Graphics_pitch(g) = Image_width(surf);
      scrH = Graphics_clipY2(g) = Graphics_height(g) = Image_height(surf);
   }
   else
   {
      scrW = Graphics_clipX2(g) = Graphics_width (g) = Graphics_pitch(g) = screen.screenW;
      scrH = Graphics_clipY2(g) = Graphics_height(g) = screen.screenH;
   }
   Graphics_clipX1(g) = Graphics_minX(g) = max32(0,p->i32[0]);
   Graphics_clipY1(g) = Graphics_minY(g) = max32(0,p->i32[1]);
   Graphics_clipX2(g) = Graphics_maxX(g) = min32(p->i32[0]+p->i32[2],scrW);
   Graphics_clipY2(g) = Graphics_maxY(g) = min32(p->i32[1]+p->i32[3],scrH);
   Graphics_transX(g) = p->i32[4];
   Graphics_transY(g) = p->i32[5];
   if (p->obj[1]) Graphics_font(g) = p->obj[1];
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawVistaRect_iiiiiiii(NMParams p) // totalcross/ui/gfx/Graphics native public void drawVistaRect(int x, int y, int width, int height, int topColor, int rightColor, int bottomColor, int leftColor);
{
   Object g = p->obj[0];
   drawVistaRect(g, p->i32[0],p->i32[1],p->i32[2],p->i32[3], makePixelRGB(p->i32[4]),makePixelRGB(p->i32[5]),makePixelRGB(p->i32[6]),makePixelRGB(p->i32[7]));
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_draw3dRect_iiiibbbI(NMParams p) // totalcross/ui/gfx/Graphics native public void draw3dRect(int x, int y, int width, int height, byte type, boolean yMirror, boolean simple, int []fourColors);
{
   Object g = p->obj[0];
   int32 x = p->i32[0];
   int32 y = p->i32[1];
   int32 width = p->i32[2];
   int32 height = p->i32[3];
   int32 type = p->i32[4];
   bool yMirror = (bool)p->i32[5];
   bool simple = (bool)p->i32[6];
   int32 *fourColorsI = (int32*)ARRAYOBJ_START(p->obj[1]);
   Pixel fourColors[4], foreColor = Graphics_forePixel(g), backColor = Graphics_backPixel(g);

   if (fourColorsI == null)
      return;

   fourColors[0] = makePixelRGB(fourColorsI[0]);
   fourColors[1] = makePixelRGB(fourColorsI[1]);
   fourColors[2] = makePixelRGB(fourColorsI[2]);
   fourColors[3] = makePixelRGB(fourColorsI[3]);

   if (type == R3D_SHADED)
   {
      drawLine(g,simple?0:1,0,simple?width-1:width-3,0,foreColor);
      drawLine(g,0,1,0,height-3,foreColor);
      drawLine(g,width-2,1,width-2,height-3,foreColor);
      drawLine(g,width-1,simple?1:2,width-1,height-3,foreColor);
      drawLine(g,1,height-2,width-2,height-2,foreColor);
      drawLine(g,2,height-1,width-3,height-1,foreColor);
   }
   else
   switch (*tcSettings.uiStylePtr)
   {
      case WinCE:
         switch (type)
         {
            case R3D_EDIT:
               drawHighLightFrame(g,x,y,width,height,fourColors[2],fourColors[1],yMirror);
               if (!simple) drawHighLightFrame(g,x+1,y+1,width-2,height-2,fourColors[3],fourColors[0],yMirror);
               break;
            case R3D_LOWERED:
               drawHighLightFrame(g,x,y,width,height,fourColors[3],fourColors[0],yMirror);
               if (!simple) drawHighLightFrame(g,x+1,y+1,width-2,height-2,fourColors[2],fourColors[1],yMirror);
               break;
            case R3D_RAISED:
               drawHighLightFrame(g,x,y,width,height,fourColors[0],fourColors[3],yMirror);
               if (!simple) drawHighLightFrame(g,x+1,y+1,width-2,height-2,fourColors[1],fourColors[2],yMirror);
               break;
            case R3D_CHECK:
               drawHighLightFrame(g,x,y,width,height,fourColors[2],fourColors[1],yMirror);
               if (!simple) drawHighLightFrame(g,x+1,y+1,width-2,height-2,fourColors[3],fourColors[0],yMirror);
               break;
         }
         break;
      case PalmOS:
         switch (type)
         {
            case R3D_CHECK:
               drawRect(g,x,y,width,height,fourColors[2]);
               break;
            case R3D_EDIT:
               drawDottedLine(g,x,y+height-1,x+width,y+height-1,fourColors[2],backColor);
               break;
            case R3D_LOWERED:
               if (simple)
                  fillRect(g,x,y,width,height,fourColors[1]);
               else
                  fillHatchedRect(g,x,y,width,height,true,true,fourColors[1]); // no break; here!
            case R3D_RAISED:
               if (simple)
                  drawRect(g,x,y,width,height,foreColor);
               else
                  drawHatchedRect(g,x,y,width,height,true,true,foreColor);
               break;
         }
         break;
      case Flat:
         switch (type)
         {
            case R3D_CHECK:
               drawRect(g,x,y,width,height,fourColors[2]);
               break;
            case R3D_EDIT:
               drawRect(g,x,y,x+width,y+height,fourColors[2]);
               break;
            case R3D_LOWERED:
               fillRect(g,x,y,width,height,fourColors[1]);
            case R3D_RAISED:
               drawRect(g,x,y,width,height,fourColors[2]);
               break;
         }
         break;
      case Android:
      case Vista:
         switch (type)
         {
            case R3D_CHECK:
               drawRect(g,x,y,width,height,fourColors[0]);
               break;
            case R3D_EDIT:
               drawRect(g,x,y,x+width,y+height,fourColors[0]);
               break;
            case R3D_RAISED:
               drawVistaRect(g,x,y,width,height, fourColors[1], fourColors[1], fourColors[2], fourColors[3]);
               break;
            case R3D_LOWERED:
               drawVistaRect(g,x,y,width,height, fourColors[2], fourColors[3], fourColors[1], fourColors[1]); // guich@tc122_10; 2113 -> 2311
               break;
         }
         break;
   }
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_fillVistaRect_iiiibbI(NMParams p) // totalcross/ui/gfx/Graphics native private void fill3dRect(int x, int y, int width, int height, boolean invert, boolean rotate, int []colors);
{
   Object g = p->obj[0];
   if (p->obj[1])
      fillVistaRect(g, p->i32[0], p->i32[1], p->i32[2], p->i32[3], (bool)p->i32[4], (bool)p->i32[5], p->obj[1]);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawArrow_iiibbi(NMParams p) // totalcross/ui/gfx/Graphics native public void drawArrow(int x, int y, int h, byte type, boolean pressed, int color);
{
   Object g = p->obj[0];
   int32 x = p->i32[0];
   int32 y = p->i32[1];
   int32 h = p->i32[2];
   int32 type = p->i32[3], step=1;
   bool pressed = (bool)p->i32[4];
   Pixel foreColor = makePixelRGB(p->i32[5]);
   if (pressed) {x++; y++;}
   if (type == ARROW_RIGHT || type == ARROW_LEFT)
   {
      if (type == ARROW_LEFT) {x += h-1; step = -1;}
      h--;
      while (h >= 0)
      {
         drawLine(g, x,y,x,y+(h<<1), foreColor);
         x += step;
         y++;
         h--;
      }
   }
   else
   {
      if (type == ARROW_UP) {y += h-1; step = -1;}
      h--;
      while (h >= 0)
      {
         drawLine(g, x,y,x+(h<<1),y, foreColor);
         y += step;
         x++;
         h--;
      }
   }
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_eraseRect_iiiiiii(NMParams p) // totalcross/ui/gfx/Graphics native public void eraseRect(int x, int y, int w, int h, int fromColor, int toColor, int textColor);
{
   Object g = p->obj[0];
   eraseRectAA(p->currentContext, g, p->i32[0], p->i32[1], p->i32[2], p->i32[3], p->i32[4], p->i32[5], p->i32[6]);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawImage_iii(NMParams p) // totalcross/ui/gfx/Graphics native public void drawImage(totalcross.ui.image.Image image, int x, int y);
{
   //copyRect(image, 0, 0, image.getWidth(),image.getHeight(), x, y);
   Object surfDest = p->obj[0];
   Object surfOrig = p->obj[1];
   if (surfOrig) drawSurface(surfDest, surfOrig, 0,0, Image_width(surfOrig), Image_height(surfOrig), p->i32[0], p->i32[1], Graphics_drawOp(surfDest),
                             Image_transparentPixel(surfOrig), Graphics_forePixel(surfDest), true);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_getRGB_Iiiiii(NMParams p) // totalcross/ui/gfx/Graphics native public int getRGB(int []data, int offset, int x, int y, int w, int h);
{
   Object g = p->obj[0];
   Object data = p->obj[1];
   p->retI = getsetRGB(p->currentContext, g, data, p->i32[0], p->i32[1], p->i32[2], p->i32[3], p->i32[4],true);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_setRGB_Iiiiii(NMParams p) // totalcross/ui/gfx/Graphics native public int setRGB(int []data, int offset, int x, int y, int w, int h);
{
   Object g = p->obj[0];
   Object data = p->obj[1];
   p->retI = getsetRGB(p->currentContext, g, data, p->i32[0], p->i32[1], p->i32[2], p->i32[3], p->i32[4],false);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_fadeScreen_i(NMParams p) // totalcross/ui/gfx/Graphics native public static void fadeScreen(int fadeValue);
{
   //int32 ini = getTimeStamp();
   if (graphicsLock(&screen, true))
   {
      int32 fadeValue = p->i32[0], j,r,g,b;
      PixelConv *f = (PixelConv*)ARRAYOBJ_START(screen.mainWindowPixels);
      bool dec = fadeValue > 0;
      PixelConv lastColor, lastFaded, rgb;
      lastColor.pixel = lastFaded.pixel = rgb.pixel = -1;
      for (j = screen.screenH * screen.screenW; j-- > 0; f++)
      {
         if (f->pixel == lastColor.pixel) // uigadgets have a cache hit of 90%
            f->pixel = lastFaded.pixel;
         else
         {
            lastColor.pixel = f->pixel;
            r = f->r - fadeValue;
            g = f->g - fadeValue;
            b = f->b - fadeValue;
            if (dec) // if the value is being decreased, it will never be greater than the max value
            {
               if (r < 0) r = 0; 
               if (g < 0) g = 0; 
               if (b < 0) b = 0; 
            }
            else
            {
               if (r > 255) r = 255;
               if (g > 255) g = 255;
               if (b > 255) b = 255;
            }                 
            f->r = r;
            f->g = g;
            f->b = b;
            lastFaded.pixel = f->pixel;
         }
      }
      graphicsLock(&screen, false);
   }                          
   //debug("elapsed %d ms",getTimeStamp()-ini);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawWindowBorder_iiiiiiiiii(NMParams p) // totalcross/ui/gfx/Graphics native public void drawWindowBorder(int xx, int yy, int ww, int hh, int titleH, int footerH, int borderColor, int titleColor, int bodyColor, int footerColor, int thickness, boolean drawSeparators);
{                      
   int32 thickness;
   Object g = p->obj[0];
   PixelConv borderColor,titleColor,bodyColor,footerColor;
   borderColor.pixel = makePixelRGB(p->i32[6]);
   titleColor .pixel = makePixelRGB(p->i32[7]);
   bodyColor  .pixel = makePixelRGB(p->i32[8]);
   footerColor.pixel = makePixelRGB(p->i32[9]);

   thickness = p->i32[10];
   if (thickness < 1) thickness = 1; else
   if (thickness > 3) thickness = 3;
   drawWindowBorder(g, p->i32[0], p->i32[1], p->i32[2], p->i32[3], p->i32[4], p->i32[5], borderColor, titleColor, bodyColor, footerColor, thickness, p->i32[11]);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_dither_iiiii(NMParams p) // totalcross/ui/gfx/Graphics native public void dither(int x, int y, int w, int h, int ignoreColor)
{
   Object g = p->obj[0];
   dither(g, p->i32[0], p->i32[1], p->i32[2], p->i32[3], p->i32[4]);
}
//////////////////////////////////////////////////////////////////////////
TC_API void tugG_drawCylindricShade_iiiiii(NMParams p) // totalcross/ui/gfx/Graphics native public void drawCylindricShade(int startColor, int endColor, int startX, int startY, int endX, int endY);
{
   Object g = p->obj[0];
   drawCylindricShade(g, p->i32[0], p->i32[1], p->i32[2], p->i32[3], p->i32[4], p->i32[5]);
}

#ifdef ENABLE_TEST_SUITE
#include "gfx_Graphics_test.h"
#endif