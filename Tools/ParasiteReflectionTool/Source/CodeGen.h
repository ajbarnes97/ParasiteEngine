#pragma once

#include "Model.h"

#include "optional"
#include "filesystem"


/**
 * Emits <className>.generated.h for one parsed header. Returns the output path,
 * or nullopt if the file had nothing reflected in it (no PE_CLASS with
 * PE_GENERATED_BODY(), no PE_ENUM).
 */
std::optional<std::filesystem::path> GenerateOutput(const std::filesystem::path& InSourcePath, const SParsedFile& InParsedFile, const std::filesystem::path& InOutputDir);