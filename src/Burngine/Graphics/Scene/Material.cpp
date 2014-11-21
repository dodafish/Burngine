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
	m_diffuseColor(0.f),
	m_specularColor(0.f),
	m_ambientColor(0.f),
	m_shininess(1.f) {

	}

	void Material::setDiffuseColor(const Vector3f& color) {
		m_diffuseColor = color;
	}

	const Vector3f& Material::getDiffuseColor() const {
		return m_diffuseColor;
	}

	const Vector3f& Material::getSpecularColor() const {
		return m_specularColor;
	}

	void Material::setSpecularColor(const Vector3f& color) {
		m_specularColor = color;
	}

	const Vector3f& Material::getAmbientColor() const {
		return m_ambientColor;
	}

	void Material::setAmbientColor(const Vector3f& color) {
		m_ambientColor = color;
	}

	void Material::setShininess(const float& shininess) {
		m_shininess = shininess;
	}

	const float& Material::getShininess() const {
		return m_shininess;
	}

	void Material::setDiffuseTexture(const Texture& texture) {
		m_diffuseTexture = texture;
	}

	const Texture& Material::getDiffuseTexture() const {
		return m_diffuseTexture;
	}

} /* namespace burn */
