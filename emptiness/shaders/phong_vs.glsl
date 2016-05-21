#version 430

// in layout( location = 0 ) vec3 position;
in layout( location = 0 ) vec4 position;

out float radius;

uniform float time;
uniform vec2 viewport;

const float pi = acos( -1 );
const float g  = 9.8;

void main()
{
  gl_Position  = position + 1 - vec4( 1, 1, 0, 0 );
  gl_PointSize = 97;
}
