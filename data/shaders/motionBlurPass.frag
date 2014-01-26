#version 420

uniform sampler2D tex1;
uniform float alpha;

out vec4 finalColor;
in vec2 vTexCoord;

void main(void) {
        vec4 valColor0 = texture(tex1, vec2(vTexCoord.x,-vTexCoord.y)); //xyz = color
        finalColor = vec4(valColor0.xyz, alpha);
}
