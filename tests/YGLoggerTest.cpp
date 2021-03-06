/**
 * Copyright (c) 2014-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#include <gtest/gtest.h>
#include <yoga/Yoga.h>
#include <stdarg.h>

namespace {
  char writeBuffer[4096];
  int _unmanagedLogger(YGLogLevel level, const char *format, va_list args) {
    return vsnprintf(writeBuffer + strlen(writeBuffer), sizeof(writeBuffer) - strlen(writeBuffer), format, args);
  }
}

TEST(YogaTest, logger_default_node_should_print_no_style_info) {
  writeBuffer[0] = '\0';
  YGSetLogger(_unmanagedLogger);
  const YGNodeRef root = YGNodeNew();
  YGNodeCalculateLayout(root, YGUnitUndefined, YGUnitUndefined, YGDirectionLTR);
  YGNodePrint(root, (YGPrintOptions)(YGPrintOptionsLayout | YGPrintOptionsChildren | YGPrintOptionsStyle));
  YGSetLogger(NULL);
  YGNodeFree(root);
  
  const char * expected = "<div layout=\"width: 0; height: 0; top: 0; left: 0;\" style=\"\" ></div>";
  ASSERT_STREQ(expected, writeBuffer);
}

TEST(YogaTest, logger_node_with_percentage_absolute_position_and_margin) {
  writeBuffer[0] = '\0';
  YGSetLogger(_unmanagedLogger);
  const YGNodeRef root = YGNodeNew();
  YGNodeStyleSetPositionType(root, YGPositionTypeAbsolute);
  YGNodeStyleSetWidthPercent(root, 50);
  YGNodeStyleSetHeightPercent(root, 75);
  YGNodeStyleSetFlex(root, 1);
  YGNodeStyleSetMargin(root, YGEdgeRight, 10);
  YGNodeStyleSetMarginAuto(root, YGEdgeLeft);
  YGNodeCalculateLayout(root, YGUnitUndefined, YGUnitUndefined, YGDirectionLTR);
  YGNodePrint(root, (YGPrintOptions)(YGPrintOptionsLayout | YGPrintOptionsChildren | YGPrintOptionsStyle));
  YGSetLogger(NULL);
  YGNodeFree(root);
  
  const char * expected = "<div layout=\"width: 0; height: 0; top: 0; left: 0;\" style=\"flex: 1; margin-left: auto; margin-right: 10px; width: 50%; height: 75%; position: absolute; \" ></div>";
  ASSERT_STREQ(expected, writeBuffer);
}

TEST(YogaTest, logger_node_with_children_should_print_indented) {
  writeBuffer[0] = '\0';
  YGSetLogger(_unmanagedLogger);
  const YGNodeRef root = YGNodeNew();
  const YGNodeRef child0 = YGNodeNew();
  const YGNodeRef child1 = YGNodeNew();
  YGNodeInsertChild(root, child0, 0);
  YGNodeInsertChild(root, child1, 1);
  YGNodeCalculateLayout(root, YGUnitUndefined, YGUnitUndefined, YGDirectionLTR);
  YGNodePrint(root, (YGPrintOptions)(YGPrintOptionsLayout | YGPrintOptionsChildren | YGPrintOptionsStyle));
  YGSetLogger(NULL);
  YGNodeFreeRecursive(root);
  
  const char * expected = "<div layout=\"width: 0; height: 0; top: 0; left: 0;\" style=\"\" >\n  <div layout=\"width: 0; height: 0; top: 0; left: 0;\" style=\"\" ></div>\n  <div layout=\"width: 0; height: 0; top: 0; left: 0;\" style=\"\" ></div>\n</div>";
  ASSERT_STREQ(expected, writeBuffer);
}
