#include <Engine.h>
#include <Graphics/OpenGL/DeviceOpenGL.h>
#include <Audio/OpenAL/AudioDeviceOpenAL.h>
#include <Graphics/OpenGL/WindowOpenGLSDL.h>

using namespace Columbus;

void a()
{

}

int main(int argc, char** argv)
{
	WindowOpenGLSDL window(Vector2(640, 480), "Columbus Engine", E_WINDOW_FLAG_RESIZABLE);
	
	Input input;
	input.setWindow(&window);

	Camera camera;
	camera.setPos(vec3(10, 10, 0));
	camera.setRot(vec3(0, 90, 0));

	gDevice = new DeviceOpenGL();
	gAudioDevice = new AudioDeviceOpenAL();

	Skybox skybox(gDevice->createCubemap("Data/Skyboxes/1.cubemap"));

	Timer timer;

	window.setVSync(true);

	Image* cur = new Image;
	cur->load("Data/Textures/cursor.tif", E_IMAGE_LOAD_FLIP_Y);

	input.showMouseCursor(false);
	//input.setSystemCursor(E_INPUT_SYSTEM_CURSOR_CROSSHAIR);
	input.setColoredCursor(cur->getData(), cur->getWidth(), cur->getHeight(), cur->getBPP(), vec2(17, 3));

	bool cursor = false;

	Scene scene;

	scene.load("Data/1.scene");

	scene.setSkybox(&skybox);
	scene.setCamera(&camera);

	AudioSource* Source = gAudioDevice->CreateSource();
	if (!Source->GetSound()->Load("Data/Sounds/cartoon001.wav"))
	{
		Log::error("Couldn't load sound");
	}

	Source->SetPosition(Vector3(0, 10, 3.5));
	Source->SetLooping(true);
	Source->SetMinDistance(1.0f);
	Source->SetMaxDistance(1000.0f);

	Source->SetSound(Source->GetSound());
	
	Source->Play();

	float xPos = 8.0f;

	const char* arr = "Fuck me";
	String str("Oh....");
	str.Append(arr, arr + 7);
	std::cout << str + ", my sweety" << std::endl;

	while (window.isOpen())
	{
		float RedrawTime = window.getRedrawTime();

		window.update();
		input.update();

		window.clear(vec4(0, 0, 0.75, 1));

		camera.perspective(60, window.getAspect(), 0.1, 1000);

		if (input.getKey(SDL_SCANCODE_W))
			camera.addPos(camera.direction() * RedrawTime * 5);
		if (input.getKey(SDL_SCANCODE_S))
			camera.addPos(-camera.direction() * RedrawTime * 5);
		if (input.getKey(SDL_SCANCODE_A))
			camera.addPos(-camera.right() * RedrawTime * 5);
		if (input.getKey(SDL_SCANCODE_D))
			camera.addPos(camera.right() * RedrawTime * 5);
		if (input.getKey(SDL_SCANCODE_UP))
			camera.addRot(Vector3(-125 * RedrawTime, 0, 0));
		if (input.getKey(SDL_SCANCODE_DOWN))
			camera.addRot(Vector3(125 * RedrawTime, 0, 0));
		if (input.getKey(SDL_SCANCODE_LEFT))
			camera.addRot(Vector3(0, 125 * RedrawTime, 0));
		if (input.getKey(SDL_SCANCODE_RIGHT))
			camera.addRot(Vector3(0, -125 * RedrawTime, 0));

		if (input.getKey(SDL_SCANCODE_LSHIFT))
			camera.addPos(-camera.up() * RedrawTime * 5);
		if (input.getKey(SDL_SCANCODE_SPACE))
			camera.addPos(camera.up() * RedrawTime * 5);
		if (input.getKey(SDL_SCANCODE_Q))
			camera.addRot(Vector3(0, 0, 125 * RedrawTime));
		if (input.getKey(SDL_SCANCODE_E))
			camera.addRot(Vector3(0, 0, -125 * RedrawTime));

		if (input.getKeyDown(SDL_SCANCODE_ESCAPE))
		{
			cursor = !cursor;
			input.showMouseCursor(cursor);
		}

		if (!cursor)
		{
			Vector2 deltaMouse = input.getMouseMovement();
			camera.addRot(Vector3(deltaMouse.Y, -deltaMouse.X, 0) * 0.3);
			input.setMousePos(window.getSize() * 0.5);
		}

		xPos -= 0.05f;

		Source->SetPosition(Vector3(xPos, 0.0f, 0.0f));

		camera.setRot(Vector3::Clamp(camera.getRot(), Vector3(-89.9, -360, 0.0), Vector3(89.9, 360, 0.0)));
		camera.update();

		scene.setContextSize(window.getSize());
		scene.update();
		scene.render();

		window.display();

		if ((timer.elapsed()) > 1.0)
		{
			printf("%i\n", window.getFPS());
			timer.reset();
		}
	}

	delete Source;
	delete gDevice;
	delete gAudioDevice;

	return 0;
}


