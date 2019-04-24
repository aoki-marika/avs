uniform float alpha;
uniform sampler2D sampler;
uniform vec4 colour;

varying vec2 uv;

void main()
{
    gl_FragColor = texture2D(sampler, uv) * colour * vec4(1, 1, 1, alpha);
}
