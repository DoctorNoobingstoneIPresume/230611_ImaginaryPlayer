#include "string_view.hpp"

namespace lyb
{

std::string ViewToString (string_view sv)
{
	return std::string {sv.data (), sv.size ()};
}

}
