#ifndef TUI_SIMPLETEXT_HPP
#define TUI_SIMPLETEXT_HPP

#include "tui/tui.hpp"
#include <vector>
namespace tui {
class SimpleTextComponent : public Component {
    std::vector<TermOutputter&> render(const RenderContext& ctx, const RenderExtent& extent) override;
    ~SimpleTextComponent() override;
};
}

#endif // !TUI_SIMPLETEXT_HPP
