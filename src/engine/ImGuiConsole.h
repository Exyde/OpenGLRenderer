#pragma once
#include <string>
#include <vector>

#include "../imgui/imgui.h"

class ImGuiConsole {
   public:
    void Log(const std::string& message) {
        logs.push_back(message);
        scrollToBottom = true;
    }

    void Clear() {
        logs.clear();
    }

    void Draw(const char* title = "Console") {
        ImGui::Begin(title);

        if (ImGui::Button("Clear"))
            Clear();
        ImGui::SameLine();
        ImGui::Text("Logs: %d", (int)logs.size());
        ImGui::Separator();

        ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
        for (const auto& line : logs) ImGui::TextUnformatted(line.c_str());
        if (scrollToBottom)
            ImGui::SetScrollHereY(1.0f);
        scrollToBottom = false;
        ImGui::EndChild();

        ImGui::End();
    }

   private:
    std::vector<std::string> logs;
    bool scrollToBottom = false;
};
