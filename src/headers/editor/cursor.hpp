#ifndef EDITOR_CURSOR_HPP
#define EDITOR_CURSOR_HPP

#include <cstddef>
namespace editor {
class Buffer;

class Cursor {
    Buffer& parent;
    size_t anchor_offset;
    size_t tail_offset;

public:
    Cursor(Buffer& parent, size_t anchor_offset, size_t tail_offset);
};
}  // namespace editor

#endif  // !EDITOR_CURSOR_HPP
