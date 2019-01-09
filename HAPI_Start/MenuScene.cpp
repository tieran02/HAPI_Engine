#include "MenuScene.hpp"
#include "SceneManager.hpp"
#include "UiManager.hpp"
#include "UiButtonElement.hpp"

using namespace HAPISPACE;

void MenuScene::OnLoad()
{
	auto playButton = std::make_shared<UiButtonElement>();
	playButton->SetText("Play");
	playButton->SetFontSize(45);
	playButton->SetPosition(Vector2f(0.45f, 0.45f));
	UiManager::Instance().AddUIElement("playButton",playButton);

	auto quitButton = std::make_shared<UiButtonElement>();
	quitButton->SetText("Quit");
	quitButton->SetFontSize(45);
	quitButton->SetPosition(Vector2f(0.45f, 0.55f));
	UiManager::Instance().AddUIElement("quitButton", quitButton);
}

void MenuScene::OnUnload()
{

}

void MenuScene::OnStart()
{
}

void MenuScene::OnUpdate()
{
	UiManager::Instance().Update();

	const HAPI_TKeyboardData& keyboard_data = HAPI.GetKeyboardData();
	if (keyboard_data.scanCode[HK_F5] || ((UiButtonElement*)UiManager::Instance().GetUIElement("playButton").get())->IsSelected())
	{
		SceneManager::Instance().LoadScene("GameScene", *m_renderer);
	}

	if(((UiButtonElement*)UiManager::Instance().GetUIElement("quitButton").get())->IsSelected())
	{
		HAPI.Close();
	}
}

void MenuScene::OnRender()
{
	m_renderer->ClearScreen(50);
	UiManager::Instance().Render();
}

