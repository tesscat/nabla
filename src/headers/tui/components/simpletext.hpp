#ifndef TUI_SIMPLETEXT_HPP
#define TUI_SIMPLETEXT_HPP

#include <optional>
#include <vector>
#include "tui/tui.hpp"
namespace tui {
class SimpleTextOutputter : public TermOutputter {
    std::string content;

public:
    SimpleTextOutputter(const std::string& string);
    void apply(std::ostream& output) override;
    ~SimpleTextOutputter() override;
};
class SimpleTextComponent : public Component {
    std::string contents;
    bool hasBeenSet = false;

public:
    std::vector<std::shared_ptr<TermOutputter>> render(
        const RenderContext& ctx,
        const RenderExtent& extent) override;
    void flushUpdate() override;
    std::optional<std::vector<PartialUpdateOutput>> partialRender(
        const RenderExtent& extent,
        const std::optional<RenderExtent>& oldExtent) override;

    SimpleTextComponent(std::string initialString = "");
    std::string getContents();
    void setContents(std::string newContents);

    ~SimpleTextComponent() override;
};
}  // namespace tui

#endif  // !TUI_SIMPLETEXT_HPP
