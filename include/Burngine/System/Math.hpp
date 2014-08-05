//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU LGPL v3 License
// ====================================================
//
//    Copyright (C) 2014 Dominik David (frischer-hering@gmx.de)
//
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Lesser General Public
//    License as published by the Free Software Foundation;
//    version 3.0 of the License
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public
//    License along with this library; if not, write to the Free Software
//    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
//    USA
//
//////////////////////////////////////////////////////////////////////////////

#ifndef MATH_HPP_
#define MATH_HPP_

#include <Burngine/Export.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace burn {

//Floating
typedef glm::detail::tvec4<float> Vector4f;    //x,y,z,w / r,g,b,a
typedef glm::detail::tvec3<float> Vector3f;    //x,y,z / r,g,b
typedef glm::detail::tvec2<float> Vector2f;    //x,y / u,v

//Double
typedef glm::detail::tvec4<double> Vector4d;
typedef glm::detail::tvec3<double> Vector3d;
typedef glm::detail::tvec2<double> Vector2d;

//Int
typedef glm::detail::tvec4<Int32> Vector4i;
typedef glm::detail::tvec3<Int32> Vector3i;
typedef glm::detail::tvec2<Int32> Vector2i;

//Unsigned Int
typedef glm::detail::tvec4<Uint32> Vector4ui;
typedef glm::detail::tvec3<Uint32> Vector3ui;
typedef glm::detail::tvec2<Uint32> Vector2ui;

typedef glm::mat4 Matrix4f;

typedef glm::quat Quaternion;

}

#endif /* MATH_HPP_ */
