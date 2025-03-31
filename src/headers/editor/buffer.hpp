#ifndef EDITOR_BUFFER_HPP
#define EDITOR_BUFFER_HPP

#include <editor/cursor.hpp>
#include <fstream>
#include <string>
#include <vector>
#include "tui/tui.hpp"

namespace editor {
class Buffer : public tui::Component {
    std::ifstream base;
    std::vector<std::string> read_lines;
    std::vector<Cursor> cursors;
    size_t primary_cursor_index = 0;
    size_t first_line_in_view = 0;
    bool wrap_lines = true;
    // false if we fail to pull a line (EOF)
    bool getLineOrPull(const size_t idx);
public:
    std::vector<std::shared_ptr<tui::TermOutputter>> render(
        const tui::RenderContext& ctx,
        const tui::RenderExtent& extent) override;
    void flushUpdate() override;
    std::optional<std::vector<tui::PartialUpdateOutput>> partialRender(
        const tui::RenderExtent& extent,
        const std::optional<tui::RenderExtent>& oldExtent) override;

    Buffer(const std::string& file_path);
    bool isEditable();
    void scroll(int down);
};
}  // namespace editor

#endif  // !EDITOR_BUFFER_HPP
