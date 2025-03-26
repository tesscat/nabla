#ifndef TUI_TUI_HPP
#define TUI_TUI_HPP

#include <optional>
#include <ostream>
#include <vector>
namespace tui {
struct RenderExtent {
    int row, col, width, height;
};
class TermOutputter {
    virtual ~TermOutputter() =0;

    virtual void apply(std::ostream output) =0;
};
struct PartialUpdateOutput {
    int row, col;
    TermOutputter& value;
};
class RenderContext {

};
class Component {
    virtual ~Component() =0;
    // Returns a list of strings, such that if outputted at the correct location, would render the component linewise
    virtual std::vector<TermOutputter&> render(const RenderContext& ctx, const RenderExtent& extent) =0;
    virtual void flushUpdate() =0;
    // Returns a list of updates that can be applied to update the component
    // or an empty optional if it would be more efficient to re-render the component completely
    virtual std::optional<std::vector<PartialUpdateOutput>> partialRender(const RenderExtent& extent, const std::optional<RenderExtent>& oldExtent) =0;
};
class RenderTheme {

};
}

#endif // !TUI_TUI_HPP
