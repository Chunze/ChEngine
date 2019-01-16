#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "DrawList.h"

DrawList::DrawList(GameContext* gameContext)
	: BaseClass(gameContext)
{

}

void DrawList::AddToDrawQ(DrawListElement& elementToAdd, bool bIsDynamic)
{
	bHasNewData = true;

	// vertex array
	if (elementToAdd.vertexArrayObject == 0)
	{
		glGenVertexArrays(1, &elementToAdd.vertexArrayObject);
	}
	glBindVertexArray(elementToAdd.vertexArrayObject);

	// vertex buffer and populate data
	if (elementToAdd.vertexBufferObject == 0)
	{
		glGenBuffers(1, &elementToAdd.vertexBufferObject);
	}
	glBindBuffer(GL_ARRAY_BUFFER, elementToAdd.vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, elementToAdd.VBsize_inByte, elementToAdd.vertexBuffer, GL_STATIC_DRAW);

	if (elementToAdd.drawingMode == DrawingMode::DRAW_ELEMENT)
	{
		// index buffer and populate data
		if (elementToAdd.indexBufferObject == 0)
		{
			glGenBuffers(1, &elementToAdd.indexBufferObject);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementToAdd.indexBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementToAdd.IBsize_inByte, elementToAdd.indexBuffer, GL_STATIC_DRAW);
	}

	size_t index = 0;
	int offset = 0;
	for (int attributeSize : elementToAdd.attributeSizes)
	{
		int attribute = elementToAdd.attributeSizes[index];

		glVertexAttribPointer(index, attribute, GL_FLOAT, GL_FALSE, elementToAdd.vertextInfoSize * sizeof(float), (void*)offset);
		offset += attributeSize * sizeof(float);
		++index;
	}

	elementToAdd.numOfVertices = elementToAdd.VBsize_inByte / (elementToAdd.vertextInfoSize * sizeof(float));
	if (bIsDynamic)
	{
		m_DynamicElements.push(elementToAdd);
	}
	else
	{
		m_StaticElements.push(elementToAdd);
	}
	
}

void DrawList::AddOnScreenText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	OnScreenTextElement TextToRender(text, x, y, scale, color);

	m_OnScreenTexts.push(TextToRender);
}

void DrawList::Clear()
{
	
}

void DrawListElement::Init()
{
	vertexBufferObject = 0;
	indexBufferObject = 0;
	vertexArrayObject = 0;
}

void DrawListElement::GetRenderReady()
{
	// bind buffers
	glBindVertexArray(vertexArrayObject);

	// enable attribute pointers
	for (size_t i = 0; i < attributeSizes.size(); i++)
	{
		glEnableVertexAttribArray(i);
	}
	
	//glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject); 
	shader.Use();
}

void DrawListElement::DisableAttributePointer()
{
	for (size_t i = 0; i < attributeSizes.size(); i++)
	{
		glDisableVertexAttribArray(i);
	}
}
