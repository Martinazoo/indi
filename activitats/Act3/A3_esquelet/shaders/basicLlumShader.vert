#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;


uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;
uniform mat3 normalMatrix;



out vec3 fcolor;

out vec3 matambfs;  
out float matshinfs;
out vec3 matspecfs; 
out vec3 matdifffs;
out vec4 vSCO;
out vec3 normSCO;
out vec3 vertexMar;
out mat4 viewMar;
out vec3 normalMar;

//******************************************************************
//***************  Vertex shader  **********************************

//******************************************************************

void main()
{	
    matambfs = matamb;  
    matshinfs = matshin;
    matspecfs = matspec; 
    matdifffs = matdiff;
    viewMar = view;
    //fcolor = matdiff;
    vertexMar = vertex;
    normalMar = normal;
    vSCO = view * TG * vec4(vertex, 1.0);
    normSCO = normalMatrix * normal;
  
    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}

