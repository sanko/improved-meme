#include "ast.hh"
#include "config.hh"

#include <string>

namespace Rocken
{

std::u32string LetStatement::string() {
    std::u32string info{literal() + U" " + name_->string() + U" = "};
    if (value_ != nullptr) { info += value_->string(); }
    info += U";";
    return info;
}
} // namespace Rocken
