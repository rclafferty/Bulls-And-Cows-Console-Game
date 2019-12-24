/* Author: Casey Lafferty
 * UnrealSyntax.h defines the necessary text exchange commands to use Unreal Engine C++
 *   syntax in all the game files
 */

#pragma once

#include <string>
#include <map>

// TMaps are the Unreal Engine equivalent of std::map
#define TMap std::map

using namespace std;

// FStrings are the Unreal Engine equivalent of std::string
using FString = std::string;

// int32 is the Unreal Engine equivalent of int
using int32 = int;