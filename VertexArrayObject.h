#pragma once
#include "VertexObject.h"
#include <glad/glad.h>
class VertexArrayObject :
    public VertexObject
{
    public VertexArrayObject()
    {
        id = glGenVertexArrays();
    }
};

