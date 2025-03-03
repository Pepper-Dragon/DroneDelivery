#version 330

in vec3 frag_color;
in float fog_factor;
in vec4 lightViewPosition;

uniform sampler2DShadow shadow;
uniform vec3 fog_color;

layout(location = 0) out vec4 out_color;

const int indexMatrix4x4[16] = int[](0,  8,  2,  10,
                                     12, 4,  14, 6,
                                     3,  11, 1,  9,
                                     15, 7,  13, 5);

float indexValue() {
    int x = int(mod(gl_FragCoord.x, 4));
    int y = int(mod(gl_FragCoord.y, 4));
    return indexMatrix4x4[(x + y * 4)] / 16.0;
}

float dither(float color) {
    float closestColor = (color < 0.5) ? 0 : 1;
    float secondClosestColor = 1 - closestColor;
    float d = indexValue();
    float distance = abs(closestColor - color);
    return (distance < d) ? closestColor : secondClosestColor;
}


void main()
{
    vec4 color = vec4(frag_color,1);

    vec3 pos = lightViewPosition.xyz / lightViewPosition.w;
    float bias = 0.005;
    
    pos = pos*0.5 + 0.5;
    pos.z -= bias;

    color =color * texture(shadow, pos)/2 + color/2;

    float x_comp = mix(vec4(fog_color,1), color, fog_factor).x;
    float y_comp = mix(vec4(fog_color,1), color, fog_factor).y;
    float z_comp = mix(vec4(fog_color,1), color, fog_factor).z;

    out_color = vec4(vec3(dither(x_comp), dither(y_comp), dither(z_comp)),1);
    //out_color = mix(vec4(fog_color,1), color, fog_factor);
}