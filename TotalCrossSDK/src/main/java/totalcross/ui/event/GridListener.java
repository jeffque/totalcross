/*********************************************************************************
 *  TotalCross Software Development Kit                                          *
 *  Copyright (C) 2000-2012 SuperWaba Ltda.                                      *
 *  Copyright (C) 2012-2020 TotalCross Global Mobile Platform Ltda.   
 *  All Rights Reserved                                                          *
 *                                                                               *
 *  This library and virtual machine is distributed in the hope that it will     *
 *  be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of    *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                         *
 *                                                                               *
 *  This file is covered by the GNU LESSER GENERAL PUBLIC LICENSE VERSION 2.1    *
 *  A copy of this license is located in file license.txt at the root of this    *
 *  SDK or can be downloaded here:                                               *
 *  http://www.gnu.org/licenses/lgpl-2.1.txt                                     *
 *                                                                               *
 *********************************************************************************/

package totalcross.ui.event;

/** Interface used to listen to Grid events. */

public interface GridListener extends EventHandler {
  /** A GRID_SELECTED_EVENT event was dispatched.
   * @see GridEvent 
   */
  public void gridSelected(GridEvent e);

  /** A CHECK_CHANGED_EVENT event was dispatched.
   * @see GridEvent 
   */
  public void gridCheckChanged(GridEvent e);

  /** A TEXT_CHANGED_EVENT event was dispatched.
   * @see GridEvent 
   */
  public void gridTextChanged(GridEvent e);
}
