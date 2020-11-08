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

		inline auto GetVertices() const { return m_Vertices; }
		inline auto GetIndices() const { return m_Indices; }
	};
}