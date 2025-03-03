#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_uv;
layout(location = 3) in vec3 vertex_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 shadowMatrix;

uniform vec2 fog_position;

out vec3 frag_color;
out float noise_value;
out float fog_factor;
out vec4 lightViewPosition;

// 2D Random
float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))
                 * 43758.5453123);
}

// 2D Noise based on Morgan McGuire @morgan3d
float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    // Smooth Interpolation

    // Cubic Hermine Curve.  Same as SmoothStep()
    vec2 u = f*f*(3.0-2.0*f);
    // u = smoothstep(0.,1.,f);

    // Mix 4 coorners percentages
    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}


void main()
{
    vec3 pos = v_position;
    pos.y = noise(v_position.xz*0.5);

    vec4 camera_pos = -View * Model * vec4(pos, 1.0);

    frag_color = vertex_color;
    noise_value = pos.y;
    fog_factor = (fog_position.y - camera_pos.z)/ (fog_position.y - fog_position.x);
    fog_factor = clamp(fog_factor, 0.0f, 1.0f);

    gl_Position = Projection * View * Model * vec4(pos, 1.0);
    lightViewPosition = shadowMatrix * vec4(v_position, 1.0);

}
