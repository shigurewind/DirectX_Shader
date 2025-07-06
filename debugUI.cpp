#include "debugUI.h"
#include "imgui.h"

#include "Camera.h"


void ShowDebugUI() 
{
	ImGui::Begin("Debug Menu");


	ImGui::Text(u8"Debug情報");
	if (ImGui::CollapsingHeader(u8"カメラ"))
	{
		Camera* cam = GetCamera();
		ImGui::DragFloat3(u8"カメラ位置", (float*)&cam->Position, 0.01f);
		ImGui::DragFloat3(u8"カメラ注目する座標", (float*)&cam->AtPosition, 0.05f);
		ImGui::SliderFloat(u8"FOV", &cam->Fov, 10.0f, 90.0f);
	}


	ImGui::End();
}