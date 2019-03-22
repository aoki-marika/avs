attribute vec3 vertexPosition;

uniform vec3 size;
uniform vec3 position;
uniform mat4 pv; //projection * view

void main()
{
    gl_Position = pv * vec4(position + ((vertexPosition * size) - size), 1);
}
