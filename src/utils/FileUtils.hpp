#pragma once

#include <string>
#include <vector>
#include <optional>
#include <unordered_map>
#include <cstddef>

namespace GameUtils {

    // Forward declarations for game-specific structures
    struct GameConfig {
        std::unordered_map<std::string, std::string> settings;
        
        // Convenience methods for common config operations
        std::string get_string(const std::string& key, const std::string& default_value = "") const {
            auto it = settings.find(key);
            return (it != settings.end()) ? it->second : default_value;
        }
        
        int get_int(const std::string& key, int default_value = 0) const {
            auto it = settings.find(key);
            if (it != settings.end()) {
                try {
                    return std::stoi(it->second);
                } catch (...) {
                    return default_value;
                }
            }
            return default_value;
        }
        
        double get_double(const std::string& key, double default_value = 0.0) const {
            auto it = settings.find(key);
            if (it != settings.end()) {
                try {
                    return std::stod(it->second);
                } catch (...) {
                    return default_value;
                }
            }
            return default_value;
        }
        
        bool get_bool(const std::string& key, bool default_value = false) const {
            auto it = settings.find(key);
            if (it != settings.end()) {
                const std::string& value = it->second;
                return (value == "true" || value == "1" || value == "yes" || value == "on");
            }
            return default_value;
        }
    };

    struct GameProgress {
        std::string player_name;
        int current_level = 0;
        double experience = 0.0;
        int completed_levels = 0;
        std::vector<std::string> inventory;
        
        // Constructor
        GameProgress() = default;
        
        GameProgress(const std::string& name, int level = 0, double exp = 0.0) 
            : player_name(name), current_level(level), experience(exp) {}
        
        // Utility methods
        bool is_level_completed(int level) const {
            return level <= completed_levels;
        }
        
        void complete_level(int level) {
            if (level > completed_levels) {
                completed_levels = level;
            }
        }
        
        void add_inventory_item(const std::string& item) {
            inventory.push_back(item);
        }
        
        bool has_inventory_item(const std::string& item) const {
            return std::find(inventory.begin(), inventory.end(), item) != inventory.end();
        }
    };

    /**
     * @brief Utility class for file and directory operations
     * 
     * This class provides comprehensive file system operations for the C++ Code Quest game,
     * including reading/writing files, directory management, and game-specific functionality.
     */
    class FileUtils {
    public:
        // === Core File Operations ===
        
        /**
         * @brief Read entire file content into a string
         * @param filepath Path to the file to read
         * @return Optional string containing file content, nullopt if error
         */
        static std::optional<std::string> read_file(const std::string& filepath);
        
        /**
         * @brief Write content to file (overwrites existing content)
         * @param filepath Path to the file to write
         * @param content Content to write to the file
         * @return True if successful, false otherwise
         */
        static bool write_file(const std::string& filepath, const std::string& content);
        
        /**
         * @brief Append content to file
         * @param filepath Path to the file to append to
         * @param content Content to append to the file
         * @return True if successful, false otherwise
         */
        static bool append_to_file(const std::string& filepath, const std::string& content);
        
        /**
         * @brief Read file line by line
         * @param filepath Path to the file to read
         * @return Vector of strings, one per line
         */
        static std::vector<std::string> read_lines(const std::string& filepath);
        
        /**
         * @brief Write lines to file
         * @param filepath Path to the file to write
         * @param lines Vector of strings to write as lines
         * @return True if successful, false otherwise
         */
        static bool write_lines(const std::string& filepath, const std::vector<std::string>& lines);
        
        // === File System Queries ===
        
        /**
         * @brief Check if file exists
         * @param filepath Path to check
         * @return True if file exists and is a regular file
         */
        static bool file_exists(const std::string& filepath);
        
        /**
         * @brief Check if directory exists
         * @param dirpath Path to check
         * @return True if directory exists
         */
        static bool directory_exists(const std::string& dirpath);
        
        /**
         * @brief Get file size in bytes
         * @param filepath Path to the file
         * @return Optional size in bytes, nullopt if error
         */
        static std::optional<std::size_t> get_file_size(const std::string& filepath);
        
        /**
         * @brief Get file extension including the dot (e.g., ".cpp")
         * @param filepath Path to the file
         * @return File extension string
         */
        static std::string get_file_extension(const std::string& filepath);
        
        /**
         * @brief Get filename without extension
         * @param filepath Path to the file
         * @return Filename without extension
         */
        static std::string get_filename_without_extension(const std::string& filepath);
        
        /**
         * @brief Get directory path from file path
         * @param filepath Path to the file
         * @return Directory path
         */
        static std::string get_directory_path(const std::string& filepath);
        
        // === Directory Operations ===
        
        /**
         * @brief Create directory (and parent directories if needed)
         * @param dirpath Path to create
         * @return True if successful or already exists
         */
        static bool create_directory(const std::string& dirpath);
        
        /**
         * @brief List files in directory
         * @param dirpath Directory to list
         * @param extension Optional extension filter (e.g., ".cpp")
         * @return Vector of file paths
         */
        static std::vector<std::string> list_files_in_directory(const std::string& dirpath, 
                                                               const std::string& extension = "");
        
        /**
         * @brief Get current working directory
         * @return Current directory path
         */
        static std::string get_current_directory();
        
        /**
         * @brief Change current directory
         * @param dirpath New directory path
         * @return True if successful
         */
        static bool change_directory(const std::string& dirpath);
        
        // === File Operations ===
        
        /**
         * @brief Copy file from source to destination
         * @param source Source file path
         * @param destination Destination file path
         * @return True if successful
         */
        static bool copy_file(const std::string& source, const std::string& destination);
        
        /**
         * @brief Move/rename file
         * @param source Source file path
         * @param destination Destination file path
         * @return True if successful
         */
        static bool move_file(const std::string& source, const std::string& destination);
        
        /**
         * @brief Delete file
         * @param filepath Path to file to delete
         * @return True if successful
         */
        static bool delete_file(const std::string& filepath);
        
        // === Path Utilities ===
        
        /**
         * @brief Join two paths safely
         * @param path1 First path component
         * @param path2 Second path component
         * @return Joined path
         */
        static std::string join_paths(const std::string& path1, const std::string& path2);
        
        // === Game-Specific Utilities ===
        
        /**
         * @brief Load game configuration from file
         * @param config_file Path to configuration file
         * @return Optional GameConfig, nullopt if error
         */
        static std::optional<GameConfig> load_game_config(const std::string& config_file);
        
        /**
         * @brief Save game progress to file
         * @param save_file Path to save file
         * @param progress Game progress to save
         * @return True if successful
         */
        static bool save_game_progress(const std::string& save_file, const GameProgress& progress);
        
        /**
         * @brief Load game progress from file
         * @param save_file Path to save file
         * @return Optional GameProgress, nullopt if error
         */
        static std::optional<GameProgress> load_game_progress(const std::string& save_file);
        
        /**
         * @brief Create complete project structure for C++ Code Quest
         * @param project_root Root directory for the project
         * @return True if successful
         */
        static bool create_project_structure(const std::string& project_root);
        
        // === Template Utilities ===
        
        /**
         * @brief Generic file serialization for simple types
         * @tparam T Type to serialize (must be compatible with operator<<)
         * @param filepath Path to output file
         * @param data Data to serialize
         * @return True if successful
         */
        template<typename T>
        static bool serialize_to_file(const std::string& filepath, const T& data) {
            std::ofstream file(filepath);
            if (!file.is_open()) {
                return false;
            }
            file << data;
            return file.good();
        }
        
        /**
         * @brief Generic file deserialization for simple types
         * @tparam T Type to deserialize (must be compatible with operator>>)
         * @param filepath Path to input file
         * @return Optional deserialized data
         */
        template<typename T>
        static std::optional<T> deserialize_from_file(const std::string& filepath) {
            std::ifstream file(filepath);
            if (!file.is_open()) {
                return std::nullopt;
            }
            
            T data;
            if (file >> data) {
                return data;
            }
            return std::nullopt;
        }
        
        // === Constants ===
        
        // Common file extensions
        static constexpr const char* CPP_EXTENSION = ".cpp";
        static constexpr const char* HPP_EXTENSION = ".hpp";
        static constexpr const char* TXT_EXTENSION = ".txt";
        static constexpr const char* CONFIG_EXTENSION = ".cfg";
        static constexpr const char* SAVE_EXTENSION = ".save";
        
        // Default file and directory names
        static constexpr const char* DEFAULT_CONFIG_FILE = "game_config.cfg";
        static constexpr const char* DEFAULT_SAVE_FILE = "game_progress.save";
        static constexpr const char* DEFAULT_PROJECT_NAME = "cpp-code-quest";
        
    private:
        // Private constructor - this is a utility class with only static methods
        FileUtils() = delete;
        ~FileUtils() = delete;
        FileUtils(const FileUtils&) = delete;
        FileUtils& operator=(const FileUtils&) = delete;
    };
    
    // === Convenience Type Aliases ===
    
    using Config = GameConfig;
    using Progress = GameProgress;
    using Files = FileUtils;
    
    // === Helper Functions ===
    
    /**
     * @brief Create a default game configuration
     * @return GameConfig with default settings
     */
    inline GameConfig create_default_config() {
        GameConfig config;
        config.settings["window_width"] = "1024";
        config.settings["window_height"] = "768";
        config.settings["fullscreen"] = "false";
        config.settings["sound_enabled"] = "true";
        config.settings["music_volume"] = "0.7";
        config.settings["sfx_volume"] = "0.8";
        config.settings["difficulty"] = "normal";
        config.settings["auto_save"] = "true";
        config.settings["theme"] = "dark";
        return config;
    }
    
    /**
     * @brief Create a new game progress
     * @param player_name Name of the player
     * @return GameProgress with initial values
     */
    inline GameProgress create_new_progress(const std::string& player_name) {
        GameProgress progress;
        progress.player_name = player_name;
        progress.current_level = 0;
        progress.experience = 0.0;
        progress.completed_levels = 0;
        progress.inventory.clear();
        return progress;
    }

} // namespace GameUtils