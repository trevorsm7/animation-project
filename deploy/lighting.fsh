#version 410

uniform sampler2D u_texture;

//uniform vec3 u_lightPos[2];
//uniform vec3 u_lightColor[2];
//uniform vec3 u_spotDir[2];
//uniform float u_spotCutoff[2]; // use cosine
//uniform float u_spotExponent[2];

//uniform vec4 u_ambient;
//uniform vec4 u_diffuse;
//uniform vec4 u_specular;
//uniform float u_shininess;

in vec3 v_vertex;
in vec2 v_texCoord;
in vec3 v_normal;

out vec4 f_color;

void main()
{
    vec3 u_lightPos[2];
    u_lightPos[0] = vec3(-5.0, 0.0, 5.0);
    u_lightPos[1] = vec3(5.0, 0.0, 0.0);
    vec3 u_lightColor[2];
    u_lightColor[0] = vec3(1.0, 0.8, 0.6);
    u_lightColor[1] = vec3(0.4, 0.6, 0.8);
    vec3 u_spotDir[2];
    u_spotDir[0] = normalize(vec3(1.0, 0.0, -1.0));
    u_spotDir[1] = vec3(-1.0, 0.0, 0.0);
    float u_spotCutoff[2];
    u_spotCutoff[0] = -1;
    u_spotCutoff[1] = -1;
    float u_spotExponent[2];
    u_spotExponent[0] = 1.0;
    u_spotExponent[1] = 1.0;
    
    //vec3 u_ambient = vec3(1.0, 1.0, 1.0);
    //vec3 u_diffuse = vec3(1.0, 1.0, 1.0);
    vec3 u_ambient = texture(u_texture, v_texCoord).rgb;
    vec3 u_diffuse = texture(u_texture, v_texCoord).rgb;
    vec3 u_specular = vec3(1.0, 1.0, 1.0);
    float u_shininess = 50.0;
    
    vec3 normal = normalize(v_normal);
    vec3 color = vec3(0.0, 0.0, 0.0);
    
    for (int i = 0; i < 2; ++i)
    {
        vec3 ambient = u_lightColor[i] * u_ambient;
        
        // Get distance and direction from vertex to light
        vec3 lightVector = u_lightPos[i] - v_vertex;
        float d = length(lightVector);
        float attenuation = 1.0 + (0.05 * d*d);
        lightVector = normalize(lightVector);
        
        // Compute diffuse component
        float NdotL = max(dot(normal, lightVector), 0.0);
        vec3 diffuse = u_lightColor[i] * u_diffuse * NdotL;
        
        // Only compute diffuse and specular inside spotlight cone
        float spotEffect = dot(u_spotDir[i], -lightVector);
        if (NdotL > 0.0 && spotEffect > u_spotCutoff[i])
        {
            // Compute specular component
            vec3 halfVector = normalize(normalize(-v_vertex) + lightVector);
            float NdotHV = max(dot(normal, halfVector), 0.0);
            vec3 specular = u_lightColor[i] * u_specular * pow(NdotHV, u_shininess);
            
            // Compute attenuated sum of component
            color += pow(spotEffect, u_spotExponent[i]) * (0.5 * diffuse + specular) / attenuation;
        }
        
        color += (0.5 * diffuse + ambient) / attenuation;
    }
    
    f_color = vec4(color, 1.0);
}
