#include "afk/ui/Log.hpp"

using Afk::Log;

Log::Log() {
  this->auto_scroll = true;
  this->clear();
}

auto Log::clear() -> void {
  this->buffer.clear();
  this->line_offsets.clear();
  this->line_offsets.push_back(0);
}

IM_FMTARGS(2) auto Log::append(const char *fmt, ...) -> void {
  int old_size = this->buffer.size();
  va_list args;
  va_start(args, fmt);
  this->buffer.appendfv(fmt, args);
  va_end(args);

  for (int new_size = this->buffer.size(); old_size < new_size; old_size++)
    if (this->buffer[old_size] == '\n')
      this->line_offsets.push_back(old_size + 1);
}

auto Log::draw(const char *title, bool *open) -> void {
  if (!ImGui::Begin(title, open)) {
    ImGui::End();
    return;
  }

  // Options menu
  if (ImGui::BeginPopup("Options")) {
    ImGui::Checkbox("Auto-scroll", &this->auto_scroll);
    ImGui::EndPopup();
  }

  // Main window
  if (ImGui::Button("Options"))
    ImGui::OpenPopup("Options");
  ImGui::SameLine();
  bool clear = ImGui::Button("Clear");
  ImGui::SameLine();
  bool copy = ImGui::Button("Copy");
  ImGui::SameLine();
  this->filter.Draw("Filter", -100.0f);

  ImGui::Separator();
  ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

  if (clear)
    this->clear();
  if (copy)
    ImGui::LogToClipboard();

  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
  const char *buf     = this->buffer.begin();
  const char *buf_end = this->buffer.end();
  if (this->filter.IsActive()) {
    // In this example we don't use the clipper when this->filter is enabled.
    // This is because we don't have a random access on the result on our filter.
    // A real application processing logs with ten of thousands of entries may want to store the result of search/filter.
    // especially if the filtering function is not trivial (e.g. reg-exp).
    for (int line_no = 0; line_no < this->line_offsets.Size; line_no++) {
      const char *line_start = buf + this->line_offsets[line_no];
      const char *line_end   = (line_no + 1 < this->line_offsets.Size)
                                 ? (buf + this->line_offsets[line_no + 1] - 1)
                                 : buf_end;

      if (this->filter.PassFilter(line_start, line_end))
        ImGui::TextUnformatted(line_start, line_end);
    }
  } else {
    // The simplest and easy way to display the entire buffer:
    //   ImGui::TextUnformatted(buf_begin, buf_end);
    // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward to skip non-visible lines.
    // Here we instead demonstrate using the clipper to only process lines that are within the visible area.
    // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them on your side is recommended.
    // Using ImGuiListClipper requires A) random access into your data, and B) items all being the  same height,
    // both of which we can handle since we an array pointing to the beginning of each line of text.
    // When using the filter (in the block of code above) we don't have random access into the data to display anymore, which is why we don't use the clipper.
    // Storing or skimming through the search result would make it possible (and would be recommended if you want to search through tens of thousands of entries)
    ImGuiListClipper clipper;
    clipper.Begin(this->line_offsets.Size);
    while (clipper.Step()) {
      for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++) {
        const char *line_start = buf + this->line_offsets[line_no];
        const char *line_end   = (line_no + 1 < this->line_offsets.Size)
                                   ? (buf + this->line_offsets[line_no + 1] - 1)
                                   : buf_end;
        ImGui::TextUnformatted(line_start, line_end);
      }
    }
    clipper.End();
  }
  ImGui::PopStyleVar();

  if (this->auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
    ImGui::SetScrollHereY(1.0f);

  ImGui::EndChild();
  ImGui::End();
}
