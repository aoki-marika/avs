uniform vec4 colour;
uniform float alpha;

void main()
{
    gl_FragColor = colour * vec4(1, 1, 1, alpha);
}
