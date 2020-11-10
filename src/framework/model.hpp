#pragma once

#include <string>
#include <vector>
#include "globals.hpp"

namespace framework
{
	class Model
	{
	private:
		std::vector<Vertex> m_Vertices;		// Contains vertices loaded from model
		std::vector<uint32_t> m_Indices;	// Contains indices loaded from model

	public:
		Model(const std::string& filepath);

		inline auto GetVertices() const { return m_Vertices; }	// Getter for vertices
		inline auto GetIndices() const { return m_Indices; }	// Getter for indices
	};
}