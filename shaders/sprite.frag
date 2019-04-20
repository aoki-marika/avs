uniform float alpha;
uniform sampler2D sampler;

varying vec2 uv;

void main()
{
    gl_FragColor = texture2D(sampler, uv) * vec4(1, 1, 1, alpha);
}
