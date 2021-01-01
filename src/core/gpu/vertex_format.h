#pragma once

#include "glm_utils.h"


struct VertexFormat
{
	VertexFormat(glm::vec3 position,
		glm::vec3 color = glm::vec3(1),
		glm::vec3 normal = glm::vec3(0, 1, 0),
		glm::vec2 text_coord = glm::vec2(0))
		: position(position), normal(normal), text_coord(text_coord), color(color) { };

	// position of the vertex
	glm::vec3 position;

	// vertex normal
	glm::vec3 normal;

	// vertex texture coordinate
	glm::vec2 text_coord;

	// vertex color
	glm::vec3 color;
};
