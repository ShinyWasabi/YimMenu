#include "gui/window.hpp"
#include "gui/tab_bar.hpp"

namespace big
{
	void window::render_player_window()
	{
		if (g_selectedPlayer.id != g_selectedPlayer.id || !g_selectedPlayer.is_online) return;

		char title[64];
		strcpy(title, "Player Options: ");
		strcat(title, g_selectedPlayer.name);
		strcat(title, "###player_options");

		ImGui::SetNextWindowSize({ 350.f, 300.f }, ImGuiCond_FirstUseEver);
		if (g_temp.windows.player && ImGui::Begin(title, &g_temp.windows.player))
		{
			ImGui::BeginTabBar("tabbar_player");
			tabbar::player_info();
			tabbar::player_griefing();
			tabbar::player_teleport();
			tabbar::player_drop();
			ImGui::EndTabBar();

			ImGui::End();
		}
	}
}