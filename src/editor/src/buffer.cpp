#include <cstdlib>
#include <editor/buffer.hpp>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "tui/components/simpletext.hpp"
#include "tui/tui.hpp"
#include "utf8/checked.h"
#include "utils.hpp"

namespace editor {
Buffer::Buffer(const std::string& file_path) : base{file_path} {
    if (!base.is_open()) 
        // TODO: better errors
        throw std::runtime_error("editor: failed to open file: " + file_path);
}

bool Buffer::getLineOrPull(const size_t idx) {
    std::string line;
    while (read_lines.size() <= idx) {
        if (base.eof()) return false;
        std::getline(base, line);
        read_lines.push_back(line);
        line.clear();
    }
    return true;
}

std::vector<std::shared_ptr<tui::TermOutputter>> Buffer::render(
    const tui::RenderContext& ctx,
    const tui::RenderExtent& extent) {
    UNUSED(ctx);

    size_t curr_line = first_line_in_view;
    size_t idx_into_line = 0;
    std::vector<std::string> outputs;

    while (outputs.size() < extent.height) {
        // if EOF then don't proceed
        if (!getLineOrPull(curr_line)) break;
        std::string& line = read_lines[curr_line];
        auto line_head = line.begin() + idx_into_line;
        size_t remaining_line_length =
            utf8::distance(line_head, line.end());
        if (remaining_line_length <= extent.width) {
            outputs.push_back(line + std::string(extent.width - remaining_line_length, ' '));
            curr_line++;
            idx_into_line = 0;
        } else {
            auto line_end = line_head;
            utf8::advance(line_end, extent.width, line.end());
            outputs.push_back(std::string(line_head, line_end));
            if (wrap_lines) {
                idx_into_line += extent.width;
            } else {
                curr_line++;
                // idx_into_line = 0 is unnecessary since we never actually touch it with wrap_lines off
            }
        }
    }

    std::vector<std::shared_ptr<tui::TermOutputter>> outputters {extent.height};

    for (size_t i = 0; i < extent.height; i++) {
        if (i >= outputs.size()) {
            outputters[i].reset(
                new tui::SimpleTextOutputter(std::string(extent.width, ' ')));
        } else {
            outputters[i].reset(new tui::SimpleTextOutputter(outputs[i]));
        }
    }

    return outputters;
}

void Buffer::flushUpdate() {}

std::optional<std::vector<tui::PartialUpdateOutput>> Buffer::partialRender(
        const tui::RenderExtent& extent,
        const std::optional<tui::RenderExtent>& oldExtent) {
    return std::optional<std::vector<tui::PartialUpdateOutput>>();
}

bool Buffer::isEditable() {return false;}

void Buffer::scroll(int down) {
    if (down < 0) {
        first_line_in_view -= std::min(first_line_in_view, static_cast<size_t>(std::abs(down)));
    } else {
        if (read_lines.size() > down + first_line_in_view) {
            first_line_in_view += down;
        } else {
            size_t target_pos = first_line_in_view + down;
            first_line_in_view = read_lines.size() - 1;
            // move 1 line at a time to ensure that we actually have it in the buffer
            while (first_line_in_view < target_pos) {
                if (!getLineOrPull(read_lines.size())){
                    // eof, backpedal
                    first_line_in_view = read_lines.size() - 1;
                    break;
                };
                first_line_in_view++;
            }
        }
    }
}
}
