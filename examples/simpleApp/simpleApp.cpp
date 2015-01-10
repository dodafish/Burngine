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

void* proc(void*) {

	burn::Lock lock(mutex);

	burn::Window wnd;

	wnd.create(	burn::Vector2i(	1600,
								900),
				"Burnlication",
				burn::Window::FIXED_SIZE,
				false);
	wnd.setFramerateLimit(0);
	wnd.clear();

	std::string s;

	burn::Camera cam;
	cam.setPosition(burn::Vector3f(	2.f,
									25.f,
									15.f));
	cam.setAspectRatio(16.f / 9.f);
	cam.setRotation(burn::Rotation(burn::Vector3f(	0.f,
													30.f,
													0.f)));

	burn::PointLight light;
	light.setPosition(burn::Vector3f(	0.f,
										7.f,
										0.f));
	light.setIntensity(30.f);
	light.setColor(burn::Vector3f(	0.95f,
									0.95f,
									1.f));

	burn::PointLight light2;
	light2.setPosition(burn::Vector3f(	-15.f,
										15.f,
										15.f));
	light2.setIntensity(270.f);
	light2.setColor(burn::Vector3f(	0.8f,
									1.f,
									0.85f));

	burn::DirectionalLight sun;
	sun.setIntensity(0.9f);
	sun.setRotation(burn::Vector3f(	0.f,
									0.f,
									60.f));

	burn::SpotLight flashlight;
	flashlight.setIntensity(80.f);
	flashlight.setPosition(burn::Vector3f(	-7.f,
											5.f,
											0.f));
	flashlight.setRotation(burn::Rotation(burn::Vector3f(	0.f,
															190.f,
															50.f)));

	burn::Model jet;
	jet.loadFromFile("../examples/data/gear.3ds");
	//jet.loadFromFile("./cube.obj");
	//jet.loadFromFile("./multitexCube.dae");
	jet.setPosition(burn::Vector3f(	0.f,
									-3.f,
									0.f));
	jet.setRotation(burn::Vector3f(	-90.f,
									0.f,
									0.f));
	jet.setScale(burn::Vector3f(100.f));

	burn::Model groundModel;
	groundModel.loadFromFile("../examples/data/ground.obj");

	burn::Renderer renderer;

	burn::Clock clock;
	//float rot = 0.f;

	int frame = 0;
	double total = 0;

	float scale = 1.f;

	burn::Renderer::Output output = burn::Renderer::FINAL;

	burn::Scene scene;
	scene.attachModel(&jet);
	scene.attachModel(&groundModel);

	scene.attachLight(&sun);
	//scene.attachLight(&flashlight);
	//scene.attachLight(&light2);

	burn::Sprite annotation;
	burn::Texture2D annotationTexture;
	annotationTexture.loadFromFile("../examples/data/annotation.tga");
	std::cout << "Done loading the fkin TGA\n";
	annotation.setTexture(annotationTexture);
	annotation.setColor(burn::Vector4f(1.f));

	burn::Vector3f camPos = cam.getPosition();

	///////////////////////////////////////////////////////////////////////////

	burn::Font font;
	font.loadFromFile("../examples/data/font.ttf");

	burn::Label label;
	label.setFont(font);
	label.setPosition(burn::Vector2f(	250,
										30));
	label.setText("The quick brown fox jumps over the lazy dog.");
	//label.setText("AABBCCDDEEFFGGHHIIJJKKLLMMNNOOPPQQRRSSTTUUVVWWXXYYZZ");
	label.setFontSize(27);
	label.setColor(burn::Vector4f(	1.f,
									0.7f,
									0.1f,
									1.f));

	burn::Billboard billboard;
	billboard.setTexture(annotationTexture);
	billboard.setPosition(burn::Vector3f(	10.f,
											5.f,
											5.f));
	scene.attachBillboard(&billboard);

	// Skybox
	burn::CubeMap skyboxTexture;
	skyboxTexture.loadFromFile("../examples/data/Skybox/skybox_right.bmp",
							   "../examples/data/Skybox/skybox_left.bmp",
							   "../examples/data/Skybox/skybox_bottom.bmp",
							   "../examples/data/Skybox/skybox_top.bmp",
							   "../examples/data/Skybox/skybox_back.bmp",
							   "../examples/data/Skybox/skybox_front.bmp");
	burn::Skybox skybox;
	skybox.setCubeMap(skyboxTexture);
	scene.attachSkybox(&skybox);

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
		cam.lookAt(burn::Vector3f(	0.f,
									-3.f,
									0.f));

		jet.setScale(burn::Vector3f(scale));
		jet.setRotation(jet.getRotation() * burn::Rotation(burn::Vector3f(	10.f * elapsed,
																			20.f * elapsed,
																			30.f * elapsed)));

		if(total >= 0.2){
			std::stringstream ss;
			ss << "FPS:" << (1.0 / elapsed);
			label.setText(ss.str());
			frame = 0;
			total = 0.0;
		}

		wnd.clear(burn::Vector4f(	0.f,
									0.f,
									0.f,
									1.f));
		renderer.setOutput(output);
		renderer.prepare(burn::Vector2ui(	wnd.getVideoMode().getWidth(),
											wnd.getVideoMode().getHeight()));

		renderer.renderScene(	scene,
								cam);

		// Gui here
		renderer.renderGuiNode(annotation);
		renderer.renderGuiNode(label);

		renderer.finalize(wnd);
		wnd.display();

	}

	return 0;
}

int main() {

	burn::BurnShaders::setBurnshadersPath("../examples/data/burnshaders");

	proc(NULL);

	burn::cleanupBurngine();

	return 0;
}
