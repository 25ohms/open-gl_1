#shader vertex
#version 120

attribute vec2 position;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
}

#shader fragment
#version 120

uniform vec4 u_Color;

void main()
{
  gl_FragColor = u_Color;
}

