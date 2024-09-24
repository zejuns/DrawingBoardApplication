 #version 400 core
 layout (location = 0) in vec3 aPos;
 // 下面两基本用不上
 layout (location = 1) in vec3 aNorm;
 layout (location = 2) in vec2 aUv;


 uniform mat4 projection;
 uniform mat4 view;
 uniform mat4 model;

 out vec2 fragUv;

 void main() 
 {
	fragUv = aUv;
   gl_Position = projection *view *model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
 }
