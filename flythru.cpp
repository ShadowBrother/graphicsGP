//
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion. 

#include <gl/Angel.h>


typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

int windowWidth = 1024 ;
int windowHeight = 1024 ;

int n = 10 ;
const int NumVertices = 36  * 10 * 10 * 10 ; //(6 faces)(2 triangles/face)(3 vertices/triangle)

vec4 camera = vec4(0.0, 0.0, -10.0, 1.0) ;
vec4 up = vec4(0.0, 1.0, 0.0, 1.0) ;
vec4 forward = vec4(0.0, 0.0, 200.0, 1.0) ;

GLuint matrix_loc ;
GLuint projection_loc ;
GLuint transform_loc ;

point4 points[NumVertices];
color4 colors[NumVertices];


// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
    point4( -0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5, -0.5, -0.5, 1.0 ),
    point4( -0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5, -0.5, -0.5, 1.0 )
};

// RGBA olors
color4 vertex_colors[8] = {
    color4( 0.0, 0.0, 0.0, 1.0 ),  // black
    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    color4( 1.0, 1.0, 1.0, 1.0 ),  // white
    color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
};

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  Trans[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat Scaler[NumAxes] = { 0.0, 0.0, 0.0 };
//----------------------------------------------------------------------------


int Index = 0;
//addPoint adds points to pointsArray and colorsArray

void addPoint(int vertexIndex, vec3 translation){

	point4 vertex ;
	for(int i = 0 ; i < 3 ; i++){

		vertex[i] = vertices[vertexIndex][i] + translation[i] ;
	
	}
	vertex.w = 1 ;

	points[Index] = vertex ;
	colors[Index++] = vertex_colors[vertexIndex] ;

}


// quad generates two triangles for each face and assigns colors
//    to the vertices

void
quad( int a, int b, int c, int d, vec3 translation )
{

	
	addPoint(a, translation) ;
	addPoint(b, translation) ;
	addPoint(c, translation) ;
	addPoint(a, translation) ;
	addPoint(c, translation) ;
	addPoint(d, translation) ;
	
	
	
	
	/*
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
	*/
	
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void
colorcube(vec3 translation)
{
    quad( 1, 0, 3, 2, translation );
    quad( 2, 3, 7, 6, translation );
    quad( 3, 0, 4, 7, translation );
    quad( 6, 5, 1, 2, translation );
    quad( 4, 5, 6, 7, translation );
    quad( 5, 4, 0, 1, translation );
}

//addCubes creates a bunch of cubes

void addCubes(){

	
	int i, j, k, cubeSpread = 3 ;
	//vec3 translation = vec3(0.0, 0.0, 0.0) ;
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			for(k = 0 ; k < n ; k++){
				//translation[Xaxis] = ( i/n - 0.5) * cubeSpread ;
				//translation[Yaxis] = ( j/n - 0.5) * cubeSpread ;
				//translation[Zaxis] = ( k/n - 0.5) * cubeSpread ;
				//colorcube(translation) ;
				colorcube(vec3(i * cubeSpread, (j * cubeSpread), (k * cubeSpread))) ;
				
			}
		}
	}
	//centers it...kind of
	for(i = 0 ; i < NumVertices ; i++) points[i] = Scale(0.125,0.125,0.125) * Translate(-cubeSpread * n/ 2, -cubeSpread * n / 2, -cubeSpread * n / 2) * points[i] ; 

	

}
//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
    addCubes();
	
	
	//for(int i = 0 ; i < NumVertices ; i++) points[i] =  Scale(1.0,1.0,1.0) * points[i] ;
	
    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
		  NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "D:\\My Documents\\GitHub\\graphicsGP\\vshaderFlythru.glsl", "D:\\My Documents\\GitHub\\graphicsGP\\fshaderFlythru.glsl" );
    glUseProgram( program );

    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(0) );

    GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(sizeof(points)) );

	matrix_loc = glGetUniformLocation( program, "model_view" );
    projection_loc = glGetUniformLocation( program, "projection" );
	transform_loc = glGetUniformLocation(program, "transform") ;

    glEnable( GL_DEPTH_TEST );
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); 


}

//----------------------------------------------------------------------------

GLuint r = 2.0 ;
float theta = 0.0 ;
float phi = 0.0 ;

void
display( void )
{
   
		
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//camera[0] = 0.0 ;//r * cos(theta) ;
	//camera[1] = 0.0 ;//r * sin(theta) * cos(phi) ;
	//camera[2] = -10.0 ;//r * sin(theta) * sin(phi) ;

	//forward = cross(camera, up) ;
	mat4 model_view = LookAt(camera, forward, up) ;
	mat4 projection =  Perspective(45,windowWidth/windowHeight,0.01,200.0) ;
	model_view = RotateY( Theta[Yaxis] ) * model_view ;
	
	 mat4  transform = (   RotateX( Theta[Xaxis] ) *	RotateY( Theta[Yaxis] )  *	RotateZ( Theta[Zaxis] ) ) ;
		/* Scale(Scaler[Zaxis])) */ //* Translate(Trans[Xaxis], Trans[Yaxis], Trans[Zaxis])) ;
	
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, model_view) ;
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection) ;
	glUniformMatrix4fv(transform_loc, 1, GL_TRUE, transform) ;

	//gluLookAt(camera[0], camera[1], camera[2],forward[0], forward[1],forward[2],up[0], up[1], up[2]) ;

	
   
		
    
	/*mat4 transform = /*Translate(Trans[Xaxis], Trans[Yaxis], Trans[Zaxis])  Translate(camera) ;

	point4  transformed_points[NumVertices ];

    for ( int i = 0; i < NumVertices ; ++i ) {
	transformed_points[i] = transform *  points[i];
    }
	//for (int i = NumVertices ; i < NumVertices * 2 ; i++) transformed_points[i] = points[i] ;

    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(transformed_points),
		     transformed_points );
*/


    glDrawArrays( GL_TRIANGLES, 0, NumVertices  );
    glutSwapBuffers();

	

}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
	case 033: // Escape Key
	case 'q': case 'Q':
	    exit( EXIT_SUCCESS );
	    break;
    }
}

//----------------------------------------------------------------------------

void
mouse( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN ) {
	switch( button ) {
	    case GLUT_LEFT_BUTTON:    Scaler[Zaxis] += .01;  break;
	    case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
	    case GLUT_RIGHT_BUTTON:   Scaler[Zaxis] -= .01;  break;
	}
    }
}

//----------------------------------------------------------------------------

void processMouseActiveMotion(int x, int y){



}

GLfloat dX = 0.0 ;
GLfloat dY = 0.0 ;

void processMousePassiveMotion(int x, int y){

	if(x > windowWidth/2) { Theta[Yaxis] += 0.1  ; dX = -1 ;}
	else if( x < windowWidth/2 ){ Theta[Yaxis] -= 0.1 ; dY = 1 ;}

	if(y > windowHeight/2 ){ Theta[Xaxis] -= 0.1  ; dY = -1 ;  }
	else if(y < windowHeight/2 ){ Theta[Xaxis] += 0.1 ; dY = 1 ; }
	//dX = x ;
	//dY = y ;
}
void
idle( void )
{
	//camera[0] -= 0.01 ;
	//camera[1] += 0.01 ;
	camera[2] += 0.01 ;

	//forward[0] -= 0.1 ;
	//forward[1] -= 0.0 ;
	//forward[2] += 0.01 ;

	//Theta[Yaxis] += .1 ;

	
    //Trans[Zaxis] -= 0.01;
	//Scaler[Zaxis] += 0.0001 ;

	if(dY == 1) Theta[Yaxis] += 0.09 ;
	else if(dY == -1) Theta[Yaxis] -= 0.09 ;
	
	if(dX == 1) Theta[Xaxis] -= 0.09 ;
	else if(dX == -1) Theta[Xaxis] += 0.09 ;

  //  if ( Theta[Zaxis] > 360.0 ) {
	//Trans[Zaxis] -= 360.0;
  //  }
   
	
    glutPostRedisplay();
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( windowWidth,windowHeight );
    //glutInitContextVersion( 3, 2 );
    //glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "Color Cube" );

    glewInit();

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutMouseFunc( mouse );
    glutIdleFunc( idle );
	glutMotionFunc(processMouseActiveMotion) ;
	glutPassiveMotionFunc(processMousePassiveMotion) ;

    glutMainLoop();
    return 0;
}
