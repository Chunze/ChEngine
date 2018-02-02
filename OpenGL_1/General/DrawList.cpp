#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "DrawList.h"

DrawList::DrawList(GameContext gameContext)
	: BaseClass(gameContext)
{

}

int DrawList::Add(DrawListElement elementToAdd, int index)
{
	int size = m_elements.size();

	if (index <= 0)
	{
		bHasNewData = true;

		// vertex array
		glGenVertexArrays(1, &elementToAdd.vertexArrayObject);
		glBindVertexArray(elementToAdd.vertexArrayObject);

		// vertex buffer and populate data
		glGenBuffers(1, &elementToAdd.vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, elementToAdd.vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, elementToAdd.VBsize, elementToAdd.vertexBuffer, GL_STATIC_DRAW);


		size_t index = 0;
		int offset = 0;
		for (int attributeSize : elementToAdd.attributeSizes)
		{
			int attribute = elementToAdd.attributeSizes[index];
			int attributeByteSize = attribute * sizeof(float);
			glVertexAttribPointer(index, attribute, GL_FLOAT, GL_FALSE, attributeByteSize, (void*)offset);
			offset += attributeByteSize;
			++index;
		}

		elementToAdd.numOfVertices = elementToAdd.VBsize / elementToAdd.vertextInfoSize;

		m_elements.push_back(elementToAdd);
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, elementToAdd.vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, elementToAdd.VBsize, elementToAdd.vertexBuffer, GL_STATIC_DRAW);
	}

	return size;
}

void DrawList::Clear()
{
	bHasNewData = false;
	m_elements.clear();
}

void DrawListElement::BindBuffers()
{
	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	shader.Use();
}
