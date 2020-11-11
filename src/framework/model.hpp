#pragma once

#include <string>
#include <vector>
#include "globals.hpp"

namespace framework
{
	class Model
	{
	private:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;

	public:
		Model(const std::string& filepath);

		inline std::vector<Vertex> GetVertices() { return m_Vertices; }
		inline std::vector<uint32_t> GetIndices()  { return m_Indices; }
	};
}