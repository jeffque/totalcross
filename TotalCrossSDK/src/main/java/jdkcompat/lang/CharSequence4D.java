// Copyright (C) 2017-2020 TotalCross Global Mobile Platform Ltda.
//
// SPDX-License-Identifier: LGPL-2.1-only

package jdkcompat.lang;

public interface CharSequence4D {
  int length();

  char charAt(int index);

  CharSequence subSequence(int start, int end);

  @Override
  public String toString();
}
