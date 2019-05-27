$input a_position, a_normal
$output v_normal, v_wpos

/*
 * Copyright 2011-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include <bgfx_shader.sh>

uniform mat4 u_invertedModel;

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );
	v_normal = mul(u_invertedModel, a_normal);

	vec3 wpos = mul(u_model[0], vec4(a_position, 1.0) ).xyz;
	v_wpos = wpos;
}
