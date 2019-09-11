#version 420 core

out vec4 FragColor;
uniform float c = 1.0;

void main()
{
	if(c == 1){
		FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	}else {
		FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
};
