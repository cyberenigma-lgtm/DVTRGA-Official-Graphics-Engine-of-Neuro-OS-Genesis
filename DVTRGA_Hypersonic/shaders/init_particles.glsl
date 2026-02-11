#version 430 core

layout(local_size_x = 256) in;

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

uint fast_rand(uint seed) {
    seed = seed * 1103515245 + 12345;
    return (seed / 65536) % 32768;
}

void main() {
    uint gid = gl_GlobalInvocationID.x;
    if (gid >= particles.length()) return;

    uint seed = gid * 1973 + 12345;
    
    float rx = float(fast_rand(seed) % uint(screen_size.x));
    float ry = float(fast_rand(seed + 1) % uint(screen_size.y));
    float rv = float((fast_rand(seed + 2) % 5) + 2);

    particles[gid].position = vec2(rx, ry);
    particles[gid].velocity = vec2(0.0, rv);
    particles[gid].color = 0xFFFFFFFF;
}
