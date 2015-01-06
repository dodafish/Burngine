//============================================================================
// Name        : Burnlication.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include <Burngine/Burngine.hpp>
#include <unistd.h>
#include <string>
#include <sstream>

#include <Burngine/System/Error.hpp>

#include <thread>

const int TOSEC = 999999;

burn::Mutex mutex;

#define RECT_COUNT 50
burn::Sprite rects[RECT_COUNT];
burn::Sprite sprite;

void* proc(void*) {

	burn::Lock lock(mutex);

	burn::Window wnd;

	wnd.create(burn::Vector2i(800, 600), "Burnlication", burn::Window::FIXED_SIZE, false);
	wnd.setFramerateLimit(0);
	wnd.clear();

	std::string s;

	burn::Camera cam;
	cam.setPosition(burn::Vector3f(2.f, 25.f, 15.f));

	cam.setRotation(burn::Rotation(burn::Vector3f(0.f, 30.f, 0.f)));

	burn::PointLight light;
	light.setPosition(burn::Vector3f(0.f, 7.f, 0.f));
	light.setIntensity(30.f);
	light.setColor(burn::Vector3f(0.95f, 0.95f, 1.f));

	burn::PointLight light2;
	light2.setPosition(burn::Vector3f(-15.f, 15.f, 15.f));
	light2.setIntensity(270.f);
	light2.setColor(burn::Vector3f(0.8f, 1.f, 0.85f));

	burn::DirectionalLight sun;
	sun.setIntensity(0.9f);
	sun.setRotation(burn::Vector3f(0.f, 0.f, 60.f));

	burn::SpotLight flashlight;
	flashlight.setIntensity(80.f);
	flashlight.setPosition(burn::Vector3f(-7.f, 5.f, 0.f));
	flashlight.setRotation(burn::Rotation(burn::Vector3f(0.f, 190.f, 50.f)));

	burn::Model jet;
	jet.loadFromFile("./data/f35/F-35_Lightning_II.obj");
	//jet.loadFromFile("./cube.obj");
	//jet.loadFromFile("./multitexCube.dae");
	jet.setPosition(burn::Vector3f(0.f, -3.f, 0.f));
	jet.setRotation(burn::Vector3f(-90.f, 0.f, 0.f));
	jet.setScale(burn::Vector3f(100.f));

	burn::Model groundModel;
	groundModel.loadFromFile("./ground.obj");

	burn::Renderer renderer;

	burn::Clock clock;
	//float rot = 0.f;

	int frame = 0;
	double total = 0;

	float scale = 0.5f;

	burn::Renderer::Output output = burn::Renderer::FINAL;

	burn::Scene scene;
	scene.attachModel(&jet);
	scene.attachModel(&groundModel);

	scene.attachLight(&sun);
	//scene.attachLight(&flashlight);
	//scene.attachLight(&light2);

	burn::Sprite annotation;
	burn::Texture2D annotationTexture;
	annotationTexture.loadFromFile("./annotation.tga");
	std::cout << "Done loading the fkin TGA\n";
	annotation.setTexture(annotationTexture);
	annotation.setColor(burn::Vector4f(1.f));

	burn::Vector3f camPos = cam.getPosition();

	///////////////////////////////////////////////////////////////////////////

	burn::Font font;
	font.loadFromFile("./font.ttf");

	burn::Label label;
	label.setFont(font);
	label.setPosition(burn::Vector2f(300, 50));
	label.setText("The quick brown fox jumps over the lazy dog.");
	label.setFontSize(24);

	while(wnd.isOpen()){
		++frame;

		const double& elapsed = clock.reset().asSeconds();
		total += elapsed;

		burn::Event event;
		while(wnd.pollEvent(event)){
			if(event.getType() == burn::Event::CLOSED){
				wnd.close();
			}else if(event.getType() == burn::Event::CHARACTER){

				s += event.getCharacter();

			}else if(event.getType() == burn::Event::KEY_PRESSED){
				if(event.getKey() == burn::Keyboard::RETURN){
					std::cout << s << "\n";
					s.clear();
				}else if(event.getKey() == burn::Keyboard::ESCAPE)
					wnd.close();
				else if(event.getKey() == burn::Keyboard::Q)
					scale += elapsed;
				else if(event.getKey() == burn::Keyboard::E)
					scale -= elapsed;
				else if(event.getKey() == burn::Keyboard::Y)
					output = burn::Renderer::FINAL;
				else if(event.getKey() == burn::Keyboard::X)
					output = burn::Renderer::DIFFUSE;
				else if(event.getKey() == burn::Keyboard::C)
					output = burn::Renderer::NORMAL;
				else if(event.getKey() == burn::Keyboard::V)
					output = burn::Renderer::POSITION;
				else if(event.getKey() == burn::Keyboard::B)
					output = burn::Renderer::LIGHTING;
				else if(event.getKey() == burn::Keyboard::R)
					renderer.setGlowEnabled(true);
				else if(event.getKey() == burn::Keyboard::F)
					renderer.setGlowEnabled(false);
			}
		}

		if(burn::Keyboard::isKeyPressed(burn::Keyboard::W))
			camPos.z -= 50.f * elapsed;
		else if(burn::Keyboard::isKeyPressed(burn::Keyboard::S))
			camPos.z += 50.f * elapsed;
		if(burn::Keyboard::isKeyPressed(burn::Keyboard::A))
			camPos.x -= 50.f * elapsed;
		else if(burn::Keyboard::isKeyPressed(burn::Keyboard::D))
			camPos.x += 50.f * elapsed;
		if(burn::Keyboard::isKeyPressed(burn::Keyboard::CONTROL))
			camPos.y -= 50.f * elapsed;
		else if(burn::Keyboard::isKeyPressed(burn::Keyboard::SHIFT))
			camPos.y += 50.f * elapsed;

		cam.setPosition(camPos);
		cam.lookAt(burn::Vector3f(0.f, -3.f, 0.f));

		jet.setScale(burn::Vector3f(scale));
		jet.setRotation(jet.getRotation() * burn::Rotation(burn::Vector3f(0.f, 0.f, 30.f * elapsed)));

		if(total >= 0.2){
			std::stringstream ss;
			ss << "Burnlication - FPS:" << (1.0 / elapsed);
			wnd.setTitle(ss.str());
			frame = 0;
			total = 0.0;
		}

		//rot = 5.f;
		for(int i = 0; i != RECT_COUNT; ++i)
			rects[i].setRotation(rects[i].getRotation() + ((((i * 512) % 1200) - 600) * elapsed));

		wnd.clear(burn::Vector4f(0.f, 0.f, 0.f, 1.f));
		renderer.setOutput(output);
		renderer.prepare(burn::Vector2ui(wnd.getVideoMode().getWidth(), wnd.getVideoMode().getHeight()));

		sprite.setRotation(sprite.getRotation() + 200.f * elapsed);

		renderer.renderScene(scene, cam);

		// Gui here
		renderer.renderGuiNode(annotation);
		renderer.renderGuiNode(label);

		renderer.finalize(wnd);
		wnd.display();

	}

	return 0;
}

int main() {

	std::string s = "   0.999999";
	float f = std::atof(s.c_str());
	std::cout << "f=" << f << "\n";

	//burn::Texture texture;
	//texture.loadFromFile("./sky.bmp");

	/*sprite.setDimensions(burn::Vector2f(400.f, 400.f));
	 sprite.setPosition(burn::Vector2f(600.f, 350.f));
	 sprite.setOrigin(burn::Vector2f(200.f, 200.f));
	 sprite.setTexture(texture);

	 for(int i = 0; i != RECT_COUNT; ++i){
	 rects[i].setDimensions(burn::Vector2f(100.f, 100.f));
	 rects[i].setPosition(burn::Vector2f(100.f + ((i * 345) % 1000),
	 100.f + ((i * 734) % 500)));
	 rects[i].setOrigin(burn::Vector2f(50.f, 50.f));
	 rects[i].setRotation(i * 13.f);
	 rects[i].setScale(burn::Vector2f(1.5f, 1.f));
	 rects[i].setColor(burn::Vector4f(	((i * 234) % 100) / 100.f,
	 ((i * 745) % 100) / 100.f,
	 ((i * 12) % 100) / 100.f,
	 1.f));
	 rects[i].setTexture(texture);
	 }*/

	proc(NULL);

	burn::cleanupBurngine();

	return 0;
}
