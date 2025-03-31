#include <editor/cursor.hpp>

namespace editor {
Cursor::Cursor(Buffer& parent_, size_t anchor_offset_, size_t tail_offset_)
    : parent{parent_},
      anchor_offset{anchor_offset_},
      tail_offset{tail_offset_} {}
}  // namespace editor
