#version 330 core


struct Camera {
    vec3 position;
    vec3 direction;
};

struct Light {
    vec3 position;
};

layout(location = 0) out vec4 m_color;

in vec2 v_texture_coordinates;
in vec3 v_world_coordinates;

uniform Camera u_camera;
uniform Light u_light;


#define INF (1. / 0.)
#define NEG_INF (-1. / 0.)

#define RAY_MARCH_STEPS 500
#define RAY_MARCH_MIN_DIST 0.001
#define RAY_MARCH_MAX_DIST 100.


// Distance from the point to the plane y=0 (floor)
float dPlane(vec3 p) {
    return p.y;
}

// Distance from the point to the sphere
float dSphere(vec3 p, float r, vec3 center) {
    return length(p - center) - r;
}

float getDistance(vec3 p) {
    float sphere = dSphere(p, .5, vec3(0.));
    // float plane = dPlane(p);
    // return min(plane, sphere);
    return sphere;
}

float rayMarch(vec3 ray_origin, vec3 ray_direction) {
    float dist = 0.;
    
    for (int i = 0; i < RAY_MARCH_STEPS; i++) {
        vec3 p = ray_origin + dist*ray_direction;
        float sphere_radius = getDistance(p);
        
        dist += sphere_radius;
        
        if (sphere_radius < RAY_MARCH_MIN_DIST) return dist;
        if (dist > RAY_MARCH_MAX_DIST) return INF;
    }
    
    return INF;
}

vec3 getNormal(vec3 p) {
    float d = getDistance(p);
    vec2 e = vec2(.01, 0);
    
    vec3 n = d - vec3(
        getDistance(p - e.xyy), 
        getDistance(p - e.yxy), 
        getDistance(p - e.yyx));
    
    return normalize(n);
}

float getLight(vec3 p) {
    // vec3 light_position = vec3(0., 3., 6.);
    // light_position.xz += vec2(cos(iTime), sin(iTime))*4.;
    
    vec3 light_direction = normalize(u_light.position - p);
    vec3 normal = getNormal(p);
    
    float ambient = 0.;
    float diffuse = clamp(dot(normal, light_direction), 0., 1.);
    float specular = 0.;
    
    float d = rayMarch(p + normal*RAY_MARCH_MIN_DIST*2., light_direction);
    if (d < length(u_light.position - p)) diffuse *= .1;
    
    return diffuse;
}


void main() {
    vec2 uv = vec2(.5 - v_texture_coordinates.x, v_texture_coordinates.y - .5);

    vec3 color = vec3(0.);

    vec3 ray_origin = u_camera.position; //vec3(0, 0, -2); // 
    vec3 ray_direction = normalize(v_world_coordinates - u_camera.position); //  normalize(vec3(uv.x, uv.y, 1)); // 

    float d = rayMarch(ray_origin, ray_direction);
    
    vec3 p = ray_origin + d * ray_direction;
    color = getLight(p)*vec3(1);

    vec3 normal = getNormal(p);
    // color = normal; // ray_direction;

    if (d == INF) {
        m_color = vec4(0.);
    } else {
        m_color = vec4(color, 1.);
    }
}
