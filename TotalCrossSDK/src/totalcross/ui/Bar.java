/*********************************************************************************
 *  TotalCross Software Development Kit                                          *
 *  Copyright (C) 2000-2012 SuperWaba Ltda.                                      *
 *  All Rights Reserved                                                          *
 *                                                                               *
 *  This library and virtual machine is distributed in the hope that it will     *
 *  be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of    *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                         *
 *                                                                               *
 *  This file is covered by the GNU LESSER GENERAL PUBLIC LICENSE VERSION 3.0    *
 *  A copy of this license is located in file license.txt at the root of this    *
 *  SDK or can be downloaded here:                                               *
 *  http://www.gnu.org/licenses/lgpl-3.0.txt                                     *
 *                                                                               *
 *********************************************************************************/

package totalcross.ui;

import totalcross.sys.*;
import totalcross.ui.event.*;
import totalcross.ui.gfx.*;
import totalcross.ui.image.*;
import totalcross.util.*;

/** 
 * This class provides a title area and a button area (at right). The title and the button are optional, although it doesn't make sense to have a 
 * <code>Bar</code> without title and buttons.
 * 
 * You can add or remove buttons, and change the title text; the title text can have an icon at left.
 * 
 * Here's an example of how to use it, taken from the old UIGadgets sample:
 *
 * <pre>
 * final Bar h1,h2;
 * Font f = Font.getFont(true,Font.NORMAL_SIZE+2);
 * h1 = new Bar("fakeboot");
 * h1.canSelectTitle = true;
 * h1.setFont(f);
 * h1.setBackForeColors(0x0A246A,Color.WHITE);
 * h1.addButton(new Image("ic_dialog_alert.png"));
 * h1.addButton(new Image("ic_dialog_info.png"));
 * add(h1, LEFT,0,FILL,PREFERRED); // use 0 instead of TOP to overwrite the default menu area
 * </pre>
 * 
 * A <code>ControlEvent.PRESSED</code> is sent to the caller, and the button index can be retrieved using the <code>getSelectedIndex()</code> method.
 * 
 * By default, the background is shaded. You can change it to plain using <code>h1.backgroundStyle = BACKGROUND_SOLID;</code>. 
 */
public class Bar extends Container
{
   private BarButton title;
   private Vector icons = new Vector(2);
   private boolean initialized;
   private int selected=-1;
   private int c1,c2,c3,c4,tcolor,pcolor;
   private Spinner spinner;
   
   /** 
    * Set to <code>true</code> to allow the title to be selected and send events. 
    */
   public boolean canSelectTitle;
   
   /** 
    * The title horizontal alignment (<code>LEFT</code>, <code>CENTER</code>, or <code>RIGHT</code>). Defaults to <code>LEFT</code>. 
    */
   public int titleAlign = LEFT;
   
   /** 
    * The buttons horizontal alignment (<code>LEFT</code> or <code>RIGHT</code>). Defaults to <code>RIGHT</code>. 
    */
   public int buttonAlign = RIGHT;
   
   private class BarButton extends Control
   {
      String title;
      Image icon0,icon;
      int gap,px,py;
      boolean pressed;
      Image leftIcon,leftIcon0;
      int autoRepeatRate;
      private TimerEvent repeatTimer;
      private int startRepeat;
      
      BarButton(String title, Image icon) // title or icon
      {
         this.title = title;
         this.icon0 = icon;
      }
      
      public void onFontChanged()
      {
         if (title != null)
         {
            gap = fm.charWidth(' ');
            if (leftIcon0 != null)
               try
               {
                  leftIcon = null;
                  leftIcon = leftIcon0.getSmoothScaledInstance(leftIcon0.getWidth()*fmH/leftIcon0.getHeight(),fmH);
               } catch (ImageException e) {icon = icon0;}
         }
         else
         try
         {
            icon = null;
            if (icon0 != null)
               icon = icon0.getSmoothScaledInstance(icon0.getWidth()*fmH/icon0.getHeight(),fmH);
         } catch (ImageException e) {icon = icon0;}
      }
      
      public void onBoundsChanged(boolean b)
      {
         onFontChanged();
         if (title != null)
         {
            px = titleAlign == LEFT ? gap+1 : titleAlign == CENTER ? (width-fm.stringWidth(title))/2 : (width-fm.stringWidth(title)-gap);
            py = (height-fmH)/2;
         }
         else
         if (icon != null)
         {
            px = (width -icon.getWidth()) /2;
            py = (height-icon.getHeight())/2;
         }
      }
      
      public void onPaint(Graphics g)
      {
         int w = width;
         int h = height;
         
         if (pressed)
            g.fillShadedRect(0,0,w,h,true,false,foreColor,pcolor,30);
         
         // draw borders
         g.foreColor = c1; g.drawLine(0,0,w,0);
         g.foreColor = c3; g.drawLine(w-1,0,w-1,h);
         g.foreColor = c4; g.drawLine(0,h-1,w,h-1);
         g.foreColor = c2; 
         if (backgroundStyle == BACKGROUND_SHADED) 
            g.fillShadedRect(0,1,1,h-2,true,false,foreColor,c2,30); // drawLine causes an unexpected effect on shaded backgrounds
         else
            g.drawLine(0,0,0,h); 

         // draw contents
         if (title != null)
         {
            g.setClip(gap,0,w-gap-gap,h);
            int tx = px;
            if (leftIcon != null)
            {
               g.drawImage(leftIcon,px,(height-leftIcon.getHeight())/2);
               tx += leftIcon.getWidth()+gap;
            }
            
            g.foreColor = tcolor;
            g.drawText(title, tx+1,py-1);
            g.foreColor = backColor;
            g.drawText(title, tx-1,py+1);
            g.foreColor = foreColor;
            g.drawText(title, tx,py);
         }
         else
         if (icon != null)
            g.drawImage(icon, px,py);
      }
      
      public void onEvent(Event e)
      {
         if ((!canSelectTitle && title != null) || Flick.currentFlick != null)
            return;
         
         switch (e.type)
         {
            case TimerEvent.TRIGGERED:
               if (repeatTimer != null && repeatTimer.triggered)
               {
                  if (startRepeat-- <= 0)
                  {
                     selected = appId;
                     parent.postPressedEvent();
                  }                     
               }
               break;
            case PenEvent.PEN_DOWN:
               pressed = true;
               Window.needsPaint = true;
               if (autoRepeatRate != 0)
               {
                  startRepeat = 2;
                  repeatTimer = addTimer(autoRepeatRate);
               }
               break;
            case PenEvent.PEN_UP:
               if (pressed)
               {
                  selected = appId;
                  boolean fired = repeatTimer != null && startRepeat <= 0;
                  pressed = false;
                  if (repeatTimer != null)
                     removeTimer(repeatTimer);
                  if (!fired)
                     parent.postPressedEvent();
               }
               else 
               {
                  selected = -1;
                  if (repeatTimer != null)
                     removeTimer(repeatTimer);
               }
               Window.needsPaint = true;
               break;
            case PenEvent.PEN_DRAG:
            {
               PenEvent pe = (PenEvent)e;
               boolean armed = isInsideOrNear(pe.x,pe.y);
               if (armed != pressed)
               {
                  pressed = armed;
                  Window.needsPaint = true;
               }
               break;
            }
            case KeyEvent.ACTION_KEY_PRESS:
               selected = appId;
               parent.postPressedEvent();
               break;
         }
      }
   }
   
   /** 
    * Constructs a <code>Bar</code> object without a title. Note that if you call the <code>setTitle()</code> method, a <code>RuntimeException</code> 
    * will be thrown. 
    *
    * If you want to change the title later, use the other constructor and pass an empty string (<code>""</code>).
    */
   public Bar()
   {
      this(null);
   }
   
   /** 
    * Constructs a <code>Bar</code> object with the given title. 
    *
    * @param title The bar title.
    */
   public Bar(String title)
   {
      this.title = title != null ? new BarButton(title,null) : null;
      this.backgroundStyle = BACKGROUND_SHADED;
      setFont(font.asBold());
   }
   
   /** 
    * An image icon that can be placed at the left of the title. It only shows if there's a title set. Pass <code>null</code> to remove the icon if 
    * it was previously set.
    * 
    * @param icon The image icon.
    */
   public void setIcon(Image icon)
   {
      if (title != null)
      {
         title.leftIcon0 = icon;
         title.leftIcon = null;
         if (initialized) initUI();
      }
   }

   /** 
    * Changes the title to the given one. 
    *
    * @param newTitle The bar new title.
    */
   public void setTitle(String newTitle)
   {
      if (this.title == null)
         throw new RuntimeException("You can only set a title if you set one in the Bar's constructor.");
      title.title = newTitle;
      title.onBoundsChanged(false);
      Window.needsPaint = true;
   }
   
   /** 
    * Retrieves the current title. 
    *
    * @return The bar title.
    */
   public String getTitle()
   {
      return this.title == null ? "" : title.title;
   }
   
   /** 
    * Adds an image button. 
    *
    * @param icon The image to the add to a button in the bar.
    * @return The button index
    */
   public int addButton(Image icon)
   {
      return addControl(new BarButton(null,icon));
   }
   
   /** 
    * Sets the given button with an auto-repeat interval in the given milliseconds. 
    *
    * @param idx The index of the button in the bar.
    * @param ms The auto-repeat interval in milliseconds.
    */
   public void setButtonRepeatRate(int idx, int ms)
   {
      ((BarButton)icons.items[idx]).autoRepeatRate = ms;
   }
   
   /** 
    * Adds a control to the bar. Not all types of controls are supported. 
    *
    * @param c The control to be added.
    * @return The button index
    */
   public int addControl(Control c)
   {
      icons.addElement(c);
      for (int i = icons.size(); --i >= 0;) ((Control)icons.items[i]).appId = i+1; // update appId used for selection
      if (initialized) initUI();
      return icons.size();
   }
   
   /** 
    * Removes a button at the given index, starting at 1. 
    *
    * @param index The index of the button to be removed.
    */
   public void removeButton(int index)
   {
      icons.removeElementAt(index-1);
      for (int i = icons.size(); --i >= 0;) ((Control)icons.items[i]).appId = i+1;
      if (initialized) initUI();
   }
   
   /** 
    * Returns the selected button, or -1 if none was selected.
    * 
    * The title always has index 0 (even if there's no title), and the button' index start at index 1.
    * 
    * @return The index of the selected button.
    */
   public int getSelectedIndex()
   {
      return selected;
   }
   
   /**
    * Called to initialize the user interface of this container. 
    */
   public void initUI()
   {
      removeAll();
      int n = icons.size();
      if (title == null) // if there's no title, make the icons take the whole size of the container
      {
         for (int i = n; --i > 0;)
            add((Control)icons.items[i], i==n-1 ? RIGHT : BEFORE, TOP, width/n, FILL);
         if (n > 0)
            add((Control)icons.items[0], LEFT, TOP, n == 1 ? FILL : FIT, FILL);
      }
      else
      {
         for (int i = n; --i >= 0;)
            add((Control)icons.items[i], i==n-1 ? buttonAlign : (buttonAlign == RIGHT ? BEFORE : AFTER), TOP, height, FILL);
         if (titleAlign == LEFT)
            add(title, LEFT, TOP, n == 0 ? FILL : FIT, FILL); 
         else
            add(title, AFTER, TOP, FILL, FILL); 
         if (spinner != null)
         {
            add(spinner,RIGHT_OF-(n==0 ? fmH/2 : height),CENTER_OF,fmH,fmH,this.title);
            spinner.setVisible(false);
         }
      }
      initialized = true;
   }
   
   /**
    * Called after a <code>setEnabled()</code>, <code>setForeColor()</code>, or <code>setBackColor()</code>; or when a control has been added to a 
    * container. If <code>colorsChanged</code> is <code>true</code>, it was called from <code>setForeColor()</code>/<code>setBackColor()</code>/
    * <code>Container.add()</code>; otherwise, it was called from <code>setEnabled()</code>.
    *
    * @param colorsChanged Indicates if the control colors have changed, which happens after a <code>setForeColor()</code>, 
    * <code>setBackColor()</code>, or <code>Container.add()</code>. 
    */
   public void onColorsChanged(boolean colorsChanged)
   {
      c1 = Color.brighter(backColor,30);
      c2 = Color.brighter(backColor,60);
      c3 = Color.darker(backColor,30);
      c4 = Color.darker(backColor,60);
      tcolor = Color.darker(backColor,32);
      pcolor = Color.interpolate(backColor,foreColor);
   }
   
   /** 
    * Returns the preferred width of this control. 
    * 
    * @return The preferred width of this control.
    */
   public int getPreferredWidth()
   {
      return parent == null ? FILL : parent.width;
   }
   
   /**
    * Returns the preferred height of this control. 
    *
    * @return The preferred height of this control.
    */
   public int getPreferredHeight()
   {
      return fmH*2;
   }
   
   /** 
    * Shows and starts the spinner (if one has been assigned to the <code>spinner</code> field).
    * 
    * @see #spinner
    */
   public void startSpinner()
   {
      spinner.setVisible(true);
      spinner.start();
   }
   
   /** Updates the spinner; sets it visible if not yet. */
   public void updateSinner()
   {
      if (!spinner.visible)
         spinner.setVisible(true);
      spinner.update();
   }
   
   /** 
    * Stops and hides the spinner (if createSpinner was called before)
    * 
    * @see #spinner
    */
   public void stopSpinner()
   {
      spinner.stop();
      spinner.setVisible(false);
   }
   
   /**
    * Repositions this control, calling again <code>setRect()</code> with the original parameters. 
    */
   public void reposition()
   {
      super.reposition();
      initUI();
   }
   
   /** 
    * Assigns the BACK key on Android (mapped to <code>SpecialKeys.ESCAPE</code>) to the given button. This can only be called after the bar has been 
    * added to a container.
    * 
    * For example, if button 1 is assigned with <code>totalcross.res.Resources.back</code>, call <code>assignBackKeyToButton(1);</code>.
    *
    * @param idx The index of the bar button, starting at 1.
    */
   public void assignBackKeyToButton(int idx)
   {
      final int i = idx;
      Window w = getParentWindow();
      w.addKeyListener(new KeyListener()
      {
         public void keyPressed(KeyEvent e) {}
         public void actionkeyPressed(KeyEvent e) {}
         public void specialkeyPressed(KeyEvent e)
         {
            if (e.key == SpecialKeys.ESCAPE)
            {
               e.consumed = true;
               selected = ((BarButton)icons.items[i]).appId;
               postPressedEvent();
            }
         }
      });
      w.callListenersOnAllTargets = true;
   }

   /** 
    * Creates a spinner with the following color. The spinner will be placed at the right of the title (it only works if there's a title).
    *
    * @param color The spinner color.
    */
   public void createSpinner(int color)
   {
      spinner = new Spinner();
      spinner.setForeColor(color);
   }
}
