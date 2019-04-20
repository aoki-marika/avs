attribute vec3 vertexPosition;
attribute vec2 vertexUV; //only used for textured fragment shaders

uniform vec3 size;
uniform vec3 position;
uniform mat4 pv; //projection * view

varying vec2 uv;

void main()
{
    gl_Position = pv * vec4(position + ((vertexPosition * size) - size), 1);
    uv = vertexUV;
}
