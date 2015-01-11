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

#include <Burngine/Graphics/Scene/Material.hpp>

namespace burn {

	Material::Material() :
	m_type(COLORED),
	m_shininess(1.f),
	m_diffuseColor(1.f, 0.7f, 0.f) {

	}

	void Material::setType(const Type& type) {
		m_type = type;
	}
	const Material::Type& Material::getType() const {
		return m_type;
	}

	void Material::setShininess(const float& shininess) {
		m_shininess = shininess;
	}
	const float& Material::getShininess() const {
		return m_shininess;
	}
	void Material::setDiffuseColor(const Vector3f& color) {
		m_diffuseColor = color;
	}
	const Vector3f& Material::getDiffuseColor() const {
		return m_diffuseColor;
	}

	void Material::setDiffuseTexture(const Texture2D& texture) {
		m_diffuseTexture = texture;
	}
	void Material::setNormalTexture(const Texture2D& texture) {
		m_normalTexture = texture;
	}
	const Texture2D& Material::getDiffuseTexture() const {
		return m_diffuseTexture;
	}
	const Texture2D& Material::getNormalTexture() const {
		return m_normalTexture;
	}

	void Material::setReflectionCubeMap(const CubeMap& cubeMap) {
		m_reflectionCubeMap = cubeMap;
	}
	const CubeMap& Material::getReflectionCubeMap() const {
		return m_reflectionCubeMap;
	}

} /* namespace burn */
