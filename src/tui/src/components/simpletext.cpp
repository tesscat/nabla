#include <utf8.h>
#include <algorithm>
#include <cstring>
#include <optional>
#include <ostream>
#include <string>
#include <tui/components/simpletext.hpp>
#include <vector>
#include "tui/tui.hpp"
#include "utf8/checked.h"
#include "utils.hpp"

namespace tui {
SimpleTextOutputter::SimpleTextOutputter(const std::string& string) : content{string} {}
void SimpleTextOutputter::apply(std::ostream& output) {output << content;}
SimpleTextOutputter::~SimpleTextOutputter() {}

SimpleTextComponent::SimpleTextComponent(std::string initialString)
    : contents{initialString} {}
SimpleTextComponent::~SimpleTextComponent() {}
void SimpleTextComponent::flushUpdate() {
    hasBeenSet = false;
}
std::string SimpleTextComponent::getContents() {
    return contents;
}
void SimpleTextComponent::setContents(std::string newContents) {
    contents = newContents;
    hasBeenSet = true;
}
std::optional<std::vector<PartialUpdateOutput>>
SimpleTextComponent::partialRender(
    const RenderExtent& extent,
    const std::optional<RenderExtent>& oldExtent) {
    UNUSED(extent);
    UNUSED(oldExtent);
    if (hasBeenSet)
        return std::optional<std::vector<PartialUpdateOutput>>();
    else
        return std::optional(std::vector<PartialUpdateOutput>());
}

std::vector<std::shared_ptr<TermOutputter>> SimpleTextComponent::render(
    const RenderContext& ctx,
    const RenderExtent& extent) {
    UNUSED(ctx);

    std::vector<std::string> outputs;
    auto currterator = contents.begin();
    while (currterator < contents.end() && outputs.size() < extent.height) {
        auto nextNewline = std::find(currterator, contents.end(), '\n');
        size_t nextNewlineDistance = utf8::distance(currterator, nextNewline);
        size_t remainingLength = utf8::distance(currterator, contents.end());
        size_t thisLineLength = std::min(
            std::min(nextNewlineDistance, extent.width), remainingLength);
        auto nexterator = currterator;
        utf8::advance(nexterator, thisLineLength, contents.end());
        outputs.push_back(std::string(currterator, nexterator) +
                          std::string(extent.width - thisLineLength, ' '));
        currterator = nexterator;
        if (nextNewlineDistance <= extent.width)
            currterator++;
    }

    // std:vector<std::shared_ptr<TermOutputter>> outputters {extent.height};
    std::vector<std::shared_ptr<TermOutputter>> outputters{extent.height};

    for (size_t i = 0; i < extent.height; i++) {
        if (i >= outputs.size()) {
            outputters[i].reset(
                new SimpleTextOutputter(std::string(extent.width, ' ')));
        } else {
            outputters[i].reset(new SimpleTextOutputter(outputs[i]));
        }
    }

    return outputters;
}
}  // namespace tui
