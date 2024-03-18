/*
 * LibrePCB - Professional EDA for everyone!
 * Copyright (C) 2013 LibrePCB Developers, see AUTHORS.md for contributors.
 * https://librepcb.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*******************************************************************************
 *  Includes
 ******************************************************************************/
#include <gtest/gtest.h>
#include <librepcb/core/geometry/polygon.h>
#include <librepcb/core/serialization/sexpression.h>
#include <librepcb/core/types/layer.h>

/*******************************************************************************
 *  Namespace
 ******************************************************************************/
namespace librepcb {
namespace tests {

/*******************************************************************************
 *  Test Class
 ******************************************************************************/

class PolygonTest : public ::testing::Test {};

/*******************************************************************************
 *  Test Methods
 ******************************************************************************/

TEST_F(PolygonTest, testConstructFromSExpression) {
  SExpression sexpr = SExpression::parse(
      "(polygon 2889d60c-1d18-44c3-bf9e-07733b67e480 (layer bot_stop_mask)\n"
      " (width 0.1) (fill true) (grab_area false)\n"
      " (vertex (position 0.0 0.0) (angle 0.0))\n"
      " (vertex (position 120.0 0.0) (angle 0.0))\n"
      " (vertex (position 120.0 4.0) (angle 0.0))\n"
      " (vertex (position 0.0 4.0) (angle 0.0))\n"
      " (vertex (position 0.0 0.0) (angle 0.0))\n"
      ")\n",
      FilePath());
  Polygon obj(sexpr);
  EXPECT_EQ(Uuid::fromString("2889d60c-1d18-44c3-bf9e-07733b67e480"),
            obj.getUuid());
  EXPECT_EQ("bot_stop_mask", obj.getLayer().getId());
  EXPECT_EQ(UnsignedLength(100000), obj.getLineWidth());
  EXPECT_EQ(true, obj.isFilled());
  EXPECT_EQ(false, obj.isGrabArea());
  EXPECT_EQ(5, obj.getPath().getVertices().count());
}

TEST_F(PolygonTest, testSerializeAndDeserialize) {
  Polygon obj1(
      Uuid::createRandom(), Layer::botCopper(), UnsignedLength(456), true,
      false,
      Path({Vertex(Point(1, 2), Angle(3)), Vertex(Point(4, 5), Angle(6))}));
  SExpression sexpr1 = SExpression::createList("obj");
  obj1.serialize(sexpr1);

  Polygon obj2(sexpr1);
  SExpression sexpr2 = SExpression::createList("obj");
  obj2.serialize(sexpr2);

  EXPECT_EQ(sexpr1.toByteArray(), sexpr2.toByteArray());
}

/*******************************************************************************
 *  End of File
 ******************************************************************************/

}  // namespace tests
}  // namespace librepcb
