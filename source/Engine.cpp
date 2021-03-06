
#if defined(USES_WINDOWS_OPENGL) || defined(USES_LINUX)
	#include "opengl/OpenGL.h"
#else
	#include "directx/DXMain.h"
#endif

#include "../include/Engine.h"
#include "../include/FileUtil.h"

#include "AppSetup.h"

#if defined(USES_WINDOWS_OPENGL) || defined(USES_LINUX)
	void OpenGL_onResizeWindow(int w, int h);//declared in OpenGL.h
#endif

//---------------------------------------------------------------------

Engine* Engine::m_instance = NULL;

//---------------------------------------------------------------------

Engine::Engine(const char* argv0, const AppSetupInfos& appSetupInfos)
:
m_frameDuration(0), m_argv0(argv0), m_isInit(false), m_isAppRunning(true), m_appSetup(new AppSetup(appSetupInfos))
{
#ifndef USES_WINDOWS8_METRO
	FileUtil::setAppDataFolderBasename(m_appSetup->getAppSetupInfos().appDataFolderBasename.c_str());
#endif
	m_instance = this;
}

//---------------------------------------------------------------------

Engine::~Engine()
{
	delete m_appSetup;
}

//---------------------------------------------------------------------

void Engine::initLowLevel()
{
	outputln("Init Engine...");
	Utils::indentLog();
	Utils::initClock();
	Utils::initRandomSeed();
	m_appSetup->init();
	m_scene2D = new Scene2D;
	m_keyboardManager = new KeyboardManager;
	m_mouseManager = new MouseManager;
	m_joystickManager = new JoystickManager;
#ifdef USES_SOUND
	m_soundManager = new SoundManager(m_argv0.c_str());
#endif 
	m_scene3D = new Scene3D;
	m_isInit = true;
	Utils::unindentLog();
	outputln("Init Engine done.");

	this->onWindowResizeInternals();
}

//---------------------------------------------------------------------

void Engine::resetSoundSystem()
{
	delete m_soundManager;
	m_soundManager = new SoundManager(m_argv0.c_str());
}

//---------------------------------------------------------------------

void Engine::deinitLowLevel()
{
	delete m_scene3D;

#ifdef USES_SOUND
	delete m_soundManager;
#endif

	delete m_joystickManager;
	delete m_keyboardManager;
	delete m_mouseManager;
	delete m_scene2D;

	m_appSetup->deinit();
}

void Engine::clearScreen(const Color& color)
{
	this->getScene2DMgr().clearScreen(color);
#if defined(USES_WINDOWS8_DESKTOP) || defined(USES_WINDOWS8_METRO)
	this->getScene3DMgr().clearScene(color);
#endif
}

Color Engine::getDefaultBackgroundColor()
{
	return AppSetup::instance().getDefaultBackgroundColor();
}

Color Engine::getDefaultForegroundColor()
{
	return AppSetup::instance().getDefaultForegroundColor();
}

void Engine::startRender()
{
#if defined(USES_WINDOWS_OPENGL) || defined(USES_LINUX)
	//AppSetup::manage();
	//this->openGL->manageOpenGL();
	this->getScene2DMgr().set2DMode();
#else
	DXMain::instance()->RunStepBegin();
#endif
}

void Engine::endRender()
{
	AppSetup::instance().manageRender();
#if defined(USES_WINDOWS8_DESKTOP) || defined(USES_WINDOWS8_METRO)
	DXMain::instance()->RunStepEnd();
#endif
}
//---------------------------------------------------------------------

void Engine::onPointerPressedInternals(int button, int x, int y)
{
	m_mouseManager->onPointerPressed(button, x, y);
}

void Engine::onPointerReleasedInternals(int button, int x, int y)
{
	m_mouseManager->onPointerReleased(button, x, y);
}

void Engine::onPointerMovedInternals(bool isClicking, int x, int y)
{
	m_mouseManager->onPointerMoving(isClicking, x, y);
}

void Engine::onPointerWheelChangedInternals(int wheelDelta, int x, int y)
{
	m_mouseManager->onPointerWheelChanged(wheelDelta, x, y);
}

void Engine::onKeyPressedInternals(int virtualKey)
{
	m_keyboardManager->onKeyPressed(virtualKey);
}

void Engine::onKeyReleasedInternals(int virtualKey)
{
	m_keyboardManager->onKeyReleased(virtualKey);
}

void Engine::onWindowResizeInternals()
{
#if defined(USES_WINDOWS8_DESKTOP) || defined(USES_WINDOWS8_METRO)
	m_scene3D->createWindowSizeDependentResources();
#endif
}

//---------------------------------------------------------------------
