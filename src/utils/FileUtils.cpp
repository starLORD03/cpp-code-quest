#include "FileUtils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <optional>

namespace fs = std::filesystem;

namespace GameUtils {

    // Read entire file content into a string
    std::optional<std::string> FileUtils::read_file(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filepath << std::endl;
            return std::nullopt;
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    // Write content to file
    bool FileUtils::write_file(const std::string& filepath, const std::string& content) {
        std::ofstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not create file " << filepath << std::endl;
            return false;
        }
        
        file << content;
        return file.good();
    }

    // Append content to file
    bool FileUtils::append_to_file(const std::string& filepath, const std::string& content) {
        std::ofstream file(filepath, std::ios::app);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file for appending " << filepath << std::endl;
            return false;
        }
        
        file << content;
        return file.good();
    }

    // Read file line by line
    std::vector<std::string> FileUtils::read_lines(const std::string& filepath) {
        std::vector<std::string> lines;
        std::ifstream file(filepath);
        
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filepath << std::endl;
            return lines;
        }
        
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        
        return lines;
    }

    // Write lines to file
    bool FileUtils::write_lines(const std::string& filepath, const std::vector<std::string>& lines) {
        std::ofstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not create file " << filepath << std::endl;
            return false;
        }
        
        for (const auto& line : lines) {
            file << line << std::endl;
        }
        
        return file.good();
    }

    // Check if file exists
    bool FileUtils::file_exists(const std::string& filepath) {
        return fs::exists(filepath) && fs::is_regular_file(filepath);
    }

    // Check if directory exists
    bool FileUtils::directory_exists(const std::string& dirpath) {
        return fs::exists(dirpath) && fs::is_directory(dirpath);
    }

    // Create directory (and parent directories if needed)
    bool FileUtils::create_directory(const std::string& dirpath) {
        try {
            return fs::create_directories(dirpath);
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error creating directory " << dirpath << ": " << e.what() << std::endl;
            return false;
        }
    }

    // Get file size
    std::optional<std::size_t> FileUtils::get_file_size(const std::string& filepath) {
        try {
            if (!file_exists(filepath)) {
                return std::nullopt;
            }
            return fs::file_size(filepath);
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error getting file size for " << filepath << ": " << e.what() << std::endl;
            return std::nullopt;
        }
    }

    // Get file extension
    std::string FileUtils::get_file_extension(const std::string& filepath) {
        fs::path path(filepath);
        return path.extension().string();
    }

    // Get filename without extension
    std::string FileUtils::get_filename_without_extension(const std::string& filepath) {
        fs::path path(filepath);
        return path.stem().string();
    }

    // Get directory path
    std::string FileUtils::get_directory_path(const std::string& filepath) {
        fs::path path(filepath);
        return path.parent_path().string();
    }

    // List files in directory
    std::vector<std::string> FileUtils::list_files_in_directory(const std::string& dirpath, 
                                                               const std::string& extension) {
        std::vector<std::string> files;
        
        try {
            if (!directory_exists(dirpath)) {
                return files;
            }
            
            for (const auto& entry : fs::directory_iterator(dirpath)) {
                if (entry.is_regular_file()) {
                    std::string filepath = entry.path().string();
                    
                    // If extension filter is specified, check it
                    if (!extension.empty()) {
                        if (get_file_extension(filepath) == extension) {
                            files.push_back(filepath);
                        }
                    } else {
                        files.push_back(filepath);
                    }
                }
            }
            
            // Sort files alphabetically
            std::sort(files.begin(), files.end());
            
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error listing files in " << dirpath << ": " << e.what() << std::endl;
        }
        
        return files;
    }

    // Copy file
    bool FileUtils::copy_file(const std::string& source, const std::string& destination) {
        try {
            return fs::copy_file(source, destination, fs::copy_options::overwrite_existing);
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error copying file from " << source << " to " << destination 
                      << ": " << e.what() << std::endl;
            return false;
        }
    }

    // Move/rename file
    bool FileUtils::move_file(const std::string& source, const std::string& destination) {
        try {
            fs::rename(source, destination);
            return true;
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error moving file from " << source << " to " << destination 
                      << ": " << e.what() << std::endl;
            return false;
        }
    }

    // Delete file
    bool FileUtils::delete_file(const std::string& filepath) {
        try {
            return fs::remove(filepath);
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error deleting file " << filepath << ": " << e.what() << std::endl;
            return false;
        }
    }

    // Get current working directory
    std::string FileUtils::get_current_directory() {
        try {
            return fs::current_path().string();
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error getting current directory: " << e.what() << std::endl;
            return "";
        }
    }

    // Change current directory
    bool FileUtils::change_directory(const std::string& dirpath) {
        try {
            fs::current_path(dirpath);
            return true;
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error changing directory to " << dirpath << ": " << e.what() << std::endl;
            return false;
        }
    }

    // Join paths safely
    std::string FileUtils::join_paths(const std::string& path1, const std::string& path2) {
        fs::path p1(path1);
        fs::path p2(path2);
        return (p1 / p2).string();
    }

    // Game-specific utility functions
    
    // Load game configuration
    std::optional<GameConfig> FileUtils::load_game_config(const std::string& config_file) {
        auto content = read_file(config_file);
        if (!content) {
            return std::nullopt;
        }
        
        GameConfig config;
        std::istringstream iss(*content);
        std::string line;
        
        while (std::getline(iss, line)) {
            if (line.empty() || line[0] == '#') continue; // Skip empty lines and comments
            
            auto pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                
                // Trim whitespace (simple implementation)
                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);
                
                config.settings[key] = value;
            }
        }
        
        return config;
    }

    // Save game progress
    bool FileUtils::save_game_progress(const std::string& save_file, const GameProgress& progress) {
        std::ostringstream oss;
        oss << "# C++ Code Quest Save File\n";
        oss << "player_name=" << progress.player_name << "\n";
        oss << "current_level=" << progress.current_level << "\n";
        oss << "experience=" << progress.experience << "\n";
        oss << "completed_levels=" << progress.completed_levels << "\n";
        
        oss << "# Inventory items\n";
        for (const auto& item : progress.inventory) {
            oss << "inventory_item=" << item << "\n";
        }
        
        return write_file(save_file, oss.str());
    }

    // Load game progress
    std::optional<GameProgress> FileUtils::load_game_progress(const std::string& save_file) {
        auto content = read_file(save_file);
        if (!content) {
            return std::nullopt;
        }
        
        GameProgress progress;
        std::istringstream iss(*content);
        std::string line;
        
        while (std::getline(iss, line)) {
            if (line.empty() || line[0] == '#') continue;
            
            auto pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                
                if (key == "player_name") {
                    progress.player_name = value;
                } else if (key == "current_level") {
                    progress.current_level = std::stoi(value);
                } else if (key == "experience") {
                    progress.experience = std::stod(value);
                } else if (key == "completed_levels") {
                    progress.completed_levels = std::stoi(value);
                } else if (key == "inventory_item") {
                    progress.inventory.push_back(value);
                }
            }
        }
        
        return progress;
    }

    // Create project structure for C++ Code Quest
    bool FileUtils::create_project_structure(const std::string& project_root) {
        const std::vector<std::string> directories = {
            "docs",
            "src",
            "src/game",
            "src/utils",
            "examples", 
            "tests"
        };
        
        // Create project root
        if (!create_directory(project_root)) {
            return false;
        }
        
        // Create subdirectories
        for (const auto& dir : directories) {
            std::string full_path = join_paths(project_root, dir);
            if (!create_directory(full_path)) {
                std::cerr << "Failed to create directory: " << full_path << std::endl;
                return false;
            }
        }
        
        // Create essential files
        const std::vector<std::pair<std::string, std::string>> files = {
            {"README.md", "# C++ Code Quest\n\nA modern C++ learning adventure game.\n"},
            {"CMakeLists.txt", "cmake_minimum_required(VERSION 3.10)\nproject(CppCodeQuest)\n"},
            {".gitignore", "build/\n*.exe\n*.obj\n*.o\n"},
            {"LICENSE", "MIT License\n\nCopyright (c) 2024 C++ Code Quest\n"}
        };
        
        for (const auto& [filename, content] : files) {
            std::string file_path = join_paths(project_root, filename);
            if (!write_file(file_path, content)) {
                std::cerr << "Failed to create file: " << file_path << std::endl;
                return false;
            }
        }
        
        std::cout << "Project structure created successfully in: " << project_root << std::endl;
        return true;
    }

} // namespace GameUtils