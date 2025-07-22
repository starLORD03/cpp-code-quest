#pragma once

#include <string>
#include <functional>
#include <vector>

class Level {
public:
    using ValidationFunction = std::function<bool(const std::string&)>;
    
    Level(const std::string& title,
          const std::string& story,
          const std::string& character,
          const std::string& dialogue,
          const std::string& conceptName,
          const std::string& conceptExplanation,
          const std::string& challenge,
          const std::string& reward,
          ValidationFunction validator);
    
    ~Level() = default;
    
    // Main level gameplay
    void play();
    
    // Getters
    const std::string& getTitle() const { return title_; }
    const std::string& getStory() const { return story_; }
    const std::string& getReward() const { return reward_; }
    const std::string& getConcept() const { return concept_; }
    bool isCompleted() const { return completed_; }
    
    // Challenge management
    void showChallenge() const;
    void showHint() const;
    void showSolution() const;
    bool validateSolution(const std::string& code) const;
    
private:
    std::string title_;
    std::string story_;
    std::string character_;
    std::string dialogue_;
    std::string concept_;
    std::string conceptExplanation_;
    std::string challenge_;
    std::string reward_;
    ValidationFunction validator_;
    bool completed_;
    
    // Helper methods
    void displayStory() const;
    void displayConcept() const;
    std::string getUserCode() const;
    void showFeedback(bool success, const std::string& message = "") const;
    
    // Default hints and solutions for each level
    std::string getHintText() const;
    std::string getSolutionText() const;
};
