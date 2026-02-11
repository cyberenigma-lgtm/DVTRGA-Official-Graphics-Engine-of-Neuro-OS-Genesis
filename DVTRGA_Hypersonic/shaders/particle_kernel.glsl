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

// SSBO Binding 0: Particles
layout(std430, binding = 0) buffer ParticleBuffer {
    Particle particles[];
};

uniform float time_delta;
uniform vec2 screen_size;
uniform uint frame_count;

// Fast LCG (GLSL version)
uint fast_rand(uint seed) {
    seed = seed * 1103515245 + 12345;
    return (seed / 65536) % 32768;
}

void main() {
    uint gid = gl_GlobalInvocationID.x;
    
    // Safety check
    if (gid >= particles.length()) return;

    Particle p = particles[gid];

    // Physics Update (Zero-Copy)
    p.position.y += p.velocity.y; // * time_delta (assuming fixed step for max throughput)

    // Boundary Wrap
    if (p.position.y >= screen_size.y) {
        p.position.y = 0.0;
        
        // Randomize X on respawn (Deterministic Chaos)
        uint seed = uint(p.position.x) + gid + frame_count;
        p.position.x = float(fast_rand(seed) % uint(screen_size.x));
    }

    // Write back to VRAM (L2 Cache optimized)
    particles[gid] = p;
}
