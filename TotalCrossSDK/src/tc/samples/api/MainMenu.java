/*********************************************************************************
 *  TotalCross Software Development Kit                                          *
 *  Copyright (C) 2000-2014 SuperWaba Ltda.                                      *
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

package tc.samples.api;

import tc.samples.api.crypto.*;
import tc.samples.api.io.*;
import tc.samples.api.io.device.*;
import tc.samples.api.lang.reflection.*;
import tc.samples.api.lang.thread.*;
import tc.samples.api.map.*;
import tc.samples.api.net.*;
import tc.samples.api.net.mail.*;
import tc.samples.api.phone.*;
import tc.samples.api.sql.*;
import tc.samples.api.sys.*;
import tc.samples.api.ui.*;
import tc.samples.api.ui.SignatureSample;
import tc.samples.api.util.*;
import tc.samples.api.xml.*;

import totalcross.sys.*;
import totalcross.ui.*;
import totalcross.ui.dialog.*;
import totalcross.ui.event.*;
import totalcross.ui.font.*;
import totalcross.ui.gfx.*;

public class MainMenu extends BaseContainer
{
   private ButtonMenu menu;
   
   static String DEFAULT_INFO = "Click Info for help";
   
   String[] uiItems =
   {
      "AlignedLabelsContainer",
      "Button",
      "ButtonMenu",
      "Camera",
      "Chart",
      "Check/Radio",
      "Combo/ListBox",
      "Dynamic ScrollContainer",
      "Edit",
      "Font sizes",
      "Grid",
      "HtmlContainer",
      "ImageControl",
      "Image Animation",
      "Image book",
      "Image modifiers",
      "ListContainer",
      "MessageBox",
      "MultiButton",
      "MultiEdit",
      "Multi touch",
      "ProgressBar",
      "ProgressBox",
      "ScrollContainer",
      "Signature",
      "TabbedContainer",
      "Other controls",
   };
   

   String[] cryptoItems =
   {
      "crypto - Cipher",
      "crypto - Digest",
      "crypto - Signature",
   };
   
   String[] ioItems = 
   {
      "File",
      "PDBFile",
      "Bluetooth Print (Citizen)",
      "Bluetooth Print (Zebra)",
      "Bluetooth Transfer",
      "GPS",
      "Scanner Internal",
      "Scanner Camera",
      "PortConnector",
   };
   
   String[] langItems = 
   {
      "Reflection",
      "Thread",
   };
   
   String[] mapItems = 
   {
      "GoogleMaps",
   };
   
   String[] netItems =
   {
      "Mail",
      "FTP",
      "Server socket",
      "Socket Http",
      "Socket Https",
   };
   
   String[] phoneItems =
   {
      "Dialer",
      "SMS",
   };
   
   String[] sqlItems =
   {
      "SQLite Bench",
   };
   
   String[] sysItems = 
   {
      "Settings",
      "External Viewers",
   };
   
   String[] utilItems =
   {
      "Zip",
      "Zlib",
   };
   
   String[] xmlItems =
   {
      "Soap",
   };
   
   String[] categs =
   {
      "ui",
      "crypto",
      "io",
      "lang",
      "map",
      "net",
      "phone",
      "sql",
      "sys",
      "util",
      "xml",
   };

   Class[] uiClasses =
   {
      AlignedLabelsSample.class,
      ButtonSample.class,
      ButtonMenuSample.class,
      CameraSample.class,
      ChartSample.class,
      CheckRadioSample.class,
      ComboListSample.class,
      DynScrollContainerSample.class,
      EditSample.class,
      FontSample.class,
      GridSample.class,
      HtmlContainerSample.class,
      ImageControlSample.class,
      AnimationSample.class,
      ImageBookSample.class,
      ImageModifiersSample.class,
      ListContainerSample.class,
      MessageBoxSample.class,
      MultiButtonSample.class,
      MultiEditSample.class,
      MultitouchSample.class,
      ProgressBarSample.class,
      ProgressBoxSample.class,
      ScrollContainerSample.class,
      SignatureSample.class,      
      TabbedContainerSample.class,
      OtherControlsSample.class,
   };
   
   Class[] cryptoClasses =
   {
      CipherSample.class,
      DigestSample.class,
      SignatureSample.class,
   };
     
   Class[] ioClasses =
   {
      FileSample.class,
      PDBFileSample.class,
      PrinterCitizen.class,
      PrinterZebra.class,
      BTTransfer.class,
      GpsSample.class,
      ScannerInternal.class,
      ScannerZXing.class,
      PortConnectorSample.class,
   };
   
   Class[] langClasses = 
   {      
      ReflectionSample.class,
      ThreadSample.class,
   };     
      Class[] mapClasses = 
   {      
      GoogleMapsSample.class,
   };     
      Class[] netClasses =
   {      
      MailSample.class,
      FTPSample.class,
      ServerSocketSample.class,
      SocketSample.class,
      SecureSocketSample.class,
   };     
   Class[] phoneClasses =
   {      
      PhoneDialerSample.class,
      PhoneSmsSample.class,
   };     
   Class[] sqlClasses =
   {      
      SQLiteBenchSample.class,
   };     
   Class[] sysClasses = 
   {      
      SettingsSample.class,
      ExternalViewersSample.class,
   };     
   Class[] utilClasses =
   {      
      ZipSample.class,
      ZLibSample.class,
   };     
   Class[] xmlClasses =
   {      
      SoapSample.class,
   };

   protected String getHelpMessage()
   {
      return "This is a TotalCross "+Settings.versionStr+"."+Settings.buildNumber+" sample that shows most of the Application Programming Interfaces available in the SDK. You may drag the menu up and down. Device information: screen: "+Settings.screenWidth+"x"+Settings.screenHeight+", device id: "+Settings.deviceId+", font size: "+Font.NORMAL_SIZE;
   }
   
   ButtonMenu topmenu;
   
   String[][] items =
   {
      uiItems,
      cryptoItems,
      ioItems,
      langItems, 
      mapItems, 
      netItems,
      phoneItems,
      sqlItems,
      sysItems,
      utilItems,
      xmlItems
   };
   
   Class[][] classes =
   {
      uiClasses,
      cryptoClasses,
      ioClasses,
      langClasses, 
      mapClasses, 
      netClasses,
      phoneClasses,
      sqlClasses,
      sysClasses,
      utilClasses,
      xmlClasses
   };
   
   public void initUI()
   {
      super.initUI(); // important!
      
      // single-row
      topmenu = new ButtonMenu(categs, ButtonMenu.SINGLE_ROW);
      topmenu.textPosition = BOTTOM;
      topmenu.buttonHorizGap = topmenu.buttonVertGap = 25;
      topmenu.setBackForeColors(Color.brighter(BKGCOLOR), Color.WHITE);
      topmenu.pressedColor = Color.CYAN;
      add(new Ruler(), LEFT,TOP,FILL,0); // this ruler makes the ScrollContainer have the same width always. otherwise, when changing the UpdateMatrix, it will be shrinked in 10 pixels at the width 
      add(topmenu,LEFT,TOP,FILL,PREFERRED);

      setInfo(DEFAULT_INFO);

/*      String cmd = MainWindow.getCommandLine();
      if (cmd != null && cmd.startsWith("/t"))
         try 
         {
            showSample(Convert.toInt(cmd.substring(2)));
            return;
         }
         catch (Exception e) {}
*/   }

   
   Class itemClasses;
   private void showMenu(String[] names)
   {
      if (menu != null) remove(menu);
      menu = new ButtonMenu(names, ButtonMenu.MULTIPLE_VERTICAL);
      
      menu.pressedColor = BKGCOLOR;
      if (Math.max(Settings.screenWidth,Settings.screenHeight)/Font.NORMAL_SIZE > 30)
      {
         menu.borderGap = 100;
         menu.buttonHorizGap = menu.buttonVertGap = 200;
      }
      else menu.buttonHorizGap = 50;
      
      add(menu,LEFT,AFTER,FILL,FILL,topmenu);
   }
   
   int topmenuSel;   
   public void onEvent(Event e)
   {
      if (e.type == ControlEvent.PRESSED)
      {
         try
         {
            if (e.target == topmenu)
            {
               topmenuSel = topmenu.getSelectedIndex();
               if (topmenuSel == -1)
                  remove(menu);
               else
                  showMenu(items[topmenuSel]);
            }
            else
            if (e.target == menu)
            {
               Class[] itemClasses = classes[topmenuSel];
               int idx = menu.getSelectedIndex();
               if (0 <= idx && idx < itemClasses.length)
                  showSample(itemClasses[idx]);
            }
         }
         catch (Exception ee)
         {
            MessageBox.showException(ee,true);
         }
      }
   }

   private void showSample(Class c) throws Exception
   {
      BaseContainer bc = (BaseContainer)c.newInstance();
      bc.info = "Press Back for main menu";
      bc.show();
      bc.setInfo(bc.info);
   }
   
   public void onAddAgain()
   {
      getParentWindow().setMenuBar(null);
   }
}