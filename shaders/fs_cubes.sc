$input v_wpos, v_normal

/*
 * Copyright 2011-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include <bgfx_shader.sh>

uniform vec4 u_color;
uniform vec4 u_viewPos;

void main()
{
	float ambientStrength = 0.1;
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 lightPos = vec3(10.0, 20.0, 10.0);
	vec3 ambient = ambientStrength * lightColor;

	vec3 normal = normalize(v_normal);
	vec3 lightDir = normalize(lightPos - v_wpos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(u_viewPos.xyz - v_wpos);
	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;  

	vec3 result = (ambient + diffuse + specular) * u_color;
	gl_FragColor = vec4(result, 1.0);
}
