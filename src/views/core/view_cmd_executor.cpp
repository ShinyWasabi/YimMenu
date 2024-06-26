#include "backend/context/default_command_context.hpp"
#include "gui.hpp"
#include "pointers.hpp"
#include "services/hotkey/hotkey_service.hpp"
#include "views/view.hpp"

namespace big
{
	void view::cmd_executor()
	{
		if (!g.cmd_executor.enabled)
			return;

		float screen_x = (float)*g_pointers->m_gta.m_resolution_x;
		float screen_y = (float)*g_pointers->m_gta.m_resolution_y;

		ImGui::SetNextWindowPos(ImVec2(screen_x * 0.25f, screen_y * 0.2f), ImGuiCond_Always);
		ImGui::SetNextWindowBgAlpha(0.65f);
		ImGui::SetNextWindowSize({screen_x * 0.5f, -1});

		if (ImGui::Begin("cmd_executor", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMouseInputs))
		{
			static std::string command_buffer;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {10.f, 15.f});
			components::sub_title("CMD_EXECUTOR_TITLE"_T);

			// set focus by default on input box
			ImGui::SetKeyboardFocusHere(0);

			ImGui::SetNextItemWidth((screen_x * 0.5f) - 30.f);
			if (components::input_text_with_hint("", "CMD_EXECUTOR_TYPE_CMD"_T, command_buffer, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				if (command::process(command_buffer, std::make_shared<default_command_context>(), true))
				{
					g.cmd_executor.enabled = false;
					command_buffer     = {};
				}
			}

			components::small_text("CMD_EXECUTOR_MULTIPLE_CMDS"_T);
			ImGui::Spacing();

			auto possible_commands = command::get_suggestions(command_buffer);
			if (possible_commands.size() == 0)
			{
				ImGui::Text("CMD_EXECUTOR_NO_CMD"_T.data());
			}
			else
			{
				for (auto cmd : possible_commands)
				{
					auto cmd_name = cmd->get_name();
					auto cmd_label = cmd->get_label();
					auto cmd_description = cmd->get_description();
					auto cmd_num_args    = cmd->get_num_args() ? cmd->get_num_args().value() : 0;

					ImGui::Text(std::vformat("CMD_EXECUTOR_CMD_TEMPLATE"_T, std::make_format_args(cmd_name, cmd_label, cmd_description, cmd_num_args)).data());

					// check if we aren't on the last iteration
					if (cmd != possible_commands.back())
						ImGui::Separator();
				}
			}

			ImGui::PopStyleVar();
		}

		ImGui::End();
	}

	bool_command
	    g_cmd_executor("cmdexecutor", "CMD_EXECUTOR", "CMD_EXECUTOR_DESC", g.cmd_executor.enabled, false);
}