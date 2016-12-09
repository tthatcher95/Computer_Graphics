#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "linmath.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

mat4x4 m;
mat4x4 transform;

typedef struct {
  float Position[2];
  float TexCoord[2];
} Vertex;

// (-1, 1)  (1, 1)
// (-1, -1) (1, -1)

Vertex vertexes[] = {
  {{1, -1}, {0.99999, 0.99999}},
  {{1, 1},  {0.99999, 0}},
  {{-1, 1}, {0, 0}},
  {{-1, 1}, {0, 0}},
  {{-1, -1}, {0, 0.99999}},
  {{1, -1}, {0.99999, 0.99999}}
};

static const char* vertex_shader_text =
"uniform mat4 MVP;\n"
"attribute vec2 TexCoordIn;\n"
"attribute vec2 vPos;\n"
"varying vec2 TexCoordOut;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    TexCoordOut = TexCoordIn;\n"
"}\n";

static const char* fragment_shader_text =
"varying vec2 TexCoordOut;\n"
"uniform sampler2D Texture;\n"
"void main()\n"
"{\n"
"    gl_FragColor = texture2D(Texture, TexCoordOut);\n"
"}\n";

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
      }
    //Rotate by 30 Degrees (R)
    if((key == GLFW_KEY_R) && (action == GLFW_PRESS)) {
        mat4x4_rotate_Z(m, m, M_PI/6);
    }

    //Rotate by -30 Degrees (F)
    if((key == GLFW_KEY_F) && (action == GLFW_PRESS)) {
        mat4x4_rotate_Z(m, m, -M_PI/6);
    }

    //Scale by 2 (+)
    if((key == GLFW_KEY_EQUAL) && (action == GLFW_PRESS)) {
        mat4x4_identity(transform);
        transform[0][0] = 2;
        transform[1][1] = 2;
        //printf("%lf" , transform[0][0]);
        mat4x4_mul(m, m, transform);

    }
    //Scale by 1/2 (-)
    if((key == GLFW_KEY_MINUS) && (action == GLFW_PRESS)) {
        mat4x4_identity(transform);
        transform[0][0] = 0.5;
        transform[1][1] = 0.5;
        //printf("%lf" , transform[0][0]);
        mat4x4_mul(m, m, transform);

    }
    //Translate x+1 (E)
    if((key == GLFW_KEY_E) && (action == GLFW_PRESS)) {
        mat4x4_identity(transform);
        mat4x4_translate(transform, 0.25, 0, 0);
        //printf("%lf" , transform[0][0]);
        mat4x4_mul(m, m, transform);

    }
    //Translate x-0.25 (Q)
    if((key == GLFW_KEY_Q) && (action == GLFW_PRESS)) {
        mat4x4_identity(transform);
        mat4x4_translate(transform, -0.25, 0, 0);
        //printf("%lf" , transform[0][0]);
        mat4x4_mul(m, m, transform);

    }

    //Translate y + 0.25 (W)
    if((key == GLFW_KEY_W) && (action == GLFW_PRESS)) {
        mat4x4_identity(transform);
        mat4x4_translate(transform, 0, 0.25, 0);
        //printf("%lf" , transform[0][0]);
        mat4x4_mul(m, m, transform);

    }

    //Translate x-0.25 (S)
    if((key == GLFW_KEY_S) && (action == GLFW_PRESS)) {
        mat4x4_identity(transform);
        mat4x4_translate(transform, 0, -0.25, 0);
        //printf("%lf" , transform[0][0]);
        mat4x4_mul(m, m, transform);

    }

    //Shear x-0.1 (Z)
    if((key == GLFW_KEY_Z) && (action == GLFW_PRESS)) {
        mat4x4_identity(transform);
        transform[0][1] = 0.1;
        //printf("%lf" , transform[0][0]);
        mat4x4_mul(m, transform, m);

    }

    //Shear x+0.1 (C)
    if((key == GLFW_KEY_C) && (action == GLFW_PRESS)) {
        mat4x4_identity(transform);
        transform[0][1] = -0.1;
        //printf("%lf" , transform[0][0]);
        mat4x4_mul(m, transform, m);

    }

    //Shear y-0.1 (A)
    if((key == GLFW_KEY_A) && (action == GLFW_PRESS)) {
      mat4x4_identity(transform);
      transform[1][0] = 0.1;
      //printf("%lf" , transform[0][0]);
      mat4x4_mul(m, transform, m);

    }

    //Shear y+0.1 (D)
    if((key == GLFW_KEY_D) && (action == GLFW_PRESS)) {
      mat4x4_identity(transform);
      transform[1][0] = -0.1;
      //printf("%lf" , transform[0][0]);
      mat4x4_mul(m, transform, m);

    }

}

void glCompileShaderOrDie(GLuint shader) {
  GLint compiled;
  glCompileShader(shader);
  glGetShaderiv(shader,
		GL_COMPILE_STATUS,
		&compiled);
  if (!compiled) {
    GLint infoLen = 0;
    glGetShaderiv(shader,
		  GL_INFO_LOG_LENGTH,
		  &infoLen);
    char* info = malloc(infoLen+1);
    GLint done;
    glGetShaderInfoLog(shader, infoLen, &done, info);
    printf("Unable to compile shader: %s\n", info);
    exit(1);
  }
}

// 4 x 4 image..
unsigned char image[] = {
  255, 0, 0, 255,
  255, 0, 0, 255,
  255, 0, 0, 255,
  255, 0, 0, 255,

  0, 255, 0, 255,
  0, 255, 0, 255,
  0, 255, 0, 255,
  0, 255, 0, 255,

  0, 0, 255, 255,
  0, 0, 255, 255,
  0, 0, 255, 255,
  0, 0, 255, 255,

  255, 0, 255, 255,
  255, 0, 255, 255,
  255, 0, 255, 255,
  255, 0, 255, 255
};

int read_P6(FILE* input, char* buffer, int width, int height) {
  for(int i = 0; i < width*height*3; i++) {
    buffer[i] = fgetc(input);
  }
}

int read_P3(FILE* input, char* buffer, int width, int height) {


  //printf("Afer\n");
  for(int i = 0; i < width*height*3; i++) {
    char current_char = fgetc(input);
    //printf("Afer\n");
    while(current_char == ' ' || current_char == '\n') {
      current_char = fgetc(input);

    }
    ungetc(current_char, input);
    fscanf(input, "%d", &buffer[i]);

  }
}

int main(int argc, char* argv[])
{
  FILE *input, *output;
	const size_t BUFFER_SIZE = 255;
	char *binary;
	char *buffer = (char*)malloc(sizeof(char)*BUFFER_SIZE);
	char current_format;
	char *input_file = argv[1];


	if (argc < 2) {
		fprintf(stderr, "Error: Incorrect number of parameters\n");
		exit(0);
	}

	//Input file
	input = fopen(input_file, "r");

	if (input == NULL) {
		fprintf(stderr, "Error: The file does not exist\n");
		exit(0);
	}

	if (buffer == NULL) {
		fprintf(stderr, "Error: Buffer does not exist");
		exit(0);
	}

	current_format = fgetc(input);

	//Checks if it is a PPM file
	if (current_format != 'P') {
		fprintf(stderr, "Error: This is not a PPM file \n");
		exit(0);
	}


  current_format = fgetc(input);

  if(current_format != '3' && current_format != '6') {
    fprintf(stderr, "%c\n", current_format);
    fprintf(stderr, "Error: This is not a valid PPM file: is NOT P3 or P6. \n");
		exit(0);
  }


  //Skip through the comments of the PPM file (if any)
	fgets(buffer, BUFFER_SIZE, input);
	fgets(buffer, BUFFER_SIZE, input);

	while (buffer[0] == '#') {
		fgets(buffer, BUFFER_SIZE, input);
	}

	//Get MAX_VAL from PPM file
	//Then realloc's memory acording to Width/Height

	int i = 0;

	while (buffer[i] != ' ') {
		i++;
	} buffer[i] = '\0';

	int img_width = atoi(buffer);
	int img_height = atoi(buffer + i + 1);
  //printf("%d\n", width);
  //printf("%d\n", height);

	fgets(buffer, BUFFER_SIZE, input);
	int MAX_VAL = atoi(buffer);

	if (MAX_VAL > 255) {
		fprintf(stderr, "Error: This file contains more than 1 byte per pixel");
	}

	if(MAX_VAL < 1) {
		fprintf(stderr, "Error: Minimum value cannot be less than one");
	}

	buffer = (char*)realloc(buffer, img_width*img_height * 3);

  if(current_format == '6')  {
      read_P6(input, buffer, img_width, img_height);
    }

  if(current_format == '3') {
    //char current_char = fgetc(input);
    //printf("Nice\n");
    read_P3(input, buffer, img_width, img_height);
  }
  //printf("Afer\n");















    GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;


    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    // gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShaderOrDie(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShaderOrDie(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    // more error checking! glLinkProgramOrDie!

    mvp_location = glGetUniformLocation(program, "MVP");
    assert(mvp_location != -1);

    vpos_location = glGetAttribLocation(program, "vPos");
    assert(vpos_location != -1);

    GLint texcoord_location = glGetAttribLocation(program, "TexCoordIn");
    assert(texcoord_location != -1);

    GLint tex_location = glGetUniformLocation(program, "Texture");
    assert(tex_location != -1);

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location,
			  2,
			  GL_FLOAT,
			  GL_FALSE,
                          sizeof(Vertex),
			  (void*) 0);

    glEnableVertexAttribArray(texcoord_location);
    glVertexAttribPointer(texcoord_location,
			  2,
			  GL_FLOAT,
			  GL_FALSE,
                          sizeof(Vertex),
			  (void*) (sizeof(float) * 2));

    int image_width = 4;
    int image_height = 4;

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB,
		 GL_UNSIGNED_BYTE, buffer);
     printf("Afer\n");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glUniform1i(tex_location, 0);

    mat4x4_identity(m);

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        mat4x4 p, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);


        //mat4x4_rotate_Z(m, m, (float) glfwGetTime());
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

//! [code]
