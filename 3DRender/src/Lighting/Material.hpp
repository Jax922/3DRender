#pragma once
#include <glm/glm.hpp>

class Material
{
public:
	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
		: m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess)
	{
	}

	inline glm::vec3 GetAmbient() const { return m_ambient; };
	inline glm::vec3 GetDiffuset() const { return m_diffuse; };
	inline glm::vec3 GetSpecular() const { return m_specular; };
	inline float GetShininess() const { return m_shininess; };

	inline void SetAmbient(glm::vec3 value) { m_ambient = value; };
	inline void SetDiffuse(glm::vec3 value) { m_diffuse = value; };
	inline void SetSpecular(glm::vec3 value) { m_specular = value; };
	inline void SetShininess(float value) { m_shininess = value; };

private:
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
	float m_shininess;
};