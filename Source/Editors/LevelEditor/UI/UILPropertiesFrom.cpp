#include "stdafx.h"

UILPropertiesFrom::UILPropertiesFrom()
{
}

UILPropertiesFrom::~UILPropertiesFrom()
{
}

void UILPropertiesFrom::Draw()
{
	if (ImGui::Begin("Properties"))
	{
		LTools->GetProperties()->Draw();
		
	}
	ImGui::End();

}
