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

#include <Burngine/System/RotationUtil.hpp>
#include <Burngine/System/Math.hpp>
#include <glm/gtx/norm.hpp>
#include <Burngine/System/Error.hpp>

namespace burn {

	using namespace glm;

	Quaternion RotationUtil::RotationBetweenVectors(Vector3f start,
													Vector3f dest) {

		// Check for valid vectors
		if(start == Vector3f(0.f) || dest == Vector3f(0.f))
			burnErr("Invalid vectors. Cannot determine rotation.");

		start = normalize(start);
		dest = normalize(dest);

		float cosTheta = dot(start, dest);
		vec3 rotationAxis;

		if(cosTheta < -1 + 0.001f){
			// special case when vectors in opposite directions:
			// there is no "ideal" rotation axis
			// So guess one; any will do as long as it's perpendicular to start
			rotationAxis = cross(vec3(0.0f, 0.0f, 1.0f), start);
			if(length2(rotationAxis) < 0.01)    // bad luck, they were parallel, try again!
				rotationAxis = cross(vec3(1.0f, 0.0f, 0.0f), start);

			rotationAxis = normalize(rotationAxis);
			return angleAxis(180.0f, rotationAxis);
		}

		rotationAxis = cross(start, dest);

		float s = sqrt((1 + cosTheta) * 2);
		float invs = 1 / s;

		return quat(s * 0.5f, rotationAxis.x * invs, rotationAxis.y * invs, rotationAxis.z * invs);
	}

} /* namespace burn */
