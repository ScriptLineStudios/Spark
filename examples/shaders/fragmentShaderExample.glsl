
#version 330 core

in vec2 texCoord;        // holds the Vertex position <-1,+1> !!!
uniform sampler2D imageTexture;    // used texture unit
uniform float tx;
uniform float ty;            // x,y waves phase

out vec4 color;

vec2 SineWave( vec2 p )
{
    // convert Vertex position <-1,+1> to texture coordinate <0,1> and some shrinking so the effect dont overlap screen
    // wave distortion
    float x = sin( 25.0*p.y + 30.0*p.x + 6.28*tx) * 0.01;
    float y = sin( 25.0*p.y + 30.0*p.x + 6.28*ty) * 0.01;
    return vec2(p.x+x, p.y+y);
}

void main()
{
    color = texture2D(imageTexture,SineWave(texCoord));
}