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


		int attributeByteSize = 0;
		for (int attributeSize : elementToAdd.attributeSizes)
		{
			attributeByteSize += attributeSize;
		}

		attributeByteSize *= sizeof(float);
		size_t index = 0;
		int offset = 0;
		for (int attributeSize : elementToAdd.attributeSizes)
		{
			int attribute = elementToAdd.attributeSizes[index];
			
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

void DrawList::UpdateElement(int index, float* newVB, int VBsize_byte)
{
	DrawListElement* elementToUpdate = &m_elements[index];
	elementToUpdate->VBsize = VBsize_byte;
	elementToUpdate->vertexBuffer = newVB;
	glBindBuffer(GL_ARRAY_BUFFER, elementToUpdate->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, elementToUpdate->VBsize, elementToUpdate->vertexBuffer, GL_STATIC_DRAW);
}

void DrawList::Clear()
{
	bHasNewData = false;
	m_elements.clear();
}

void DrawListElement::GetRenderReady()
{
	// enable attribute pointers
	for (int i = 0; i < attributeSizes.size(); i++)
	{
		glEnableVertexAttribArray(i);
	}
	// bind buffers
	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	shader.Use();
}

void DrawListElement::DisableAttributePointer()
{
	for (int i = 0; i < attributeSizes.size(); i++)
	{
		glDisableVertexAttribArray(i);
	}
}
