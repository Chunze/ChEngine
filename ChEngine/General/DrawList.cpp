#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "DrawList.h"

DrawList::DrawList(GameContext* gameContext)
	: BaseClass(gameContext)
{

}

void DrawList::AddToDrawQ(DrawListElement& elementToAdd, bool bIsDynamic)
{
	if (!elementToAdd.bInitialized)
	{
		elementToAdd.Init();
	}
	
	if (bIsDynamic)
	{
		m_DynamicElements.push(elementToAdd);
	}
	else
	{
		m_StaticElements.push(elementToAdd);
	}
	
}

void DrawList::AddToDrawQ(std::vector<DrawListElement> elements, bool bIsDynamic /*= true*/)
{
	for (DrawListElement& element : elements)
	{
		AddToDrawQ(element, bIsDynamic);
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
	if (vertexArrayObject == 0)
	{
		glGenVertexArrays(1, &vertexArrayObject);
	}
	glBindVertexArray(vertexArrayObject);

	// vertex buffer and populate data
	if (vertexBufferObject == 0)
	{
		glGenBuffers(1, &vertexBufferObject);
	}
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, VBsize_inByte, vertexBuffer, GL_STATIC_DRAW);

	if (drawingMode == DrawingMode::DRAW_ELEMENT)
	{
		// index buffer and populate data
		if (indexBufferObject == 0)
		{
			glGenBuffers(1, &indexBufferObject);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IBsize_inByte, indexBuffer, GL_STATIC_DRAW);
	}

	size_t index = 0;
	int offset = 0;
	for (int attributeSize : attributeSizes)
	{
		int attribute = attributeSizes[index];

		glVertexAttribPointer(index, attribute, GL_FLOAT, GL_FALSE, vertextInfoSize * sizeof(float), (void*)offset);
		offset += attributeSize * sizeof(float);
		++index;
	}

	numOfVertices = VBsize_inByte / (vertextInfoSize * sizeof(float));

	bInitialized = true;
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
