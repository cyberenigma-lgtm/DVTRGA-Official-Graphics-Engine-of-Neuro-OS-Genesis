#version 430 core

// ---------------- VERTEX SHADER ----------------
#ifdef VERTEX_SHADER

struct Particle {
    vec2 position;
    vec2 velocity;
    uint color;
    float padding1;
    float padding2;
    float padding3;
};

layout(std430, binding = 0) buffer ParticleBuffer {
    Particle particles[];
};

uniform vec2 screen_size;

out vec4 fragColor;

void main() {
    Particle p = particles[gl_VertexID];
    
    // Normalize to NDC [-1, 1]
    // Screen [0, W] -> NDC [-1, 1]
    float x = (p.position.x / screen_size.x) * 2.0 - 1.0;
    float y = (p.position.y / screen_size.y) * 2.0 - 1.0;
    y = -y; // Invert Y for OpenGL (Top-Left Origin)

    gl_Position = vec4(x, y, 0.0, 1.0);
    gl_PointSize = 4.0;

    // Decode Color (Assuming UINT 0xAABBGGRR)
    // float r = float((p.color >> 16) & 0xFF) / 255.0; // Incorrect for LE?
    // Let's debug color. Just use WHITE for now or simple logic.
    fragColor = vec4(1.0, 1.0, 1.0, 1.0);
    if (p.color == 0xFFFFFF00) fragColor = vec4(0.0, 1.0, 1.0, 1.0); // Cyan
}
#endif

// ---------------- FRAGMENT SHADER ----------------
#ifdef FRAGMENT_SHADER

in vec4 fragColor;
out vec4 outColor;

void main() {
    outColor = fragColor;
}
#endif
