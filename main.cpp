
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window.hpp>
//#include <GL/gl.h>
//#include <GL/freeglut.h>
#include <iostream>
#include <math.h>

GLfloat position_z = -10.0f;
GLfloat mouse_rotation[2] = {0.0f, 0.0f};
GLint mouse_initial_position[2] = {0,0};

GLfloat camera_position[3] = {0.f, 0.f, 0.f};
GLfloat camera_velocity[3] = {0.f, 0.f, 0.f};
GLfloat camera_angle = 0.f;
GLfloat camera_angle_velocity = 0.f;
GLfloat camera_azimut = 0.f;
GLfloat camera_velocity_strafe[3] = {0.f, 0.f, 0.f};

int axis_x = 0;
int axis_y = 0;
int axis_u = 0;
int axis_v = 0;
int axis_r = 0;
int axis_z = 0;

	static GLint vertices[] =
			{
				0,0,0,
				0,0,1,
				0,1,0,
				0,1,1,
				1,0,0,
				1,0,1,
				1,1,0,
				1,1,1,
			};

	static GLfloat colors[] =
		{
			1.0, 0.2, 0.2,
			0.2, 0.2, 1.0,
			0.8, 1.0, 0.2,
			0.75, 0.75, 0.75,
			0.35, 0.35, 0.35,
			0.5, 0.5, 0.5,
			0.5, 0.5, 0.5,
			0.5, 0.5, 0.5,
		};

float ORG[3] = {0,0,0};

float XP[3] = {1,0,0}, XN[3] = {-1,0,0},
YP[3] = {0,1,0}, YN[3] = {0,-1,0},
ZP[3] = {0,0,1}, ZN[3] = {0,0,-1};

#define AXIS_SCALE 100000
void drawAxes (void)
{
	glPushMatrix ();

		//glTranslatef (-2.4, -1.5, -5);
		//glRotatef (0 , 1,0,0);
		//glRotatef (0, 0,1,0);
		glScalef (AXIS_SCALE,AXIS_SCALE,AXIS_SCALE);

		glLineWidth (1.0);

		glBegin (GL_LINES);
			glColor3f (1,0,0); // X axis is red.
			glVertex3fv (ORG);
			glVertex3fv (XP ); 
			glColor3f (0,1,0); // Y axis is green.
			glVertex3fv (ORG);
			glVertex3fv (YP );
			glColor3f (0,0,1); // z axis is blue.
			glVertex3fv (ORG);
			glVertex3fv (ZP ); 
		glEnd();

	glPopMatrix ();
}

void drawGrid3D (int xmin, int xmax, int ymin, int ymax, int zmin, int zmax)
{
	glPushMatrix ();

		//glScalef (AXIS_SCALE,AXIS_SCALE,AXIS_SCALE);

		glLineWidth (1.0);
		glPointSize(1.5);
		glColor3f(1,1,1);

		for(int i=xmin; i<=xmax; i++)
		{
			for(int j=ymin; j<=ymax; j++)
			{
				for(int k=zmin; k<=zmax; k++)
				{
					glBegin (GL_POINTS);
						//glColor3f (i,j,k);
						glVertex3i(i,j,k);
					glEnd();
				}
			}
		}

	glPopMatrix ();
}

void processEvents(sf::Window& App, const float& elapsed_time)
{
		//process real time inputs
		const sf::Input& Input = App.GetInput();

		//dead zones
		axis_u = Input.GetJoystickAxis(0, sf::Joy::AxisU); if(abs(axis_u) < 15) axis_u = 0;
		axis_v = Input.GetJoystickAxis(0, sf::Joy::AxisV); if(abs(axis_v) < 15) axis_v = 0;
		axis_x = Input.GetJoystickAxis(0, sf::Joy::AxisX); if(abs(axis_x) < 15) axis_x = 0;
		axis_y = Input.GetJoystickAxis(0, sf::Joy::AxisY); if(abs(axis_y) < 15) axis_y = 0;
		axis_r = Input.GetJoystickAxis(0, sf::Joy::AxisR); if(abs(axis_r) < 15) axis_r = 0;
		axis_z = Input.GetJoystickAxis(0, sf::Joy::AxisZ); if(abs(axis_z) < 15) axis_z = 0;

		//escala a eje R y Z
		axis_r = (axis_r + 100) * 0.5f;
		axis_z = (axis_z + 100) * 0.5f;

		camera_angle_velocity = axis_u * 0.01f * 270;

		//calculo velocidad angular
		camera_azimut = axis_v * 0.01f * 90.f;
		camera_angle = camera_angle + camera_angle_velocity * elapsed_time;

		//calculo velocidad
		// v_x = v_x0 +a_xt
		camera_velocity[0] = axis_y * 0.01f * sin(camera_angle * 3.14159265/180);
		camera_velocity[1] = axis_y * -0.01f * cos(camera_angle * 3.14159265/180);
		camera_velocity[2] = axis_y * -0.01f * sin(camera_azimut * 3.14159265/180);
		//strafe
		camera_velocity_strafe[0] = axis_x * -0.01f * cos(camera_angle * 3.14159265/180);
		camera_velocity_strafe[1] = axis_x * -0.01f * sin(camera_angle * 3.14159265/180);
		camera_velocity_strafe[2] = axis_r * -0.01f - axis_z * -0.01f;

		//calculo posicion
		//p_x = p_x + v_x * t + 1/2 a_x*t^2
		camera_position[0] = camera_position[0] + camera_velocity[0] * 5.f * elapsed_time;
		camera_position[1] = camera_position[1] + camera_velocity[1] * 5.f * elapsed_time;
		camera_position[2] = camera_position[2] + camera_velocity[2] * 5.f * elapsed_time;
		//strafe
		camera_position[0] = camera_position[0] + camera_velocity_strafe[0] * 5.f * elapsed_time;
		camera_position[1] = camera_position[1] + camera_velocity_strafe[1] * 5.f * elapsed_time;
		camera_position[2] = camera_position[2] + camera_velocity_strafe[2] * 5.f * elapsed_time;

		// std::cout
		// 	<< "(AxisR) = ("
		// 	<< Input.GetJoystickAxis(0, sf::Joy::AxisR)
		// 	<< ")"
		// 	<< std::endl;

		//Process events
		sf::Event Event;
		while (App.GetEvent(Event))
		{
			switch(Event.Type)
			{
				case sf::Event::Closed:
					App.Close();
					break;
				case sf::Event::KeyPressed:
					if(Event.Key.Code == sf::Key::Escape)
						App.Close();
					break;
				case sf::Event::Resized:
					glViewport(0, 0, Event.Size.Width, Event.Size.Height);
					break;

				case sf::Event::MouseButtonPressed:

					mouse_initial_position[0] += Event.MouseButton.X;
					mouse_initial_position[1] += Event.MouseButton.Y;

					std::cout
						<< "[EVENT][MBP] (sf::Joy::Count) = "
						<< "("
						<< sf::Joy::Count
						<< ")"
						<< std::endl;

					break;

				case sf::Event::MouseWheelMoved:

					position_z += (GLfloat) Event.MouseWheel.Delta * 1.f;
					std::cout << "[EVENT][MWM] delta = " << Event.MouseWheel.Delta << ", " << position_z << std::endl;

					break;

				default:
					break;
			}

			if(Input.IsMouseButtonDown(sf::Mouse::Left))
			{
				mouse_rotation[0] = -(mouse_initial_position[0] - Input.GetMouseX());
				mouse_rotation[1] = (mouse_initial_position[1] - Input.GetMouseY());
				std::cout
					<< "[EVENT][ISMBD] (mouse_rotation[0], mouse_rotation[1]) = "
					<< "("
					<< mouse_rotation[0]
					<< ", "
					<< mouse_rotation[1]
					<< ")"
					<< std::endl;
			}
		}
}

int main()
{
	// Create the main window
	int resX = sf::VideoMode::GetMode(0).Width * 0.7f;
	int resY = sf::VideoMode::GetMode(0).Height * 0.7f;

	sf::Window App;

	sf::VideoMode video_mode(resX, resY, 32);
	App.Create(
		video_mode,
		"OpenGL xbox cam [Joe]",
		sf::Style::Titlebar | sf::Style::Close// | sf::Style::Fullscreen
	);
	App.SetFramerateLimit(60);

	// Set color and depth clear value
	//glClearDepth(1.f);
	glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

	//game clock
		sf::Clock Clock;

	// Start game loop
	while (App.IsOpened())
	{
		float elapsed_time = Clock.GetElapsedTime();
		Clock.Reset();

		processEvents(App, elapsed_time);

		// Set the active window before using OpenGL commands
		// It's useless here because active window is always the same,
		// but don't forget it if you use multiple windows or controls
		App.SetActive();

//		// Clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Apply some transformations
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//draw using vertex arrays
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, colors);
		glVertexPointer(3, GL_INT, 0, vertices);

		glBegin(GL_QUAD_STRIP);
			for (unsigned int i = 0; i < 8; i++)
			{
				glArrayElement(i);
			}
		glEnd();

		drawGrid3D(-3,3,-3,3,-3,3);

		drawAxes();


		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//glOrtho(-200, 200, -200, 200, -200, 200);
		gluPerspective(
			90.f,
			float(sf::VideoMode::GetMode(0).Width)/float(sf::VideoMode::GetMode(0).Height),
			0.f, -500.f
		);
		glRotatef(camera_azimut, 1.f, 0.f, 0.f);
		glRotatef(camera_angle, 0.f, 1.f, 0.f);
		glTranslatef(camera_position[0], camera_position[2], camera_position[1]);

		// Finally, display rendered frame on screen
		App.Display();
	}

	return EXIT_SUCCESS;
}
